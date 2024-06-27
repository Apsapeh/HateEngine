use std::io::{Read, Write};

use crate::error::Error;
use physis::blowfish::Blowfish;

pub struct HerResource {
    pub name: String,
    pub file: String,
    pub original_size: u64,
    pub aligned_size: u64,
}

impl HerResource {
    pub fn new(name: &str, file: &str) -> Self {
        // Get file attributes
        let metadata = std::fs::metadata(file).unwrap();

        let mut align = metadata.len() % 8;
        let original_size = metadata.len();
        let mut aligned_size = metadata.len();
        if align != 0 {
            align = 8 - align;
            aligned_size += align;
        }

        Self {
            name: name.to_string(),
            file: file.to_string(),
            original_size,
            aligned_size,
        }
    }
}

pub struct HerFile {
    pswd: Vec<u8>,
    resources: Vec<HerResource>,
}

impl HerFile {
    pub fn new(pswd: &str) -> Result<Self, Error> {
        if !pswd.is_ascii() {
            return Err(Error::PasswordIsNotASCII);
        }

        let pswd_bytes = pswd.as_bytes();
        if pswd_bytes.len() < 4 || pswd_bytes.len() > 56 {
            return Err(Error::PasswordIsNotValidLength);
        }

        Ok(Self {
            pswd: pswd_bytes.to_vec(),
            resources: Vec::new(),
        })
    }

    pub fn add_resource(&mut self, resource: HerResource) {
        self.resources.push(resource);
    }

    pub fn save(&self, file: &str) {
        let mut header = vec![];

        let version = 1u32;
        let mut data_pointer = 20u64;
        let res_count = self.resources.len() as u64;

        // res header
        // nul terminated string - name
        // u64 - original size
        // u64 - aligned_size
        // u64 - file position
        let mut res_header = vec![];
        let mut offset = 0u64;
        for res in &self.resources {
            let name = res.name.clone().into_bytes();
            //name.push(0);
            let original_size = res.original_size.to_le_bytes();
            let aligned_size = res.aligned_size.to_le_bytes();
            let file_position = offset.to_le_bytes();

            res_header.extend((name.len() as u64).to_le_bytes());
            res_header.extend(name);
            res_header.extend(original_size);
            res_header.extend(aligned_size);
            res_header.extend(file_position);

            offset += res.aligned_size as u64;
        }
        data_pointer += res_header.len() as u64;

        header.extend_from_slice(&version.to_le_bytes());
        header.extend_from_slice(&data_pointer.to_le_bytes());
        header.extend_from_slice(&res_count.to_le_bytes());
        header.extend(res_header);

        let mut filename = file.to_string();
        if !filename.ends_with(".her") {
            filename.push_str(".her");
        }

        let mut her_file = std::fs::File::create(filename).unwrap();
        her_file.write_all(&header).unwrap();

        let fish = Blowfish::new(&self.pswd);
        for res in &self.resources {
            let mut file = std::fs::File::open(&res.file).unwrap();
            let mut left_to_read = res.original_size as u64;

            while left_to_read > 0 {
                let mut buffer = [0u8; 1024 * 1024 * 4];
                let readed = file.read(&mut buffer).unwrap();

                let mut align = 0;
                if readed % 8 != 0 {
                    align = 8 - (readed % 8);
                }

                let encrypted = fish.encrypt(&buffer[0..readed + align]).unwrap();

                her_file.write_all(&encrypted).unwrap();

                left_to_read -= readed as u64;
            }
        }
    }
}
