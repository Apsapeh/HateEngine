#include "OpenGL15.h"
#include <glad/gl.h>

using namespace Old3DEngine;

GLfloat color[] = {
        0.7608, 0.9333, 0.051, 0.6549, 0.6235, 0.1647, 0.7255, 0.0157, 0.8471, 0.5098, 0.8314, 0.8627, 0.4078, 0.3216, 0.8863, 0.9098, 0.7294, 0.1804, 0.8314, 0.451, 0.5412, 0.5647, 0.4235, 0.4196
};

#include <iostream>

OpenGL15::OpenGL15(std::vector<Engine::SceneObject> *m) {
    meshes = m;
}

void OpenGL15::Draw() {
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (Engine::SceneObject &obj : *meshes) {
        Mesh *mesh = (Mesh*)obj.obj;
        if (mesh->getVisible()) {
            glm::vec3 pos = mesh->getPosition();

            glPushMatrix();
            glTranslatef(pos.x, pos.y, pos.z);
            glRotatef(mesh->getRotation().x, 1, 0, 0);
            glRotatef(mesh->getRotation().y, 0, 1, 0);
            glRotatef(mesh->getRotation().z, 0, 0, 1);

            glVertexPointer(3, GL_FLOAT, 0, &mesh->verticies[0]);
            glColorPointer(3, GL_FLOAT, 0, color);
            glDrawElements(GL_TRIANGLES, mesh->indicies.size(), GL_UNSIGNED_INT, &mesh->indicies[0]);
            glPopMatrix();
        }
    }
}


/*void OpenGL15::eraseObjects() {
    std::lock_guard<std::mutex> guard(meshesVecMutex);
    for (OGLObject &mesh : meshes) {
        if (not mesh.is_ref)
            delete mesh.mesh;
    }
    meshes.clear();
}*/


