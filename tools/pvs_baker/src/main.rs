use std::{
    collections::HashMap,
    sync::{Arc, Mutex},
};

use cfor::cfor;
use cgmath::{Point3, Vector3};
use std::io::Write;

mod obj_loader;
mod render;
mod shader;
mod window;

const WIDTH: u32 = 512;
const HEIGHT: u32 = 512;

const USAGE_EXAMPLE_ERROR: &str = "Usage: ./pvs_baker -m <.obj map> [-o <output>] [-g <grid size>] [-c <cell size>] [-s <cell step>] [--norender]";

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
    let mut no_render = false;
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
        else if arg == "--norender" {
            no_render = true;
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
        run(no_render, map, grid_size, cell_size, cell_step, output);
    }
}

unsafe fn run(
    no_render: bool,
    path: String,
    grid_size: f32,
    cell_size: f32,
    cell_step: f32,
    out: String,
) {
    let (mut glfw, mut _window, view_loc) = window::create_window(WIDTH, HEIGHT);
    let (vao, indices_count, colors, obj) = obj_loader::load_obj_vao(path.clone(), grid_size);

    let colors_arc = Arc::new(colors);
    let objects = colors_arc
        .iter()
        .map(|(_, color)| color.clone())
        .collect::<Vec<String>>();

    // create cache dir if not exists
    let filename = path.split("/").last().unwrap();
    let filename = filename.split(".").next().unwrap();
    let dirpath = format!(".hepvs/{}", filename);
    std::fs::create_dir_all(&dirpath).unwrap();

    /*
    let mut pos = Point3::new(34.0, 5.0, 17.0);
    let mut prev_time = glfw.get_time();
    while !window.should_close() {
        let current_time = glfw.get_time();
        let elapsed = current_time - prev_time;
        prev_time = current_time;
        let view = render::get_view(pos, Point3::new(0.0, 0.0, 1.0), Vector3::new(0.0, 1.0, 0.0));
        gl::UniformMatrix4fv(view_loc, 1, gl::FALSE, view.as_ptr());

        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

        gl::ClearColor(0.0, 0.0, 0.0, 1.0);

        gl::BindVertexArray(vao);
        gl::DrawElements(gl::TRIANGLES, indices_count as i32, gl::UNSIGNED_INT, 0 as *const _);

        gl::BindVertexArray(0);
        window.swap_buffers();
        glfw.poll_events();

        if window.get_key(Key::Right) == Action::Press {
            pos.x += 10.0 * elapsed as f32;
        }
        if window.get_key(Key::Left) == Action::Press {
            pos.x -= 10.0 * elapsed as f32;
        }
        if window.get_key(Key::Up) == Action::Press {
            pos.z -= 10.0 * elapsed as f32;
        }
        if window.get_key(Key::Down) == Action::Press {
            pos.z += 10.0 * elapsed as f32;
        }
        if window.get_key(Key::Space) == Action::Press {
            pos.y += 10.0 * elapsed as f32;
        }
        if window.get_key(Key::LeftShift) == Action::Press {
            pos.y -= 10.0 * elapsed as f32;
        }
    }*/

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

    let pos = get_grid_hwd_by_pos(Vector3::new(30.0, 5.0, -25.0), min_point, cell_size);
    println!("{} {} {}", pos.0, pos.1, pos.2);
    //panic!();

    if !no_render {
        render_obj(
            height_count,
            width_count,
            depth_count,
            cell_size,
            cell_step,
            min_point,
            &mut glfw,
            vao,
            indices_count as u32,
            view_loc,
            &dirpath,
            colors_arc,
        );
    }

    println!("Rendering done, packing...");

    pack_hepvs(
        height_count,
        width_count,
        depth_count,
        cell_size,
        min_point.x,
        min_point.y,
        min_point.z,
        objects,
        dirpath,
        out,
    );
}

fn render_obj(
    height_count: u32,
    width_count: u32,
    depth_count: u32,
    cell_size: f32,
    cell_step: f32,
    min_point: Vector3<f32>,
    glfw: &mut glfw::Glfw,
    vao: u32,
    indices_count: u32,
    view_loc: i32,
    dirpath: &String,
    colors_arc: Arc<HashMap<(u8, u8, u8), String>>,
) {
    let mut point_renderers = vec![
        render::PointRenderer::new(WIDTH, HEIGHT, vao, indices_count as u32, view_loc),
        render::PointRenderer::new(WIDTH, HEIGHT, vao, indices_count as u32, view_loc),
        render::PointRenderer::new(WIDTH, HEIGHT, vao, indices_count as u32, view_loc),
        render::PointRenderer::new(WIDTH, HEIGHT, vao, indices_count as u32, view_loc),
        render::PointRenderer::new(WIDTH, HEIGHT, vao, indices_count as u32, view_loc),
    ];
    let pool = threadpool::ThreadPool::new(100);

    let mut prev_time = glfw.get_time();
    let mut progress_counter = 0;
    for h in 0..height_count {
        for w in 0..width_count {
            for d in 0..depth_count {
                //let pos = Vector3::new(min_point.x + (x as f32 * cell_size), min_point.y + (y as f32 * cell_size), min_point.z + (z as f32 * cell_size));
                //point_renderers[0].render(pos);
                glfw.poll_events();
                let center = Point3::new(
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

                let points_result = Arc::new(Mutex::new(vec![vec![vec![false; 16]; 16]; 16]));
                for point in points {
                    'waiter: loop {
                        for renderer in &mut point_renderers {
                            if renderer.is_copy_ready() {
                                renderer.bind_maps();

                                let thread_ready = renderer.thread_ready.clone();
                                let maps = renderer.maps.clone();
                                let points_result = points_result.clone();
                                pool.execute(move || {
                                    let maps = maps.lock().unwrap();
                                    //let mut unique_colors = HashSet::new();
                                    let mut uinque_colors: Vec<Vec<Vec<bool>>> = vec![vec![vec![false; 16]; 16]; 16];
                                    for map in &*maps {
                                        let map_ptr = map.0 as *const (u8, u8, u8);
                                        let arr = unsafe { std::slice::from_raw_parts(map_ptr, (WIDTH * HEIGHT) as usize) };
                                        //println!("map: {}", arr.len());
                                        for a in arr {
                                            //let n: u32 = (a.0 as u32) << 16 | (a.1 as u32) << 8 | a.2 as u32;
                                            let r = (a.0 >> 4) as usize;
                                            let g = (a.1 >> 4) as usize;
                                            let b = (a.2 >> 4) as usize;
                                            uinque_colors[r][g][b] = true;
                                            //unique_colors.insert(n);
                                        }
                                    }

                                    let mut points_result = points_result.lock().unwrap();
                                    cfor!(let mut x = 0; x < 16; x += 1; {
                                        cfor!(let mut y = 0; y < 16; y += 1; {
                                            cfor!(let mut z = 0; z < 16; z += 1; {
                                                points_result[x][y][z] = points_result[x][y][z] || uinque_colors[x][y][z];
                                            })
                                        })
                                    });

                                    /*let mut string_out = String::new();
                                    for color in unique_colors {
                                        string_out.push_str(&format!("{} {} {}\n", color.0, color.1, color.2));
                                    }

                                    std::fs::write(format!("outs/{}-{}-{}_{}-{}-{}.txt", h, w, d, px, py, pz), string_out).unwrap();*/
                                                                        //renderer.copy_to_texture(&point);
                                    *thread_ready.lock().unwrap() = true;

                                });

                                break 'waiter;
                            } else if renderer.is_thread_ready() {
                                //println!("Start");
                                renderer.render(point);
                                break 'waiter;
                            }
                        }
                    }
                }
                pool.join();

                let dirpath_ref = dirpath.clone();
                let colors_arc = colors_arc.clone();
                pool.execute(move || {
                    let points_result = points_result.lock().unwrap();
                    let mut string_out = String::new();
                    cfor!(let mut x = 0; x < 16; x += 1; {
                        cfor!(let mut y = 0; y < 16; y += 1; {
                            cfor!(let mut z = 0; z < 16; z += 1; {
                                if points_result[x][y][z] {
                                    //string_out.push_str(&format!("{} {} {}\n", x, y, z));
                                    if let Some(color) = colors_arc.get(&(x as u8, y as u8, z as u8)) {
                                        string_out.push_str(&format!("{}\n", color));
                                    }
                                }
                            })
                        })
                    });

                    std::fs::write(format!("{}/{}-{}-{}.txt", dirpath_ref, h, w, d), string_out).unwrap();
                });
                //panic!();

                progress_counter += 1;
                println!(
                    "[{}/{}] Elapsed: {}",
                    progress_counter,
                    height_count * width_count * depth_count,
                    glfw.get_time() - prev_time
                );
                prev_time = glfw.get_time();
            }
        }
    }

    pool.join();
}

fn get_grid_hwd_by_pos(
    pos: Vector3<f32>,
    min_point: Vector3<f32>,
    cell_size: f32,
) -> (u32, u32, u32) {
    let h = ((pos.y - min_point.y) / cell_size) as u32;
    let w = ((pos.x - min_point.x) / cell_size) as u32;
    let d = ((pos.z - min_point.z) / cell_size) as u32;
    (h, w, d)
}

fn pack_hepvs(
    h: u32,
    w: u32,
    d: u32,
    cell_size: f32,
    min_x: f32,
    min_y: f32,
    min_z: f32,
    objects: Vec<String>,
    folder: String,
    out: String,
) {
    let mut bin = vec![];
    let version = 1u32;

    bin.extend_from_slice(&version.to_le_bytes());
    bin.extend_from_slice(&min_x.to_le_bytes());
    bin.extend_from_slice(&min_y.to_le_bytes());
    bin.extend_from_slice(&min_z.to_le_bytes());
    bin.extend_from_slice(&cell_size.to_le_bytes());
    bin.extend_from_slice(&(w as u16).to_le_bytes());
    bin.extend_from_slice(&(h as u16).to_le_bytes());
    bin.extend_from_slice(&(d as u16).to_le_bytes());
    bin.extend_from_slice(&(objects.len() as u16).to_le_bytes());

    for object in &objects {
        bin.extend_from_slice(&(object.len() as u16).to_le_bytes());
        bin.extend_from_slice(&object.as_bytes());
    }

    for x in 0..w {
        for y in 0..h {
            for z in 0..d {
                let point = format!("{}/{}-{}-{}.txt", folder, y, x, z);
                let point_str = std::fs::read_to_string(point).unwrap().trim().to_string();
                let point_str = point_str.split("\n").collect::<Vec<&str>>();
                bin.extend_from_slice(&(point_str.len() as u16).to_le_bytes());

                for line in point_str {
                    let idx = objects.iter().position(|x| x == line).unwrap();
                    bin.extend_from_slice(&(idx as u16).to_le_bytes());
                }
            }
        }
    }

    let mut out = std::fs::File::create(out).unwrap();
    out.write_all(&bin).unwrap();
}
