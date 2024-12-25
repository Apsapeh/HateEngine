use std::collections::HashMap;

use cfor::cfor;
use nalgebra::Vector3;

#[derive(Debug, Clone)]
pub struct Property {
    pub value: String
}

impl Property {
    pub fn new(value: String) -> Self {
        Self {
            value
        }
    }

    #[allow(dead_code)]
    pub fn as_f32(&self) -> f32 {
        self.value.parse::<f32>().unwrap()
    }

    #[allow(dead_code)]

    pub fn as_i32(&self) -> i32 {
        self.value.parse::<i32>().unwrap()
    }

    #[allow(dead_code)]
    pub fn as_bool(&self) -> bool {
        self.value.parse::<bool>().unwrap()
    }

    pub fn as_vec3(&self) -> Vector3<f32> {
        let splitted = self.value.split(" ").collect::<Vec<&str>>();
        let x = splitted[0].parse::<f32>().unwrap();
        let y = splitted[1].parse::<f32>().unwrap();
        let z = splitted[2].parse::<f32>().unwrap();
        Vector3::new(x, y, z)
    }

    pub fn as_vec3_xzy(&self) -> Vector3<f32> {
        let v = self.as_vec3();
        Vector3::new(v.x, v.z, -v.y)
    }
}

#[derive(Debug, Clone)]
pub struct Entity {
    pub classname: String,
    pub pos: Vector3<f32>,
    pub properties: HashMap<String, Property>
}

impl Entity {
    pub fn new(classname: String) -> Entity {
        Entity { classname, pos: Vector3::default(), properties: HashMap::new()}
    }
}

pub fn load_entity_map(path: String, grid: f32) -> Vec<Entity> {
    let data = std::fs::read_to_string(path).unwrap();
    let lines = data.split("\n");

    let mut entities = Vec::new();
    let mut current_etity = Entity::new(String::from(""));
    let mut bracket_depth = 0u8;
    for line in lines {
        let trimmed = line.trim();
        if trimmed.len() == 0 {
            continue;
        }

        let chars = trimmed.chars().collect::<Vec<char>>();

        let mut words = Vec::new();
        let mut word = String::from("");
        let mut in_quotes = false;
        //println!("chars: {:?}", chars);
        //println!("Count: {}", chars.len());
        cfor!(let mut i = 0; i < chars.len(); i += 1; {
            //println!("i: {}", i);
            //println!("char: {:?}", chars[i]);
            if chars[i] == '"' {
                 in_quotes = !in_quotes;
                 if !in_quotes && !word.is_empty() {
                     words.push(word);
                     word = String::from("");
                 }
            } else if chars[i] == '\\' && i < chars.len() - 1 {
                 if chars[i+1] == '"' || chars[i+1] == '\\' {
                     word.push(chars[i+1]);
                     i += 1;
                 }
            } else {
                 word.push(chars[i]);
            }
        });

        if chars.len() > 1 && chars[0] == '/' && chars[1] == '/' {
            continue;
        }

        // Skip brushes
        if bracket_depth == 2 {
            if chars[0] == '}' {
                bracket_depth -= 1;
            }
            continue;
        }

        if chars[0] == '{' {
            bracket_depth += 1;
            if bracket_depth > 2 {
                panic!("Unexpected depth {}", bracket_depth);
            }

            if bracket_depth == 1 {
                current_etity = Entity::new(String::from(""));
            }
        } else if chars[0] == '}' {
            if bracket_depth == 0 {
                panic!("Unexpected depth {}", bracket_depth);
            }
            bracket_depth -= 1;

            if !current_etity.classname.is_empty() && current_etity.classname != "worldspawn" {
                entities.push(current_etity.clone());
            }
        } else {
            if bracket_depth != 1 {
                eprintln!("Warning: Unexpected depth {}", bracket_depth);
                //println!("{:?}", words);
                continue;
            }
            if words.len() < 2 {
                eprintln!("Warning: Unexpected word count {}", words.len());
                continue;
            }

            let word0 = words[0].clone();
            let word1 = words[1].clone();
            let word0 = word0.trim().to_string();
            let word1 = word1.trim().to_string();

            if word0 == "classname" {
                current_etity.classname = word1;
            } else if word0 == "origin" {
                let p = Property::new(word1);
                current_etity.pos = p.as_vec3_xzy() / grid;
            } else {
                let p = Property::new(word1);
                current_etity.properties.insert(word0, p);
            }
        }
    }

    entities
}
