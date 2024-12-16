

use cfor::cfor;
use cgmath::{Matrix, Matrix4, Point3, Vector3};

use glad_gl::gl;
use glfw::{Context, Key, Action};

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
                                    //let mut unique_colors = HashSet::new();
                                    let mut uinque_colors: Vec<Vec<Vec<u32>>> = vec![vec![vec![0; 16]; 16]; 16];
                                    for map in &*maps {
                                        let map_ptr = map.0 as *const (u8, u8, u8);
                                        let arr = unsafe { std::slice::from_raw_parts(map_ptr, (WIDTH * HEIGHT) as usize) };
                                        //println!("map: {}", arr.len());
                                        for a in arr {
                                            //let n: u32 = (a.0 as u32) << 16 | (a.1 as u32) << 8 | a.2 as u32;
                                            let r = (a.0 >> 4) as usize;
                                            let g = (a.1 >> 4) as usize;
                                            let b = (a.2 >> 4) as usize;
                                            uinque_colors[r][g][b] += 1;
                                            //unique_colors.insert(n);
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
}
