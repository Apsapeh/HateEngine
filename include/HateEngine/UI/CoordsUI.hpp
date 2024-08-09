#pragma once

namespace HateEngine {
    class CoordsUI {
    public:
        enum Anchor {
            TopLeft,
            TopRight,
            BottomLeft,
            BottomRight,
        };

        enum Units {
            Pixels,
            Percent,
        };

        struct CoordsData {
            float x;
            float y;
        };

    private:
    public:
        Anchor anchor = TopLeft;
        Units units = Pixels;
        float scale = 1.0f;
        float x = 0.0f;
        float y = 0.0f;

        CoordsUI();
        CoordsUI(
                float x, float y, float scale = 1.0f, Anchor anchor = TopLeft, Units units = Pixels
        );

        CoordsData getCoords(int screenWidth, int screenHeight) const;
        CoordsData getTopLeftCoords(int screenWidth, int screenHeight) const;
        CoordsData getRawCoords() const;
        ~CoordsUI();
    };
} // namespace HateEngine
