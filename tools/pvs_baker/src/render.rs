use std::{ptr, thread};
use std::sync::{Arc, Mutex, RwLock};

use cgmath::{Matrix, Matrix4, Point3, Vector3};
use glad_gl::gl;

pub struct PointRenderer {
    width: u32,
    height: u32,
    pub fbos: Vec<u32>,
    pub pbos: Vec<u32>,
    pub vao: u32,
    pub indices_count: u32,
    pub view_loc: i32,
    pub sync: gl::types::GLsync,
}

impl PointRenderer {
    pub fn new(width: u32, height: u32, vao: u32, indices_count: u32, view_loc: i32) -> Self {
        let fbos = unsafe { gen_fbos(6, width, height) };
        let pbos = unsafe { gen_pbos(6, width, height) };
        PointRenderer {
            width,
            height,
            fbos,
            pbos,
            vao,
            indices_count,
            view_loc,
            sync: 0 as gl::types::GLsync,
        }
    }

    pub fn render(&mut self, pos: Point3<f32>) {
        unsafe {
            if self.sync != 0 as gl::types::GLsync {
                gl::DeleteSync(self.sync);
                self.sync = 0 as gl::types::GLsync;
            }

            let view = get_view(pos, Point3::new(0.0, 0.0, 1.0), Vector3::new(0.0, 1.0, 0.0));
            gl::UniformMatrix4fv(self.view_loc, 1, gl::FALSE, view.as_ptr());
            self.render_to_fbo_async_copy_to_pbo(
                self.fbos[0],
                self.pbos[0],
                self.vao,
                self.indices_count as i32,
            );

            let view = get_view(
                pos,
                Point3::new(0.0, 0.0, -1.0),
                Vector3::new(0.0, 1.0, 0.0),
            );
            gl::UniformMatrix4fv(self.view_loc, 1, gl::FALSE, view.as_ptr());
            self.render_to_fbo_async_copy_to_pbo(
                self.fbos[1],
                self.pbos[1],
                self.vao,
                self.indices_count as i32,
            );

            let view = get_view(pos, Point3::new(0.0, 1.0, 0.0), Vector3::new(0.0, 0.0, 1.0));
            gl::UniformMatrix4fv(self.view_loc, 1, gl::FALSE, view.as_ptr());
            self.render_to_fbo_async_copy_to_pbo(
                self.fbos[2],
                self.pbos[2],
                self.vao,
                self.indices_count as i32,
            );

            let view = get_view(
                pos,
                Point3::new(0.0, -1.0, 0.0),
                Vector3::new(0.0, 0.0, 1.0),
            );
            gl::UniformMatrix4fv(self.view_loc, 1, gl::FALSE, view.as_ptr());
            self.render_to_fbo_async_copy_to_pbo(
                self.fbos[3],
                self.pbos[3],
                self.vao,
                self.indices_count as i32,
            );

            let view = get_view(pos, Point3::new(1.0, 0.0, 0.0), Vector3::new(0.0, 1.0, 0.0));
            gl::UniformMatrix4fv(self.view_loc, 1, gl::FALSE, view.as_ptr());
            self.render_to_fbo_async_copy_to_pbo(
                self.fbos[4],
                self.pbos[4],
                self.vao,
                self.indices_count as i32,
            );

            let view = get_view(
                pos,
                Point3::new(-1.0, 0.0, 0.0),
                Vector3::new(0.0, 1.0, 0.0),
            );
            gl::UniformMatrix4fv(self.view_loc, 1, gl::FALSE, view.as_ptr());
            self.render_to_fbo_async_copy_to_pbo(
                self.fbos[5],
                self.pbos[5],
                self.vao,
                self.indices_count as i32,
            );

            let sync = gl::FenceSync(gl::SYNC_GPU_COMMANDS_COMPLETE, 0);
            self.sync = sync;
            gl::Flush();
        }
    }

    pub fn is_ready(&mut self) -> bool {
        unsafe {
            if self.sync == 0 as gl::types::GLsync {
                return true;
            } else {
                if gl::ClientWaitSync(self.sync, 0, 0) == gl::ALREADY_SIGNALED {
                    //println!("Render is ready!");
                    return true;
                } else {
                    //println!("Render is not ready!");
                    return false;
                }
                //return true;
            }
        }
    }

    #[inline]
    unsafe fn render_to_fbo_async_copy_to_pbo(&self, fbo: u32, pbo: u32, vao: u32, count: i32) {
        gl::BindFramebuffer(gl::FRAMEBUFFER, fbo);

        gl::Clear(gl::COLOR_BUFFER_BIT | gl::DEPTH_BUFFER_BIT);

        gl::BindVertexArray(vao);
        gl::DrawElements(gl::TRIANGLES, count, gl::UNSIGNED_INT, 0 as *const _);

        gl::BindVertexArray(0);

        gl::BindBuffer(gl::PIXEL_PACK_BUFFER, pbo);
        gl::ReadPixels(
            0,
            0,
            self.width as i32,
            self.height as i32,
            gl::RGB,
            gl::UNSIGNED_BYTE,
            0 as *mut _,
        );

        gl::BindBuffer(gl::PIXEL_PACK_BUFFER, 0);
    }
}

unsafe fn gen_fbo(width: u32, height: u32) -> u32 {
    let mut fbo: u32 = 0;

    gl::GenFramebuffers(1, &mut fbo);
    gl::BindFramebuffer(gl::FRAMEBUFFER, fbo);

    gl::Enable(gl::DEPTH_TEST);

    let mut texture: u32 = 0;
    gl::GenTextures(1, &mut texture);
    gl::BindTexture(gl::TEXTURE_2D, texture);
    gl::TexImage2D(
        gl::TEXTURE_2D,
        0,
        gl::RGB as i32,
        width as i32,
        height as i32,
        0,
        gl::RGB,
        gl::UNSIGNED_BYTE,
        std::ptr::null(),
    );
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MIN_FILTER, gl::NEAREST as i32);
    gl::TexParameteri(gl::TEXTURE_2D, gl::TEXTURE_MAG_FILTER, gl::NEAREST as i32);
    gl::FramebufferTexture2D(
        gl::FRAMEBUFFER,
        gl::COLOR_ATTACHMENT0,
        gl::TEXTURE_2D,
        texture,
        0,
    );

    let mut rbo: u32 = 0;
    gl::GenRenderbuffers(1, &mut rbo);
    gl::BindRenderbuffer(gl::RENDERBUFFER, rbo);
    gl::RenderbufferStorage(
        gl::RENDERBUFFER,
        gl::DEPTH24_STENCIL8,
        width as i32,
        height as i32,
    );
    gl::FramebufferRenderbuffer(
        gl::FRAMEBUFFER,
        gl::DEPTH_STENCIL_ATTACHMENT,
        gl::RENDERBUFFER,
        rbo,
    );

    if gl::CheckFramebufferStatus(gl::FRAMEBUFFER) != gl::FRAMEBUFFER_COMPLETE {
        panic!("Framebuffer creation failed");
    }

    gl::BindFramebuffer(gl::FRAMEBUFFER, 0);
    gl::BindTexture(gl::TEXTURE_2D, 0);

    fbo
}

unsafe fn gen_fbos(count: u32, width: u32, height: u32) -> Vec<u32> {
    let mut fbos = Vec::new();

    for _ in 0..count {
        fbos.push(gen_fbo(width, height));
    }

    fbos
}

unsafe fn gen_pbos(count: u32, width: u32, height: u32) -> Vec<u32> {
    let mut pbos = Vec::new();

    for _ in 0..count {
        let mut pbo: u32 = 0;
        gl::GenBuffers(1, &mut pbo);
        pbos.push(pbo);
        gl::BindBuffer(gl::PIXEL_PACK_BUFFER, pbo);
        gl::BufferData(
            gl::PIXEL_PACK_BUFFER,
            (width * height * 3) as isize,
            std::ptr::null(),
            gl::STREAM_READ,
        );
        gl::BindBuffer(gl::PIXEL_PACK_BUFFER, 0);
    }
    pbos
}

fn get_view(pos: Point3<f32>, dir: Point3<f32>, up: Vector3<f32>) -> Matrix4<f32> {
    let mut eye = pos;
    eye.x += dir.x;
    eye.y += dir.y;
    eye.z += dir.z;
    Matrix4::look_at_rh(eye, pos, up)
}
