#include <HateEngine/Objects/Camera.hpp>
#include <glm/ext.hpp>
#include <glm/glm.hpp>

using namespace HateEngine;

Camera::Camera(float fov, float render_dist) {
    this->setFOV(fov);
    this->setRenderDist(render_dist);

    this->skybox.disableLightShading();
    this->skybox.setIndicies({35, 34, 33, 32, 31, 30, 29, 28, 27, 26, 25, 24,
                              23, 22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12,
                              11, 10, 9,  8,  7,  6,  5,  4,  3,  2,  1,  0});

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

UUID Camera::bindObj(Object* obj, bool bind_pos, bool bind_rot, bool bind_scale) {
    return this->root_obj.bindObj(obj, bind_pos, bind_rot, bind_scale);
}

bool Camera::unbindObj(UUID uuid) {
    return this->root_obj.unbindObj(uuid);
}

void Camera::setRotation(glm::vec3 value) {
    Object::setRotation(-value);
    // root_obj.setRotation(value);
}
void Camera::setRotation(float x, float y, float z) {
    setRotation({x, y, z});
}

void Camera::setParentPosition(glm::vec3 vec) {
    Object::setParentPosition(vec);
    root_obj.setPosition(getGlobalPosition());
}

void Camera::setParentScale(glm::vec3 vec) {
    Object::setParentScale(vec);
    // root_obj.setParentScale(vec);
}

void Camera::setParentRotationMatrix(const glm::mat4& mat) {
    // std::cout << "Camera::setParentRotationMatrix" << std::endl;
    // this->parent_rotation_matrix = glm::scale(mat, glm::vec3(-1, 1, -1));
    // this->parent_rotation_matrix = glm::inverse(mat);
    root_obj.setParentRotationMatrix(mat);
    this->parent_rotation_matrix = mat;
}

void Camera::setRotationMatrix(glm::mat4 mat) {
    Object::setRotationMatrix(mat);
    // root_obj.setRotationMatrix(mat);
}

void Camera::setPosition(const glm::vec3 value) {
    Object::setPosition(value);
    root_obj.setPosition(value);
}
void Camera::setPosition(float x, float y, float z) {
    setPosition({x, y, z});
}

void Camera::offset(glm::vec3 vec) {
    Object::offset(vec);
    root_obj.offset(vec);
}
void Camera::offset(float x, float y, float z) {
    Camera::offset({x, y, z});
}

void Camera::rotate(glm::vec3 vec, bool global) {
    Object::rotate(-vec, global);
    root_obj.rotate(vec, not global);
    // glm::vec3 rot = this->getGlobalRotationEuler();
    // root_obj.setRotation(glm::vec3(-rot.x, -rot.y, 0));
}
void Camera::rotate(float x, float y, float z, bool global) {
    rotate({x, y, z}, global);
}
