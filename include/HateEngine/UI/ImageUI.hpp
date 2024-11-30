//
// Created by Тимофей Кириченко on 30.11.2024.
//

#pragma once

#include "ObjectUI.hpp"
#include "../Resources/Texture.hpp"

namespace HateEngine {
    class ImageUI : public ObjectUI {
        friend class OpenGL15;

    private:
        Texture* texture = nullptr;
        void* nk_image = nullptr;

        // nk
    public:
        // ImageUI(const ImageUI& lbl);
        ImageUI(Texture* texture);
        ~ImageUI();

        const Texture* getTexture() const;

        // LabelUI();
    };
} // namespace HateEngine
