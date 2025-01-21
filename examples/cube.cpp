#include "GLFW/glfw3.h"
#include "HateEngine/AudioBus.hpp"
#include "HateEngine/Input.hpp"
#include "HateEngine/NavMeshAgent.hpp"
#include "HateEngine/Objects/Decal.hpp"
#include "HateEngine/Objects/Light/OmniLight.hpp"
#include "HateEngine/Objects/Light/SpotLight.hpp"
#include "HateEngine/Objects/Physics/CapsuleShape.hpp"
#include "HateEngine/Objects/GLTFAnimationPlayer.hpp"
#include "HateEngine/Objects/Physics/CollisionShape.hpp"
#include "HateEngine/Objects/Physics/StaticBody.hpp"
#include "HateEngine/Objects/Physics/TriggerArea.hpp"
#include "HateEngine/Resources/HENFile.hpp"
#include "HateEngine/Resources/NavMesh.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

#include "HateEngine/Objects/Light/Light.hpp"
#include "HateEngine/Objects/Object.hpp"
#include "HateEngine/Objects/Physics/BoxShape.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <HateEngine/Log.hpp>
#include <HateEngine/HateEngine.hpp>
#include <HateEngine/Objects/Camera.hpp>
#include <HateEngine/Objects/CubeMesh.hpp>
#include <HateEngine/Objects/Light/DirectionalLight.hpp>
#include <HateEngine/Objects/Mesh.hpp>
#include <HateEngine/Objects/Particles.hpp>
#include <HateEngine/Objects/BillboardMesh.hpp>
#include <HateEngine/Resources/GLTFModel.hpp>
#include <HateEngine/Resources/Texture.hpp>
#include <HateEngine/Level.hpp>
#include <HateEngine/Resources/HERFile.hpp>
#include <HateEngine/Resources/ObjMapModel.hpp>
#include <cmath>
#include <cstdint>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <HateEngine/Objects/Physics/SphereShape.hpp>
#include <HateEngine/Objects/Physics/RayCast.hpp>
#include <HateEngine/Resources/UIFont.hpp>

#include <HateEngine/UI/WidgetUI.hpp>
#include <HateEngine/UI/LabelUI.hpp>
#include <HateEngine/UI/ButtonUI.hpp>
#include <HateEngine/UI/CheckboxUI.hpp>
#include <HateEngine/UI/ImageUI.hpp>

#include <HateEngine/Resources/Audio.hpp>
#include <HateEngine/Objects/AudioPlayer.hpp>
#include <HateEngine/AudioServer.hpp>
#include <sys/types.h>

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501


void _process(HateEngine::Engine*, double);
void _physics_process(HateEngine::Engine*, double);
void _input_event(HateEngine::Engine*, HateEngine::Engine::InputEventInfo);

const int WIDTH = 800;
const int HEIGHT = 600;


int main() {
    HateEngine::Engine game("HateEngine Test", WIDTH, HEIGHT);
    game.setMouseCapture(false);
    game.setOneThreadMode(true);
    game.setVSync(false);

    HateEngine::Level lvl;
    HateEngine::Camera camera(60, 600);
    lvl.setCameraRef(&camera);

    HateEngine::CubeMesh mesh;
    mesh.setPosition(0, 0, -2);
    lvl.addObjectRef(&mesh);

    HateEngine::Texture texture("examples/Assets/brick.png");
    mesh.setTexture(&texture);


    game.setLevelRef(&lvl);
    game.setProcessLoop(_process);
    game.setFixedProcessLoop(_physics_process);
    game.setInputEvent(_input_event);
    game.Run();
}



void _process(HateEngine::Engine* engine, double delta) {

}

void _physics_process(HateEngine::Engine* engine, double delta) {

}


float lastX = 0;
float lastY = 0;
float sensitivity = 0.05;
bool is_first_iter = true;
void _input_event(HateEngine::Engine* engine, HateEngine::Engine::InputEventInfo event) {
    if (event.type == HateEngine::Engine::InputEventKey) {
        if (event.key == HateEngine::KeyEscape) {
            engine->Exit();
        }
    }
}
