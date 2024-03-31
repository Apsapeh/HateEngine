#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <HateEngine/Objects/Camera.hpp>

using namespace HateEngine;

Camera::Camera(float view_aspect, float fov, float render_dist) {
    this->viewAspect = view_aspect;
    this->FOV = fov;
    this->renderDist = render_dist;
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

float Camera::getViewAspect() const{
    return this->viewAspect;
}

float Camera::getFOV() const {
    return this->FOV;
}

float Camera::getRenderDist() const {
    return this->renderDist;
}
