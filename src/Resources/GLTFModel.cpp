#include <HateEngine/Log.hpp>
#include <HateEngine/Resources/GLTFModel.hpp>
#include <unordered_map>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>

using namespace HateEngine;
using tgModel = tinygltf::Model;
using tinygltf::TinyGLTF;

// DRY
static bool process_error(std::string err, std::string warn, bool loader_ret) {
    if (!warn.empty()) {
        HATE_WARNING(warn);
        return true;
    }
    if (!err.empty()) {
        HATE_ERROR(err);
        return true;
    }
    if (!loader_ret) {
        HATE_WARNING("Failed to parse glTF");
        return true;
    }
    return false;
}

// ========================================================================
// This code section out of class, because I don't want have a tinyGLTF lib
// as include dependencies in an app using with this engine

static void Load(tgModel& model, std::vector<Mesh*>* meshes, std::vector<Texture>* textures) {
    size_t meshes_size = 0;
    for (const auto& model_mesh: model.meshes)
        meshes_size += model_mesh.primitives.size();
    meshes->reserve(meshes_size);

    // Load Textures only with Base Color
    std::unordered_map<size_t, size_t> t_id;
    for (const auto& material: model.materials) {
        if (material.values.find("baseColorTexture") != material.values.end()) {
            const auto& texture_index = material.values.at("baseColorTexture").TextureIndex();
            const auto& texture = model.textures[texture_index];
            const auto& image_index = texture.source;
            const auto& image = model.images[image_index];

            Texture::TexType t_form;
            if (image.component == 3)
                t_form = Texture::TexType::RGB;
            else if (image.component == 4)
                t_form = Texture::TexType::RGBA;
            else {
                HATE_WARNING("Неизвестный формат");
                continue;
            }

            textures->push_back(
                    Texture(image.image, image.width, image.height, t_form, Texture::Repeat,
                            Texture::Linear, true, -1.0, false)
            );
            t_id[texture_index] = textures->size() - 1;
        }
    }

    // Bug fix, when vector reallocate, textures unload
    for (auto& t: *textures)
        t.setAutoload(true);

    // Load mesh properties
    std::unordered_map<int, tinygltf::Node> mesh_properties;
    for (auto const& node: model.nodes) {
        if (node.mesh != -1)
            mesh_properties[node.mesh] = node;
    }

    int model_mesh_counter = 0;
    for (const auto& model_mesh: model.meshes) {
        for (const auto& primitive: model_mesh.primitives) {
            Mesh* mesh = new Mesh();

            // model_mesh.name
            // HATE_DEBUG_F("Mesh name: %s", model_mesh.name.c_str())
            // std::cout << "Mesh name: " << model_mesh.name << std::endl;

            const auto& attributes = primitive.attributes;

            // =====> Get Verticies <=====
            const auto& position_accessor = model.accessors[attributes.find("POSITION")->second];
            const auto& position_view = model.bufferViews[position_accessor.bufferView];
            const auto& position_buffer = model.buffers[position_view.buffer];
            // Get verticies pointer
            const float* verticies_data = (const float*) &(
                    position_buffer.data[position_view.byteOffset + position_accessor.byteOffset]
            );
            size_t data_size = position_accessor.count;
            mesh->setVertices(std::vector<float>(verticies_data, verticies_data + data_size * 3));

            // =====> Get Indicies <=====
            const auto& indices_accessor = model.accessors[primitive.indices];
            const auto& indices_view = model.bufferViews[indices_accessor.bufferView];
            const auto& indices_buffer = model.buffers[indices_view.buffer];
            // Get indicies pointer
            const uint8_t* indices_data =
                    &(indices_buffer.data[indices_view.byteOffset + indices_accessor.byteOffset]);
            // FIXME: ADD other modes
            int mode = primitive.mode;
            data_size = indices_accessor.count;
            int ind_type = indices_accessor.componentType;

            // I don't know other way
#define addIndiciesToVec(type)                                                                     \
    mesh->setIndicies(std::vector<uint32_t>(                                                       \
            (const type*) indices_data, (const type*) indices_data + data_size                     \
    ))
            if (ind_type == 5120)
                addIndiciesToVec(int8_t);
            else if (ind_type == 5121)
                addIndiciesToVec(uint8_t);
            else if (ind_type == 5122)
                addIndiciesToVec(int16_t);
            else if (ind_type == 5123)
                addIndiciesToVec(uint16_t);
            else if (ind_type == 5125)
                addIndiciesToVec(uint32_t);
            else if (ind_type == 5126)
                addIndiciesToVec(float);
#undef addIndiciesToVec

            // =====> Get Normals <=====
            const auto& normal_accessor = model.accessors[attributes.find("NORMAL")->second];
            const auto& normal_view = model.bufferViews[normal_accessor.bufferView];
            const auto& normal_buffer = model.buffers[normal_view.buffer];
            // Get normals pointer
            const float* normals_data = (const float*) &(
                    normal_buffer.data[normal_view.byteOffset + normal_accessor.byteOffset]
            );
            data_size = normal_accessor.count;
            mesh->setNormals(std::vector<float>(normals_data, normals_data + data_size * 3));

            // =====> Get UV's <=====
            if (attributes.find("TEXCOORD_0") != attributes.end()) {
                const auto& texcoord_accessor =
                        model.accessors[attributes.find("TEXCOORD_0")->second];
                const auto& texcoord_view = model.bufferViews[texcoord_accessor.bufferView];
                const auto& texcoord_buffer = model.buffers[texcoord_view.buffer];
                // Get UV's pointer
                const float* texcoords_data = (const float*) &(
                        texcoord_buffer
                                .data[texcoord_view.byteOffset + texcoord_accessor.byteOffset]
                );
                data_size = texcoord_accessor.count;
                mesh->setUV(std::vector<float>(texcoords_data, texcoords_data + data_size * 2));
            }

            // =====> Get Base Color <=====
            const auto& material_index = primitive.material;
            // std::cout << "Material index: " << material_index << std::endl;
            if (material_index != -1) {
                const auto& material = model.materials[material_index];
                if (material.values.find("baseColorTexture") != material.values.end()) {
                    const auto& texture_index =
                            material.values.at("baseColorTexture").TextureIndex();
                    if (t_id.count(texture_index) != 0)
                        mesh->setTexture(&(*textures)[t_id[texture_index]]);
                }
            }

            // std::cout << "LOAD\n";

            // =====> Set Mesh Properties <=====
            if (mesh_properties.count(model_mesh_counter) != 0) {
                // std::cout << "Mesh properties: " << model_mesh.name << std::endl;

                const auto& node = mesh_properties[model_mesh_counter];
                mesh->setName(node.name);
                HATE_DEBUG_F("Mesh name: %s", node.name.c_str());

                if (node.scale.size() == 3)
                    mesh->setScale({node.scale[0], node.scale[1], node.scale[2]});
                if (node.translation.size() == 3)
                    mesh->setPosition(
                            {node.translation[0], node.translation[1], node.translation[2]}
                    );

                // HATE_DEBUG_F("Rotation size: %d", (int)node.rotation.size())
                if (node.rotation.size() == 4) {
                    glm::quat q = glm::quat(
                            node.rotation[3], node.rotation[0], node.rotation[1], node.rotation[2]
                    );
                    // to matrix
                    glm::mat4 rot = glm::mat4_cast(q);
                    mesh->setRotationMatrix(rot);
                }
                // std::cout << "Rotation size: " << node.rotation.size() << std::endl;
                //  if (node.rotation.size() == 4)
                //  mesh->setRotationMatrix({node.rotation[0], node.rotation[1],
                //  node.rotation[2], node.rotation[3]});
            }

            mesh->enableLightShading();
            meshes->push_back(mesh);
        }
        ++model_mesh_counter;
    }

    // std::cout << "Size: " << model.nodes.size() << std::endl;
}

// ========================================================================

GLTFModel::GLTFModel(std::string file_name) {
    tgModel model;
    TinyGLTF loader;
    std::string err;
    std::string warn;

    std::string file_ext;
    if (file_name.length() >= 4)
        file_ext = std::string(file_name.end() - 4, file_name.end());
    else {
        HATE_WARNING("");
        return;
    }

    bool ret;
    if (file_ext == ".gltf")
        ret = loader.LoadASCIIFromFile(&model, &err, &warn, file_name);
    else if (file_ext == ".glb")
        ret = loader.LoadBinaryFromFile(&model, &err, &warn, file_name);

    if (process_error(err, warn, ret))
        return;

    // Load model
    std::vector<Mesh*> meshes;
    Load(model, &meshes, &this->textures);

    // Bind meshes
    bindedObjects.reserve(meshes.size());
    for (auto& m: meshes)
        bindObj((Object*) m);

    addLOD(0, meshes);
    is_loaded = true;
}

GLTFModel::GLTFModel(const char* data, uint32_t size, std::string dir) {
    tgModel model;
    TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadASCIIFromString(&model, &err, &warn, data, size, dir);
    if (process_error(err, warn, ret))
        return;

    // Load model
    std::vector<Mesh*> meshes;
    Load(model, &meshes, &this->textures);
    // Bind meshes
    bindedObjects.reserve(meshes.size());
    for (auto& m: meshes)
        bindObj((Object*) m);

    addLOD(0, meshes);
    is_loaded = true;
}

GLTFModel::GLTFModel(const uint8_t* data, uint32_t size, std::string dir) {
    tgModel model;
    TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadBinaryFromMemory(&model, &err, &warn, data, size, dir);
    if (process_error(err, warn, ret))
        return;

    // Load model
    std::vector<Mesh*> meshes;
    Load(model, &meshes, &this->textures);
    // Bind meshes
    bindedObjects.reserve(meshes.size());
    for (auto& m: meshes)
        bindObj((Object*) m);

    addLOD(0, meshes);
    is_loaded = true;
}
