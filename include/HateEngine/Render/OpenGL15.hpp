#pragma once
#include <cstdint>
#include <vector>
#include "../HateEngine.hpp"
#include "../Objects/Camera.hpp"
#include "../Objects/Light/Light.hpp"
#include "../Objects/Mesh.hpp"
#include "../Objects/Particles.hpp"
#include "../Resources/Level.hpp"
#include "../Resources/Texture.hpp"

namespace HateEngine {
    class OpenGL15 {
    public:
        OpenGL15(Engine* engine);

        void Render(

        );

    private:
        /**
         * \brief Draw the scene
         * \param meshes  Vector of meshes
         * \param particles  Vector of particles
         * \param lights  Vector of lights
         */
        inline void
        Draw3D(Camera* camera, std::vector<Mesh*>* meshes, std::vector<Particles*>* particles,
               std::vector<Light*>* lights);

        void DrawNuklearUI(std::unordered_map<UUID, Level::SceneUIWidget>* widgets);

        Engine* engine = nullptr;
        uint8_t maxLightCount = 8;
        float maxLightRenderDist = 2500; // TODO: Make it configurable

        void initNuklearUI();

        /**
         * \brief Render mesh
         * \param mesh  Mesh to render
         * \param lights_vec  Vector of lights
         */
        inline void render(const Mesh* mesh, std::vector<Light*>* lights_vec);

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
        inline void renderLight(std::vector<Light*>* lights_vec, const std::vector<int>& indicies);

        /**
         * \brief Calculate nearest lights to the object
         * \param lights_vec  Vector of lights
         * \param position  Position of the object
         * \return Vector of indicies of the nearest lights
         */
        inline std::vector<int> getNearestLights(
                std::vector<Light*>* lights_vec, glm::vec3 position, float max_light_render_dist
        );

        static void loadTexture(Texture* texture_ptr);
        static void unloadTexture(Texture* texture_ptr);
    };
} // namespace HateEngine
