//
// Created by Тимофей Кириченко on 12.05.2024.
//

#include <glad/gl.h>

#include <HateEngine/Render/OpenGL_1_3.hpp>
#include <HateEngine/UI/ButtonUI.hpp>
#include <HateEngine/UI/LabelUI.hpp>
#include <HateEngine/UI/WidgetUI.hpp>
#include <HateEngine/UI/ImageUI.hpp>
#include <climits>
#include <glm/ext.hpp>
#include <string>

#include <HateEngine/UI/CheckboxUI.hpp>
#include <HateEngine/UI/CoordsUI.hpp>
#include <HateEngine/UI/ObjectUI.hpp>
#include <HateEngine/Resources/UIFont.hpp>
#include <glm/ext/matrix_clip_space.hpp>
#include <glm/ext/vector_int2.hpp>
#include "HateEngine/HateEngine.hpp"
#include "HateEngine/Input.hpp"
#include "HateEngine/Log.hpp"

#define NK_INCLUDE_FIXED_TYPES
#define NK_INCLUDE_STANDARD_IO
#define NK_INCLUDE_DEFAULT_ALLOCATOR
#define NK_INCLUDE_VERTEX_BUFFER_OUTPUT

#define NK_INCLUDE_FONT_BAKING
#define NK_INCLUDE_DEFAULT_FONT
#define NK_IMPLEMENTATION
#include <nuklear.h>

using namespace HateEngine;

static unsigned int device_upload_atlas(const void* image, int width, int height);
static long draw(
        int width, int height, const void* vertices, const nk_draw_index* offset, struct nk_buffer* cmds
);
static void pump_input(struct nk_context* ctx, Engine* win);

#define MAX_MEMORY 512 * 1024
// #define MAX_ELEMENT_MEMORY 128 * 1024

nk_context ctx;
nk_font_atlas atlas;

// unsigned int font_tex = 0;
nk_draw_null_texture tex_null;
struct your_vertex {
    float pos[2]; // important to keep it to 2 floats
    float uv[2];
    unsigned char col[4];
};


// TODO: Refactor this file


void OpenGL_1_3::initNuklearUI() {
    const void* image;
    int w, h;
    struct nk_font* font;

    struct nk_font_config config = nk_font_config(18);
    config.oversample_h = 1;
    config.oversample_v = 1;
    config.range = nk_font_cyrillic_glyph_ranges();

    nk_font_atlas_init_default(&atlas);
    nk_font_atlas_begin(&atlas);
    font = nk_font_atlas_add_default(&atlas, 18, &config);
    image = nk_font_atlas_bake(&atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
    unsigned int font_tex = device_upload_atlas(image, w, h);
    nk_font_atlas_end(&atlas, nk_handle_id((int) font_tex), &tex_null);
    nk_init_default(&ctx, &font->handle);

    // ctx.style.window.border = 1;
    ctx.style.window.padding = nk_vec2(0, 0);
    // ctx.style.window.header. = nk_vec2(0, 0);
    ctx.style.button.border = 0;
    ctx.style.button.padding = nk_vec2(0, 0);

    ctx.style.text.padding = nk_vec2(0, 0);

    this->UINuklearBuffer_cmds = new struct nk_buffer;
    this->UINuklearBuffer_verts = new struct nk_buffer;
    this->UINuklearBuffer_idx = new struct nk_buffer;
    nk_buffer_init_default((struct nk_buffer*) this->UINuklearBuffer_cmds);
    nk_buffer_init_default((struct nk_buffer*) this->UINuklearBuffer_verts);
    nk_buffer_init_default((struct nk_buffer*) this->UINuklearBuffer_idx);
}

static unsigned int device_upload_atlas(const void* image, int width, int height) {
    unsigned int font_id = 0;
    glGenTextures(1, &font_id);
    glBindTexture(GL_TEXTURE_2D, font_id);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    glTexImage2D(
            GL_TEXTURE_2D, 0, GL_RGBA, (GLsizei) width, (GLsizei) height, 0, GL_RGBA, GL_UNSIGNED_BYTE,
            image
    );
    return font_id;
}

// nk_font_atlas atlas;

struct UIFontRAPI_Data {
    nk_font* font;
    nk_font_atlas atlas;
    nk_draw_null_texture null;
};

void OpenGL_1_3::loadFont(UIFont* font) {
    struct nk_font_config config = nk_font_config(font->pixel_height);
    config.oversample_h = 1;
    config.oversample_v = 1;
    config.range = nk_font_cyrillic_glyph_ranges();

    UIFontRAPI_Data* rapi_data = new UIFontRAPI_Data;

    memset(&rapi_data->atlas, 0, sizeof(nk_font_atlas));

    nk_font_atlas_init_default(&rapi_data->atlas);
    nk_font_atlas_begin(&rapi_data->atlas);

    nk_font* fnt = nk_font_atlas_add_from_memory(
            &rapi_data->atlas, font->data.data(), font->data.size(), font->pixel_height, &config
    );

    int w, h;
    const void* image = nk_font_atlas_bake(&rapi_data->atlas, &w, &h, NK_FONT_ATLAS_RGBA32);
    unsigned int font_tex = device_upload_atlas(image, w, h);
    nk_font_atlas_end(&rapi_data->atlas, nk_handle_id((int) font_tex), &rapi_data->null);


    font->is_gpu_loaded = true;
    font->textureGL_ID = font_tex;
    rapi_data->font = fnt;
    font->render_api_data = rapi_data;

    // nk_init_default(&ctx, &font->handle);
}

void OpenGL_1_3::unloadFont(UIFont* font) {
    nk_font_atlas_clear(&((UIFontRAPI_Data*) font->render_api_data)->atlas);
    glDeleteTextures(1, &font->textureGL_ID);
    delete (UIFontRAPI_Data*) font->render_api_data;
}

void OpenGL_1_3::DrawNuklearUI(std::unordered_map<UUID, WidgetUI*>* widgets) {
    Level* start_level = engine->getLevel();

    UIUpdateDelay += engine->getFrameDelta();

    if (UIUpdateDelay >= 1.0 / 60.0) {
        // UpdateUI
        UIUpdateDelay = 0;

        nk_buffer_clear((struct nk_buffer*) this->UINuklearBuffer_cmds);
        nk_buffer_clear((struct nk_buffer*) this->UINuklearBuffer_verts);
        nk_buffer_clear((struct nk_buffer*) this->UINuklearBuffer_idx);
        nk_clear(&ctx);

        pump_input(&ctx, engine);

        for (const auto& it: *widgets) {
            const WidgetUI* widget = it.second;
            glm::ivec2 res = engine->getResolution();
            CoordsUI::CoordsData position = widget->position.getCoords(res.x, res.y);
            CoordsUI::CoordsData size = widget->size.getTopLeftCoords(res.x, res.y);
            nk_flags widget_flags = (widget->has_border ? NK_WINDOW_BORDER : 0) |
                                    (widget->is_movable ? NK_WINDOW_MOVABLE : 0) |
                                    (widget->is_scalable ? NK_WINDOW_SCALABLE : 0) |
                                    (widget->is_closable ? NK_WINDOW_CLOSABLE : 0) |
                                    (widget->is_minimizable ? NK_WINDOW_MINIMIZABLE : 0) |
                                    (!widget->has_scrollbar ? NK_WINDOW_NO_SCROLLBAR : 0) |
                                    (widget->has_title ? NK_WINDOW_TITLE : 0) |
                                    (widget->is_scroll_autohide ? NK_WINDOW_SCROLL_AUTO_HIDE : 0) |
                                    (widget->has_background ? NK_WINDOW_BACKGROUND : 0) |
                                    (widget->is_scalable_left ? NK_WINDOW_SCALE_LEFT : 0) |
                                    (!widget->has_input ? NK_WINDOW_NO_INPUT : 0) |
                                    (!widget->is_interactive ? NK_WINDOW_NOT_INTERACTIVE : 0);

            ctx.style.window.fixed_background = nk_style_item_color(
                    nk_rgba(widget->color.x, widget->color.y, widget->color.z, widget->color.w)
            );
            if (nk_begin(
                        &ctx, widget->title.c_str(), nk_rect(position.x, position.y, size.x, size.y),
                        widget_flags
                )) {
                nk_layout_space_begin(&ctx, NK_STATIC, (int) size.y, INT_MAX);


                for (const auto& child: widget->elements) {
                    const ObjectUI* obj = child.obj;
                    const CoordsUI::CoordsData obj_size = obj->size.getTopLeftCoords(size.x, size.y);
                    const CoordsUI::CoordsData obj_position = obj->position.getCoords(size.x, size.y);

                    if (!obj->visible)
                        continue;

                    nk_layout_space_push(
                            &ctx, nk_rect(obj_position.x, obj_position.y, obj_size.x, obj_size.y)
                    );

                    nk_style_push_font(&ctx, ctx.style.font);

                    ctx.style.text.color =
                            nk_rgba(obj->text_color.x, obj->text_color.y, obj->text_color.z,
                                    obj->text_color.w);

                    // const nk_user_font* prev_font = ctx.style.font;
                    if (obj->font != nullptr and obj->font->is_loaded) {
                        if (not obj->font->is_gpu_loaded) {
                            obj->font->Load(loadFont, unloadFont);
                        }

                        ctx.style.font = &((UIFontRAPI_Data*) obj->font->render_api_data)->font->handle;
                    }

                    if (obj->type == ObjectUI::Type::Label) {
                        const LabelUI* label = (LabelUI*) obj;

                        if (label->text_align == LabelUI::TextAlign::Wrap) {
                            nk_label_wrap(&ctx, label->text.c_str());
                            continue;
                        }

                        nk_flags text_flags = 0;
                        if (label->text_align == LabelUI::TextAlign::Left)
                            text_flags = NK_TEXT_LEFT;
                        else if (label->text_align == LabelUI::TextAlign::Center)
                            text_flags = NK_TEXT_CENTERED;
                        else if (label->text_align == LabelUI::TextAlign::Right)
                            text_flags = NK_TEXT_RIGHT;
                        nk_label(&ctx, label->text.c_str(), text_flags);
                        // ctx.style.font = prev_font;


                    } else if (obj->type == ObjectUI::Type::Button) {
                        ButtonUI* button = (ButtonUI*) obj;

                        struct nk_rect widget_bounds = nk_window_get_bounds(&ctx);
                        struct nk_rect bounds = {
                                obj_position.x + widget_bounds.x, obj_position.y + widget_bounds.y,
                                obj_size.x, obj_size.y
                        };

                        if (widget->has_border) {
                            bounds.x += ctx.style.window.border;
                            bounds.y += ctx.style.window.border;
                        }

                        if (widget->has_title || widget->is_closable || widget->is_minimizable) {
                            float padding_y = ctx.style.window.header.padding.y;
                            float spacing_y = ctx.style.window.spacing.y;
                            const struct nk_user_font* font = ctx.style.font;
                            float font_height = font->height;
                            bounds.y += font_height + 2 * padding_y + spacing_y * 2;
                        }

                        // clang-format off
                        #define BUTTON_BODY                                                                                \
                            button->call_on_click(this->engine);                                                           \
                            // Because the level can be changed in the callback
                            if (start_level != engine->getLevel()) {
                                nk_layout_space_end(&ctx);
                                nk_end(&ctx);
                                return;
                            }
                        // clang-format on

                        struct nk_image* button_image = nullptr;
                        if (button->pressed_texture != nullptr and
                            nk_input_is_mouse_hovering_rect(&ctx.input, bounds) and
                            nk_input_is_mouse_down(&ctx.input, NK_BUTTON_LEFT) and
                            button->pressed_texture->is_loaded) {

                            if (not button->getPressedTexture()->is_gpu_loaded)
                                button->getPressedTexture()->Load(loadTexture, unloadTexture);

                            if (button->nk_img_pressed == nullptr) {
                                struct nk_image img =
                                        nk_image_id(button->getPressedTexture()->getTextureID());
                                button->nk_img_pressed = new struct nk_image(img);
                            }

                            button_image = (struct nk_image*) button->nk_img_pressed;
                        } else if (button->hover_texture != nullptr and
                                   nk_input_is_mouse_hovering_rect(&ctx.input, bounds) and
                                   button->hover_texture->is_loaded) {
                            if (not button->getHoverTexture()->is_gpu_loaded)
                                if (!button->getHoverTexture()->Load(loadTexture, unloadTexture))
                                    continue;

                            if (button->nk_img_hover == nullptr) {
                                struct nk_image img =
                                        nk_image_id(button->getHoverTexture()->getTextureID());
                                button->nk_img_hover = new struct nk_image(img);
                            }

                            button_image = (struct nk_image*) button->nk_img_hover;
                        } else if (button->normal_texture != nullptr and
                                   button->normal_texture->is_loaded) {
                            if (not button->getNormalTexture()->is_gpu_loaded)
                                button->getNormalTexture()->Load(loadTexture, unloadTexture);

                            if (button->nk_img_normal == nullptr) {
                                struct nk_image img =
                                        nk_image_id(button->getNormalTexture()->getTextureID());
                                button->nk_img_normal = new struct nk_image(img);
                            }

                            button_image = (struct nk_image*) button->nk_img_normal;
                        }


                        if (button_image != nullptr and button->ignore_color_with_image) {
                            ctx.style.button.normal = nk_style_item_color(nk_rgba(0, 0, 0, 0));
                            ctx.style.button.hover = nk_style_item_color(nk_rgba(0, 0, 0, 0));
                            ctx.style.button.active = nk_style_item_color(nk_rgba(0, 0, 0, 0));
                        } else {
                            ctx.style.button.normal = nk_style_item_color(nk_rgba(
                                    button->color.x, button->color.y, button->color.z, button->color.w
                            ));

                            ctx.style.button.hover = nk_style_item_color(
                                    nk_rgba(button->hover_color.x, button->hover_color.y,
                                            button->hover_color.z, button->hover_color.w)
                            );

                            ctx.style.button.active = nk_style_item_color(
                                    nk_rgba(button->pressed_color.x, button->pressed_color.y,
                                            button->pressed_color.z, button->pressed_color.w)
                            );
                        }

                        // ctx.style.button.text_normal = nk_rgba(0, 0, 255, 255);

                        ctx.style.button.rounding = button->roundness;

                        if (button_image != nullptr) {
                            nk_flags text_flags = 0;
                            if (nk_button_image_label(
                                        &ctx, *button_image, button->text.c_str(), NK_TEXT_ALIGN_CENTERED
                                )) {
                                button->call_on_click(this->engine);
                                // Because the level can be changed in the callback
                                if (start_level != engine->getLevel()) {
                                    nk_layout_space_end(&ctx);
                                    nk_end(&ctx);
                                    return;
                                }
                            }
                        } else {
                            if (nk_button_label(&ctx, button->text.c_str())) {
                                if (button->on_click != nullptr) {
                                    button->call_on_click(this->engine);
                                    // Because the level can be changed in the callback
                                    if (start_level != engine->getLevel()) {
                                        nk_layout_space_end(&ctx);
                                        nk_end(&ctx);
                                        return;
                                    }
                                }
                            }
                        }


                    } else if (obj->type == ObjectUI::Type::Checkbox) {
                        CheckboxUI* checkbox = (CheckboxUI*) obj;

                        nk_bool checked = checkbox->get_checked();
                        nk_checkbox_label(&ctx, checkbox->text.c_str(), &checked);
                        checkbox->set_checked(checked);
                    } else if (obj->type == ObjectUI::Type::Image) {
                        ImageUI* image = (ImageUI*) obj;
                        Texture* texture = image->texture;

                        if (texture->is_loaded) {
                            if (not texture->is_gpu_loaded)
                                texture->Load(loadTexture, unloadTexture);

                            if (image->nk_image == nullptr) {
                                struct nk_image img = nk_image_id(texture->getTextureID());
                                image->nk_image = new struct nk_image(img);
                            }

                            nk_image(&ctx, *((struct nk_image*) image->nk_image));
                        }
                    }
                    nk_style_pop_font(&ctx);
                }
                nk_layout_space_end(&ctx);
            }
            nk_end(&ctx);
        }


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
        nk_convert(
                &ctx, (struct nk_buffer*) this->UINuklearBuffer_cmds,
                (struct nk_buffer*) this->UINuklearBuffer_verts,
                (struct nk_buffer*) this->UINuklearBuffer_idx, &cfg
        );
    }


    glm::ivec2 resolution = this->engine->getResolution();
    const void* vertices = nk_buffer_memory_const((struct nk_buffer*) this->UINuklearBuffer_verts);
    const nk_draw_index* offset =
            (const nk_draw_index*) nk_buffer_memory_const((struct nk_buffer*) this->UINuklearBuffer_idx);
    gpu_time +=
            draw(resolution.x, resolution.y, vertices, offset,
                 (struct nk_buffer*) this->UINuklearBuffer_cmds);
}

static long draw(
        int width, int height, const void* vertices, const nk_draw_index* offset, struct nk_buffer* cmds
) {
    glEnable(GL_BLEND);

    glEnableClientState(GL_COLOR_ARRAY);
    glDisableClientState(GL_NORMAL_ARRAY);
    glActiveTexture(GL_TEXTURE1);
    glDisable(GL_TEXTURE_2D);
    glActiveTexture(GL_TEXTURE0);
    glClientActiveTexture(GL_TEXTURE0);
    glPushMatrix();

    glm::mat4 Mp = glm::ortho(0.0f, (float) width, (float) height, 0.0f);
    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(Mp));

    auto time_start = std::chrono::high_resolution_clock::now();

    glVertexPointer(2, GL_FLOAT, sizeof(your_vertex), vertices);
    glTexCoordPointer(2, GL_FLOAT, sizeof(your_vertex), (nk_byte*) vertices + offsetof(your_vertex, uv));
    glColorPointer(
            4, GL_UNSIGNED_BYTE, sizeof(your_vertex), (nk_byte*) vertices + offsetof(your_vertex, col)
    );

    const struct nk_draw_command* cmd;
    nk_draw_foreach(cmd, &ctx, cmds) {
        if (!cmd->elem_count)
            continue;
        glBindTexture(GL_TEXTURE_2D, (GLuint) cmd->texture.id);
        glDrawElements(GL_TRIANGLES, (GLsizei) cmd->elem_count, GL_UNSIGNED_SHORT, offset);
        offset += cmd->elem_count;
        // n++;
    }

    auto time_end = std::chrono::high_resolution_clock::now();
    long gpu_time = std::chrono::duration_cast<std::chrono::nanoseconds>(time_end - time_start).count();

    glPopMatrix();
    glDisable(GL_BLEND);
    glDisableClientState(GL_COLOR_ARRAY);
    glEnableClientState(GL_NORMAL_ARRAY);
    glActiveTexture(GL_TEXTURE1);
    glEnable(GL_TEXTURE_2D);
    return gpu_time;
}

static void pump_input(struct nk_context* ctx, Engine* engine) {
    nk_input_begin(ctx);
    // glfwPollEvents();

    nk_input_key(ctx, NK_KEY_DEL, engine->Input.isPhysicalKeyPressed(HateEngine::KeyDelete));
    nk_input_key(ctx, NK_KEY_ENTER, engine->Input.isPhysicalKeyPressed(HateEngine::KeyReturn));
    nk_input_key(ctx, NK_KEY_TAB, engine->Input.isPhysicalKeyPressed(HateEngine::KeyTab));
    nk_input_key(ctx, NK_KEY_BACKSPACE, engine->Input.isPhysicalKeyPressed(HateEngine::KeyBackspace));
    nk_input_key(ctx, NK_KEY_LEFT, engine->Input.isPhysicalKeyPressed(HateEngine::KeyLeft));
    nk_input_key(ctx, NK_KEY_RIGHT, engine->Input.isPhysicalKeyPressed(HateEngine::KeyRight));
    nk_input_key(ctx, NK_KEY_UP, engine->Input.isPhysicalKeyPressed(HateEngine::KeyUp));
    nk_input_key(ctx, NK_KEY_DOWN, engine->Input.isPhysicalKeyPressed(HateEngine::KeyDown));

    if (engine->Input.isPhysicalKeyPressed(HateEngine::KeyLeftCtrl) ||
        engine->Input.isPhysicalKeyPressed(HateEngine::KeyRightCtrl)) {
        nk_input_key(ctx, NK_KEY_COPY, engine->Input.isPhysicalKeyPressed(HateEngine::KeyC));
        nk_input_key(ctx, NK_KEY_PASTE, engine->Input.isPhysicalKeyPressed(HateEngine::KeyP));
        nk_input_key(ctx, NK_KEY_CUT, engine->Input.isPhysicalKeyPressed(HateEngine::KeyX));
        nk_input_key(ctx, NK_KEY_CUT, engine->Input.isPhysicalKeyPressed(HateEngine::KeyE));
        nk_input_key(ctx, NK_KEY_SHIFT, 1);
    } else {
        nk_input_key(ctx, NK_KEY_COPY, 0);
        nk_input_key(ctx, NK_KEY_PASTE, 0);
        nk_input_key(ctx, NK_KEY_CUT, 0);
        nk_input_key(ctx, NK_KEY_SHIFT, 0);
    }

    glm::vec2 cursorPos = engine->Input.getCursorPosition();
    nk_input_motion(ctx, (int) cursorPos.x, (int) cursorPos.y);
    nk_input_button(
            ctx, NK_BUTTON_LEFT, (int) cursorPos.x, (int) cursorPos.y,
            engine->Input.isMouseButtonPressed(HateEngine::MouseButtonLeft)
    );
    nk_input_button(
            ctx, NK_BUTTON_MIDDLE, (int) cursorPos.x, (int) cursorPos.y,
            engine->Input.isMouseButtonPressed(HateEngine::MouseButtonMiddle)
    );
    nk_input_button(
            ctx, NK_BUTTON_RIGHT, (int) cursorPos.x, (int) cursorPos.y,
            engine->Input.isMouseButtonPressed(HateEngine::MouseButtonRight)
    );
    nk_input_end(ctx);
}
