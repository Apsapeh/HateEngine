#include <Old3DEngine/Resources/GLTFModel.hpp>
#include <Old3DEngine/Error.hpp>
#include <unordered_map>
#include <iostream>

#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
#include <tiny_gltf.h>


using namespace Old3DEngine;
using tgModel = tinygltf::Model;
using tinygltf::TinyGLTF;

// DRY
static bool process_error(std::string err, std::string warn, bool loader_ret) {
    if (!warn.empty()) {
        Error::throwWarning(warn);
        return true;
    }
    if (!err.empty()) {
        Error::throwError(err);
        return true;
    }
    if (!loader_ret) {
        Error::throwWarning("Failed to parse glTF");
        return true;
    }
    return false;
}


// ========================================================================
// This code section out of class, because I don't want have a tinyGLTF lib
// as include dependencies in an app using with this engine

static void Load(tgModel& model, std::vector<Mesh*>* meshes, std::vector<Texture>* textures) {
    size_t meshes_size = 0;
    for (const auto& model_mesh : model.meshes)
        meshes_size += model_mesh.primitives.size();
    meshes->reserve(meshes_size);

    // Load Textures only with Base Color
    std::unordered_map<size_t, size_t> t_id;
    for (const auto& material : model.materials) {
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
                Error::throwWarning("Неизвестный формат");
                continue;
            }

            textures->push_back(Texture(
                image.image, image.width, image.height, t_form,
                Texture::Repeat, Texture::Linear, false
            ));
            t_id[texture_index] = textures->size()-1;
        }
    }

    // Bug fix, when vector reallocate, textures unload
    for (auto &t : *textures)
        t.setAutoload(true);



    for (const auto& model_mesh : model.meshes) {
        for (const auto& primitive : model_mesh.primitives) {
            Mesh* mesh = new Mesh();
            meshes->push_back(mesh);

            const auto& attributes = primitive.attributes;

            // =====> Get Verticies <=====
            const auto& position_accessor = model.accessors[attributes.find("POSITION")->second];
            const auto& position_view = model.bufferViews[position_accessor.bufferView];
            const auto& position_buffer = model.buffers[position_view.buffer];
            // Get verticies pointer
            const float* verticies_data = (const float*)&(position_buffer.data[
                position_view.byteOffset + position_accessor.byteOffset
            ]);
            size_t data_size = position_accessor.count;
            mesh->setVertices(std::vector<float> (verticies_data, verticies_data+data_size*3));


            // =====> Get Indicies <=====
            const auto& indices_accessor = model.accessors[primitive.indices];
            const auto& indices_view = model.bufferViews[indices_accessor.bufferView];
            const auto& indices_buffer = model.buffers[indices_view.buffer];
            // Get indicies pointer
            const uint32_t* indices_data = (const uint32_t*)&(indices_buffer.data[
                indices_view.byteOffset + indices_accessor.byteOffset
            ]);
            data_size = indices_accessor.count;
            mesh->setIndicies(std::vector<uint32_t> (indices_data, indices_data+data_size));
            //FIXME: ADD other modes
            int mode = primitive.mode;


            // =====> Get Normals <=====
            const auto& normal_accessor = model.accessors[attributes.find("NORMAL")->second];
            const auto& normal_view = model.bufferViews[normal_accessor.bufferView];
            const auto& normal_buffer = model.buffers[normal_view.buffer];
            // Get normals pointer
            const float* normals_data = (const float*)&(normal_buffer.data[
                normal_view.byteOffset + normal_accessor.byteOffset
            ]);
            data_size = normal_accessor.count;
            mesh->setNormals(std::vector<float> (normals_data, normals_data+data_size*3));


            // =====> Get UV's <=====
            if (attributes.find("TEXCOORD_0") != attributes.end()) {
                const auto& texcoord_accessor = model.accessors[attributes.find("TEXCOORD_0")->second];
                const auto& texcoord_view = model.bufferViews[texcoord_accessor.bufferView];
                const auto& texcoord_buffer = model.buffers[texcoord_view.buffer];
                // Get UV's pointer
                const float* texcoords_data = (const float*)&(texcoord_buffer.data[
                    texcoord_view.byteOffset + texcoord_accessor.byteOffset
                ]);
                data_size = texcoord_accessor.count;
                mesh->setUV(std::vector<float> (texcoords_data, texcoords_data+data_size*2));
            }


            // =====> Get Base Color <=====
            const auto& material_index = primitive.material;
            const auto& material = model.materials[material_index];
            if (material.values.find("baseColorTexture") != material.values.end()) {
                const auto& texture_index = material.values.at("baseColorTexture").TextureIndex();
                if (t_id.count(texture_index) != 0)
                    mesh->setTexture(&(*textures)[t_id[texture_index]]);
            }
        }
    }
}

// ========================================================================


GLTFModel::GLTFModel(std::string file_name) {
    tgModel model;
    TinyGLTF loader;
    std::string err;
    std::string warn;

    std::string file_ext;
    if (file_name.length() >= 4)
        file_ext = std::string(file_name.end()-4, file_name.end());
    else {
        Error::throwWarning("");
        return;
    }

    bool ret;
    if (file_ext == ".gltf")
        ret = loader.LoadASCIIFromFile(&model, &err, &warn, file_name);
    else if (file_ext == ".glb")
        ret = loader.LoadBinaryFromFile(&model, &err, &warn, file_name);

    if (process_error(err, warn, ret)) return;

    // Load model
    Load(model, &this->meshes, &this->textures);
    // Bind meshes
    bindedObjects.reserve(this->meshes.size());
    for (auto& m : this->meshes)
        bindObj((Object*)m);
}

GLTFModel::GLTFModel(const char* data, uint32_t size, std::string dir) {
    tgModel model;
    TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadASCIIFromString(&model, &err, &warn, data, size, dir);
    if (process_error(err, warn, ret)) return;

    // Load model
    Load(model, &this->meshes, &this->textures);
    // Bind meshes
    bindedObjects.reserve(this->meshes.size());
    for (auto& m : this->meshes)
        bindObj((Object*)m);
}

GLTFModel::GLTFModel(const uint8_t* data, uint32_t size, std::string dir) {
    tgModel model;
    TinyGLTF loader;
    std::string err;
    std::string warn;

    bool ret = loader.LoadBinaryFromMemory(&model, &err, &warn, data, size, dir);
    if (process_error(err, warn, ret)) return;

    // Load model
    Load(model, &this->meshes, &this->textures);
    // Bind meshes
    bindedObjects.reserve(this->meshes.size());
    for (auto& m : this->meshes)
        bindObj((Object*)m);
}

