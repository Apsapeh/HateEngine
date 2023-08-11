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

#include <iostream>

glm::vec3 prevTranslate = {0, 0, 0};
glm::vec3 prevRotate = {0, 0, 0};

OpenGL15::OpenGL15(std::vector<Engine::SceneObject> *m) {
    meshes = m;
}

void OpenGL15::Draw() {
    prevTranslate = {0, 0, 0};
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    for (Engine::SceneObject &obj : *meshes) {
        Mesh *mesh = (Mesh*)obj.obj;
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

        //glRotatef(-mesh->getRotation().y, 0, 1, 0);


        //glRotatef(-30, 0, 1, 0);

        prevTranslate = mesh->getPosition();
        prevRotate = {0, 30, 0};
    }

    /*mesha.setPosition({-1, 0, 0});
    mesha2.setPosition({2, 0, 0});
    // TODO: Add mutex lock?
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    glTranslatef(mesha.getPosition().x, mesha.getPosition().z, mesha.getPosition().y);
    glRotatef(30,0.0,1,0);
    glVertexPointer(3, GL_FLOAT, 0, mesha.verticies.data());
    glColorPointer(3, GL_FLOAT,0, color);
    glDrawElements(GL_TRIANGLES, mesha.indicies.size(), GL_UNSIGNED_INT, mesha.indicies.data());
    glRotatef(-30, 0.0, 1.0, 0.0);

    glTranslatef(mesha2.getPosition().x, mesha2.getPosition().z, mesha2.getPosition().y);
    glRotatef(30,0.0,1,0);
    glVertexPointer(3, GL_FLOAT, 0, mesha2.verticies.data());
    glColorPointer(3, GL_FLOAT,0, color);
    glDrawElements(GL_TRIANGLES, mesha2.indicies.size(), GL_UNSIGNED_INT, mesha2.indicies.data());*/


}


/*void OpenGL15::eraseObjects() {
    std::lock_guard<std::mutex> guard(meshesVecMutex);
    for (OGLObject &mesh : meshes) {
        if (not mesh.is_ref)
            delete mesh.mesh;
    }
    meshes.clear();
}*/


