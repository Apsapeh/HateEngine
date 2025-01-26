#include <HateEngine/UI/ButtonUI.hpp>
#define NK_INCLUDE_FIXED_TYPES
#include <nuklear.h>

using namespace HateEngine;

void on_click_wrapper(HateEngine::Engine* engine, void* data_p);

ButtonUI::ButtonUI(const ButtonUI& btn) : ObjectUI(btn) {
    this->on_click = btn.on_click;
    this->data_p = btn.data_p;
    this->color = btn.color;
    this->hover_color = btn.hover_color;
    this->pressed_color = btn.pressed_color;
    this->roundness = btn.roundness;
    this->ignore_color_with_image = btn.ignore_color_with_image;
    this->normal_texture = btn.normal_texture;
    this->hover_texture = btn.hover_texture;
    this->pressed_texture = btn.pressed_texture;
}

ButtonUI::ButtonUI(void (*on_click)(HateEngine::Engine* engine, void* data_p), void* data_p) :
    ObjectUI(Type::Button) {
    this->size = {100, 100};
    this->color = {50, 50, 50, 255};
    this->data_p = data_p;
    this->on_click = on_click;
}

ButtonUI::ButtonUI(void (*on_click)(HateEngine::Engine* engine)) : ObjectUI(Type::Button) {
    this->size = {100, 100};
    this->color = {50, 50, 50, 255};
    this->data_p = (void*) on_click;
    this->on_click = on_click_wrapper;
}

ButtonUI::~ButtonUI() {
    if (nk_img_normal != nullptr) {
        delete (struct nk_image*) nk_img_normal;
    }
    if (nk_img_hover != nullptr) {
        delete (struct nk_image*) nk_img_hover;
    }
    if (nk_img_pressed != nullptr) {
        delete (struct nk_image*) nk_img_pressed;
    }
}

void ButtonUI::call_on_click(HateEngine::Engine* engine) const {
    if (on_click != nullptr) {
        on_click(engine, data_p);
    }
}

void on_click_wrapper(HateEngine::Engine* engine, void* data_p) {
    void (*on_click)(HateEngine::Engine* engine) = (void (*)(HateEngine::Engine* engine)) data_p;
    on_click(engine);
}


void ButtonUI::setNormalTexture(Texture* texture) {
    this->normal_texture = texture;
}

void ButtonUI::setHoverTexture(Texture* texture) {
    this->hover_texture = texture;
}

void ButtonUI::setPressedTexture(Texture* texture) {
    this->pressed_texture = texture;
}


Texture* ButtonUI::getNormalTexture() {
    return this->normal_texture;
}

Texture* ButtonUI::getHoverTexture() {
    return this->hover_texture;
}

Texture* ButtonUI::getPressedTexture() {
    return this->pressed_texture;
}


void ButtonUI::clearNormalTexture() {
}

void ButtonUI::clearHoverTexture() {
}

void ButtonUI::clearPressedTexture() {
}
