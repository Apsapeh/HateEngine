use std::os::raw::c_void;
use std::{ptr, thread};
use std::sync::{Arc, Mutex, RwLock};

use cgmath::{Matrix, Matrix4, Point3, Vector3};
use glad_gl::gl;

#[derive(Copy, Clone)]
pub struct PtrWrapper<T> (pub *const T);

unsafe impl<T> Send for PtrWrapper<T> {}
unsafe impl<T> Sync for PtrWrapper<T> {}


pub struct PointRenderer {
    width: u32,
    height: u32,
    pub fbos: Vec<u32>,
    pub pbos: Vec<u32>,
    pub vao: u32,
    pub indices_count: u32,
    pub view_loc: i32,
    pub sync: gl::types::GLsync,
    pub maps: Arc<Mutex<[PtrWrapper<c_void>; 6]>>,
    pub is_maps_binded: bool,
    pub thread_ready: Arc<Mutex<bool>>,
}

impl PointRenderer {
    pub fn new(width: u32, height: u32, vao: u32, indices_count: u32, view_loc: i32) -> Self {
        let fbos = unsafe { gen_fbos(6, width, height) };
        let pbos = unsafe { gen_pbos(6, width, height) };
        let maps: [PtrWrapper<c_void>; 6] = [
            PtrWrapper(ptr::null_mut()),
            PtrWrapper(ptr::null_mut()),
            PtrWrapper(ptr::null_mut()),
            PtrWrapper(ptr::null_mut()),
            PtrWrapper(ptr::null_mut()),
            PtrWrapper(ptr::null_mut()),
        ];
        PointRenderer {
            width,
            height,
            fbos,
            pbos,
            vao,
            indices_count,
            view_loc,
            sync: 0 as gl::types::GLsync,
            is_maps_binded: false,
            maps: Arc::new(Mutex::new(maps)),
            thread_ready: Arc::new(Mutex::new(true)),
        }
    }

    pub fn render(&mut self, pos: Point3<f32>) {
        unsafe {
            
            *self.thread_ready.lock().unwrap() = false;

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

    pub fn bind_maps(&mut self) {
        unsafe {
            for i in 0..6 {
                gl::BindBuffer(gl::PIXEL_PACK_BUFFER, self.pbos[i]);
                let data = gl::MapBuffer(gl::PIXEL_PACK_BUFFER, gl::READ_ONLY);
                if data.is_null() {
                    panic!("data is null {i}");
                }
                self.maps.lock().unwrap()[i].0 = data;
            }
        }
        self.is_maps_binded = true;
    }

    pub fn unbind_maps(& self) {
        //println!("Unbind maps");
        unsafe {
            for i in 0..6 {
                gl::BindBuffer(gl::PIXEL_PACK_BUFFER, self.pbos[i]);
                gl::UnmapBuffer(gl::PIXEL_PACK_BUFFER);
            }
            gl::BindBuffer(gl::PIXEL_PACK_BUFFER, 0);
        }
    }

    pub fn is_copy_ready(&mut self) -> bool {
        unsafe {
            // Render is not started
            if self.sync == 0 as gl::types::GLsync {
                return false;
            } else {
                // Render is done
                if gl::ClientWaitSync(self.sync, 0, 0) == gl::ALREADY_SIGNALED {
                    //println!("Render done");
                    if self.sync != 0 as gl::types::GLsync {
                        gl::DeleteSync(self.sync);
                        self.sync = 0 as gl::types::GLsync;
                    }
                    return true;
                // Render is not done
                } else {
                    return false;
                }
            }
        }
    }

    pub fn is_thread_ready(&mut self) -> bool {
            let state = self.thread_ready.lock().unwrap();
            if *state {
                if self.is_maps_binded {
                    self.unbind_maps();
                    self.is_maps_binded = false;
                }
                return true;
            } else {
                return false;
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
