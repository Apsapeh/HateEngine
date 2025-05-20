#pragma once
#include <cstdint>
#include <unordered_map>
#include <vector>
#include "../HateEngine.hpp"
#include "../Objects/Camera.hpp"
#include "../Objects/Light/Light.hpp"
#include "../Objects/Mesh.hpp"
#include "../Resources/Texture.hpp"
#include "../Resources/UIFont.hpp"
#include "../Objects/Interfaces/Renderable3DInterface.hpp"
#include "HateEngine/UI/WidgetUI.hpp"
#include "RenderInterface.hpp"
#include <glm/glm.hpp>
#include <vector>

#include "FrustumCuller.hpp"

namespace HateEngine {
    class OpenGL_1_3 : public RenderInterface {
    public:
        OpenGL_1_3(Engine* engine);

        void Render() override;
        void renderMesh(const Mesh* mesh) override;

        Camera* getCamera() override;

    private:
        Engine* engine = nullptr;
        uint8_t maxLightCount = 8;
        float UIScale = 1.0f;

        double UIUpdateDelay = 1.0;

        ////// Nuklear //////
        void* UINuklearBuffer_cmds;
        void* UINuklearBuffer_verts;
        void* UINuklearBuffer_idx;
        /////////////////////

        FrustumCuller frustrum;
        std::vector<const Mesh*> corrent_transparent_buffer_meshes = {};

        void initNuklearUI();

        /**
         * \brief Draw the scene
         * \param meshes  Vector of meshes
         * \param particles  Vector of particles
         * \param lights  Vector of lights
         */
        inline void Draw3D(
                Camera* camera, std::unordered_map<UUID, Renderable3DInterface*>* renderable,
                std::unordered_map<UUID, Light*>* lights
        );
        void DrawNuklearUI(std::unordered_map<UUID, WidgetUI*>* widgets);

        inline void rawRenderMesh(const Mesh* mesh, bool skip_transparency_check = false);

        /**
         * \brief Render camera. Set Prespective and ModelView matrices
         * \param camera  Camera to render
         */
        inline void renderCamera(Camera* camera);

        /**
         * \brief Render light
         * \param lights_vec  Vector of lights
         * \param indicies  Vector of indicies of the nearest lights
         */
        inline void renderLight(std::vector<Light*>* lights_vec);

        /**
         * \brief Calculate nearest lights to the object
         * \param position  Position of the object
         * \return Vector of Lights* of the nearest lights
         */
        inline std::vector<Light*> getNearestLights(const Mesh* mesh);

        static void loadTexture(Texture* texture_ptr);
        static void unloadTexture(Texture* texture_ptr);

        static void loadFont(UIFont* font_ptr);
        static void unloadFont(UIFont* font_ptr);
    };
} // namespace HateEngine
