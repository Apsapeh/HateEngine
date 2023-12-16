#include <glad/gl.h>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <Old3DEngine/Objects/Camera.hpp>

using namespace Old3DEngine;

Camera::Camera(float view_aspect, float fov, float render_dist) {
    this->viewAspect = view_aspect;
    this->FOV = fov;
    this->renderDist = render_dist;
}

void Camera::renderOpenGL15() {
    glm::mat4 Mp = glm::perspective(glm::radians(FOV), viewAspect, 0.1f, renderDist);

    glMatrixMode(GL_PROJECTION);
    glLoadMatrixf(glm::value_ptr(Mp));

    glm::mat4 mat = getRotationMatrix();
    mat = glm::translate(mat, {-position.x, -position.y, -position.z} ) ;

    glMatrixMode(GL_MODELVIEW);
    glLoadMatrixf(glm::value_ptr(mat));
}

void Camera::setViewAspect(float view_aspect) {
    this->viewAspect = view_aspect;
}

void Camera::setFOV(float fov) {
    this->FOV = fov;
}

void Camera::setRenderDist(float dist) {
    this->renderDist = dist;
}

float Camera::getViewAspect(float view_aspect) {
    return this->viewAspect;
}

float Camera::getFOV(float fov) {
    return this->FOV;
}

float Camera::getRenderDist(float dist) {
    return this->renderDist;
}
