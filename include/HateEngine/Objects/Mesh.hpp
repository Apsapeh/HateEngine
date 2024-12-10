#pragma once
#include <cstdint>
#include <sys/types.h>
#include <vector>
#include "../Resources/Texture.hpp"
#include "Object.hpp"
#include "glm/fwd.hpp"

namespace HateEngine {
    // Object -> Mesh
    class Mesh : public Object {
    private:
        std::vector<float> verticies;
        std::vector<uint32_t> indicies;
        std::vector<float> normals;
        std::vector<float> colors;
        Texture* texture = nullptr;
        std::vector<float> UV = {};
        uint8_t color_channels = 4;
        float max_light_dist = 0;
        float center_max_size = 0;
        glm::vec3 AABB_min;
        glm::vec3 AABB_max;
        bool is_shaded = true;
        // uint8_t draw_priority = 255;
        bool correct_transparency = false;
        bool face_culling = true;
        bool is_color_enabled = false;

        void updateCenterMaxSize();
        void updateAABB();

    public:
        ~Mesh();
        Mesh();
        Mesh(std::vector<float> vert, std::vector<uint32_t> ind, std::vector<float> norm);
        Mesh(const Mesh& mesh, bool copy_texture = false);

        void setVertices(std::vector<float> vec);
        void setIndicies(std::vector<uint32_t> vec);
        void setNormals(std::vector<float> vec);
        void setTexture(Texture* tex);
        void setUV(std::vector<float> UV);
        void setColors(std::vector<float> colors, uint8_t channels = 4);
        void setColor(glm::vec4 color);
        void setColor(glm::vec3 color);

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
        void enableColor();
        void disableColor();
        bool isColorEnabled() const;
        float getAABBRadius() const;
        glm::vec3 getAABBMin() const;
        glm::vec3 getAABBMax() const;
        float getAABBDistanceToPoint(glm::vec3 point) const;

        const std::vector<float>* getVertices() const;
        const std::vector<uint32_t>* getIndicies() const;
        const std::vector<float>* getNormals() const;
        Texture* getTexture() const;
        const std::vector<float>* getUV() const;
        const std::vector<float>* getColors() const;
        const uint8_t getColorChannels() const;
        // const std::vector<std::vector<float>>* getTexturesCoords();
    };
} // namespace HateEngine
