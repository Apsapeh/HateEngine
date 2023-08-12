#include <iostream>
#include <cmath>
#include "Old3DEngine.h"
#include "Objects/CubeMesh.h"
#include "Objects/Camera.h"

void _process(Old3DEngine::Engine*, double);
void _physics_process(Old3DEngine::Engine*, double);

Old3DEngine::CubeMesh mesh1;
Old3DEngine::CubeMesh meshes[400];

int main() {
    Old3DEngine::Camera camera(800.0/600.0, 60, 60);
    camera.setPosition(0, 0, 10);

    Old3DEngine::Engine game("Old3DE Test", 800, 600);
    game.addObjectRef(&mesh1);
    /*int n = 20;
    for (int a = 0; a < n; ++a) {
        for (int b = 0; b < n; ++b) {
            Old3DEngine::CubeMesh m;
            m.setPosition(-(n / 2) + b, -(n / 2) + a, 0.0);
            meshes[a*n + b] = m;
            game.addObjectRef(&meshes[a*n + b]);
        }
    }*/


    game.setProcessLoop(_process);
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
    /*float y = 0.125 * 0.125 * sin(glfwGetTime());
    glm::vec3 rot = meshes[0].getRotation() + glm::vec3 {0, 1, 0};
    for (Old3DEngine::CubeMesh &cube : meshes) {
        cube.setRotation(rot);
        cube.setPosition(cube.getPosition().x,  cube.getPosition().y + y, 0.0);
    }*/
    if (engine->Input.isKeyPressed(GLFW_KEY_LEFT_SHIFT))
        mesh1.setPosition(mesh1.getPosition() + glm::vec3 {0, 0.1, -0});
    else if (engine->Input.isKeyPressed(GLFW_KEY_LEFT_CONTROL))
        mesh1.setPosition(mesh1.getPosition() + glm::vec3 {0, -0.1, 0});

    glm::vec2 dir = engine->Input.getVector(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S);
    mesh1.setPosition(mesh1.getPosition() + glm::vec3 {dir.x * 0.1, 0, dir.y * 0.1});

    //std::cout << "Fixed FPS: " << 1.0 / delta <<  delta<< std::endl;
}
