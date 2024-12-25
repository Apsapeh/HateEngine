use nalgebra::Vector2;

use std::io::Write;

struct HeluvObject {
    name: String,
    uvs: Vec<Vec<Vector2<f32>>>,
}

pub struct Heluv {
    objects: Vec<HeluvObject>,
}

impl Heluv {
    pub fn new() -> Heluv {
        Heluv {
            objects: Vec::new(),
        }
    }

    pub fn add_object(&mut self, name: String, uvs: Vec<Vec<Vector2<f32>>>) {
        self.objects.push(HeluvObject { name, uvs });
    }

    pub fn serialize_to_file(&self, path: String) {
        let mut file = std::fs::File::create(path).unwrap();

        let version = 1u32;
        file.write_all(&version.to_le_bytes()).unwrap();
        let count = self.objects.len() as u32;
        file.write_all(&count.to_le_bytes()).unwrap();

        for obj in &self.objects {
            let name_len = obj.name.len() as u16;
            file.write_all(&name_len.to_le_bytes()).unwrap();
            let uv_count = obj.uvs.len() as u32;
            file.write_all(&uv_count.to_le_bytes()).unwrap();

            file.write_all(obj.name.as_bytes()).unwrap();

            for uv in &obj.uvs {
                let uv_count = uv.len() as u8;
                file.write_all(&uv_count.to_le_bytes()).unwrap();
                for p in uv {
                    file.write_all(&p.x.to_le_bytes()).unwrap();
                    file.write_all(&p.y.to_le_bytes()).unwrap();
                }
            }
        }
    }
}
