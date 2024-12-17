use crate::shader;
use cgmath::{perspective, Deg, Matrix};
use glad_gl::gl;
use glfw::{fail_on_errors, Context};
use std::ffi::CString;

pub unsafe fn create_window(width: u32, height: u32) -> (glfw::Glfw, glfw::PWindow, i32) {
    let mut glfw = glfw::init(fail_on_errors!()).unwrap();

    glfw.window_hint(glfw::WindowHint::ContextVersion(3, 3));
    glfw.window_hint(glfw::WindowHint::OpenGlProfile(
        glfw::OpenGlProfileHint::Core,
    ));
    glfw.window_hint(glfw::WindowHint::OpenGlForwardCompat(true));

    // Create a windowed mode window and its OpenGL context
    let (mut window, _) = glfw
        .create_window(
            width,
            height,
            "Hello this is window",
            glfw::WindowMode::Windowed,
        )
        .expect("Failed to create GLFW window.");

    // Make the window's context current
    window.make_current();
    window.set_key_polling(false);
    glfw.set_swap_interval(glfw::SwapInterval::None);

    gl::load(|s| window.get_proc_address(s) as *const _);

    let shader_program = shader::load_shader_program();

    let name = CString::new("projection").unwrap();
    let projection_loc = gl::GetUniformLocation(shader_program, name.as_ptr());
    let name = CString::new("view").unwrap();
    let view_loc = gl::GetUniformLocation(shader_program, name.as_ptr());

    gl::Enable(gl::DEPTH_TEST);
    gl::Enable(gl::CULL_FACE);
    gl::FrontFace(gl::CCW);
    gl::Viewport(0, 0, width as i32, height as i32);
    gl::ClearColor(1.0, 1.0, 1.0, 1.0);

    let projection = perspective(Deg(90.0), 1.0, 0.1, 10000.0);
    gl::UseProgram(shader_program);
    gl::UniformMatrix4fv(projection_loc, 1, gl::FALSE, projection.as_ptr());

    (glfw, window, view_loc)
}
