use nalgebra::{Vector2, Vector3};
use nalgebra_glm as glm;

use crate::obj_loader;

struct PolygonProjector;

impl PolygonProjector {
    /// Вычисляет нормаль полигона по его вершинам
    pub fn calculate_normal(vertices: &[glm::Vec3]) -> glm::Vec3 {
        if vertices.len() < 3 {
            return glm::Vec3::zeros();
        }

        let v1 = vertices[1] - vertices[0];
        let v2 = vertices[2] - vertices[0];
        let normal = glm::cross(&v1, &v2);

        // Нормализуем вектор
        glm::normalize(&normal)
    }

    /// Создает матрицу поворота для выравнивания нормали с вектором (0,0,1)
    fn create_rotation_matrix(normal: &glm::Vec3) -> glm::Mat4 {
        let target = glm::vec3(0.0, 0.0, 1.0);

        // Если векторы уже параллельны
        if (glm::dot(normal, &target)).abs() > 0.9999 {
            return glm::Mat4::identity();
        }

        // Находим ось вращения и угол
        let rotation_axis = glm::normalize(&glm::cross(normal, &target));
        let angle = glm::angle(normal, &target);

        // Создаем матрицу поворота
        glm::rotate(&glm::Mat4::identity(), angle, &rotation_axis)
    }

    /// Проецирует 3D полигон на плоскость, перпендикулярную его нормали
    pub fn project_polygon(vertices: &[glm::Vec3]) -> Vec<glm::Vec2> {
        if vertices.is_empty() {
            return Vec::new();
        }

        // Вычисляем нормаль
        let normal = Self::calculate_normal(vertices);

        // Получаем матрицу поворота
        let rotation_matrix = Self::create_rotation_matrix(&normal);

        // Проецируем каждую вершину
        vertices
            .iter()
            .map(|vertex| {
                // Применяем поворот
                let rotated = rotation_matrix * glm::vec4(vertex.x, vertex.y, vertex.z, 1.0);

                // Проецируем на XY-плоскость (отбрасываем Z)
                glm::vec2(rotated.x, rotated.y)
            })
            .collect()
    }
}

#[derive(Debug, Clone)]
struct FaceUV {
    face: obj_loader::ObjFace,
    normal: Vector3<f32>,
    raw_uv: [Vector2<f32>; 3],
    original_idx: usize,
}

#[derive(Debug, Clone)]
struct FaceUVPair {
    pub uvs: Vec<FaceUV>,
}

impl FaceUVPair {
    pub fn new(uvs: Vec<FaceUV>) -> Self {
        Self { uvs }
    }

    pub fn get_width(&self) -> f32 {
        let mut max = f32::MIN;
        for uv in &self.uvs {
            max = f32::max(max, uv.raw_uv[0].x);
            max = f32::max(max, uv.raw_uv[1].x);
            max = f32::max(max, uv.raw_uv[2].x);
        }
        max
    }

    pub fn get_height(&self) -> f32 {
        let mut max = f32::MIN;
        for uv in &self.uvs {
            max = f32::max(max, uv.raw_uv[0].y);
            max = f32::max(max, uv.raw_uv[1].y);
            max = f32::max(max, uv.raw_uv[2].y);
        }
        max
    }

    pub fn get_area(&self) -> f32 {
        self.get_width() * self.get_height()
    }
}

pub fn smart_unwrap(
    faces: Vec<obj_loader::ObjFace>,
) -> (
    Vec<Vector2<f32>>,
    Vec<((Vector3<f32>, Vector3<f32>), Vector2<f32>)>,
) {
    let mut uvs = Vec::new();
    let mut idx = 0;
    for f in faces {
        let v0 = f.vertices[0];
        let v1 = f.vertices[1];
        let v2 = f.vertices[2];

        let normal = PolygonProjector::calculate_normal(&[v0, v1, v2]);
        let poly = PolygonProjector::project_polygon(&f.vertices);

        uvs.push(FaceUV {
            face: f,
            normal,
            raw_uv: poly.try_into().unwrap(),
            //out_uv: poly.try_into().unwrap(),
            original_idx: idx,
        });
        idx += 1;
    }

    let mut uvs_pairs: Vec<FaceUVPair> = Vec::new();

    'outer: for uv in uvs {
        for u in &mut uvs_pairs {
            if u.uvs
                .iter()
                .find(|f| {
                    (f.normal - uv.normal).magnitude() < 0.01
                        && two_point_is_same(&f.face, &uv.face)
                })
                .is_some()
            {
                u.uvs.push(uv.clone());
                continue 'outer;
            }
        }
        uvs_pairs.push(FaceUVPair::new(vec![uv]));
    }

    for u in &mut uvs_pairs {
        //u.uvs.sort_by(|a, b| a.original_idx.cmp(&b.original_idx));
        //println!("{:?}", u.uvs);
        let mut min = Vector2::new(f32::INFINITY, f32::INFINITY);
        for v in &u.uvs {
            //println!("{:?}", v);
            for raw in &v.raw_uv {
                min.x = f32::min(min.x, raw.x);
                min.y = f32::min(min.y, raw.y);
            }
        }
        for v in &mut u.uvs {
            for raw in &mut v.raw_uv {
                *raw -= min;
            }
        }
    }

    /*for u in &mut uvs_pairs {
        //u.uvs.sort_by(|a, b| a.original_idx.cmp(&b.original_idx));
        //println!("{:?}", u.uvs);
        for v in &u.uvs {
            println!("{:?}", v);
        }
        println!();
    }*/

    collage_uvs(uvs_pairs.clone())
}

fn two_point_is_same(f1: &obj_loader::ObjFace, f2: &obj_loader::ObjFace) -> bool {
    let mut counter = 0;
    for a in 0..3 {
        for b in 0..3 {
            if (f1.vertices[a] - f2.vertices[b]).magnitude() < 0.01 {
                counter += 1;
                break;
            }
        }
    }
    counter == 2
}

#[derive(Debug)]
struct PastedUV {
    x: f32,
    y: f32,
    width: f32,
    height: f32,
    uvs: Vec<FaceUV>,
}

fn collage_uvs(
    mut uvs: Vec<FaceUVPair>,
) -> (
    Vec<Vector2<f32>>,
    Vec<((Vector3<f32>, Vector3<f32>), Vector2<f32>)>,
) {
    //println!("COLLAGE");
    uvs.sort_by(|a, b| a.get_area().partial_cmp(&b.get_area()).unwrap());

    let area_sum = uvs.iter().map(|f| f.get_area()).sum::<f32>();
    let height_sum = uvs.iter().map(|f| f.get_height()).sum::<f32>();
    let min_side = uvs
        .iter()
        .map(|f| f.get_width())
        .max_by(|a, b| a.partial_cmp(b).unwrap())
        .unwrap();
    let max_side = f32::sqrt(area_sum);
    let max_side = f32::max(max_side, min_side);

    let mut available_area = vec![[0.0, 0.0, max_side, height_sum * 2.0]];
    let mut posted_uvs = Vec::new();

    for uv in &uvs {
        for (i, area) in available_area.clone().iter().enumerate() {
            if area[2] >= uv.get_width() && area[3] >= uv.get_height() {
                posted_uvs.push(PastedUV {
                    x: area[0],
                    y: area[1],
                    width: uv.get_width(),
                    height: uv.get_height(),
                    uvs: uv.uvs.clone(),
                });

                let original_area = &mut available_area[i];

                if (uv.get_width() - area[2]).abs() < 0.01 {
                    original_area[1] += uv.get_height();
                    original_area[3] -= uv.get_height();
                } else if (uv.get_height() - area[3]).abs() < 0.01 {
                    original_area[0] += uv.get_width();
                    original_area[2] -= uv.get_width();
                } else {
                    available_area.remove(i);
                    available_area.push([
                        area[0] + uv.get_width(),
                        area[1],
                        area[2] - uv.get_width(),
                        uv.get_height(),
                    ]);
                    available_area.push([
                        area[0],
                        area[1] + uv.get_height(),
                        area[2],
                        area[3] - uv.get_height(),
                    ]);
                }

                break;
            }

            if i == available_area.len() - 1 {
                println!("No space for {}*{}", uv.get_width(), uv.get_height());
                /*println!("Max side: {}", max_side);
                println!("Area sum: {}", area_sum);
                println!("Available area:");
                for a in &available_area {
                    println!("\t{}x{}: {}x{}", a[0], a[1], a[2], a[3]);
                }
                println!();

                println!("Posted uvs:");
                for p in &posted_uvs {
                    println!("\t{}x{}: {}x{}", p.x, p.y, p.width, p.height);
                }*/
                panic!();
            }
        }

        available_area.sort_by(|a, b| {
            let a_area = a[2] * a[3];
            let b_area = b[2] * b[3];
            a_area.partial_cmp(&b_area).unwrap()
        });
        available_area.retain(|a| a[2] > 0.0 && a[3] > 0.0);
    }

    let collage_width = posted_uvs
        .iter()
        .map(|p| p.x + p.width)
        .max_by(|a, b| a.partial_cmp(b).unwrap())
        .unwrap();
    let collage_height = posted_uvs
        .iter()
        .map(|p| p.y + p.height)
        .max_by(|a, b| a.partial_cmp(b).unwrap())
        .unwrap();

    let mut final_uvs = Vec::new();
    for p in &posted_uvs {
        for uv in &p.uvs {
            let mut u = uv.clone();
            for raw_uv in &mut u.raw_uv {
                raw_uv.x = p.x / collage_width + raw_uv.x / collage_width;
                raw_uv.y = p.y / collage_height + raw_uv.y / collage_height;
            }
            final_uvs.push(u);
        }
    }

    final_uvs.sort_by(|a, b| a.original_idx.partial_cmp(&b.original_idx).unwrap());

    let mut result_raw_data = Vec::new();
    let mut relsult_uv_vertex_pairs = Vec::new();
    for p in &final_uvs {
        for (i, uv) in p.raw_uv.iter().enumerate() {
            result_raw_data.push(Vector2::new(uv.x, uv.y));
            relsult_uv_vertex_pairs.push((
                (p.face.vertices[i], p.face.get_normal()),
                Vector2::new(uv.x, uv.y),
            ));
        }
    }

    (result_raw_data, relsult_uv_vertex_pairs)
}
