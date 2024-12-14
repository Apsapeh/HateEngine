use glfw::{Action, Context, Key};
use cgmath::{perspective, Deg, Matrix, Matrix4, Point3, Vector3};
use std::{ffi::CString, process::exit};

mod gl;

const VERTEX_SHADER_SOURCE: &str = r#"
#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
}
"#;

const FRAGMENT_SHADER_SOURCE: &str = r#"
#version 330 core
out vec4 FragColor;

void main() {
    FragColor = vec4(1.0, 0.5, 0.2, 1.0);
}
"#;

unsafe fn main_fn() {
    use glfw::fail_on_errors;
    let mut glfw = glfw::init(fail_on_errors!()).unwrap();

    glfw.window_hint(glfw::WindowHint::ContextVersion(3, 3));
    glfw.window_hint(glfw::WindowHint::OpenGlProfile(glfw::OpenGlProfileHint::Core));
    glfw.window_hint(glfw::WindowHint::OpenGlForwardCompat(true));
    //let minor = 3;
    //glfw.window_hint(glfw::WindowHint::ContextVersion(3, minor));

    // Create a windowed mode window and its OpenGL context
    let (mut window, events) = glfw
        .create_window(
            1024,
            1024,
            "Hello this is window",
            glfw::WindowMode::Windowed,
        )
        .expect("Failed to create GLFW window.");

    // Make the window's context current
    window.make_current();
    window.set_key_polling(true);
    window.set_size(1024, 1024);
    // disable vsync
    glfw.set_swap_interval(glfw::SwapInterval::None);

    gl::load(|s| window.get_proc_address(s) as *const _);

    

    


    // Компилируем шейдеры и создаем программу
    // Компилируем шейдеры и создаем программу
    let vertex_shader = gl::CreateShader(gl::VERTEX_SHADER);
    let c_str_vert = CString::new(VERTEX_SHADER_SOURCE.as_bytes()).unwrap();
    gl::ShaderSource(vertex_shader, 1, &c_str_vert.as_ptr(), std::ptr::null());
    gl::CompileShader(vertex_shader);
    check_shader_compile_errors(vertex_shader);

    let fragment_shader = gl::CreateShader(gl::FRAGMENT_SHADER);
    let c_str_frag = CString::new(FRAGMENT_SHADER_SOURCE.as_bytes()).unwrap();
    gl::ShaderSource(fragment_shader, 1, &c_str_frag.as_ptr(), std::ptr::null());
    gl::CompileShader(fragment_shader);
    check_shader_compile_errors(fragment_shader);

    let shader_program = gl::CreateProgram();
    gl::AttachShader(shader_program, vertex_shader);
    gl::AttachShader(shader_program, fragment_shader);
    gl::LinkProgram(shader_program);
    check_program_link_errors(shader_program);

    gl::DeleteShader(vertex_shader);
    gl::DeleteShader(fragment_shader);

    // Задаем данные для треугольника
    let vertices: [f32; 9] = [
        -0.5, -0.5, 0.0, // Нижний левый угол
        0.5, -0.5, 0.0, // Нижний правый угол
        0.0, 0.5, 0.0, // Верхний угол
    ];

    let mut vbo = 0;
    let mut vao = 0;
    let mut fbo = 0;
    let mut rbo = 0;
    let mut texture = 0;
    gl::GenVertexArrays(1, &mut vao);
    gl::GenBuffers(1, &mut vbo);

    // Настраиваем VAO и VBO
    gl::BindVertexArray(vao);

    gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
    gl::BufferData(
        gl::ARRAY_BUFFER,
        (vertices.len() * std::mem::size_of::<f32>()) as isize,
        vertices.as_ptr() as *const _,
        gl::STATIC_DRAW,
    );

    gl::VertexAttribPointer(
        0,
        3,
        gl::FLOAT,
        gl::FALSE,
        (3 * std::mem::size_of::<f32>()) as i32,
        std::ptr::null(),
    );
    gl::EnableVertexAttribArray(0);

    gl::BindBuffer(gl::ARRAY_BUFFER, 0);
    gl::BindVertexArray(0);

    
    let mut vbos = Vec::new();
    let mut vaos = Vec::new();

    let path = "examples/E1M1.obj";
    let mut obj = tobj::load_obj(path, &tobj::GPU_LOAD_OPTIONS).unwrap().0;

    for o in &obj {
        let mut vbo = 0;
        gl::GenBuffers(1, &mut vbo);
        vbos.push(vbo);

        let mut vao = 0;
        gl::GenVertexArrays(1, &mut vao);
        vaos.push(vao);
        gl::BindVertexArray(vao);

        gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
        gl::BufferData(
            gl::ARRAY_BUFFER,
            (o.mesh.positions.len() * std::mem::size_of::<f32>()) as isize,
            o.mesh.positions.as_ptr() as *const _,
            gl::STATIC_DRAW,
        );
        
        
        gl::VertexAttribPointer(
            0,
            3,
            gl::FLOAT,
            gl::FALSE,
            (3 * std::mem::size_of::<f32>()) as i32,
            std::ptr::null(),
        );
        gl::EnableVertexAttribArray(0);
        gl::BindVertexArray(0);
        gl::BindBuffer(gl::ARRAY_BUFFER, 0);
    }


    gl::Viewport(0, 0, 1024, 1024);


    // Матрицы камеры
    let projection = perspective(Deg(90.0), 1.0, 0.1, 10000.0);
    let view = Matrix4::look_at_rh(
        Point3::new(0.0, 0.0, 2.0), // Позиция камеры
        Point3::new(0.0, 0.0, -1.0), // Направление камеры
        Vector3::new(0.0, 1.0, 0.0), // Вектор "вверх"
    );

    let name = CString::new("projection").unwrap();
    let projection_loc = gl::GetUniformLocation(shader_program, name.as_ptr());
    let name = CString::new("view").unwrap();
    let view_loc = gl::GetUniformLocation(shader_program, name.as_ptr());

    /*println!("projection_loc: {}", projection_loc);
    println!("view_loc: {}", view_loc);
    exit(0);*/

    // Loop until the user closes the window
    let mut last_time = std::time::Instant::now();
    let mut frame_counter = 0;
    let mut all_elapsed = 0f32;
    while !window.should_close() {
        let now = std::time::Instant::now();
        let elapsed = now.duration_since(last_time);
        //println!("fps: {}", 1.0 / elapsed.as_secs_f32());
        last_time = now;

        if all_elapsed >= 1.0 {
            println!("fps: {}", frame_counter);
            frame_counter = 0;
            all_elapsed = 0f32;
        } else {
            all_elapsed += elapsed.as_secs_f32();
            frame_counter += 1;
        }

        gl::ClearColor(0.2, 0.3, 0.3, 1.0);
        gl::Clear(gl::COLOR_BUFFER_BIT);
        gl::Viewport(0, 0, 1024, 1024);



        gl::UseProgram(shader_program);

        gl::UniformMatrix4fv(projection_loc, 1, gl::FALSE, projection.as_ptr());
        gl::UniformMatrix4fv(view_loc, 1, gl::FALSE, view.as_ptr());

        //l::BindVertexArray(vao);
        //gl::DrawArrays(gl::TRIANGLES, 0, 3);

        for i in 0..obj.len() {
            gl::BindVertexArray(vaos[i]);

            //gl::BindBuffer(gl::ARRAY_BUFFER, vbos[i]);
            gl::DrawArrays(gl::TRIANGLES, 0, obj[i].mesh.indices.len() as i32);
            
            //gl::BindBuffer(gl::ARRAY_BUFFER, 0);
            //gl::BindVertexArray(0);
        }

        let mut buffer = vec![0u8; 1024 * 1024 * 3];
        gl::ReadPixels(0, 0, 1024, 1024, gl::RGB, gl::UNSIGNED_BYTE, buffer.as_mut_ptr() as *mut _);


        //let image: image::ImageBuffer<image::Rgb<u8>, Vec<u8>> = image::ImageBuffer::from_raw(1024, 1024, buffer).unwrap();
        //let image = image::DynamicImage::ImageRgb8(image);
        //image.save("out.png").unwrap();
        //panic!()

        
        // Swap front and back buffers
        //window.swap_buffers();

        // Poll for and process events
        glfw.poll_events();
    }
}



fn check_shader_compile_errors(shader: u32) {
    unsafe {
        let mut success = gl::FALSE as gl::types::GLint;
        gl::GetShaderiv(shader, gl::COMPILE_STATUS, &mut success);
        if success != gl::TRUE as gl::types::GLint {
            let mut len = 0;
            gl::GetShaderiv(shader, gl::INFO_LOG_LENGTH, &mut len);
            let mut buffer = Vec::with_capacity(len as usize);
            buffer.set_len((len as usize) - 1);
            gl::GetShaderInfoLog(
                shader,
                len,
                std::ptr::null_mut(),
                buffer.as_mut_ptr() as *mut gl::types::GLchar,
            );
            eprintln!("Shader compilation error: {}", String::from_utf8_lossy(&buffer));
        }
    }
}

fn check_program_link_errors(program: u32) {
    unsafe {
        let mut success = gl::FALSE as gl::types::GLint;
        gl::GetProgramiv(program, gl::LINK_STATUS, &mut success);
        if success != gl::TRUE as gl::types::GLint {
            let mut len = 0;
            gl::GetProgramiv(program, gl::INFO_LOG_LENGTH, &mut len);
            let mut buffer = Vec::with_capacity(len as usize);
            buffer.set_len((len as usize) - 1);
            gl::GetProgramInfoLog(
                program,
                len,
                std::ptr::null_mut(),
                buffer.as_mut_ptr() as *mut gl::types::GLchar,
            );
            eprintln!("Program linking error: {}", String::from_utf8_lossy(&buffer));
        }
    }
}



fn main() {
    unsafe {
        main_fn();
    }
}

/*use std::collections::HashSet;

use cfor::cfor;

const USAGE_EXAMPLE_ERROR: &str = "Usage: ./pvs_baker -m <.obj map> [-o <output>] [-g <grid size>] [-c <cell size>] [-s <cell step>]";

fn main() {
    let args: Vec<String> = std::env::args().collect();

    if args.len() < 2 {
        eprintln!("{}", USAGE_EXAMPLE_ERROR);
        return;
    }

    let mut map = String::from("");
    let mut output = String::from("out.hepvs");
    let mut grid_size = 16.0f32;
    let mut cell_size = 16.0f32;
    let mut cell_step = 1.0f32;
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
        else if arg == "-c" && i+1 < args.len() {
            cell_size = args[i+1].parse().unwrap();
            i += 1;
        }
        else if arg == "-s" && i+1 < args.len() {
            cell_step = args[i+1].parse().unwrap();
            i += 1;
        }
        else if arg == "-o" && i+1 < args.len() {
            output = String::from(&args[i+1]);
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

    let objects = parse_obj(&map, grid_size);
    //bake_pvs(objects, cell_size, cell_step);
    let center = Vec3::new(30.0f32, 5.0f32, 17.0f32);
    let v = visible_objects(center, objects);
    println!("Visible objects: {}", v.len());
}

#[derive(Copy, Clone, Debug)]
struct Vec3 {
    x: f32,
    y: f32,
    z: f32,
}
impl Vec3 {
    fn new(x: f32, y: f32, z: f32) -> Vec3 {
        Vec3 { x, y, z }
    }

    fn distance_to(&self, other: &Vec3) -> f32 {
        let dx = self.x - other.x;
        let dy = self.y - other.y;
        let dz = self.z - other.z;

        (dx * dx + dy * dy + dz * dz).sqrt()
    }

    fn normalize(&self) -> Vec3 {
        let length = (self.x * self.x + self.y * self.y + self.z * self.z).sqrt();
        Vec3 {
            x: self.x / length,
            y: self.y / length,
            z: self.z / length,
        }
    }

    fn cross(&self, other: &Vec3) -> Vec3 {
        Vec3 {
            x: self.y * other.z - self.z * other.y,
            y: self.z * other.x - self.x * other.z,
            z: self.x * other.y - self.y * other.x,
        }
    }

    fn dot(&self, other: &Vec3) -> f32 {
        self.x * other.x + self.y * other.y + self.z * other.z
    }
}

impl std::ops::Sub for Vec3 {
    type Output = Self;

    fn sub(self, other: Self) -> Self::Output {
        Vec3 {
            x: self.x - other.x,
            y: self.y - other.y,
            z: self.z - other.z,
        }
    }
}

#[derive(Debug, Clone, Copy)]
struct AABB {
    min: Vec3,
    max: Vec3,
}

impl AABB {
    fn new(min: Vec3, max: Vec3) -> AABB {
        AABB { min, max }
    }

    fn center(&self) -> Vec3 {
        Vec3 {
            x: (self.min.x + self.max.x) / 2.0,
            y: (self.min.y + self.max.y) / 2.0,
            z: (self.min.z + self.max.z) / 2.0,
        }
    }

    /// Проверка пересечения двух AABB
    fn intersects(&self, other: &AABB) -> bool {
        self.min.x <= other.max.x
            && self.max.x >= other.min.x
            && self.min.y <= other.max.y
            && self.max.y >= other.min.y
            && self.min.z <= other.max.z
            && self.max.z >= other.min.z
    }

    /// Проверка, содержит ли AABB заданную точку
    fn contains_point(&self, point: &Vec3) -> bool {
        point.x >= self.min.x
            && point.x <= self.max.x
            && point.y >= self.min.y
            && point.y <= self.max.y
            && point.z >= self.min.z
            && point.z <= self.max.z
    }

    /// Проверяет, перекрывает ли первый AABB видимость второго из заданной точки
    fn blocks_view(&self, other: &AABB, viewpoint: &Vec3) -> bool {
        // Проверяем, лежат ли грани первого AABB на пути к другому AABB
        let self_center = Vec3 {
            x: (self.min.x + self.max.x) / 2.0,
            y: (self.min.y + self.max.y) / 2.0,
            z: (self.min.z + self.max.z) / 2.0,
        };

        let other_center = Vec3 {
            x: (other.min.x + other.max.x) / 2.0,
            y: (other.min.y + other.max.y) / 2.0,
            z: (other.min.z + other.max.z) / 2.0,
        };

        // Вектор от точки наблюдения к центру второго AABB
        let direction_to_other = Vec3 {
            x: other_center.x - viewpoint.x,
            y: other_center.y - viewpoint.y,
            z: other_center.z - viewpoint.z,
        };

        // Проверяем, пересекает ли этот вектор первый AABB
        self.ray_intersects(viewpoint, &direction_to_other)
    }

    /// Проверяет, пересекает ли луч, исходящий из заданной точки, AABB
    fn ray_intersects(&self, origin: &Vec3, direction: &Vec3) -> bool {
        let mut tmin = (self.min.x - origin.x) / direction.x;
        let mut tmax = (self.max.x - origin.x) / direction.x;

        if tmin > tmax {
            std::mem::swap(&mut tmin, &mut tmax);
        }

        let mut tymin = (self.min.y - origin.y) / direction.y;
        let mut tymax = (self.max.y - origin.y) / direction.y;

        if tymin > tymax {
            std::mem::swap(&mut tymin, &mut tymax);
        }

        if (tmin > tymax) || (tymin > tmax) {
            return false;
        }

        if tymin > tmin {
            tmin = tymin;
        }

        if tymax < tmax {
            tmax = tymax;
        }

        let mut tzmin = (self.min.z - origin.z) / direction.z;
        let mut tzmax = (self.max.z - origin.z) / direction.z;

        if tzmin > tzmax {
            std::mem::swap(&mut tzmin, &mut tzmax);
        }

        if (tmin > tzmax) || (tzmin > tmax) {
            return false;
        }

        true
    }

    fn distance_to_point(&self, point: &Vec3) -> f32 {
        let clamped_x = point.x.max(self.min.x).min(self.max.x);
        let clamped_y = point.y.max(self.min.y).min(self.max.y);
        let clamped_z = point.z.max(self.min.z).min(self.max.z);

        let dx = point.x - clamped_x;
        let dy = point.y - clamped_y;
        let dz = point.z - clamped_z;

        (dx * dx + dy * dy + dz * dz).sqrt()
    }
}

#[derive(Debug, Clone)]
struct Object {
    name: String,
    faces: Vec<Vec<Vec3>>,
    aabb: AABB,
}

fn parse_obj(filename: &str, grid_size: f32) -> Vec<Object> {
    let file = std::fs::read_to_string(filename).unwrap();

    let mut vertices: Vec<Vec3> = Vec::new();

    let mut objects: Vec<Object> = Vec::new();
    let mut current_object: Option<Object> = None;

    for line in file.lines() {
        let words: Vec<&str> = line.split_whitespace().collect();

        if words.len() == 0 {
            continue;
        }

        if words[0] == "v" {
            vertices.push(Vec3::new(
                words[1].parse::<f32>().unwrap() / grid_size,
                words[2].parse::<f32>().unwrap() / grid_size,
                words[3].parse::<f32>().unwrap() / grid_size,
            ));
        } else if words[0] == "o" {
            if let Some(mut obj) = current_object {
                for face in &obj.faces {
                    for vertex in face {
                        obj.aabb.min.x = obj.aabb.min.x.min(vertex.x);
                        obj.aabb.min.y = obj.aabb.min.y.min(vertex.y);
                        obj.aabb.min.z = obj.aabb.min.z.min(vertex.z);
                        obj.aabb.max.x = obj.aabb.max.x.max(vertex.x);
                        obj.aabb.max.y = obj.aabb.max.y.max(vertex.y);
                        obj.aabb.max.z = obj.aabb.max.z.max(vertex.z);
                    }
                }

                objects.push(obj);
            }

            let name = words[1];

            current_object = Some(Object {
                name: String::from(name),
                faces: Vec::new(),
                aabb: AABB::new(
                    Vec3::new(f32::MAX, f32::MAX, f32::MAX),
                    Vec3::new(f32::MIN, f32::MIN, f32::MIN),
                ),
            });
        } else if words[0] == "f" {
            if let Some(obj) = &mut current_object {
                let mut faces = Vec::new();
                for word in &words[1..] {
                    let v_indices: Vec<&str> = word.split('/').collect();
                    let mut v_index = v_indices[0].parse::<i32>().unwrap() - 1;
                    if v_index < 0 {
                        v_index = vertices.len() as i32 + v_index + 1;
                    }

                    faces.push(vertices[v_index as usize]);
                }
                obj.faces.push(faces);
            }
        }
    }

    if let Some(obj) = current_object {
        objects.push(obj);
    }

    objects
}

/// Проверка, видно ли второй меш за первым из заданной точки
fn check_mesh_visibility(aabb1: AABB, aabb2: AABB, viewpoint: Vec3) -> bool {
    // Если первый AABB блокирует видимость второго, возвращаем false
    if aabb1.blocks_view(&aabb2, &viewpoint) {
        return false;
    }

    true
}

fn bake_pvs(objects: Vec<Object>, cell_size: f32, cell_step: f32) {
    let mut min_point: Vec3 = Vec3::new(f32::MAX, f32::MAX, f32::MAX);
    let mut max_point: Vec3 = Vec3::new(f32::MIN, f32::MIN, f32::MIN);

    for obj in &objects {
        for face in &obj.faces {
            for vertex in face {
                min_point.x = min_point.x.min(vertex.x);
                min_point.y = min_point.y.min(vertex.y);
                min_point.z = min_point.z.min(vertex.z);
                max_point.x = max_point.x.max(vertex.x);
                max_point.y = max_point.y.max(vertex.y);
                max_point.z = max_point.z.max(vertex.z);
            }
        }
    }

    let width_count = ((max_point.x - min_point.x) / cell_size).abs().ceil() as u32;
    let height_count = ((max_point.y - min_point.y) / cell_size).abs().ceil() as u32;
    let depth_count = ((max_point.z - min_point.z) / cell_size).abs().ceil() as u32;

    println!("Min: {:?}, Max: {:?}", min_point, max_point);
    println!(
        "Width: {}, Height: {}, Depth: {}",
        width_count, height_count, depth_count
    );

    let mut pvs = Vec::new();

    for h in 0..height_count {
        let mut pvs_row = Vec::new();
        for w in 0..width_count {
            let mut pvs_col = Vec::new();
            for d in 0..depth_count {
                let x = min_point.x + w as f32 * cell_size;
                let y = min_point.y + h as f32 * cell_size;
                let z = min_point.z + d as f32 * cell_size;

                let center = Vec3::new(30.0f32, 5.0f32, 17.0f32);

                let mut sorted_objects = objects.clone();
                sorted_objects.sort_by(|a, b| {
                    a.aabb
                        .distance_to_point(&center)
                        .partial_cmp(&b.aabb.distance_to_point(&center))
                        .unwrap()
                });

                /*let mut inversed_sorted_objects = Vec::new();
                for obj in sorted_objects.iter().rev() {
                    inversed_sorted_objects.push(obj);
                }*/

                let mut invisible_objects = HashSet::new();
                for obj_i in 0..sorted_objects.len() {
                    /*if invisible_objects.contains(&obj_i) {
                        continue;
                    }*/

                    let mut r = false;
                    for obj_j in 0..sorted_objects.len() {
                        if obj_i == obj_j {
                            continue;
                        }

                        /*if invisible_objects.contains(&obj_j) {
                            continue;
                        }*/

                        let obj = &sorted_objects[obj_i];
                        let other_obj = &sorted_objects[obj_j];

                        let obj_center = obj.aabb.center();
                        let other_obj_center = other_obj.aabb.center();

                        let distance = obj_center.distance_to(&other_obj_center);

                        let point_to_obj_distance = center.distance_to(&obj_center);
                        let point_to_other_obj_distance = center.distance_to(&other_obj_center);
                        let point_gipotenuze = (point_to_obj_distance * point_to_obj_distance
                            + point_to_other_obj_distance * point_to_other_obj_distance)
                            .sqrt();

                        if obj.name == "entity0_brush47" && other_obj.name == "entity0_brush48" {
                            println!("distance: {}, point_to_obj_distance: {}, point_to_other_obj_distance: {}, point_gipotenuze: {}", distance, point_to_obj_distance, point_to_other_obj_distance, point_gipotenuze);
                        }

                        if distance > point_gipotenuze
                            || point_to_other_obj_distance > point_to_obj_distance
                        {
                            continue;
                        }

                        /*if check_mesh_visibility(obj.aabb, other_obj.aabb, center) {
                            invisible_objects.insert(obj_j);
                            break;
                        }*/
                        let points = vec![
                            other_obj.aabb.min,
                            Vec3::new(
                                other_obj.aabb.min.x,
                                other_obj.aabb.min.y,
                                other_obj.aabb.max.z,
                            ),
                            Vec3::new(
                                other_obj.aabb.max.x,
                                other_obj.aabb.min.y,
                                other_obj.aabb.max.z,
                            ),
                            Vec3::new(
                                other_obj.aabb.max.x,
                                other_obj.aabb.min.y,
                                other_obj.aabb.min.z,
                            ),
                            Vec3::new(
                                other_obj.aabb.min.x,
                                other_obj.aabb.max.y,
                                other_obj.aabb.max.z,
                            ),
                            Vec3::new(
                                other_obj.aabb.min.x,
                                other_obj.aabb.max.y,
                                other_obj.aabb.min.z,
                            ),
                            Vec3::new(
                                other_obj.aabb.max.x,
                                other_obj.aabb.max.y,
                                other_obj.aabb.min.z,
                            ),
                            other_obj.aabb.max,
                        ];

                        let mut add = true;
                        for point in points {
                            let direction = (point - center).normalize();

                            add = add && obj.aabb.ray_intersects(&center, &direction);
                        }

                        if add {
                            invisible_objects.insert(obj_j);
                            break;
                        }
                    }

                    /*if invisible_objects.contains(&obj_i) {
                        continue;
                    }

                    let obj = &sorted_objects[obj_i];

                    let mut obj_j = sorted_objects.len() - 1;
                    while obj_j > 0 {
                        if invisible_objects.contains(&obj_j) {
                            obj_j -= 1;
                            continue;
                        }

                        let other_obj = &sorted_objects[obj_j];
                        if !check_mesh_visibility(other_obj.aabb, obj.aabb, center) {
                            invisible_objects.insert(obj_j);
                        }

                        obj_j -= 1;
                    }*/
                }

                for obj in &sorted_objects {}

                let mut visible_objects_names = Vec::new();
                for obj_i in 0..sorted_objects.len() {
                    if invisible_objects.contains(&obj_i) {
                        let obj = &sorted_objects[obj_i];
                        visible_objects_names.push(obj.name.clone());
                        continue;
                    }
                }

                pvs_col.push(visible_objects_names);
            }
            pvs_row.push(pvs_col);
        }
        pvs.push(pvs_row);
    }

    let test_point: Vec3 = Vec3::new(30.0, 5.0, 14.0);

    let cell_h_idx = ((test_point.y - min_point.y) / cell_size).abs().floor() as u32;
    let cell_w_idx = ((test_point.x - min_point.x) / cell_size).abs().floor() as u32;
    let cell_d_idx = ((test_point.z - min_point.z) / cell_size).abs().floor() as u32;

    println!(
        "{}-{}-{}: {}",
        cell_h_idx,
        cell_w_idx,
        cell_d_idx,
        pvs[cell_h_idx as usize][cell_w_idx as usize][cell_d_idx as usize].len()
    );

    let a = &pvs[cell_h_idx as usize][cell_w_idx as usize][cell_d_idx as usize];

    println!("{{");
    for obj in a {
        println!("\"{}\",", obj);
    }
    println!("}}");

    println!("0-0-0: {}", pvs[0][0][0].len());
}










fn is_point_inside_aabb(point: &Vec3, aabb: &AABB) -> bool {
    point.x >= aabb.min.x && point.x <= aabb.max.x &&
    point.y >= aabb.min.y && point.y <= aabb.max.y &&
    point.z >= aabb.min.z && point.z <= aabb.max.z
}

fn is_face_visible(point: &Vec3, face: &[Vec3]) -> bool {
    if face.len() < 3 {
        return false;
    }

    let normal = (face[1] - face[0]).cross(&(face[2] - face[0]));
    let to_point = face[0] - *point;
    normal.dot(&to_point) > 0.0
}

fn is_object_visible(point: &Vec3, object: &Object) -> bool {
    if is_point_inside_aabb(point, &object.aabb) {
        return false;
    }

    for face in &object.faces {
        if is_face_visible(point, face) {
            return true;
        }
    }

    false
}

fn visible_objects(point: Vec3, objects: Vec<Object>) -> Vec<Object> {
    objects
        .into_iter()
        .filter(|object| is_object_visible(&point, object))
        .collect()
}*/
