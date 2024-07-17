#pragma once

#include "ObjectUI.hpp"
#include "../HateEngine.hpp"

namespace HateEngine {
    class ButtonUI : public ObjectUI {
    public:
        enum TextAlign {
            Left,
            Center,
            Right
        };
    
        //glm::ivec2 size = {100, 100};
        void (*on_click)(HateEngine::Engine* engine) = nullptr;
        glm::ivec3 color = {0, 0, 0};
        TextAlign text_align = Left;

    public:
        std::string text = "";
        
        ButtonUI(void (*on_click)(HateEngine::Engine* engine));


        //LabelUI();
    };
}