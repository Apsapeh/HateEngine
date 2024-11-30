#pragma once

// #include "../HateEngine.hpp"
#include "../Resources/Texture.hpp"
#include "ObjectUI.hpp"


namespace HateEngine {
    class Engine;
    class ButtonUI : public ObjectUI {
        friend class OpenGL15;

    protected:
        Texture* normal_texture = nullptr;
        Texture* hover_texture = nullptr;
        Texture* pressed_texture = nullptr;

        void* nk_img_normal = nullptr;
        void* nk_img_hover = nullptr;
        void* nk_img_pressed = nullptr;

    public:
        void (*on_click)(HateEngine::Engine* engine, void* data_p) = nullptr;
        void* data_p = nullptr;

        float roundness = 4.0f;
        glm::ivec4 hover_color = {40, 40, 40, 255};
        glm::ivec4 pressed_color = {35, 35, 35, 255};
        bool ignore_color_with_image = true;


        // TextAlign text_align = Left;

    public:
        ButtonUI(const ButtonUI& btn);
        ButtonUI(void (*on_click)(HateEngine::Engine* engine, void* data_p), void* data_p);
        ButtonUI(void (*on_click)(HateEngine::Engine* engine));
        ~ButtonUI();

        void call_on_click(HateEngine::Engine* engine) const;

        void setNormalTexture(Texture* texture);
        void setHoverTexture(Texture* texture);
        void setPressedTexture(Texture* texture);

        Texture* getNormalTexture();
        Texture* getHoverTexture();
        Texture* getPressedTexture();

        void clearNormalTexture();
        void clearHoverTexture();
        void clearPressedTexture();
        // LabelUI();
    };
} // namespace HateEngine
