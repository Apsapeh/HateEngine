#include <HateEngine/Resources/Level.hpp>
#include "../globalStaticParams.hpp"

using namespace HateEngine;

Level::Level() {

}

Level::~Level() {

}


void Level::setCameraRef(Camera* camera) {
    this->camera = camera;
}

void Level::removeCameraRef() {
    this->camera = nullptr;
}




// TODO: Add mutexes
// TODO: Add Particles, Model

/*UUID_Generator::UUID Level::addObjectClone(Object object) {
    UUID_Generator::UUID id = uuidGenerator.gen();
    Object *new_mesh = new Object;
    *new_mesh = object;
    objects.push_back({new_mesh, id, false});
    return id;
}*/


UUID_Generator::UUID Level::addObjectClone(const Mesh& object, bool copy_tex) {
    std::lock_guard<std::mutex> guard(meshesMutex);
    const UUID_Generator::UUID id = global_uuid_generator.gen();
    Mesh* new_mesh = new Mesh(object, copy_tex);
    meshes_obj[id] = {new_mesh, false};
    updateMeshesVector();
    return id;
}

/*UUID_Generator::UUID Level::addObjectClone(Particles object) {
    std::lock_guard<std::mutex> guard(meshesMutex);
    UUID_Generator::UUID id = uuidGenerator.gen();
    Particles *new_particles = new Particles(std::move(object));

    //delete new_mesh;
    // *new_mesh = std::move(object);
    particles.push_back({new_particles, id, false});
    return id;
}*/

UUID_Generator::UUID Level::addObjectClone(const Light& object) {
    std::lock_guard<std::mutex> guard(lightsMutex);
    const UUID_Generator::UUID id = global_uuid_generator.gen();
    Light* new_mesh = new Light(object.getLightType());
    *new_mesh = object;
    lights_obj[id] = {new_mesh, false};
    updateLightsVector();
    return id;
}

// FIX IT: Wrong copy, because meshes are not copied
UUID_Generator::UUID Level::addObjectClone(const Model& object, bool copy_tex) {
    std::lock_guard<std::mutex> guard(modelsMutex);
    const UUID_Generator::UUID id = global_uuid_generator.gen();
    Model* new_mesh = new Model(object, copy_tex);
    models_obj[id] = {new_mesh, false};
    updateMeshesVector();
    return id;
}

/*UUID_Generator::UUID Level::addObjectRef(Object *object) {
    UUID_Generator::UUID id = uuidGenerator.gen();
    objects.push_back({object, id, true});
    return id;
}*/

UUID_Generator::UUID Level::addObjectRef(Mesh *object) {
    std::lock_guard<std::mutex> guard(meshesMutex);
    const UUID_Generator::UUID id = global_uuid_generator.gen();
    meshes_obj[id] = {object, false};
    updateMeshesVector();
    return id;
}

UUID_Generator::UUID Level::addObjectRef(Light *object) {
    std::lock_guard<std::mutex> guard(lightsMutex);
    UUID_Generator::UUID id = global_uuid_generator.gen();
    lights_obj[id] = {object, false};
    updateLightsVector();
    return id;
}

UUID_Generator::UUID Level::addObjectRef(Model *object) {
    std::lock_guard<std::mutex> guard(modelsMutex);
    const UUID_Generator::UUID id = global_uuid_generator.gen();
    models_obj[id] = {object, false};
    updateMeshesVector();
    return id;
}

bool Level::removeObject(const UUID_Generator::UUID& uuid) {
    std::lock_guard<std::mutex> mesh_guard(meshesMutex);
    std::lock_guard<std::mutex> model_guard(modelsMutex);
    std::lock_guard<std::mutex> particles_guard(particlesMutex);
    std::lock_guard<std::mutex> lights_guard(lightsMutex);

    if (meshes_obj.count(uuid) == 1) {
        if (!meshes_obj[uuid].is_ref)
            delete meshes_obj[uuid].obj;
        meshes_obj.erase(uuid);
        return true;
    }

    if (models_obj.count(uuid) == 1) {
        if (!models_obj[uuid].is_ref)
            delete models_obj[uuid].obj;
        models_obj.erase(uuid);
        return true;
    }

    if (particles_obj.count(uuid) == 1) {
        if (!particles_obj[uuid].is_ref)
            delete particles_obj[uuid].obj;
        particles_obj.erase(uuid);
        return true;
    }

    if (lights_obj.count(uuid) == 1) {
        if (!lights_obj[uuid].is_ref)
            delete lights_obj[uuid].obj;
        lights_obj.erase(uuid);
        return true;
    }

    return false;
}



/* =============> UPDATE RENDER VECTORS <============= */
void Level::updateMeshesVector() {
    meshes.clear();
    meshes.reserve(this->meshes_obj.size() + 5*this->models_obj.size());
    for (const auto& obj : meshes_obj)
        meshes.push_back((Mesh*)(obj.second.obj));
    for (const auto& obj : models_obj) {
        auto model_meshes = ((Model*)obj.second.obj)->getMeshes();
        meshes.insert(meshes.end(), model_meshes.begin(), model_meshes.end());
    }
    meshes.shrink_to_fit();
}

void Level::updateParticlesVector() {
    particles.clear();
    particles.reserve(this->particles_obj.size());
    for (const auto& obj : particles_obj)
        particles.push_back((Particles*)obj.second.obj);
    particles.shrink_to_fit();
}

void Level::updateLightsVector() {
    lights.clear();
    lights.reserve(this->lights_obj.size());
    for (const auto& obj : lights_obj)
        lights.push_back((Light*)obj.second.obj);
    lights.shrink_to_fit();
}
/* ===========> END UPDATE RENDER VECTORS <=========== */