use std::vec;

use cfor::cfor;
use lightmap::{
    input::{Mesh, WorldVertex},
    light::{LightDefinition, PointLightDefinition},
    LightMap,
};
use nalgebra::{Vector2, Vector3};

mod entity_map_loader;
mod heluv_serializer;
mod obj_loader;
mod uvier;

#[derive(Copy, Clone, Debug, Default, PartialEq)]
pub struct Vertex {
    pub position: Vector3<f32>,
    pub tex_coord: Vector2<f32>,
}

impl Vertex {
    fn new(x: f32, y: f32, z: f32) -> Self {
        Self {
            position: Vector3::new(x, y, z),
            tex_coord: Default::default(),
        }
    }
}

const USAGE_EXAMPLE_ERROR: &str = "Usage: ./helight_baker -m <.obj map> -e <.map light entities> [-o <output directory>] [-g <grid size>] [-t <texel per unit>]";

fn main() {
    let args: Vec<String> = std::env::args().collect();

    if args.len() < 2 {
        eprintln!("{}", USAGE_EXAMPLE_ERROR);
        return;
    }

    let mut map = String::from("");
    let mut light_entities = String::from("");
    let mut output = String::from("");
    let mut grid_size = 16.0f32;
    let mut texel_per_unit = 4usize;
    cfor!(let mut i = 1; i < args.len(); i += 1; {
        let arg = &args[i];

        if arg == "-m" && i+1 < args.len() {
            map = String::from(&args[i+1]);
            i += 1;
        }
        else if arg == "-g" && i+1 < args.len() {
            grid_size = args[i+1].parse().unwrap();
            i += 1;
        }
        else if arg == "-e" && i+1 < args.len() {
            light_entities = String::from(&args[i+1]);
            i += 1;
        }
        else if arg == "-o" && i+1 < args.len() {
            output = String::from(&args[i+1]);
            i += 1;
        }
        else if arg == "-t" && i+1 < args.len() {
            texel_per_unit = args[i+1].parse().unwrap();
            i += 1;
        }
        else {
            eprintln!("{}", USAGE_EXAMPLE_ERROR);
            return;
        }
    });

    if map.is_empty() {
        eprintln!("{}", USAGE_EXAMPLE_ERROR);
        return;
    }

    if output.is_empty() {
        output = format!(
            "helight_baker/{}",
            map.split("/").last().unwrap().split(".").next().unwrap()
        );
    }

    std::fs::create_dir_all(&output).unwrap();

    /*let entities = entity_map_loader::load_entity_map(light_entities, grid_size);

    for entity in entities {
        println!("{}", entity.classname);
    }

    panic!();*/

    run(map, light_entities, output, grid_size, texel_per_unit);
}

fn run(path: String, entity_map_path: String, output: String, grid_size: f32, texel_per_unit: usize) {
    let mut obj = obj_loader::load_obj(path.to_string());
    for o in &mut obj {
        o.faces
            .iter_mut()
            .flat_map(|f| f.vertices.iter_mut())
            .for_each(|v| {
                *v /= grid_size;
            });
    }

    let mut meshes = vec![];
    let mut meshes_names = vec![];
    let mut heluv = heluv_serializer::Heluv::new();

    /*for o in &obj {
        if o.name == "entity0_brush428" {
            let mut faces = vec![];

            for f in &o.faces {
                for v in f.get_triangulated() {
                    faces.push(v);
                }
            }
            uvier::smart_unwrap(faces);
        }
    }*/
    //panic!();

    for o in obj {
        let mut faces = vec![];

        for f in &o.faces {
            for v in f.get_triangulated() {
                faces.push(v);
            }
        }

        let (uvs, uvs_pairs) = uvier::smart_unwrap(faces);
        let (vertices_raw, triangles) = o.get_raw_data();

        let mut heluv_obj_uvs = vec![];
        for f in &o.faces {
            let mut face_uvs = vec![];
            //println!("{}: {:?}", o.name, f.get_normal());
            /*for v in &f.vertices {
                println!("V: {:?}", v);
            }*/
            for v in &f.vertices {
                //println!("{:?}: {:?}", v, f.get_normal());
                let paired_uv = uvs_pairs
                    .iter()
                    .find(|p| {
                        //println!("{:?}", p);
                        (p.0 .0 - v).magnitude() < 0.01
                            && (p.0 .1 - f.get_normal()).magnitude() < 0.01
                    })
                    .unwrap();
                face_uvs.push(paired_uv.1);
            }
            heluv_obj_uvs.push(face_uvs);
        }

        heluv.add_object(o.name.clone(), heluv_obj_uvs);

        let mut vertices = vec![];
        for i in 0..vertices_raw.len() {
            vertices.push(Vertex::new(
                vertices_raw[i].x,
                vertices_raw[i].y,
                vertices_raw[i].z,
            ));
        }

        for v in vertices.iter_mut().zip(&uvs) {
            v.0.tex_coord = Vector2::new(v.1[0], v.1[1]);
        }

        let mesh = lightmap_mesh_new(vertices, triangles);
        meshes.push(mesh);
        meshes_names.push(o.name.clone());
    }

    heluv.serialize_to_file(format!("{}/light.heluv", output));

    // Step 5. Generate light map.
    let lights = load_lights(entity_map_path, grid_size);
    /*let lights = [
        LightDefinition::Point(PointLightDefinition {
            intensity: 1.0,
            color: Vector3::new(1.0, 1.0, 1.0),
            radius: 2.0,
            position: Vector3::new(2.0, 0.0, 0.0),
            sqr_radius: 4.0,
        }),
        LightDefinition::Point(PointLightDefinition {
            intensity: 1.0,
            color: Vector3::new(1.0, 1.0, 1.0),
            radius: 2.0,
            position: Vector3::new(-2.0, 0.0, 0.0),
            sqr_radius: 4.0,
        }),
        LightDefinition::Point(PointLightDefinition {
            intensity: 1.0,
            color: Vector3::new(0.0, 1.0, 0.0),
            radius: 2.0,
            position: Vector3::new(0.0, 2.0, 0.0),
            sqr_radius: 4.0,
        }),
        LightDefinition::Point(PointLightDefinition {
            intensity: 1.0,
            color: Vector3::new(1.0, 1.0, 1.0),
            radius: 2.0,
            position: Vector3::new(0.0, -2.0, 0.0),
            sqr_radius: 4.0,
        }),
        LightDefinition::Point(PointLightDefinition {
            intensity: 1.0,
            color: Vector3::new(1.0, 1.0, 1.0),
            radius: 2.0,
            position: Vector3::new(0.0, 0.0, 2.0),
            sqr_radius: 4.0,
        }),
        LightDefinition::Point(PointLightDefinition {
            intensity: 1.0,
            color: Vector3::new(1.0, 1.0, 1.0),
            radius: 2.0,
            position: Vector3::new(0.0, 0.0, -2.0),
            sqr_radius: 4.0,
        }),
        LightDefinition::Point(PointLightDefinition {
            intensity: 1.0,
            color: Vector3::new(1.0, 1.0, 1.0),
            radius: 8.0,
            position: Vector3::new(0.0, 0.0, 0.0),
            sqr_radius: 64.0,
        }),
    ];*/

    let mut counter = 0;
    for m in &meshes {
        println!(
            "[{}/{}] Baking {}...",
            counter + 1,
            meshes.len(),
            meshes_names[counter]
        );
        let light_map = LightMap::new(m, &meshes, &lights, texel_per_unit);
        image::save_buffer(
            format!("{}/{}.png", output, meshes_names[counter]),
            &light_map.pixels,
            light_map.width as u32,
            light_map.height as u32,
            image::ColorType::Rgb8,
        )
        .unwrap();
        counter += 1;
    }
}

fn lightmap_mesh_new(vertices: Vec<Vertex>, triangles: Vec<[u32; 3]>) -> Mesh {
    let mut mesh = Mesh::new(
        vertices
            .iter()
            .map(|v| WorldVertex {
                world_normal: Default::default(),
                world_position: v.position,
                second_tex_coord: v.tex_coord,
            })
            .collect(),
        triangles,
    )
    .unwrap();

    // Calculate normals.
    for triangle in mesh.triangles.iter() {
        let pos_a = mesh.vertices[triangle[0] as usize].world_position;
        let pos_b = mesh.vertices[triangle[1] as usize].world_position;
        let pos_c = mesh.vertices[triangle[2] as usize].world_position;

        let normal = (pos_c - pos_a)
            .cross(&(pos_c - pos_b))
            .try_normalize(f32::EPSILON)
            .unwrap_or_default();

        mesh.vertices[triangle[0] as usize].world_normal = normal;
        mesh.vertices[triangle[1] as usize].world_normal = normal;
        mesh.vertices[triangle[2] as usize].world_normal = normal;
    }
    mesh
}


/// Функця для расстановки света по .map файлу, мне лень писать файл конфига через .toml,
/// движком вряд ли кто-то будет пользоваться, а я и тут могу написать всё, что мне надо 
fn load_lights(path: String, grid: f32) -> Vec<LightDefinition> {
    let mut result = vec![];
    let entities = entity_map_loader::load_entity_map(path, grid);
    
    for e in entities {
        if e.classname == "light" {
            let radius = match e.properties.get("light") {
                Some(l) => l.as_f32() / grid,
                None => grid / 2.0
            };

            let l = LightDefinition::Point(PointLightDefinition {
                intensity: 1.0,
                color: Vector3::new(1.0, 1.0, 1.0),
                radius: radius,
                position: e.pos,
                sqr_radius: radius.powi(2),
            });
            result.push(l);
        }
    }

    result
}