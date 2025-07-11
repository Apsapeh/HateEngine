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
void _input_event(HateEngine::Engine*, const HateEngine::InputEventInfo&);

const int WIDTH = 800;
const int HEIGHT = 600;


int main() {
    HateEngine::Engine game("HateEngine Test", WIDTH, HEIGHT);
    game.setOneThreadMode(true);
    game.setVSync(false);

    HateEngine::Level lvl;
    HateEngine::Camera camera(60, 600);
    lvl.setCamera(&camera);

    HateEngine::CubeMesh mesh;
    mesh.setPosition(0, 0, -2);
    lvl.addObject(&mesh);

    HateEngine::Texture texture("examples/Assets/brick.png");
    mesh.setTexture(&texture);


    game.setLevel(&lvl);
    game.setProcessLoop(_process);
    game.setFixedProcessLoop(_physics_process);
    game.setInputEvent(_input_event);
    game.Run();
}


float timer = 0;
int frame_count = 0;
double cpu_time = 0;
double gpu_time = 0;
void _process(HateEngine::Engine* engine, double delta) {
    timer += delta;
    frame_count++;
    cpu_time += engine->getCPUTimeMS();
    gpu_time += engine->getGPUTimeMS();

    if (timer >= 1.0) {
        std::cout << frame_count << " frames per second (CPU: " << cpu_time / frame_count
                  << "ms, GPU: " << gpu_time / frame_count << "ms)" << std::endl;
        frame_count = 0;
        cpu_time = 0;
        gpu_time = 0;
        timer = 0;
    }
}

void _physics_process(HateEngine::Engine* engine, double delta) {
}


float lastX = 0;
float lastY = 0;
float sensitivity = 0.05;
bool is_first_iter = true;
void _input_event(HateEngine::Engine* engine, const HateEngine::InputEventInfo& event) {
    if (event.type == HateEngine::InputEventKey) {
        if (event.key == HateEngine::KeyEscape) {
            engine->Exit();
        }
    }
}
