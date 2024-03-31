#include <HateEngine/Error.hpp>
#include <HateEngine/HateEngine.hpp>
#include <HateEngine/Objects/Camera.hpp>
#include <HateEngine/Objects/CubeMesh.hpp>
#include <HateEngine/Objects/Light/DirectionalLight.hpp>
#include <HateEngine/Objects/Mesh.hpp>
#include <HateEngine/Objects/Particles.hpp>
#include <HateEngine/Resources/GLTFModel.hpp>
#include <HateEngine/Resources/Texture.hpp>
#include <HateEngine/Resources/Level.hpp>
#include <cmath>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>

#include <reactphysics3d/reactphysics3d.h>

using namespace reactphysics3d;

void _process(HateEngine::Engine *, double);
void _physics_process(HateEngine::Engine *, double);
void _input_event(HateEngine::Engine *, HateEngine::Engine::InputEventInfo);

PhysicsCommon physicsCommon;
PhysicsWorld *physicsWorld = physicsCommon.createPhysicsWorld();

Vector3 floor_body_vec(0, 0, 0);
Quaternion floor_body_quat = Quaternion::fromEulerAngles(0, 0, 0);
Transform floor_body_trans(floor_body_vec, floor_body_quat);
RigidBody *floor_body = physicsWorld->createRigidBody(floor_body_trans);

BoxShape *floor_col_shape = physicsCommon.createBoxShape({12.5f, 0.5f, 12.5f});
Collider *floor_col =
      floor_body->addCollider(floor_col_shape, Transform::identity());

Vector3 rbody_body_vec(0, 6, 0);
Quaternion rbody_body_quat = Quaternion::fromEulerAngles(0, 2, 2);
Transform rbody_body_trans(rbody_body_vec, rbody_body_quat);
RigidBody *rbody_body = physicsWorld->createRigidBody(rbody_body_trans);

BoxShape *rbody_col_shape = physicsCommon.createBoxShape({0.5f, 0.5f, 0.5f});
Collider *rbody_col =
      rbody_body->addCollider(rbody_col_shape, Transform::identity());

HateEngine::CubeMesh mesh1;
HateEngine::CubeMesh mesh2;
HateEngine::CubeMesh xAxMesh;

const int WIDTH = 800;
const int HEIGHT = 600;

// HateEngine::CubeMesh meshes[22500];
HateEngine::Camera camera(float(WIDTH) / float(HEIGHT), 60, 600);
HateEngine::Light sun(HateEngine::Light::DirectionalLight);
HateEngine::Particles *part;

int main() {
    std::cout << "Hello\n";
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

    HateEngine::CubeMesh floor;
    floor.setPosition(0, 0, 0);
    floor.setSize(25, 1, 25);

    HateEngine::Camera came2(0, 0, 0);

    Quaternion qufloor = floor_body->getTransform().getOrientation();
    glm::quat qefloor(qufloor.w, qufloor.z, qufloor.y, qufloor.x);
    glm::mat4 matfloor = glm::toMat4(qefloor);
    floor.setRotationMatrix(matfloor);
    // floor.matrix

    sun.setPosition({1.0, 1.0, 1.0});

    mesh2.setPosition(3, 3, 3);

    HateEngine::Engine engine("Window Name", 800, 600);
    HateEngine::Level level;
    level.addObjectRef(&sun);
    level.addObjectRef(&mesh1);
    level.setCameraRef(&camera);
    engine.setLevelRef(&level);

    // Setting textures for the cube and floor meshes

    HateEngine::Texture tex_floor("examples/Assets/ground.png");
    HateEngine::Level lvl;
    HateEngine::Engine game("Old3DE Test", WIDTH, HEIGHT);
    //    HateEngine::GLTFModel glmodel("examples/Assets/ignore/R.glb");
    HateEngine::GLTFModel glmodel("examples/Assets/employee.glb");
    HateEngine::Texture tex("examples/Assets/brick.png",
                            HateEngine::Texture::Repeat,
                            HateEngine::Texture::Nearest);

    //HateEngine::GLTFModel glmodel2("examples/Assets/ignore/bolg.glb");

    game.setLevelRef(&lvl);

    std::cout << glmodel.getGlobalPosition().x << " "
              << glmodel.getGlobalPosition().y << " "
              << glmodel.getGlobalPosition().z << "\n";

    floor.setTexture(&tex_floor);
    floor.setUV({
        0, 0, 3, 0, 3, 3, 3, 3, 0, 3, 0, 0, 0, 0, 3, 0, 3, 3, 3, 3, 0, 3, 0, 0,
        0, 0, 3, 0, 3, 3, 3, 3, 0, 3, 0, 0, 0, 0, 3, 0, 3, 3, 3, 3, 0, 3, 0, 0,
        0, 0, 3, 0, 3, 3, 3, 3, 0, 3, 0, 0, 0, 0, 3, 0, 3, 3, 3, 3, 0, 3, 0, 0,
    });
    mesh1.setTexture(&tex);
    mesh2.setTexture(&tex);
    // mesh1.bindObj(&mesh2);
    mesh1.bindObj(&glmodel);
    glmodel.bindObj(&mesh2);
    // mesh1.bindObj(&glmodel);
    lvl.addObjectRef(&mesh1);
    lvl.addObjectRef(&mesh2);
    lvl.addObjectRef(&xAxMesh);
    lvl.addObjectClone(floor);
    lvl.addObjectRef(&sun);
    lvl.addObjectRef(&glmodel);
    //lvl.addObjectClone(glmodel2);
    
    /*int poly_count = 0;
    int mesh_count = 0;
    for (const auto& m : glmodel2.getMeshes()) {
        ++mesh_count;
        poly_count += m->getIndicies()->size() / 3;
    }
    std::cout << "Poly count: " << poly_count << "\n";
    std::cout << "Mesh count: " << mesh_count << "\n";*/

    // game.addObjectRef(&tomato2);
    HateEngine::Particle::ParticleSettings pa_set = {
        6.0f, 6.0f, false, {0, 4, 0}, {4, 4, 4}};

    HateEngine::Texture snow_tex("examples/Assets/snow.png",
                                 HateEngine::Texture::Clamp,
                                 HateEngine::Texture::Linear);
    HateEngine::CubeMesh snow_mesh;
    snow_mesh.setSize(0.01, 0.01, 0.01);

    snow_mesh.setTexture(&snow_tex);
    // snow_mesh.setSize(0.8, 0.8, 0.8);
    snow_mesh.setPosition(0, 5, 0);
    lvl.addObjectRef(&snow_mesh);
    // tomato.setScale({0.05, 0.05, 0.05});
    HateEngine::Particles cube_part((HateEngine::Mesh)snow_mesh, 100, pa_set);
    /*cube_part.calculateFunc =  [] (HateEngine::Particle* p, double delta) {
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
    // std::cout << part->getPosition().z << "\n";
    // game.addObjectClone(cube_part);

    HateEngine::PhysicalBody rigidBody;
    HateEngine::PhysEngine *ph_w = game.getPhysEngine();
    ph_w->addObjectRef(&rigidBody);

    game.setProcessLoop(_process);
    game.setFixedProcessLoop(_physics_process);
    game.setInputEvent(_input_event);
    lvl.setCameraRef(&camera);
    game.Run();
}

int count = 0;
double del = 0.0;
void _process(HateEngine::Engine *engine, double delta) {
    if (count < 1000) {
      ++count;
      del += delta;
    } else {
      std::cout << "FPS: " << (float)count / del << std::endl;

      count = 0;
      del = 0.0;
    }
}

void _physics_process(HateEngine::Engine *engine, double delta) {
    // std::cout << mesh2.getGlobalPosition().x << " " <<
    // mesh2.getGlobalPosition().y << '\n';
    physicsWorld->update((float)delta / 1.0f);

    decimal dec;
    Vector3 vec;
    rbody_body->getTransform().getOrientation().getRotationAngleAxis(dec, vec);
    Quaternion qu = rbody_body->getTransform().getOrientation();
    glm::quat qe(qu.w, -qu.z, -qu.y, -qu.x);

    glm::mat4 mat = glm::toMat4(qe);
    mesh1.setRotationMatrix(mat);

    Vector3 rbodyPosVect = rbody_body->getTransform().getPosition();
    mesh1.setPosition(rbodyPosVect.z, rbodyPosVect.y, rbodyPosVect.x);

 /* if (glfwGetKey(engine->getWindow(), GLFW_KEY_ESCAPE) == GLFW_PRESS)
      glfwSetWindowShouldClose(engine->getWindow(), true);*/

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
      //glfwSetWindowMonitor(engine->getWindow(), monitor, 0, 0, 300, 600, 75);
    }

    glm::vec2 dir =
        engine->Input.getVector(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S);
    glm::vec3 cam_rot = glm::radians(camera.getRotationEuler());
    camera.offset(cos(cam_rot.y) * dir.y * 0.1, 0, -sin(cam_rot.y) * dir.y * 0.1);
}

float lastX = 0;
float lastY = 0;
float sensitivity = 0.05;
bool is_first_iter = true;
void _input_event(HateEngine::Engine *engine,
                    HateEngine::Engine::InputEventInfo event) {
    if (event.type == HateEngine::Engine::InputEventType::InputEventMouseMove) {
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
