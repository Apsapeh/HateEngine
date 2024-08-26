#pragma once

// #include "../HateEngine.hpp"
#include "ObjectUI.hpp"


namespace HateEngine {
    class Engine;
    class ButtonUI : public ObjectUI {
    public:
        void (*on_click)(HateEngine::Engine* engine, void* data_p) = nullptr;
        void* data_p = nullptr;

        // TextAlign text_align = Left;

    public:
        ButtonUI(const ButtonUI& btn);
        ButtonUI(void (*on_click)(HateEngine::Engine* engine, void* data_p), void* data_p);
        ButtonUI(void (*on_click)(HateEngine::Engine* engine));

        void call_on_click(HateEngine::Engine* engine) const;
        // LabelUI();
    };
} // namespace HateEngine
