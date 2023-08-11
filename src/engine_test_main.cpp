#include <iostream>
#include "Old3DEngine.h"
#include "Render/OpenGL15.h"
#include "Objects/Mesh.h"
#include "Objects/CubeMesh.h"
#include "Objects/Camera.h"
#include <cmath>


void _process(Old3DEngine::Engine*, double);
void _physics_process(Old3DEngine::Engine*, double);

Old3DEngine::CubeMesh mesh1;

int main() {
    mesh1.setPosition({-1, 0, 0});
    Old3DEngine::Camera camera(800.0/600.0, 60, 60);
    camera.setPosition(0, 0, 3);

    Old3DEngine::Engine game("Old3DE Test", 800, 600);
    game.addObjectRef(&mesh1);
    //game.setProcessLoop(_process);
    game.setFixedProcessLoop(_physics_process);
    game.setCameraRef(&camera);
    game.Run();
}



int count = 0;
double del = 0.0;
void _process(Old3DEngine::Engine* engine, double delta) {
    if (count < 1000) {
        ++count;
        del += delta;
    }
    else {
        std::cout << "FPS: " << (float)count / del << std::endl;
        count = 0;
        del = 0.0;
    }
}

void _physics_process(Old3DEngine::Engine* engine, double delta) {
    mesh1.setRotation(mesh1.getRotation() + glm::vec3 {0, 1, 0});
    mesh1.setPosition(mesh1.getPosition().x, 0.125 * sin(glfwGetTime()), mesh1.getPosition().z);
    //std::cout << "Fixed FPS: " << 1.0 / delta <<  delta<< std::endl;
}
