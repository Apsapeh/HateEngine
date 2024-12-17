use glad_gl::gl;
use std::ffi::CString;

const VERTEX_SHADER_SOURCE: &str = r#"
#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in uvec3 aColor;

out vec3 color;

uniform mat4 projection;
uniform mat4 view;

void main() {
    gl_Position = projection * view * vec4(aPos, 1.0);
    color = vec3(float(aColor.x) / 255.0, float(aColor.y) / 255.0, float(aColor.z) / 255.0);
}
"#;

const FRAGMENT_SHADER_SOURCE: &str = r#"
#version 330 core
in vec3 color;
out vec4 FragColor;


void main() {
    FragColor = vec4(color.x, color.y, color.z, 1.0);
    //FragColor = vec4(0.5, 0.5, 0.5, 1.0);
}
"#;

pub unsafe fn load_shader_program() -> u32 {
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

    shader_program
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
            eprintln!(
                "Shader compilation error: {}",
                String::from_utf8_lossy(&buffer)
            );
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
            eprintln!(
                "Program linking error: {}",
                String::from_utf8_lossy(&buffer)
            );
        }
    }
}
