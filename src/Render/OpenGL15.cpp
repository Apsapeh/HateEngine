#include "OpenGL15.h"
#include <glad/gl.h>
#include <GLFW/glfw3.h>
#include <algorithm>
#include "../Objects/CubeMesh.h"

using namespace Old3DEngine;

GLfloat vertices[] = {
        -0.5, -0.5,
        0.5,  -0.5,
        0.5, 0.5,
        -0.5, 0.5
};
GLfloat vertices2[] = {
        0.25, 0.25,
        0.75, 0.25,
        0.75, 0.75,
        0.25, 0.75
};
GLfloat vertices3[] = {
        -0.25, -0.25,
        -0.75, -0.25,
        -0.75, -0.75,
        -0.25, -0.75
};

GLubyte indicies[] = {
        0, 1, 2,
        2, 3, 0
};
CubeMesh mesh;

GLfloat color[] = {
        0.7608, 0.9333, 0.051, 0.6549, 0.6235, 0.1647, 0.7255, 0.0157, 0.8471, 0.5098, 0.8314, 0.8627, 0.4078, 0.3216, 0.8863, 0.9098, 0.7294, 0.1804, 0.8314, 0.451, 0.5412, 0.5647, 0.4235, 0.4196
};

void OpenGL15::Draw() {

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glVertexPointer(3, GL_FLOAT, 0, mesh.verticies);
    glColorPointer(3, GL_FLOAT,0, color);
    glDrawElements(GL_TRIANGLES, mesh.indiciesSize, GL_UNSIGNED_INT, mesh.indicies);
    glRotatef(0.01, 0.1, 0.2, 0.3);

/*    glVertexPointer(2, GL_FLOAT, 0, vertices);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indicies);
    glVertexPointer(2, GL_FLOAT, 0, vertices2);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indicies);
    glVertexPointer(2, GL_FLOAT, 0, vertices3);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_BYTE, indicies);*/
}

UUID_Generator::UUID OpenGL15::addObject(float *buffer, uint32_t buffer_size) {
    UUID_Generator::UUID id = uuid_generator.gen();
    float* buf = new float[buffer_size];
    std::copy(buffer, buffer+buffer_size, buf);
    objects.push_back({buf, buffer_size, id});
    return id;
}

bool OpenGL15::removeObject(UUID_Generator::UUID id) {
    std::vector<OGLObject>::iterator iter = std::find_if(
        objects.begin(), objects.end(), [&id] (OGLObject &obj) -> bool {return obj.id == id;}
    );

    if (iter == objects.end())
        return false;

    delete [] objects[iter-objects.begin()].buffer;
    objects.erase(iter);
    return true;
}
