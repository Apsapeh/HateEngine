#pragma once
#include <cstddef>
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include "Model.hpp"
#include "../Objects/Physics/ConvexShape.hpp"
#include "../Objects/Physics/StaticBody.hpp"
#include "../Objects/Light/Light.hpp"
#include "../Objects/BillboardMesh.hpp"
#include "../Objects/GLTFAnimationPlayer.hpp"

#include "../Objects/Particles.hpp"
#include "HateEngine/Resources/Texture.hpp"
#include "glm/fwd.hpp"

namespace HateEngine {
    class ObjMapModel : public Model {
        friend class Level;

    public:
        class Property {
            std::string value;

        public:
            Property(std::string value);
            float asFloat() const;
            int asInt() const;
            bool asBool() const;
            std::string asString();
            glm::vec3 asVec3() const;
            glm::vec3 asVec3XZY() const;
            glm::vec2 asVec2() const;
        };

        struct Entity {
            std::string classname;
            glm::vec3 position;
            std::unordered_map<std::string, Property> properties;
        };

#define HE_ENTITY_DE_PARAMS                                                                        \
    HateEngine::ObjMapModel *model, const HateEngine::ObjMapModel::Entity &entity, void *data

        typedef void (*EntityDeserialzer)(HE_ENTITY_DE_PARAMS);


    private:
        struct Material {
            size_t texture_id;
        };

        struct ObjFace {
            std::vector<int32_t> indices;
            glm::vec3 normal = {0.0f, 0.0f, 0.0f};
            std::vector<int32_t> tex_indices;
            std::vector<glm::vec2> light_tex;
        };

        struct ObjObject {
            std::string name;
            std::string material = "";
            std::vector<ObjFace> faces;
        };

        struct HeluvObj {
            std::vector<std::vector<glm::vec2>> uv;
            Texture* texture;
        };

        void parseHeluv(std::vector<uint8_t>& data);

        void parseObj(
                std::string data, float grid_size, float lod_dist, float lod_step,
                class HERFile* her = nullptr
        );
        std::unordered_map<std::string, Material> parseMtlLib(
                std::string data, class HERFile* her = nullptr
        );

        void parseMap(std::string data, float grid_size);

        std::vector<Mesh*> generateLod(
                std::vector<glm::vec3> vertices, std::vector<glm::vec2> tex_coords,
                std::vector<ObjObject> objects, float step = 1.0f
        );

        void generateCollision(std::vector<glm::vec3>& vertices, std::vector<ObjObject>& objects);

        std::string obj_file_name;
        std::string map_file_name;
        std::string obj_file_path;
        std::string heluv_file_path;
        std::unordered_map<std::string, HeluvObj> heluv;
        std::unordered_map<std::string, Material> materials;
        std::vector<Entity> entities;
        void* entities_data = nullptr;
        void (*entities_data_deleter)(void*) = nullptr;

        std::vector<Mesh*> add_to_level_meshes;
        std::vector<Model*> add_to_level_models;
        std::vector<ObjMapModel*> add_to_level_objMapModels;
        std::vector<BillboardMesh*> add_to_level_billboards;
        std::vector<GLTFAnimationPlayer*> add_to_level_animationPlayers;
        std::vector<Particles*> add_to_level_particles;
        std::vector<Light*> add_to_level_lights;

        bool generate_collision = true;
        std::vector<ConvexShape> convex_shapes;
        StaticBody static_body;

    public:
        /**
         * @brief Load model from file (.gltf/.glb)
         *
         * @param str File name
         */
        ObjMapModel(
                std::string obj_file_name, std::string map_file_name,
                std::string lightmap_file_name, float grid_size = 16.0f,
                bool generate_collision = true, float lod_dist = 15, float lod_step = 1.0
        );

        /**
         * @brief Load model from memory as ASCII string (.gltf)
         *
         * @param data ASCII string
         * @param size Data length
         * @param dir Directory
         */
        ObjMapModel(
                class HERFile* her, std::string obj_file_data, std::string map_file_data,
                float grid_size = 16.0f, bool generate_collision = true, float lod_dist = 15,
                float lod_step = 1.0
        );

        ~ObjMapModel();

        void deserializeEntities(
                const std::unordered_map<std::string, EntityDeserialzer>& entity_deserializers,
                void* data, void (*data_deleter)(void*) = nullptr
        );

        void addEntityObjectToLevel(Mesh* object);
        void addEntityObjectToLevel(BillboardMesh* object);
        void addEntityObjectToLevel(Light* object);
        void addEntityObjectToLevel(Model* object);
        void addEntityObjectToLevel(ObjMapModel* object);
        void addEntityObjectToLevel(GLTFAnimationPlayer* object);
        void addEntityObjectToLevel(Particles* object);

        StaticBody* getStaticBody();
    };
} // namespace HateEngine
