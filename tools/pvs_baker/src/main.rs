use core::panic;
use std::{collections::HashSet, thread};

use cfor::cfor;
use cgmath::{Matrix, Matrix4, Point3, Vector3};

use glad_gl::gl;

mod obj_loader;
mod render;
mod shader;
mod window;

const WIDTH: u32 = 512;
const HEIGHT: u32 = 512;

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

    unsafe {
        run(map, grid_size, cell_size, cell_step);
    }
}

struct PtrWrapper<T>(pub *const T);

unsafe impl<T> Send for PtrWrapper<T> {}

unsafe fn run(path: String, grid_size: f32, cell_size: f32, cell_step: f32) {
    let (mut glfw, mut window, view_loc) = window::create_window(WIDTH, HEIGHT);
    let (vao, indices_count, colors, obj) = obj_loader::load_obj_vao(path, 16.0);

    let mut point_renderers = vec![
        render::PointRenderer::new(WIDTH, HEIGHT, vao, indices_count as u32, view_loc),
        render::PointRenderer::new(WIDTH, HEIGHT, vao, indices_count as u32, view_loc),
        render::PointRenderer::new(WIDTH, HEIGHT, vao, indices_count as u32, view_loc),
        render::PointRenderer::new(WIDTH, HEIGHT, vao, indices_count as u32, view_loc),
        render::PointRenderer::new(WIDTH, HEIGHT, vao, indices_count as u32, view_loc),
    ];

    let mut min_point = Vector3::new(f32::MAX, f32::MAX, f32::MAX);
    let mut max_point = Vector3::new(f32::MIN, f32::MIN, f32::MIN);
    for obj in &obj {
        cfor!(let mut i = 0; i < obj.mesh.positions.len(); i += 3; {
            let vertex = Vector3::new(obj.mesh.positions[i], obj.mesh.positions[i+1], obj.mesh.positions[i+2]);
            min_point.x = min_point.x.min(vertex.x);
            min_point.y = min_point.y.min(vertex.y);
            min_point.z = min_point.z.min(vertex.z);

            max_point.x = max_point.x.max(vertex.x);
            max_point.y = max_point.y.max(vertex.y);
            max_point.z = max_point.z.max(vertex.z);
        })
    }

    let height_count = ((max_point.y - min_point.y) / cell_size).abs().ceil() as u32;
    let width_count = ((max_point.x - min_point.x) / cell_size).abs().ceil() as u32;
    let depth_count = ((max_point.z - min_point.z) / cell_size).abs().ceil() as u32;

    let pool = threadpool::ThreadPool::new(100);

    let mut prev_time = glfw.get_time();
    for h in 0..height_count {
        for w in 0..width_count {
            for d in 0..depth_count {
                //let pos = Vector3::new(min_point.x + (x as f32 * cell_size), min_point.y + (y as f32 * cell_size), min_point.z + (z as f32 * cell_size));
                //point_renderers[0].render(pos);
                glfw.poll_events();
                let mut center = Point3::new(
                    min_point.x + (w as f32 * cell_size) + (cell_size / 2.0),
                    min_point.y + (h as f32 * cell_size) + (cell_size / 2.0),
                    min_point.z + (d as f32 * cell_size) + (cell_size / 2.0),
                );
                let mut points = vec![center];
                cfor!(let mut x = cell_step; x < cell_size + cell_step; x += cell_step; {
                    cfor!(let mut y = cell_step; y < cell_size + cell_step; y += cell_step; {
                        cfor!(let mut z = cell_step; z < cell_size + cell_step; z += cell_step; {
                            points.push(
                                Point3::new(
                                    min_point.x + (w as f32 * cell_size) + x,
                                    min_point.y + (h as f32 * cell_size) + y,
                                    min_point.z + (d as f32 * cell_size) + z
                                )
                            );
                        })
                    })
                });

                println!("points: {}, elapsed: {}", points.len(), glfw.get_time() - prev_time);
                prev_time = glfw.get_time();

                //let threads = vec![];
                for point in points {
                    'waiter: loop {
                        for renderer in &mut point_renderers {
                            if renderer.is_copy_ready() {
                                renderer.bind_maps();

                                let px = point.x as i32;
                                let py = point.y as i32;
                                let pz = point.z as i32;
                                
                                let thread_ready = renderer.thread_ready.clone();
                                let maps = renderer.maps.clone();
                                pool.execute(move || {
                                    let maps = maps.lock().unwrap();
                                    let mut unique_colors = HashSet::new();
                                    for map in &*maps {
                                        let map_ptr = map.0 as *const (u8, u8, u8);
                                        let arr = unsafe { std::slice::from_raw_parts(map_ptr, (WIDTH * HEIGHT) as usize) };
                                        //println!("map: {}", arr.len());
                                        for a in arr {
                                            unique_colors.insert(a);
                                        }
                                    }

                                    /*let mut string_out = String::new();
                                    for color in unique_colors {
                                        string_out.push_str(&format!("{} {} {}\n", color.0, color.1, color.2));
                                    }

                                    std::fs::write(format!("outs/{}-{}-{}_{}-{}-{}.txt", h, w, d, px, py, pz), string_out).unwrap();*/
                                    
                                    //renderer.copy_to_texture(&point);
                                    *thread_ready.lock().unwrap() = true;

                                });

                                break 'waiter;
                            }
                            
                            else if renderer.is_thread_ready() {
                                //println!("Start");
                                renderer.render(point);
                                break 'waiter;
                            }
                        }
                    }
                }
                //panic!();
            }
        }
    }

    /*// Loop until the user closes the window
    let mut last_time = std::time::Instant::now();
    let mut frame_counter = 0;
    let mut all_elapsed = 0f32;
    //let mut cam_pos = Point3::new(0.0, 0.0, 2.0);
    let mut cam_pos = Point3::new(30.0, 5.0, 17.0);

    let point_renderer = render::PointRenderer::new(WIDTH, HEIGHT, vao, indices_count as u32, view_loc);

    while !window.should_close() {
        /**/


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

        point_renderer.render(cam_pos);

        /*gl::Finish();

        //gl::MapBuffer(gl::PIXEL_PACK_BUFFER, gl::READ_ONLY);
        for pbo in point_renderer.pbos {
            gl::BindBuffer(gl::PIXEL_PACK_BUFFER, pbo);
            let data = gl::MapBuffer(gl::PIXEL_PACK_BUFFER, gl::READ_ONLY);
            let data_arr =
                std::slice::from_raw_parts(data as *const u8, WIDTH as usize * HEIGHT as usize * 3);
            let buffer = data_arr.to_vec();

            let image: image::ImageBuffer<image::Rgb<u8>, Vec<u8>> =
                image::ImageBuffer::from_raw(WIDTH, HEIGHT, buffer).unwrap();
            let image = image::DynamicImage::ImageRgb8(image);
            image.save(format!("{}.png", pbo)).unwrap();
        }
        panic!();*/


        /*let mut unique_colors = std::collections::HashSet::new();
        for i in 0..buffer.len() {
            unique_colors.insert(buffer[i]);
        }
        for color in unique_colors {
            //println!("color: ({}, {}, {})", color.0, color.1, color.2);
            //println!("color: ({}, {}, {})", color.0, color.1, color.2);
            if let Some(color_idx) = colors.iter().position(|&x| {
                x[0] == color.0 as u32 && x[1] == color.1 as u32 && x[2] == color.2 as u32
            }) {

                //println!("color_idx: {}", color_idx);
                //println!("\"{}\",", obj[color_idx].name);
            }
        }*/
        glfw.poll_events();
    }*/
}

/*use std::collections::HashSet;

use cfor::cfor;




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
*/
