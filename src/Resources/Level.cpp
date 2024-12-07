#include <HateEngine/Resources/Level.hpp>
#include <mutex>

#include "HateEngine/Objects/BillboardMesh.hpp"
#include "HateEngine/Objects/Light/DirectionalLight.hpp"
#include "HateEngine/Objects/Light/Light.hpp"
#include "HateEngine/Objects/Light/OmniLight.hpp"
#include "HateEngine/Objects/Light/SpotLight.hpp"
#include "HateEngine/Objects/Model.hpp"
#include "HateEngine/Objects/Particles.hpp"
#include "HateEngine/Objects/GLTFAnimationPlayer.hpp"
#include "HateEngine/Resources/ObjMapModel.hpp"
#include "HateEngine/UI/WidgetUI.hpp"
#include "HateEngine/Utilities/UUID.hpp"

#define DESTRUCTOR_DELETE_POINTERS(ptr_obj, type)                                                  \
    for (const auto& obj: ptr_obj) {                                                               \
        if (not obj.second.is_ref)                                                                 \
            delete (type*) obj.second.obj;                                                         \
    }                                                                                              \
    ptr_obj.clear();

using namespace HateEngine;

Level::Level() {
}

Level::~Level() {
    DESTRUCTOR_DELETE_POINTERS(ui_widgets, WidgetUI)
    DESTRUCTOR_DELETE_POINTERS(meshes_obj, Mesh)
    DESTRUCTOR_DELETE_POINTERS(models_obj, Model)
    DESTRUCTOR_DELETE_POINTERS(animationPlayers_obj, GLTFAnimationPlayer)
    DESTRUCTOR_DELETE_POINTERS(particles_obj, Particles)

    // !!!: There may be a memory leak

    for (const auto& obj: lights_obj) {
        if (not obj.second.is_ref) {
            switch (((Light*) obj.second.obj)->getLightType()) {
                case Light::DirectionalLight:
                    delete (DirectionalLight*) obj.second.obj;
                    break;
                case Light::OmniLight:
                    delete (OmniLight*) obj.second.obj;
                    break;
                case Light::SpotLight:
                    delete (SpotLight*) obj.second.obj;
                    break;
            }
        }
    }
    lights_obj.clear();
}

void Level::Update(double delta) {
    for (auto& obj: this->particles)
        obj->Update(delta);
    for (auto& obj: this->billboards_obj)
        ((BillboardMesh*) (obj.second.obj))->Update();
}

void Level::setCameraRef(Camera* camera) {
    this->camera = camera;
}

Camera* Level::getCameraRef() {
    return this->camera;
}

void Level::removeCameraRef() {
    this->camera = nullptr;
}

void Level::setProcessLoop(void (*processLoop)(void*, double)) {
    this->processLoop = processLoop;
}

void Level::setFixedProcessLoop(void (*fixedProcessLoop)(void*, double)) {
    this->fixedProcessLoop = fixedProcessLoop;
}

PhysEngine* Level::getPhysEngine() {
    return &this->physEngine;
}


/*   Settings   */

void Level::setFogMode(FogMode mode) {
    this->fogMode = mode;
}

void Level::setFogDensity(float density) {
    this->fogDensity = density;
}

void Level::setFogStart(float start) {
    this->fogStart = start;
}

void Level::setFogEnd(float end) {
    this->fogEnd = end;
}

void Level::setFogColor(glm::vec4 color) {
    this->fogColor = color;
}

void Level::setFogColor(float r, float g, float b, float a) {
    this->setFogColor({r, g, b, a});
}

void Level::setBackgroundColor(glm::vec4 color) {
    this->backgroundColor = color;
}

void Level::setBackgroundColor(float r, float g, float b, float a) {
    this->setBackgroundColor({r, g, b, a});
}

void Level::setAmbientLightColor(glm::vec3 color) {
    this->ambientLightColor = color;
    this->updateAmbientLight();
}

void Level::setAmbientLightColor(float r, float g, float b) {
    this->setAmbientLightColor({r, g, b});
}

void Level::setAmbientLightIntensity(float intensity) {
    this->ambientLightIntensity = intensity;
    this->updateAmbientLight();
}


Level::FogMode Level::getFogMode() {
    return this->fogMode;
}

float Level::getFogDensity() {
    return this->fogDensity;
}

float Level::getFogStart() {
    return this->fogStart;
}

float Level::getFogEnd() {
    return this->fogEnd;
}

glm::vec4 Level::getFogColor() {
    return this->fogColor;
}

glm::vec4 Level::getBackgroundColor() {
    return this->backgroundColor;
}

glm::vec3 Level::getAmbientLightColor() {
    return this->ambientLightColor;
}

float Level::getAmbientLightIntensity() {
    return this->ambientLightIntensity;
}
// Settings end

void Level::updateAmbientLight() {
    this->calculatedAmbientLight[0] = this->ambientLightColor[0] * this->ambientLightIntensity;
    this->calculatedAmbientLight[1] = this->ambientLightColor[1] * this->ambientLightIntensity;
    this->calculatedAmbientLight[2] = this->ambientLightColor[2] * this->ambientLightIntensity;
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

UUID Level::addObjectClone(const Mesh& object, bool copy_tex) {
    std::lock_guard<std::mutex> guard(meshesMutex);
    Mesh* new_mesh = new Mesh(object, copy_tex);
    meshes_obj[new_mesh->getUUID()] = {new_mesh, false};
    updateMeshesVector();
    return new_mesh->getUUID();
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

UUID Level::addObjectClone(const DirectionalLight& object) {
    std::lock_guard<std::mutex> guard(lightsMutex);
    DirectionalLight* new_mesh = new DirectionalLight(object);
    lights_obj[new_mesh->getUUID()] = {new_mesh, false};
    updateLightsVector();
    return new_mesh->getUUID();
}

UUID Level::addObjectClone(const OmniLight& object) {
    std::lock_guard<std::mutex> guard(lightsMutex);
    OmniLight* new_mesh = new OmniLight(object);
    lights_obj[new_mesh->getUUID()] = {new_mesh, false};
    updateLightsVector();
    return new_mesh->getUUID();
}

UUID Level::addObjectClone(const SpotLight& object) {
    std::lock_guard<std::mutex> guard(lightsMutex);
    SpotLight* new_mesh = new SpotLight(object);
    lights_obj[new_mesh->getUUID()] = {new_mesh, false};
    updateLightsVector();
    return new_mesh->getUUID();
}

// FIXME: Wrong copy, because meshes are not copied
UUID Level::addObjectClone(const Model& object, bool copy_tex) {
    std::lock_guard<std::mutex> guard(modelsMutex);
    ;
    Model* new_mesh = new Model(object, copy_tex);
    models_obj[new_mesh->getUUID()] = {new_mesh, false};
    updateMeshesVector();
    return new_mesh->getUUID();
}

/*UUID_Generator::UUID Level::addObjectRef(Object *object) {
    UUID_Generator::UUID id = uuidGenerator.gen();
    objects.push_back({object, id, true});
    return id;
}*/

UUID Level::addObjectRef(WidgetUI* object) {
    std::lock_guard<std::mutex> guard(uiWidgetsMutex);
    ui_widgets[object->getUUID()] = {object, true};
    return object->getUUID();
}

UUID Level::addObjectRef(Mesh* object) {
    std::lock_guard<std::mutex> guard(meshesMutex);
    meshes_obj[object->getUUID()] = {object, true};
    updateMeshesVector();
    return object->getUUID();
}

UUID Level::addObjectRef(BillboardMesh* object) {
    std::lock_guard<std::mutex> guard(billboardsMutex);
    billboards_obj[object->getUUID()] = {object, true};
    updateMeshesVector();
    return object->getUUID();
}

UUID Level::addObjectRef(Light* object) {
    std::lock_guard<std::mutex> guard(lightsMutex);
    lights_obj[object->getUUID()] = {object, true};
    updateLightsVector();
    return object->getUUID();
}

UUID Level::addObjectRef(Model* object) {
    std::lock_guard<std::mutex> guard(modelsMutex);
    models_obj[object->getUUID()] = {object, true};
    updateModelsVector();
    return object->getUUID();
}

UUID Level::addObjectRef(ObjMapModel* object) {
    std::lock_guard<std::mutex> guard(modelsMutex);
    objMapModels_obj[object->getUUID()] = {object, true};
    this->physEngine.addObjectRef(object->getStaticBody());

#define ADD_OBJMAP_DEPS(type)                                                                      \
    for (const auto& obj: object->add_to_level_##type)                                             \
        addObjectRef(obj);

    this->auto_updates_enabled = false;
    ADD_OBJMAP_DEPS(meshes)
    ADD_OBJMAP_DEPS(models)
    ADD_OBJMAP_DEPS(objMapModels)
    ADD_OBJMAP_DEPS(billboards)
    ADD_OBJMAP_DEPS(animationPlayers)
    ADD_OBJMAP_DEPS(particles)
    ADD_OBJMAP_DEPS(lights)
    this->auto_updates_enabled = true;

    updateModelsVector();
    return object->getUUID();
}

UUID Level::addObjectRef(GLTFAnimationPlayer* object) {
    std::lock_guard<std::mutex> guard(animationPlayersMutex);
    animationPlayers_obj[object->getUUID()] = {object, true};
    updateAnimationPlayersVector();
    return object->getUUID();
}

UUID Level::addObjectRef(Particles* object) {
    std::lock_guard<std::mutex> guard(particlesMutex);
    particles_obj[object->getUUID()] = {object, true};
    updateParticlesVector();
    return object->getUUID();
}

bool Level::removeObject(const UUID& uuid) {
    std::lock_guard<std::mutex> ui_guard(uiWidgetsMutex);
    std::lock_guard<std::mutex> mesh_guard(meshesMutex);
    std::lock_guard<std::mutex> model_guard(modelsMutex);
    std::lock_guard<std::mutex> billboard_guard(billboardsMutex);
    std::lock_guard<std::mutex> animation_guard(animationPlayersMutex);
    std::lock_guard<std::mutex> particles_guard(particlesMutex);
    std::lock_guard<std::mutex> lights_guard(lightsMutex);

    if (ui_widgets.count(uuid) == 1) {
        if (!ui_widgets[uuid].is_ref)
            delete (WidgetUI*) ui_widgets[uuid].obj;
        ui_widgets.erase(uuid);
        // updateUIWidgetsVector();
        return true;
    }

    if (meshes_obj.count(uuid) == 1) {
        if (!meshes_obj[uuid].is_ref)
            delete (Mesh*) meshes_obj[uuid].obj;
        meshes_obj.erase(uuid);
        updateMeshesVector();
        return true;
    }

    if (models_obj.count(uuid) == 1) {
        if (!models_obj[uuid].is_ref)
            delete (Model*) models_obj[uuid].obj;
        models_obj.erase(uuid);
        updateModelsVector();
        return true;
    }

    if (objMapModels_obj.count(uuid) == 1) {
        if (!objMapModels_obj[uuid].is_ref)
            delete (ObjMapModel*) objMapModels_obj[uuid].obj;
        physEngine.removeObject(
                ((ObjMapModel*) objMapModels_obj[uuid].obj)->getStaticBody()->getUUID()
        );

#define REMOVE_OBJMAP_DEPS(type)                                                                   \
    for (const auto& obj: ((ObjMapModel*) objMapModels_obj[uuid].obj)->add_to_level_##type)        \
        removeObject(obj->getUUID());
        this->auto_updates_enabled = false;
        REMOVE_OBJMAP_DEPS(meshes)
        REMOVE_OBJMAP_DEPS(models)
        REMOVE_OBJMAP_DEPS(objMapModels)
        REMOVE_OBJMAP_DEPS(billboards)
        REMOVE_OBJMAP_DEPS(animationPlayers)
        REMOVE_OBJMAP_DEPS(particles)
        REMOVE_OBJMAP_DEPS(lights)
        this->auto_updates_enabled = true;

        objMapModels_obj.erase(uuid);
        updateModelsVector();
        return true;
    }

    if (billboards_obj.count(uuid) == 1) {
        if (!billboards_obj[uuid].is_ref)
            delete (BillboardMesh*) billboards_obj[uuid].obj;
        billboards_obj.erase(uuid);
        updateMeshesVector();
        return true;
    }

    if (animationPlayers_obj.count(uuid) == 1) {
        if (!animationPlayers_obj[uuid].is_ref)
            delete (GLTFAnimationPlayer*) animationPlayers_obj[uuid].obj;
        animationPlayers_obj.erase(uuid);
        updateAnimationPlayersVector();
        return true;
    }

    if (particles_obj.count(uuid) == 1) {
        if (!particles_obj[uuid].is_ref)
            delete (Particles*) particles_obj[uuid].obj;
        particles_obj.erase(uuid);
        updateParticlesVector();
        return true;
    }

    // !!! Memory leak
    if (lights_obj.count(uuid) == 1) {
        if (!lights_obj[uuid].is_ref)
            delete (Light*) lights_obj[uuid].obj;
        lights_obj.erase(uuid);
        updateLightsVector();
        return true;
    }

    return false;
}

/* =============> UPDATE RENDER VECTORS <============= */
#define CHECK_AUTO_UPDATES_ENABLED()                                                               \
    if (!auto_updates_enabled)                                                                     \
        return;

void Level::updateMeshesVector() {
    CHECK_AUTO_UPDATES_ENABLED()
    meshes.clear();
    meshes.reserve(this->meshes_obj.size() + this->billboards_obj.size());
    for (const auto& obj: meshes_obj)
        meshes.push_back((Mesh*) (obj.second.obj));
    for (const auto& obj: billboards_obj)
        meshes.push_back((Mesh*) obj.second.obj);
    meshes.shrink_to_fit();
}

void Level::updateModelsVector() {
    CHECK_AUTO_UPDATES_ENABLED()
    models.clear();
    models.reserve(this->models_obj.size());
    for (const auto& obj: models_obj)
        models.push_back((Model*) obj.second.obj);
    for (const auto& obj: objMapModels_obj)
        models.push_back((Model*) obj.second.obj);
    models.shrink_to_fit();
}

void Level::updateAnimationPlayersVector() {
    CHECK_AUTO_UPDATES_ENABLED()
    animationPlayers.clear();
    animationPlayers.reserve(this->animationPlayers_obj.size());
    for (const auto& obj: animationPlayers_obj)
        animationPlayers.push_back((GLTFAnimationPlayer*) obj.second.obj);
    animationPlayers.shrink_to_fit();
}

void Level::updateParticlesVector() {
    CHECK_AUTO_UPDATES_ENABLED()
    particles.clear();
    particles.reserve(this->particles_obj.size());
    for (const auto& obj: particles_obj)
        particles.push_back((Particles*) obj.second.obj);
    particles.shrink_to_fit();
}

void Level::updateLightsVector() {
    CHECK_AUTO_UPDATES_ENABLED()
    lights.clear();
    lights.reserve(this->lights_obj.size());
    for (const auto& obj: lights_obj)
        lights.push_back((Light*) obj.second.obj);
    lights.shrink_to_fit();
}
/* ===========> END UPDATE RENDER VECTORS <=========== */
