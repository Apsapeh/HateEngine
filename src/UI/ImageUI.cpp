#include <HateEngine/UI/ImageUI.hpp>
#include "HateEngine/UI/ObjectUI.hpp"

#include <nuklear.h>

using namespace HateEngine;

/*ImageUI::ImageUI(const ImageUI& lbl) {
}*/

ImageUI::ImageUI(Texture* texture) : ObjectUI(Image) {
    this->texture = texture;
    this->size = {(float) texture->getWidth(), (float) texture->getHeight()};
}

ImageUI::~ImageUI() {
    delete (struct nk_image*) this->nk_image;
}

const Texture* ImageUI::getTexture() const {
    return texture;
}
