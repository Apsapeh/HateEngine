#include <HateEngine/UI/CoordsUI.hpp>
#include <cstdint>
#include <iostream>

using namespace HateEngine;

CoordsUI::CoordsUI() {
}

CoordsUI::CoordsUI(float x, float y, float scale, Anchor anchor, Units units) {
    this->x = x;
    this->y = y;
    this->scale = scale;
    this->anchor = anchor;
    this->units = units;
}

CoordsUI::~CoordsUI() {
}

CoordsUI::CoordsData CoordsUI::getCoords(int screenWidth, int screenHeight) const {
    CoordsData result = {0.0f, 0.0f};
    int8_t coord_multiplier[2] = {1, 1};

    switch (this->anchor) {
        case Anchor::TopLeft:
            break;
        case Anchor::TopRight:
            result = {(float) screenWidth, 0.0f};
            coord_multiplier[0] = -1;
            break;
        case Anchor::BottomLeft:
            result = {0.0f, (float) screenHeight};
            coord_multiplier[1] = -1;
            break;
        case Anchor::BottomRight:
            result = {(float) screenWidth, (float) screenHeight};
            coord_multiplier[0] = -1;
            coord_multiplier[1] = -1;
            break;
        case Anchor::Center:
            result = {(float) screenWidth / 2.0f, (float) screenHeight / 2.0f};
            break;
        case Anchor::CenterLeft:
            result = {0.0f, (float) screenHeight / 2.0f};
            // coord_multiplier[1] = -1;
            break;
        case Anchor::CenterRight:
            result = {(float) screenWidth, (float) screenHeight / 2.0f};
            // coord_multiplier[0] = -1;
            // coord_multiplier[1] = -1;
            break;
        case Anchor::CenterTop:
            result = {(float) screenWidth / 2.0f, 0.0f};
            // coord_multiplier[1] = -1;
            break;
        case Anchor::CenterBottom:
            result = {(float) screenWidth / 2.0f, (float) screenHeight};
            // coord_multiplier[1] = -1;
            break;
    }

    float X = this->x * this->scale;
    float Y = this->y * this->scale;

    if (this->units == Units::Percent) {
        X = (float) screenWidth * this->x / 100.0f;
        Y = (float) screenHeight * this->y / 100.0f;
    }

    result.x += X * coord_multiplier[0];
    result.y += Y * coord_multiplier[1];

    return result;
}

CoordsUI::CoordsData CoordsUI::getTopLeftCoords(int screenWidth, int screenHeight) const {
    CoordsData result = {0.0f, 0.0f};

    float X = this->x * this->scale;
    float Y = this->y * this->scale;

    if (this->units == Units::Percent) {
        X = (float) screenWidth * this->x / 100.0f;
        Y = (float) screenHeight * this->y / 100.0f;
    }

    result.x += X;
    result.y += Y;

    return result;
}

CoordsUI::CoordsData CoordsUI::getRawCoords() const {
    return {this->x, this->y};
}
