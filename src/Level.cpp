#include <HateEngine/Level.hpp>

#include <HateEngine/Objects/BillboardMesh.hpp>
#include <HateEngine/Objects/Light/DirectionalLight.hpp>
#include <HateEngine/Objects/Light/Light.hpp>
#include <HateEngine/Objects/Light/OmniLight.hpp>
#include <HateEngine/Objects/Light/SpotLight.hpp>
#include <HateEngine/Resources/Model.hpp>
#include <HateEngine/Objects/Particles.hpp>
#include <HateEngine/Objects/GLTFAnimationPlayer.hpp>
#include <HateEngine/Resources/ObjMapModel.hpp>
#include <HateEngine/UI/WidgetUI.hpp>
#include <HateEngine/Utilities/UUID.hpp>
#include "HateEngine/Objects/Interfaces/Renderable3DInterface.hpp"
#include "HateEngine/Objects/Interfaces/UpdatableInterface.hpp"
#include "HateEngine/Objects/Object.hpp"
#include <HateEngine/Log.hpp>

#define DESTRUCTOR_DELETE_POINTERS(ptr_obj, type)                                                  \
    for (const auto& obj: ptr_obj) {                                                               \
        if (not obj.second.is_ref)                                                                 \
            delete (type*) obj.second.obj;                                                         \
    }                                                                                              \
    ptr_obj.clear();

using namespace HateEngine;

Level::Level() {
}


#define NORMAL_EXIT_FROM_MAP(map)                                                                  \
    for (auto& obj: map) {                                                                         \
        ((Object*) obj->second)->exitLevel(this);                                                  \
    }

#define NORMAL_EXIT_FROM_MAP_POLYMORPHIC(map)                                                      \
    for (auto& obj: map) {                                                                         \
        if (auto obj = dynamic_cast<Object*>(obj.second)) {                                        \
            obj->exitLevel(this);                                                                  \
        }                                                                                          \
    }

Level::~Level() {
    for (auto& o: light) {
        o.second->_exitLevel(this);
    }

    for (auto& o: objects) {
        o.second->_exitLevel(this);
    }
}


void Level::Update(class Engine* engine, double delta) {
    for (auto& updatable: this->updatable_objects) {
        updatable.second->_process(engine, delta);
    }

    if (this->processLoop != nullptr) {
        this->processLoop(engine, delta);
    }

    _process(engine, delta);
}

void Level::UpdateFixed(class Engine* engine, double delta) {
    for (auto& updatable: this->updatable_objects) {
        updatable.second->_fixedProcess(engine, delta);
    }

    if (this->fixedProcessLoop != nullptr) {
        this->fixedProcessLoop(engine, delta);
    }

    _fixedProcess(engine, delta);
}

void Level::UpdateInput(class Engine* engine, const InputClass::InputEventInfo& event) {
    for (auto& updatable: this->updatable_objects) {
        updatable.second->_inputEvent(engine, event);
    }

    if (this->inputEvent != nullptr) {
        this->inputEvent(engine, event);
    }

    _inputEvent(engine, event);
}


std::unordered_map<UUID, Light*>* Level::getLights() {
    return &light;
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

void Level::setProcessLoop(void (*processLoop)(class Engine*, double)) {
    this->processLoop = processLoop;
}

void Level::setFixedProcessLoop(void (*fixedProcessLoop)(class Engine*, double)) {
    this->fixedProcessLoop = fixedProcessLoop;
}

void Level::setInputEvent(void (*inputEvent)(class Engine*, const InputClass::InputEventInfo&)) {
    this->inputEvent = inputEvent;
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


Level::FogMode Level::getFogMode() const {
    return this->fogMode;
}

float Level::getFogDensity() const {
    return this->fogDensity;
}

float Level::getFogStart() const {
    return this->fogStart;
}

float Level::getFogEnd() const {
    return this->fogEnd;
}

glm::vec4 Level::getFogColor() const {
    return this->fogColor;
}

glm::vec4 Level::getBackgroundColor() const {
    return this->backgroundColor;
}

glm::vec3 Level::getAmbientLightColor() const {
    return this->ambientLightColor;
}

float Level::getAmbientLightIntensity() const {
    return this->ambientLightIntensity;
}
// Settings end

void Level::updateAmbientLight() {
    this->calculatedAmbientLight[0] = this->ambientLightColor[0] * this->ambientLightIntensity;
    this->calculatedAmbientLight[1] = this->ambientLightColor[1] * this->ambientLightIntensity;
    this->calculatedAmbientLight[2] = this->ambientLightColor[2] * this->ambientLightIntensity;
}

UUID Level::addObject(WidgetUI* object) {
    this->ui_widgets[object->getUUID()] = object;
    return object->getUUID();
}

UUID Level::addObject(Light* obj) {
    // HATE_WARNING("addObject(Light*) is not implemented");
    this->light[obj->getUUID()] = obj;
    return obj->getUUID();
}

UUID Level::addObject(Object* obj) {
    if (auto renderable = dynamic_cast<Renderable3DInterface*>(obj)) {
        renderable_objects[obj->getUUID()] = renderable;
    }

    if (auto updatable = dynamic_cast<UpdatableInterface*>(obj)) {
        this->updatable_objects[obj->getUUID()] = updatable;
    }

    this->objects[obj->getUUID()] = obj;

    obj->_enterLevel(this);

    return obj->getUUID();
}


// I gues it will be removed in the future
#define DELETE_FROM_MAP(map, uuid)                                                                 \
    {                                                                                              \
        auto it = map.find(uuid);                                                                  \
        if (it != map.end()) {                                                                     \
            result = true;                                                                         \
            /*((Object*) it->second)->exitLevel(this);*/                                           \
            map.erase(it);                                                                         \
        }                                                                                          \
    }

#define DELETE_FROM_MAP_POLYMORPHIC(map, uuid)                                                     \
    {                                                                                              \
        auto it = map.find(uuid);                                                                  \
        if (it != map.end()) {                                                                     \
            result = true;                                                                         \
            if (auto obj = dynamic_cast<Object*>(it->second)) {                                    \
                obj->exitLevel(this);                                                              \
            }                                                                                      \
            map.erase(it);                                                                         \
        }                                                                                          \
    }

#define DELETE_FROM_INTERFACE_MAP(map, uuid)                                                       \
    {                                                                                              \
        auto it = map.find(uuid);                                                                  \
        if (it != map.end()) {                                                                     \
            map.erase(it);                                                                         \
        }                                                                                          \
    }

bool Level::removeObject(const UUID& uuid) {
    bool result = false;

    DELETE_FROM_MAP(this->ui_widgets, uuid);

    auto obj_it = this->objects.find(uuid);
    if (obj_it != this->objects.end()) {
        obj_it->second->_exitLevel(this);
        this->objects.erase(obj_it);

        DELETE_FROM_INTERFACE_MAP(this->renderable_objects, uuid);
        DELETE_FROM_INTERFACE_MAP(this->updatable_objects, uuid);
        result = true;
    }

    // DELETE_FROM_MAP_POLYMORPHIC(this->light, uuid);

    return result;
}
