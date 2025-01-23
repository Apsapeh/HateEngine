#include "HateEngine/Input.hpp"

#include <HateEngine/HateEngine.hpp>
#include <HateEngine/Objects/Camera.hpp>
#include <HateEngine/Objects/CubeMesh.hpp>
#include <HateEngine/Resources/Texture.hpp>
#include <HateEngine/Level.hpp>

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
