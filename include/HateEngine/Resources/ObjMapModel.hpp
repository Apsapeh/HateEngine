#pragma once
#include <cstddef>
#include <cstdint>
#include <string>
#include <sys/types.h>
#include <unordered_map>
#include "HateEngine/Objects/Interfaces/Renderable3DInterface.hpp"
#include "HateEngine/Objects/LODMesh.hpp"
#include "HateEngine/Objects/Object.hpp"
#include "HateEngine/Objects/Physics/PhysicalBody.hpp"
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
    protected:
        void _enterLevel(class Level* level) override;
        void _exitLevel(class Level* level) override;

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

        void parseHeluv(std::vector<uint8_t>& data, class HERFile* her = nullptr);

        void parseObj(
                std::string data, float grid_size, bool generate_lod, float lod_dist,
                float lod_step, class HERFile* her = nullptr
        );
        std::unordered_map<std::string, Material> parseMtlLib(
                std::string data, class HERFile* her = nullptr
        );

        void parseMap(std::string data, float grid_size);

        void parseHepvs(std::vector<uint8_t>& data);

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
        float hepvs_cell_size = 0.0f;
        glm::vec3 hepvs_min_point = {0.0f, 0.0f, 0.0f};
        glm::ivec3 hepvs_cell_count = {0, 0, 0};
        std::vector<std::vector<LODMesh*>> hepvs_table;
        std::vector<Entity> entities;
        void* entities_data = nullptr;
        void (*entities_data_deleter)(void*) = nullptr;

        std::vector<Light*> lights;
        std::vector<Object*> objects;
        std::vector<PhysicalBody*> phys_bodies;
        // TODO: Add physical bodies

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
                std::string lightmap_file_name, std::string hepvs_file_name,
                float grid_size = 16.0f, bool generate_collision = true, bool generate_lod = true,
                float lod_dist = 15, float lod_step = 1.0
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
                std::vector<uint8_t> heluv_data, std::vector<uint8_t> hepvs_data,
                float grid_size = 16.0f, bool generate_collision = true, float lod_dist = 15,
                float lod_step = 1.0
        );

        ~ObjMapModel();

        void deserializeEntities(
                const std::unordered_map<std::string, EntityDeserialzer>& entity_deserializers,
                void* data, void (*data_deleter)(void*) = nullptr
        );


        void addEntityObjectToLevel(Light* object);
        void addEntityObjectToLevel(Object* object);
        void addEntityPhysicalBodyToLevel(PhysicalBody* body);

        StaticBody* getStaticBody();
        // std::vector<LODMesh>* getLODMeshes(glm::vec3 pos);
        void render(class RenderInterface* renderer) override;
    };
} // namespace HateEngine
