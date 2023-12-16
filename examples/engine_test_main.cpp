#include <iostream>
#include <cmath>
#include <glm/glm.hpp>
#include <glm/ext.hpp>
#include <glm/gtx/quaternion.hpp>
#include <Old3DEngine/Error.hpp>
#include <Old3DEngine/Old3DEngine.hpp>
#include <Old3DEngine/Objects/CubeMesh.hpp>
#include <Old3DEngine/Objects/Mesh.hpp>
#include <Old3DEngine/Objects/Camera.hpp>
#include <Old3DEngine/Objects/Light/DirectionalLight.hpp>
#include <Old3DEngine/Resources/Texture.hpp>
#include <Old3DEngine/Resources/GLTFModel.hpp>
#include <Old3DEngine/Objects/Particles.hpp>


#include <reactphysics3d/reactphysics3d.h>

using namespace reactphysics3d;



void _process(Old3DEngine::Engine*, double);
void _physics_process(Old3DEngine::Engine*, double);
void _input_event(Old3DEngine::Engine*, Old3DEngine::Engine::InputEventInfo);


PhysicsCommon physicsCommon;
PhysicsWorld* physicsWorld = physicsCommon.createPhysicsWorld();

Vector3 floor_body_vec(0, 0, 0);
Quaternion floor_body_quat = Quaternion::fromEulerAngles(0, 0, 0);
Transform floor_body_trans(floor_body_vec, floor_body_quat);
RigidBody* floor_body = physicsWorld->createRigidBody(floor_body_trans);

BoxShape* floor_col_shape = physicsCommon.createBoxShape({12.5f, 0.5f, 12.5f});
Collider* floor_col = floor_body->addCollider(floor_col_shape, Transform::identity());


Vector3 rbody_body_vec(0, 6, 0);
Quaternion rbody_body_quat = Quaternion::fromEulerAngles(0, 2, 2);
Transform rbody_body_trans(rbody_body_vec, rbody_body_quat);
RigidBody* rbody_body = physicsWorld->createRigidBody(rbody_body_trans);

BoxShape* rbody_col_shape = physicsCommon.createBoxShape({0.5f, 0.5f, 0.5f});
Collider* rbody_col = rbody_body->addCollider(rbody_col_shape, Transform::identity());


Old3DEngine::CubeMesh mesh1;
Old3DEngine::CubeMesh mesh2;
Old3DEngine::CubeMesh xAxMesh;


//Old3DEngine::CubeMesh meshes[22500];
Old3DEngine::Camera camera(800.0/600.0, 60, 60);
Old3DEngine::Light sun(Old3DEngine::Light::DirectionalLight);
Old3DEngine::Particles *part;

int main() {
    std::string popusk = "file.gltf";
    // Old3DEngine::GLTFModel glmodel("examples/Assets/employee.glb");
    // Old3DEngine::GLTFModel glmodel("examples/Assets/ignore/ahouse.glb");
    Old3DEngine::GLTFModel glmodel("examples/Assets/ignore/tree.glb");
    //glmodel.setScale({1, -1, 1});
    glmodel.rotate(180, 0, 0);
    // Old3DEngine::GLTFModel glmodel("examples/Assets/ignore/dodge.glb");
    glmodel.offset(0, 1, 0);
    //glmodel.setScale({0.1, 0.1, 0.1});
    //glmodel.setScale({10, 10, 10});


    std::string path = "examples/Assets";
    //rbody_body->updateMassPropertiesFromColliders();
    rbody_col->getMaterial().setBounciness(0);
    rbody_col->getMaterial().setFrictionCoefficient(1);
    floor_body->setType(BodyType::STATIC);
    floor_col->getMaterial().setBounciness(0);
    floor_col->getMaterial().setFrictionCoefficient(1);

    xAxMesh.setSize(1, 0.1, 0.1);
    xAxMesh.offset(0, 6, 0);

    camera.setPosition(0, 6, 3);
    camera.setRotation(0, 0, 0);
    mesh1.setRotation(0, 10, 0);
    mesh1.setSize(1, 1, 1);

    Old3DEngine::CubeMesh floor;
    floor.setPosition(0, 0, 0);
    floor.setSize(25, 1, 25);



    Old3DEngine::Camera came2(0, 0, 0);


    Quaternion qufloor = floor_body->getTransform().getOrientation();
    glm::quat qefloor(qufloor.w, qufloor.z, qufloor.y, qufloor.x);
    glm::mat4 matfloor = glm::toMat4(qefloor);
    floor.setRotationMatrix(matfloor);
    //floor.matrix


    sun.setPosition({1.0, 1.0, 1.0});

    mesh2.setPosition(0, 0, 3);


    // Setting textures for the cube and floor meshes
    Old3DEngine::Texture tex_floor("examples/Assets/ground.png", Old3DEngine::Texture::Repeat, Old3DEngine::Texture::Linear);
    Old3DEngine::Texture tex("examples/Assets/brick.png", Old3DEngine::Texture::Repeat, Old3DEngine::Texture::Nearest);
    Old3DEngine::Engine game("Old3DE Test", 800, 600);




    floor.setTexture(&tex_floor);
    floor.setUV({
            0, 0, 3, 0, 3, 3, 3, 3, 0, 3, 0, 0,
            0, 0, 3, 0, 3, 3, 3, 3, 0, 3, 0, 0,
            0, 0, 3, 0, 3, 3, 3, 3, 0, 3, 0, 0,
            0, 0, 3, 0, 3, 3, 3, 3, 0, 3, 0, 0,
            0, 0, 3, 0, 3, 3, 3, 3, 0, 3, 0, 0,
            0, 0, 3, 0, 3, 3, 3, 3, 0, 3, 0, 0,
    });
    mesh1.setTexture(&tex);
    mesh2.setTexture(&tex);
    mesh1.bindObj(&mesh2);
    game.addObjectRef(&mesh1);
    game.addObjectRef(&mesh2);
    game.addObjectRef(&xAxMesh);
    game.addObjectClone(floor);
    game.addObjectRef(&sun);

    for (auto &m : glmodel.getMeshes()) {
        game.addObjectRef(m);
    }


    //game.addObjectRef(&tomato2);
    Old3DEngine::Particle::ParticleSettings pa_set = {
            6.0f, 6.0f, false,
            {0, 4, 0}, {4, 4, 4}
    };

    Old3DEngine::Texture snow_tex("examples/Assets/snow.png", Old3DEngine::Texture::Clamp, Old3DEngine::Texture::Linear);
    Old3DEngine::CubeMesh snow_mesh;
    snow_mesh.setSize(0.01, 0.01, 0.01);

    snow_mesh.setTexture(&snow_tex);
    //snow_mesh.setSize(0.8, 0.8, 0.8);
    snow_mesh.setPosition(0, 5, 0);
    game.addObjectRef(&snow_mesh);
    //tomato.setScale({0.05, 0.05, 0.05});
    Old3DEngine::Particles cube_part((Old3DEngine::Mesh)snow_mesh, 100, pa_set);
    /*cube_part.calculateFunc =  [] (Old3DEngine::Particle* p, double delta) {
        if (p->data.count("vel") == 0)
            p->data["vel"] = (void*) new glm::vec3(0, 0, 0);
        //((glm::vec3*)(p->data["vel"]))->y += delta * -9.8;
        ((glm::vec3*)(p->data["vel"]))->y -= delta * 9.8;
        //if (p->index == 0)
            //std::cout << ((glm::vec3*)(p->data["vel"]))->y << "\n";
            //std::cout << float(delta * 9.8) << "\n";
        //std::cout << ((glm::vec3*)(p->data["vel"]))->y << "\n";
        glm::vec3 off = *((glm::vec3*)(p->data["vel"])) * glm::vec3(delta);
        //std::cout << off << "\n";
        //p->offset(off);
        p->offset({0, -0.25 * delta, 0});

        //std::cout <<
    };*/

    cube_part.setPosition(0, 3, 0);
    cube_part.pause = true;
    //std::cout << part->getPosition().z << "\n";
    //game.addObjectClone(cube_part);


    Old3DEngine::PhysicalBody rigidBody;
    Old3DEngine::PhysEngine* ph_w = game.getPhysEngine();
    ph_w->addObjectRef(&rigidBody);


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
        std::cout << "FPS: " << (float)count / del << std::endl;
        count = 0;
        del = 0.0;
    }
}



void _physics_process(Old3DEngine::Engine* engine, double delta) {
    physicsWorld->update((float) delta / 1.0f);

    decimal dec;
    Vector3 vec;
    rbody_body->getTransform().getOrientation().getRotationAngleAxis(dec, vec);
    Quaternion qu = rbody_body->getTransform().getOrientation();
    glm::quat qe(qu.w, -qu.z, -qu.y, -qu.x);

    glm::mat4 mat = glm::toMat4(qe);
    mesh1.setRotationMatrix(mat);

    Vector3 rbodyPosVect = rbody_body->getTransform().getPosition();
    mesh1.setPosition(rbodyPosVect.z, rbodyPosVect.y, rbodyPosVect.x);

    if (glfwGetKey(engine->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(engine->getWindow(), true);

    if (engine->Input.isKeyPressed(GLFW_KEY_LEFT_SHIFT))
        rbody_body->setLinearVelocity({0, 10, 0});
    if (engine->Input.isKeyPressed(GLFW_KEY_UP))
        rbody_body->setLinearVelocity({0, -1, 0});

    if (engine->Input.isKeyPressed(GLFW_KEY_LEFT_CONTROL))
        camera.offset(0, -0.1, 0);
    if (engine->Input.isKeyPressed(GLFW_KEY_SPACE))
        camera.offset(0, 0.1, 0);

    if (engine->Input.isKeyPressed(GLFW_KEY_Q))
        mesh2.rotate({0.0, 1, 0.0});

    if (engine->Input.isKeyPressed(GLFW_KEY_E))
        camera.rotate(0, 0.5, 0);

    if (engine->Input.isKeyPressed(GLFW_KEY_T))
        xAxMesh.rotate(0, 0, 0);



    if (engine->Input.isKeyPressed(GLFW_KEY_P)) {
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        glfwSetWindowMonitor(engine->getWindow(), monitor, 0, 0, 300, 600, 75);
    }

    glm::vec2 dir = engine->Input.getVector(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S);
    glm::vec3 cam_rot = glm::radians(camera.getRotationEuler());
    camera.offset(cos(cam_rot.y) * dir.y * 0.1,
                  0,
                  -sin(cam_rot.y) * dir.y * 0.1);
}

float lastX = 0;
float lastY = 0;
float sensitivity = 0.05;
bool is_first_iter = true;
void _input_event(Old3DEngine::Engine* engine, Old3DEngine::Engine::InputEventInfo event) {
    if (event.type == Old3DEngine::Engine::InputEventType::InputEventMouseMove) {
        float xoffset = event.position.x - lastX;
        float yoffset = event.position.y - lastY;
        lastX = event.position.x;
        lastY = event.position.y;

        xoffset *= sensitivity;
        yoffset *= sensitivity;

        if (is_first_iter) {
            xoffset = 0;
            yoffset = 0;
            is_first_iter = false;
        }

        camera.rotate(0, -xoffset, 0);
        camera.rotate(-yoffset, 0, 0, false);
    }
}
