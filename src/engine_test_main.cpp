#include <iostream>
#include "Old3DEngine.h"
#include "Render/OpenGL15.h"
#include "Objects/Mesh.h"


void _process(double);
void _physics_process(double);

int main() {
    Old3DEngine::Engine game("Old3DE Test", 800, 600);
    game.setProcessLoop(_process);
    game.setFixedProcessLoop(_physics_process);
    game.Run();
    Old3DEngine::Mesh m;
}



int count = 0;
double del = 0.0;
void _process(double delta) {
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

void _physics_process(double delta) {
    //std::cout << "Fixed FPS: " << 1.0 / delta <<  delta<< std::endl;
}
