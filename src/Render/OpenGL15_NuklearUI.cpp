//
// Created by Тимофей Кириченко on 12.05.2024.
//

#include <HateEngine/Render/OpenGL15.hpp>
#include <climits>
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <glm/ext.hpp>
#include <string>
#include "HateEngine/UI/CoordsUI.hpp"
#include "HateEngine/UI/ObjectUI.hpp"
#include "glm/ext/matrix_clip_space.hpp"
#include "glm/ext/vector_int2.hpp"

#include <HateEngine/UI/WidgetUI.hpp>
#include <HateEngine/UI/LabelUI.hpp>

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT

#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#include <nuklear.h>

using namespace HateEngine;


static void device_upload_atlas(const void *image, int width, int height);
static void draw(int width, int height);

#define MAX_MEMORY 512 * 1024
//#define MAX_ELEMENT_MEMORY 128 * 1024

nk_context ctx;
nk_font_atlas atlas;

unsigned int font_tex = 0;
nk_draw_null_texture tex_null;

struct your_vertex
{
    float pos[2]; // important to keep it to 2 floats
    float uv[2];
    unsigned char col[4];
};

void OpenGL15::initNuklearUI() {
    const void *image; int w, h;
    struct nk_font *font;

    struct nk_font_config config = nk_font_config(18);
    config.oversample_h = 1;
    config.oversample_v = 1;
    config.range = nk_font_cyrillic_glyph_ranges();


    nk_font_atlas_init_default(&atlas);
    nk_font_atlas_begin(&atlas);
    //font = nk_font_atlas_add_default(&atlas, 18, &config);
    //font = nk_font_atlas_add_from_file(&atlas, "/Users/ghost/Desktop/C++ Projects/Projects/Nuklear_GLFW3_GL1_Driver/OpenSans-Regular.ttf", 18, &config);
    font = nk_font_atlas_add_from_file(&atlas, "/Users/ghost/CLionProjects/HateEngine/examples/Assets/Comfortaa-Regular.ttf", 18, &config);
    image = nk_font_atlas_bake(&atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
    device_upload_atlas(image, w, h);
    nk_font_atlas_end(&atlas, nk_handle_id((int)font_tex), &tex_null);
    nk_init_default(&ctx, &font->handle);
}


static void device_upload_atlas(const void *image, int width, int height)
{
    glGenTextures(1, &font_tex);
    glBindTexture(GL_TEXTURE_2D, font_tex);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei)width, (GLsizei)height, 0,
                GL_RGBA, GL_UNSIGNED_BYTE, image);
}



void OpenGL15::DrawNuklearUI(std::unordered_map<UUID, Level::SceneUIWidget>* widgets) {
    /*if (nk_begin(&ctx, "Show", nk_rect(0, 0, 220, 220),
    NK_WINDOW_NOT_INTERACTIVE|NK_WINDOW_BORDER)) {
        ctx.style.window.fixed_background = nk_style_item_color(nk_rgba(0, 0, 0, 0));
        ctx.style.text.color = nk_rgb(255, 0, 0);
        nk_layout_row_static(&ctx, 30, 80, 1);
        nk_label(&ctx, ("FPS: " + std::to_string(glfwGetTime())).c_str(), NK_TEXT_LEFT);

    }
    nk_end(&ctx);*/
    for (const auto& it : *widgets) {
        const WidgetUI* widget = it.second.obj;
        glm::ivec2 res = engine->getResolution();
        CoordsUI::CoordsData position = widget->position.getCoords(res.x, res.y);
        CoordsUI::CoordsData size = widget->size.getTopLeftCoords(res.x, res.y);
        nk_flags widget_flags = (widget->has_border ? NK_WINDOW_BORDER : 0)
                        | (widget->is_movable ? NK_WINDOW_MOVABLE : 0)
                        | (widget->is_scalable ? NK_WINDOW_SCALABLE : 0)
                        | (widget->is_closable ? NK_WINDOW_CLOSABLE : 0)
                        | (widget->is_minimizable ? NK_WINDOW_MINIMIZABLE : 0)
                        | (!widget->has_scrollbar ? NK_WINDOW_NO_SCROLLBAR : 0)
                        | (widget->has_title ? NK_WINDOW_TITLE : 0)
                        | (widget->is_scroll_autohide ? NK_WINDOW_SCROLL_AUTO_HIDE : 0)
                        | (widget->has_background ? NK_WINDOW_BACKGROUND : 0)
                        | (widget->is_scalable_left ? NK_WINDOW_SCALE_LEFT : 0)
                        | (!widget->has_input ? NK_WINDOW_NO_INPUT : 0)
        ;

        ctx.style.window.fixed_background = nk_style_item_color(nk_rgba(widget->color.x, widget->color.y, widget->color.z, widget->color.w));
        if (nk_begin(
            &ctx, widget->title.c_str(),
            nk_rect(position.x, position.y, size.x, size.y),
            widget_flags))
        {
            
            for(const auto& child : widget->elements) {
                const ObjectUI* obj = child.second.obj;
                if (obj->type == ObjectUI::Type::Label) {
                    const LabelUI* label = (LabelUI*)obj;
                    const CoordsUI::CoordsData label_size = label->size.getTopLeftCoords(size.x, size.y);
                    const CoordsUI::CoordsData label_position = label->position.getCoords(label_size.x, label_size.y);
                    ctx.style.text.color = nk_rgb(label->color.x, label->color.y, label->color.z);
                    nk_layout_space_begin(&ctx, NK_STATIC, (int)label_size.y, INT_MAX);
                    //nk_layout_row_static(&ctx, 30, 700, 1);
                    nk_layout_space_push(&ctx, nk_rect(label_position.x, label_position.y, label_size.x, label_size.y));
                    
                    //nk_layout
                    
                    nk_flags text_flags = 0;
                    if (label->text_align == LabelUI::TextAlign::Left)
                        text_flags = NK_TEXT_LEFT;
                    else if (label->text_align == LabelUI::TextAlign::Center)
                        text_flags = NK_TEXT_CENTERED;
                    else if (label->text_align == LabelUI::TextAlign::Right)
                        text_flags = NK_TEXT_RIGHT;
                    nk_label(&ctx, label->text.c_str(), text_flags);
                    nk_layout_space_end(&ctx);
                }
            }
            
        }
        nk_end(&ctx);
    }
    glm::ivec2 resolution = this->engine->getResolution();
    draw(resolution.x, resolution.y);
}



static void draw(int width, int height) {
    glEnable(GL_TEXTURE_2D);

    glEnableClientState(GL_COLOR_ARRAY);
    glPushMatrix();

    glm::mat4 Mp = glm::ortho(0.0f, (float)width, (float)height, 0.0f);//(glm::radians(60), 0.5, 0.1f, 30);
    //glm::mat4 Mp = glm::perspective(glm::radians(60.0f), (float)(WIDTH / HEIGHT), 0.1f, 30000.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(Mp));

    struct nk_convert_config cfg = {};
    static const struct nk_draw_vertex_layout_element vertex_layout[] = {
        {NK_VERTEX_POSITION, NK_FORMAT_FLOAT, NK_OFFSETOF(struct your_vertex, pos)},
        {NK_VERTEX_TEXCOORD, NK_FORMAT_FLOAT, NK_OFFSETOF(struct your_vertex, uv)},
        {NK_VERTEX_COLOR, NK_FORMAT_R8G8B8A8, NK_OFFSETOF(struct your_vertex, col)},
        {NK_VERTEX_LAYOUT_END}
    };
    cfg.shape_AA = NK_ANTI_ALIASING_ON;
    cfg.line_AA = NK_ANTI_ALIASING_ON;
    cfg.vertex_layout = vertex_layout;
    cfg.vertex_size = sizeof(struct your_vertex);
    cfg.vertex_alignment = NK_ALIGNOF(struct your_vertex);
    cfg.circle_segment_count = 22;
    cfg.curve_segment_count = 22;
    cfg.arc_segment_count = 22;
    cfg.global_alpha = 1.0f;
    cfg.tex_null = tex_null;
    //
    // setup buffers and convert
    struct nk_buffer cmds, verts, idx;
    nk_buffer_init_default(&cmds);
    nk_buffer_init_default(&verts);
    nk_buffer_init_default(&idx);
    nk_convert(&ctx, &cmds, &verts, &idx, &cfg);

    GLsizei vs = sizeof(struct your_vertex);
    size_t vp = offsetof(struct your_vertex, pos);
    size_t up = offsetof(struct your_vertex, uv);
    size_t cp = offsetof(struct your_vertex, col);
    const nk_draw_index *offset = NULL;
    //
    // draw
    const void *vertices = nk_buffer_memory_const(&verts);

    glVertexPointer(2, GL_FLOAT, vs, vertices);
    glTexCoordPointer(2, GL_FLOAT, vs, (nk_byte*)vertices + up);
    glColorPointer(4, GL_UNSIGNED_BYTE, vs, (nk_byte*)vertices + cp);
    const struct nk_draw_command *cmd;
    offset = (const nk_draw_index*)nk_buffer_memory_const(&idx);


    nk_draw_foreach(cmd, &ctx, &cmds) {
        if (!cmd->elem_count) continue;
        glBindTexture(GL_TEXTURE_2D, (GLuint)cmd->texture.id);
        glDrawElements(GL_TRIANGLES, (GLsizei)cmd->elem_count, GL_UNSIGNED_SHORT, offset);
        offset += cmd->elem_count;
    }

    nk_buffer_free(&cmds);
    nk_buffer_free(&verts);
    nk_buffer_free(&idx);
    nk_clear(&ctx);
    glPopMatrix();
    glDisable(GL_TEXTURE_2D);
    glDisableClientState(GL_COLOR_ARRAY);
}





static void pump_input(struct nk_context *ctx, GLFWwindow *win)
{
    double x, y;
    nk_input_begin(ctx);
    glfwPollEvents();

    nk_input_key(ctx, NK_KEY_DEL, glfwGetKey(win, GLFW_KEY_DELETE) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_ENTER, glfwGetKey(win, GLFW_KEY_ENTER) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_TAB, glfwGetKey(win, GLFW_KEY_TAB) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_BACKSPACE, glfwGetKey(win, GLFW_KEY_BACKSPACE) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_LEFT, glfwGetKey(win, GLFW_KEY_LEFT) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_RIGHT, glfwGetKey(win, GLFW_KEY_RIGHT) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_UP, glfwGetKey(win, GLFW_KEY_UP) == GLFW_PRESS);
    nk_input_key(ctx, NK_KEY_DOWN, glfwGetKey(win, GLFW_KEY_DOWN) == GLFW_PRESS);

    if (glfwGetKey(win, GLFW_KEY_LEFT_CONTROL) == GLFW_PRESS ||
        glfwGetKey(win, GLFW_KEY_RIGHT_CONTROL) == GLFW_PRESS) {
        nk_input_key(ctx, NK_KEY_COPY, glfwGetKey(win, GLFW_KEY_C) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_PASTE, glfwGetKey(win, GLFW_KEY_P) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_CUT, glfwGetKey(win, GLFW_KEY_X) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_CUT, glfwGetKey(win, GLFW_KEY_E) == GLFW_PRESS);
        nk_input_key(ctx, NK_KEY_SHIFT, 1);
    } else {
        nk_input_key(ctx, NK_KEY_COPY, 0);
        nk_input_key(ctx, NK_KEY_PASTE, 0);
        nk_input_key(ctx, NK_KEY_CUT, 0);
        nk_input_key(ctx, NK_KEY_SHIFT, 0);
    }

    glfwGetCursorPos(win, &x, &y);
    nk_input_motion(ctx, (int)x, (int)y);
    nk_input_button(ctx, NK_BUTTON_LEFT, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
    nk_input_button(ctx, NK_BUTTON_MIDDLE, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_MIDDLE) == GLFW_PRESS);
    nk_input_button(ctx, NK_BUTTON_RIGHT, (int)x, (int)y, glfwGetMouseButton(win, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
    nk_input_end(ctx);
}
