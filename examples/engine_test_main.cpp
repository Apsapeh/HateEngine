#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <Old3DEngine/Old3DEngine.hpp>
#include <Old3DEngine/Objects/CubeMesh.hpp>
#include <Old3DEngine/Objects/Camera.hpp>
#include <Old3DEngine/Objects/Light/DirectionalLight.hpp>
#include <Old3DEngine/Resources/Texture.hpp>

#include <reactphysics3d/reactphysics3d.h>

using namespace reactphysics3d;



void _process(Old3DEngine::Engine*, double);
void _physics_process(Old3DEngine::Engine*, double);
void _input_event(Old3DEngine::Engine*, Old3DEngine::Engine::InputEventInfo);


PhysicsCommon physicsCommon;
PhysicsWorld* physicsWorld = physicsCommon.createPhysicsWorld();
Vector3 posvec3(0.0, -6.0, 0.0);
Quaternion quaternion = Quaternion::identity();
Transform transform(posvec3, quaternion);
RigidBody* floor_col = physicsWorld->createRigidBody(transform);
BoxShape* boxShape = physicsCommon.createBoxShape({50.0f, 0.5f, 50.0f});
Transform box_transform = Transform::identity();
Collider* collider_box = floor_col->addCollider(boxShape, transform);

Vector3 rbodypos3(0, 0, 0);
Quaternion rbodyQuat = Quaternion::identity();
Transform rbodyTrans(rbodypos3, rbodyQuat);
RigidBody* rbody = physicsWorld->createRigidBody(rbodyTrans);


BoxShape* sphereShape = physicsCommon.createBoxShape({1, 1, 1});
Transform sphere_transform = Transform::identity();
Collider* collider = rbody->addCollider(sphereShape, rbodyTrans);


Old3DEngine::CubeMesh mesh1;
//Old3DEngine::CubeMesh meshes[22500];
Old3DEngine::Camera camera(800.0/600.0, 60, 60);
Old3DEngine::Light sun(Old3DEngine::Light::DirectionalLight);
int main() {
    //physicsWorld->setGravity({0, -0.01, 0});
    floor_col->setType(BodyType::STATIC);

    camera.setPosition(0, 0, 3);
    camera.setRotation(0, -90, 0);
    mesh1.setRotation(0, 0, 0);
    mesh1.setSize(2, 2, 2);

    Old3DEngine::CubeMesh floor;
    floor.setPosition(0, -12, 0);
    floor.setSize(25, 1, 25);

//    sun.setPosition(camera.getPosition());
    sun.setPosition({1.0, 1.0, 1.0});


    Old3DEngine::Texture tex("examples/Assets/brick.png", Old3DEngine::Texture::Repeat, Old3DEngine::Texture::Nearest);
    Old3DEngine::Engine game("Old3DE Test", 800, 600);
    floor.addTexture(&tex, {
            0, 0, 10, 0, 10, 10, 10, 10, 0, 10, 0, 0,
            0, 0, 10, 0, 10, 10, 10, 10, 0, 10, 0, 0,
            0, 0, 10, 0, 10, 10, 10, 10, 0, 10, 0, 0,
            0, 0, 10, 0, 10, 10, 10, 10, 0, 10, 0, 0,
            0, 0, 10, 0, 10, 10, 10, 10, 0, 10, 0, 0,
            0, 0, 10, 0, 10, 10, 10, 10, 0, 10, 0, 0,
    });
    mesh1.addTexture(&tex, {
            0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0,
            0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0,
            0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0,
            0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0,
            0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0,
            0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0,
    });
    game.addObjectRef(&mesh1);
    game.addObjectClone(floor);
    game.addObjectRef(&sun);
    std::cout << sizeof(Old3DEngine::Mesh) << "\n";

    /*int n = 150;
    for (int a = 0; a < n; ++a) {
        for (int b = 0; b < n; ++b) {
            Old3DEngine::CubeMesh m;
            m.addTexture(&tex, {
                    0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0,
                    0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0,
                    0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0,
                    0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0,
                    0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0,
                    0, 0, 1, 0, 1, 1, 1, 1, 0, 1, 0, 0,
            });
            m.setPosition(-(n / 2) + b, -(n / 2) + a, 0.0);
            meshes[a*n + b] = m;
            game.addObjectRef(&meshes[a*n + b]);
        }
    }*/


    game.setProcessLoop(_process);
    game.setFixedProcessLoop(_physics_process);
    game.setInputEvent(_input_event);
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
        //std::cout << "FPS: " << (float)count / del << std::endl;
        count = 0;
        del = 0.0;
    }
}



bool t = true;
void _physics_process(Old3DEngine::Engine* engine, double delta) {
    physicsWorld->update((float)delta);
    Vector3 rbodyPosVect = rbody->getTransform().getPosition();
    mesh1.setPosition(rbodyPosVect.x, rbodyPosVect.y, rbodyPosVect.z);
    //std::cout << rbodyPosVect.y << "\n";




    /*float y = 0.125 * 0.125 * sin(glfwGetTime());
    glm::vec3 rot = meshes[0].getRotation() + glm::vec3 {0, 1, 0};
    for (Old3DEngine::CubeMesh &cube : meshes) {
        //cube.setRotation(rot);
        //cube.setPosition(cube.getPosition().x,  cube.getPosition().y + y, 0.0);
    }*/
    if (glfwGetKey(engine->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS) {
        //physicsWorld.
        glfwSetWindowShouldClose(engine->getWindow(), true);
    }

    if (engine->Input.isKeyPressed(GLFW_KEY_LEFT_SHIFT)) {
        Vector3 newPos = rbodyPosVect;
        newPos.y += 0.1;
        Transform newTrans = Transform(newPos, rbody->getTransform().getOrientation());
        //rbody->setTransform(newTrans);
        rbody->setLinearVelocity({0, 10, 0});
        //mesh1.offset({0, 0.1, 0});
    }
    else if (engine->Input.isKeyPressed(GLFW_KEY_LEFT_CONTROL))
        mesh1.offset({0, -0.1, 0});

    if (engine->Input.isKeyPressed(GLFW_KEY_Q))
        mesh1.rotate({0, 0.5, 0});

    if (engine->Input.isKeyPressed(GLFW_KEY_E))
        camera.rotate(0, 0, .1);

    if (engine->Input.isKeyPressed(GLFW_KEY_R) and t) {
        Vector3 newPos = rbodyPosVect;
        newPos.y += 10;
        Transform newTrans = Transform(newPos, rbody->getTransform().getOrientation());
        rbody->setTransform(newTrans);
        t = false;
    }

    if (not engine->Input.isKeyPressed(GLFW_KEY_R))
        t = true;


    if (engine->Input.isKeyPressed(GLFW_KEY_F11)) {
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        glfwSetWindowMonitor(engine->getWindow(), monitor, 0, 0, 3440, 1440, 144);
    }

    //mesh1.rotate({1, 2, 3});

    glm::vec2 dir = engine->Input.getVector(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S);
    glm::vec3 direction;
    direction.x = cos(glm::radians(camera.getRotation().y)) * cos(glm::radians(camera.getRotation().x));
    direction.y = sin(glm::radians(camera.getRotation().x));
    direction.z = sin(glm::radians(camera.getRotation().y)) * cos(glm::radians(camera.getRotation().x));
    glm::vec3 cameraFront = glm::normalize(direction);
    camera.setPosition(camera.getPosition() + glm::vec3 {
        dir.y * 0.1 * direction.x / cos(glm::radians(camera.getRotation().x)) + dir.x * 0.1 * direction.z * -1 / cos(glm::radians(camera.getRotation().x)),
        direction.y * 0.1 * dir.y,
        dir.y * 0.1 * direction.z + dir.x * 0.1 * direction.x
    });

    std::cout << "Fixed FPS: " << 1.0 / delta <<  delta<< std::endl;
}

float lastX = 400;
float lastY = 300;
float sensitivity = 0.05;
void _input_event(Old3DEngine::Engine* engine, Old3DEngine::Engine::InputEventInfo event) {
    if (event.type == Old3DEngine::Engine::InputEventType::InputEventMouseMove) {
        float xoffset = event.position.x - lastX;
        float yoffset = lastY - event.position.y;
        lastX = event.position.x;
        lastY = event.position.y;

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        //camera.setRotation(30, -90, 0);
        float rotX = camera.getRotation().x + yoffset;
        if (rotX > 89)
            rotX = 89;
        if (rotX < -89)
            rotX = -89;
        camera.setRotation(rotX, camera.getRotation().y + xoffset, camera.getRotation().z);

        //camera.yaw += xoffset;
        //camera.pitch += yoffset;
    }
    float posit[4] = {camera.getPosition().x, camera.getPosition().y,camera.getPosition().z,1.0};
    //glLightfv(GL_LIGHT0, GL_POSITION, posit);
    //sun.setPosition(camera.getPosition());
}