#pragma once

// #include "../HateEngine.hpp"
#include "ObjectUI.hpp"

namespace HateEngine {
    class Engine;
    class ButtonUI : public ObjectUI {
    public:
        // enum TextAlign { Left, Center, Right };

        // glm::ivec2 size = {100, 100};
        void (*on_click)(HateEngine::Engine* engine, void* data_p) = nullptr;
        void* data_p = nullptr;
        glm::ivec3 color = {0, 0, 0};
        // TextAlign text_align = Left;

    public:
        std::string text = "";

        ButtonUI(const ButtonUI& btn);
        ButtonUI(void (*on_click)(HateEngine::Engine* engine, void* data_p), void* data_p);
        ButtonUI(void (*on_click)(HateEngine::Engine* engine));

        void call_on_click(HateEngine::Engine* engine) const;
        // LabelUI();
    };
} // namespace HateEngine
