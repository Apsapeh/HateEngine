#include <HateEngine/Objects/Camera.hpp>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include "glm/fwd.hpp"

using namespace HateEngine;

Camera::Camera(float fov, float render_dist) {
    this->setFOV(fov);
    this->setRenderDist(render_dist);

    this->skybox.disableLightShading();
    this->skybox.setIndicies({35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, 22, 21, 20, 19, 18,
                              17, 16, 15, 14, 13, 12, 11, 10, 9,  8,  7,  6,  5,  4,  3,  2,  1,  0});

    struct CubeMapUVSide {
        float x0, y0, x1, y1;
    };

    CubeMapUVSide left = {0, 1.0 / 3, 1.0 / 4, 2.0 / 3};
    CubeMapUVSide front = {1.0 / 4, 1.0 / 3, 2.0 / 4, 2.0 / 3};
    CubeMapUVSide right = {2.0 / 4, 1.0 / 3, 3.0 / 4, 2.0 / 3};
    CubeMapUVSide back = {3.0 / 4, 1.0 / 3, 1.0, 2.0 / 3};
    CubeMapUVSide top = {1.0 / 4, 0.0, 2.0 / 4, 1.0 / 3};
    CubeMapUVSide bottom = {1.0 / 4, 2.0 / 3, 2.0 / 4, 1.0};

    this->skybox.setUV({
            back.x1,   back.y1,   back.x0,   back.y1,   back.x0,   back.y0,   back.x0,   back.y0,
            back.x1,   back.y0,   back.x1,
            back.y1, // Зад, право, перед, лево, верх низ
            right.x1,  right.y1,  right.x0,  right.y1,  right.x0,  right.y0,  right.x0,  right.y0,
            right.x1,  right.y0,  right.x1,  right.y1,  front.x0,  front.y1,  front.x0,  front.y0,
            front.x1,  front.y0,  front.x1,  front.y0,  front.x1,  front.y1,  front.x0,  front.y1,
            left.x1,   left.y1,   left.x0,   left.y1,   left.x0,   left.y0,   left.x0,   left.y0,
            left.x1,   left.y0,   left.x1,   left.y1,   top.x0,    top.y0,    top.x1,    top.y0,
            top.x1,    top.y1,    top.x1,    top.y1,    top.x0,    top.y1,    top.x0,    top.y0,
            bottom.x0, bottom.y1, bottom.x0, bottom.y0, bottom.x1, bottom.y0, bottom.x1, bottom.y0,
            bottom.x1, bottom.y1, bottom.x0, bottom.y1,
    });

    /*
    this->skybox.setUV({
        0, 0, -1, 0, -1, -1, -1, -1, 0, -1, 0, 0,   // Зад, право, перед, лево,
    верх низ 0, 0, -1, 0, -1, -1, -1, -1, 0, -1, 0, 0, 0, 1,  0, 0,  1,  0,  1,
    0, 1,  1, 0, 1, 0, 0, -1, 0, -1, -1, -1, -1, 0, -1, 0, 0, 0, 0,  1, 0,  1,
    1, 1,  1, 0,  1, 0, 0, 0, 1,  0, 0,  1,  0,  1,  0, 1,  1, 0, 1,
    });
     */
    this->bindObj(&this->skybox, true, false, false);
}

void Camera::setFOV(float fov) {
    this->FOV = fov;
}

void Camera::setRenderDist(float dist) {
    this->renderDist = dist;
    float sb_size = dist * 2 / std::sqrt(3);
    this->skybox.setSize(sb_size, sb_size, sb_size);
}

void Camera::setSkyBoxTexture(Texture* tex) {
    this->skybox.setTexture(tex);
}

void Camera::setSkyBoxEnabled(bool enable) {
    this->skyboxEnabled = enable;
}

float Camera::getFOV() const {
    return this->FOV;
}

float Camera::getRenderDist() const {
    return this->renderDist;
}

const CubeMesh* Camera::getSkyBox() const {
    return &this->skybox;
}

bool Camera::isSkyBoxEnabled() const {
    return this->skyboxEnabled;
}


glm::mat4 Camera::getProjectionMatrix(float aspect_ratio) {
    if (this->prev_aspect_ratio != aspect_ratio) {
        this->prev_aspect_ratio = aspect_ratio;
        this->projectionMatrix =
                glm::perspective(glm::radians(this->FOV), aspect_ratio, 0.1f, this->renderDist);
    }
    return this->projectionMatrix;
}

glm::mat4 Camera::getViewMatrix() const {
    // return this->viewMatrix;
    glm::mat4 mat = this->getGlobalRotationMatrix();
    mat = glm::transpose(mat); // Invert rotation matrix
    // mat = glm::mat4(1.0f);
    mat = glm::translate(mat, -this->getGlobalPosition());
    return mat;
}


glm::vec3 Camera::getProjectRayFromScreen(glm::vec2 pos, glm::vec2 screen) {
    glm::vec3 rayNDC;
    rayNDC.x = (2.0f * pos.x) / screen.x - 1.0f;
    rayNDC.y = 1.0f - (2.0f * pos.y) / screen.y; // Инверсия оси Y
    rayNDC.z = 1.0f; // Глубина для "дальнего плана"

    // 2. Преобразуем из NDC в координаты камеры (view space)
    glm::vec4 rayClip(rayNDC.x, rayNDC.y, -1.0f, 1.0f); // -1.0f для задней плоскости
    glm::vec4 rayEye = glm::inverse(getProjectionMatrix(screen.x / screen.y)) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f); // Устанавливаем w = 0 для направления

    // 3. Преобразуем из координат камеры в мировое пространство
    glm::vec4 rayWorld = glm::inverse(getViewMatrix()) * rayEye;
    glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));

    return rayDirection;
}
