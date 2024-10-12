#pragma once
#include <cstdint>
#include <vector>
#include "../Resources/Texture.hpp"
#include "Object.hpp"

namespace HateEngine {
    // Object -> Mesh
    class Mesh : public Object {
    private:
        std::vector<float> verticies;
        std::vector<uint32_t> indicies;
        std::vector<float> normals;
        Texture* texture = nullptr;
        std::vector<float> UV = {};
        float max_light_dist = 0;
        float center_max_size = 0;
        bool is_shaded = true;
        std::string name = "";
        // uint8_t draw_priority = 255;
        bool correct_transparency = false;
        bool face_culling = true;

        void updateCenterMaxSize();

    public:
        ~Mesh();
        Mesh();
        Mesh(std::vector<float> vert, std::vector<uint32_t> ind, std::vector<float> norm);
        Mesh(const Mesh& mesh, bool copy_texture = false);

        void setName(std::string name);
        const std::string getName();

        void setVertices(std::vector<float> vec);
        void setIndicies(std::vector<uint32_t> vec);
        void setNormals(std::vector<float> vec);
        void setTexture(Texture* tex);
        void setUV(std::vector<float> UV);

        void enableCustomMaxLightDist(float dist);
        const float getCustomMaxLightDist() const;
        void disableCustomMaxLightDist();

        /**
         * @brief Enable or disable transparency correction by distance to camera.
         * @brief ENABLE ONLY FOR TRANSPARENT OBJECTS
         */
        void setCorrectTransparency(bool correct);
        bool getCorrectTransparency() const;

        void setFaceCulling(bool cull);
        bool getFaceCulling() const;

        void enableLightShading();
        void disableLightShading();
        bool isLightShading() const;
        float getAABBRadius() const;

        const std::vector<float>* getVertices() const;
        const std::vector<uint32_t>* getIndicies() const;
        const std::vector<float>* getNormals() const;
        Texture* getTexture() const;
        const std::vector<float>* getUV() const;
        // const std::vector<std::vector<float>>* getTexturesCoords();
    };
} // namespace HateEngine
