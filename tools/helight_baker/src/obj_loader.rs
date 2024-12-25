use cfor::cfor;
use nalgebra::Vector3;
use std::vec;

#[derive(Debug, Clone)]
pub struct ObjFace {
    pub vertices: Vec<Vector3<f32>>,
}

impl ObjFace {
    pub fn get_triangulated(&self) -> Vec<ObjFace> {
        let mut faces = Vec::new();

        for i in 2..self.vertices.len() {
            let face = ObjFace {
                vertices: vec![self.vertices[0], self.vertices[i - 1], self.vertices[i]],
            };
            faces.push(face);
        }

        faces
    }

    pub fn get_normal(&self) -> Vector3<f32> {
        let v0 = self.vertices[0];
        let v1 = self.vertices[1];
        let v2 = self.vertices[2];

        let u = v1 - v0;
        let v = v2 - v0;

        u.cross(&v).normalize()
    }
}

#[derive(Debug, Clone)]
pub struct Object {
    pub name: String,
    pub faces: Vec<ObjFace>,
}

impl Object {
    pub fn get_raw_data(&self) -> (Vec<Vector3<f32>>, Vec<[u32; 3]>) {
        let mut vertices = Vec::new();

        for f in &self.faces {
            for t in f.get_triangulated() {
                for v in t.vertices {
                    vertices.push(v);
                }
            }
        }
        let mut indices = Vec::new();
        cfor!(let mut i = 0; i < vertices.len(); i += 3; {
            indices.push([i as u32, i as u32 + 1, i as u32 + 2]);

        });

        (vertices, indices)
    }
}

pub fn load_obj(path: String) -> Vec<Object> {
    let data = std::fs::read_to_string(path).unwrap();
    let lines = data.split("\n");

    let mut vertices = Vec::new();

    let mut objects = Vec::new();
    let mut current_object = Object {
        name: String::from(""),
        faces: Vec::new(),
    };

    for line in lines {
        let trimmed = line.trim();

        if trimmed.len() == 0 {
            continue;
        }

        let words = trimmed
            .split(" ")
            .filter(|s| s.len() > 0)
            .collect::<Vec<&str>>();
        if words[0] == "v" {
            let x = words[1].parse::<f32>().unwrap();
            let y = words[2].parse::<f32>().unwrap();
            let z = words[3].parse::<f32>().unwrap();
            vertices.push(Vector3::new(x, y, z));
        } else if words[0] == "o" {
            if current_object.name.len() > 0 {
                objects.push(current_object);
            }

            current_object = Object {
                name: String::from(words[1]),
                faces: Vec::new(),
            };
        } else if words[0] == "f" {
            //println!("{:?}", words);
            let mut obj_face = ObjFace {
                vertices: Vec::new(),
            };
            for i in 1..words.len() {
                let vertex = words[i].split("/").collect::<Vec<&str>>();
                let index = vertex[0].parse::<usize>().unwrap() - 1;
                let vertex = vertices[index];
                obj_face.vertices.push(vertex);
            }
            current_object.faces.push(obj_face);
        }
    }

    if current_object.name.len() > 0 {
        objects.push(current_object);
    }

    objects
}
