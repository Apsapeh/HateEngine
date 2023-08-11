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
CubeMesh mesha;
CubeMesh mesha2;


GLfloat color[] = {
        0.7608, 0.9333, 0.051, 0.6549, 0.6235, 0.1647, 0.7255, 0.0157, 0.8471, 0.5098, 0.8314, 0.8627, 0.4078, 0.3216, 0.8863, 0.9098, 0.7294, 0.1804, 0.8314, 0.451, 0.5412, 0.5647, 0.4235, 0.4196
};

void OpenGL15::Draw() {
    //mesha.setPosition({0, 0, mesha.getPosition().z - 1 * 0.1});
    mesha.setPosition({-1, 0, 0});
    mesha2.setPosition({1, 0, 0});
    // TODO: Add mutex lock?
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTranslatef(mesha.getPosition().x, mesha.getPosition().z, mesha.getPosition().y);
    glRotatef(10,0.0,1,0);
    glVertexPointer(3, GL_FLOAT, 0, mesha.verticies);
    glColorPointer(3, GL_FLOAT,0, color);
    glDrawElements(GL_TRIANGLES, mesha.indiciesSize, GL_UNSIGNED_INT, mesha.indicies);

    glTranslatef(mesha2.getPosition().x, mesha2.getPosition().z, mesha2.getPosition().y);
    glRotatef(-10,0.0,1,0);
    glVertexPointer(3, GL_FLOAT, 0, mesha2.verticies);
    glColorPointer(3, GL_FLOAT,0, color);
    glDrawElements(GL_TRIANGLES, mesha2.indiciesSize, GL_UNSIGNED_INT, mesha2.indicies);


}


UUID_Generator::UUID OpenGL15::addObjectClone(Mesh mesh) {
    std::lock_guard<std::mutex> guard(meshesVecMutex);
    UUID_Generator::UUID id = uuid_generator.gen();
    Mesh *new_mesh = new Mesh;
    *new_mesh = mesh;
    meshes.push_back({new_mesh, id, false});
    return id;
}

UUID_Generator::UUID OpenGL15::addObjectRef(Mesh *mesh) {
    std::lock_guard<std::mutex> guard(meshesVecMutex);
    UUID_Generator::UUID id = uuid_generator.gen();
    meshes.push_back({mesh, id, true});
    return id;
}

bool OpenGL15::removeObject(UUID_Generator::UUID id) {
    std::lock_guard<std::mutex> guard(meshesVecMutex);
    std::vector<OGLObject>::iterator iter = std::find_if(
            meshes.begin(), meshes.end(), [&id] (OGLObject &obj) -> bool {return obj.id == id;}
    );

    if (iter == meshes.end())
        return false;

    if (not meshes[iter-meshes.begin()].is_ref)
        delete meshes[iter-meshes.begin()].mesh;

    meshes.erase(iter);
    return true;
}

void OpenGL15::eraseObjects() {
    std::lock_guard<std::mutex> guard(meshesVecMutex);
    for (OGLObject &mesh : meshes) {
        if (not mesh.is_ref)
            delete mesh.mesh;
    }
    meshes.clear();
}


