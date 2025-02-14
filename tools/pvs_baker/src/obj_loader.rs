use std::collections::HashMap;

use glad_gl::gl;
use tobj;

pub unsafe fn load_obj_vao(
    path: String,
    grid_size: f32,
) -> (u32, usize, HashMap<(u8, u8, u8), String>, Vec<tobj::Model>) {
    let mut obj = tobj::load_obj(path, &tobj::GPU_LOAD_OPTIONS).unwrap().0;

    for o in &mut obj {
        for v in &mut o.mesh.positions {
            *v /= grid_size;
        }
    }

    let mut big_vbo = Vec::new();
    let mut big_ebo = Vec::new();
    let mut big_color: Vec<f32> = Vec::new();
    let mut colors = HashMap::new();

    for o in &mut obj {
        for i in &mut o.mesh.indices {
            *i += big_vbo.len() as u32 / 3;
        }
        big_ebo.extend_from_slice(&o.mesh.indices);
        big_vbo.extend_from_slice(&o.mesh.positions);

        let color = gen_color();
        for _ in 0..o.mesh.positions.len() / 3 {
            big_color.push(color[0] as f32 / 255.0);
            big_color.push(color[1] as f32 / 255.0);
            big_color.push(color[2] as f32 / 255.0);
        }

        let color = (color[0] >> 4, color[1] >> 4, color[2] >> 4);
        colors.insert(color, o.name.clone());
    }
    //panic!();

    let mut vbo = 0;
    gl::GenBuffers(1, &mut vbo);

    let mut vao = 0;
    gl::GenVertexArrays(1, &mut vao);
    gl::BindVertexArray(vao);

    gl::BindBuffer(gl::ARRAY_BUFFER, vbo);
    gl::BufferData(
        gl::ARRAY_BUFFER,
        (big_vbo.len() * std::mem::size_of::<f32>()) as isize,
        big_vbo.as_ptr() as *const _,
        gl::STATIC_DRAW,
    );

    let mut ebo = 0;
    gl::GenBuffers(1, &mut ebo);
    gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, ebo);
    gl::BufferData(
        gl::ELEMENT_ARRAY_BUFFER,
        (big_ebo.len() * std::mem::size_of::<u32>()) as isize,
        big_ebo.as_ptr() as *const _,
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

    let mut color_vbo = 0;
    gl::GenBuffers(1, &mut color_vbo);
    gl::BindBuffer(gl::ARRAY_BUFFER, color_vbo);
    gl::BufferData(
        gl::ARRAY_BUFFER,
        (big_color.len() * std::mem::size_of::<f32>()) as isize,
        big_color.as_ptr() as *const _,
        gl::STATIC_DRAW,
    );
    gl::VertexAttribPointer(1, 3, gl::FLOAT, gl::FALSE, 0, std::ptr::null());

    gl::EnableVertexAttribArray(1);
    gl::EnableVertexAttribArray(0);

    gl::BindVertexArray(0);
    gl::BindBuffer(gl::ARRAY_BUFFER, 0);
    gl::BindBuffer(gl::ELEMENT_ARRAY_BUFFER, 0);

    (vao, big_ebo.len(), colors, obj)
}

static mut COLOR_R: u8 = 0;
static mut COLOR_G: u8 = 0;
static mut COLOR_B: u8 = 0;

const COLOR_INCREMENT: u8 = 16; // 2^4 for optimized division, 4096 colors

fn gen_color() -> [u8; 3] {
    unsafe {
        let r = [COLOR_R, COLOR_G, COLOR_B];

        if COLOR_B > 255 - COLOR_INCREMENT {
            COLOR_B = 0;

            if COLOR_G > 255 - COLOR_INCREMENT {
                COLOR_G = 0;
                if COLOR_R > 255 - COLOR_INCREMENT {
                    COLOR_R = 0;
                } else {
                    COLOR_R += COLOR_INCREMENT;
                }
            } else {
                COLOR_G += COLOR_INCREMENT;
            }
        } else {
            COLOR_B += COLOR_INCREMENT;
        }

        r
    }
}
