#include "HateEngine/Objects/Light/Light.hpp"
#include "HateEngine/Objects/Object.hpp"
#include "HateEngine/Objects/Physics/BoxShape.hpp"
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
#include <HateEngine/Resources/HERFile.hpp>
#include <cmath>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <HateEngine/Objects/Physics/SphereShape.hpp>

#include <HateEngine/UI/WidgetUI.hpp>
#include <HateEngine/UI/LabelUI.hpp>



void _process(HateEngine::Engine *, double);
void _physics_process(HateEngine::Engine *, double);
void _input_event(HateEngine::Engine *, HateEngine::Engine::InputEventInfo);

HateEngine::CubeMesh mesh1;
HateEngine::CubeMesh mesh2;
HateEngine::CubeMesh xAxMesh;
//HateEngine::GLTFModel glmodel("examples/Assets/employee.glb");
HateEngine::GLTFModel glmodel("examples/Assets/billy-plane-sep.glb");

const int WIDTH = 800;
const int HEIGHT = 600;

// HateEngine::CubeMesh meshes[22500];
HateEngine::Camera camera(60, 600);
HateEngine::Light sun(HateEngine::Light::DirectionalLight);
HateEngine::Light light(HateEngine::Light::OmniLight);

HateEngine::Particles *part;

HateEngine::LabelUI fps_label;
HateEngine::WidgetUI* fps_widget_ptr = nullptr;
HateEngine::CubeMesh test2;
int main() {
    std::cout << "Hello\n";

    xAxMesh.setSize(1, 0.1, 0.1);
    xAxMesh.offset(0, 6, 0);

    camera.setPosition(0, 6, 3);
    //camera.setPosition(0, 25, 0);
    camera.setRotation(0, 0, 0);
    camera.setSkyBoxTexture(new HateEngine::Texture("examples/Assets/skybox.jpg", HateEngine::Texture::ClampToEdge));
    camera.setSkyBoxEnabled(true);
    mesh1.setRotation(0, 0, 0);
    mesh1.setSize(1, 1, 1);

    HateEngine::CubeMesh floor;
    floor.setPosition(0, 0, 0);
    floor.setSize(25, 1, 25);
    //floor.setRotation(20, 0, 0);


    //floor.setRotationMatrix(matfloor);
    // floor.matrix

    sun.setPosition({1.0, 1.0, 1.0});

    mesh2.setPosition(3, 3, 3);

    HateEngine::Engine game("HateEngine Test", WIDTH, HEIGHT);
    game.setMouseCapture(true);
    // Setting textures for the cube and floor meshes
    
    
    HateEngine::HERFile herfile("examples/Assets/test.her", "password");
    HateEngine::Texture tex_floor = herfile["ground.png"].asTexture();
    
    

    //HateEngine::Texture tex_floor("examples/Assets/ground.png");
    HateEngine::Level lvl;
    //    HateEngine::GLTFModel glmodel("examples/Assets/ignore/R.glb");
    //HateEngine::GLTFModel glmodel("examples/Assets/employee.glb");
    HateEngine::Texture tex("examples/Assets/brick.png",
                            HateEngine::Texture::Repeat,
                            HateEngine::Texture::Nearest);

    //HateEngine::GLTFModel glmodel2("examples/Assets/ignore/bolg.glb");

    game.setLevelRef(&lvl);
    // HateEngine::CubeMesh test_mesh;
    // HateEngine::Level level2;
    // level2.addObjectRef(&test2);
    // level2.setCameraRef(&camera);
    // game.setLevelRef(&level2);
    

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
    //mesh2.setTexture(&tex);
    //mesh1.bindObj(&mesh2);
    //mesh1.bindObj(&glmodel);


    //glmodel.bindObj(&mesh2);
    //glmodel.bindObj(&camera);
    // mesh1.bindObj(&glmodel);

    lvl.addObjectRef(&mesh1);
    //lvl.addObjectRef(&mesh2);
    lvl.addObjectRef(&xAxMesh);
    lvl.addObjectRef(&floor);
    lvl.addObjectRef(&sun);
    lvl.addObjectRef(&glmodel);
    
    //print glmodel meshes id
    
    
    glmodel.offset({30, 0, 30});
    //glmodel.getMeshes()[0]->disableLightShading();
    
    //light.setPosition({0, 2, 0});
    //l
    lvl.addObjectRef(&light);
    //light.color = {10, 10, 10, 1};
    
    camera.bindObj(&light);
    
    lvl.setFixedProcessLoop([](void *engine, double delta) {
        ////TODO: Change engine pointer to struct with Engine*, Level* 
        HateEngine::Engine *_engine = (HateEngine::Engine *)engine;
       // _engine->getLevel()->ob
        //std::cout << "Level fixed process loop dealay: " << delta << "\n";
        //_process(engine, delta);
    });

    // game.addObjectRef(&tomato2);
    HateEngine::Particle::ParticleSettings pa_set = {
        6.0f, 6.0f, false, {0, 4, 0}, {4, 4, 4}
    };

    HateEngine::Texture snow_tex("examples/Assets/snow.png",
                                 HateEngine::Texture::Clamp,
                                 HateEngine::Texture::Linear);
    HateEngine::CubeMesh snow_mesh;
    snow_mesh.setSize(0.01, 0.01, 0.01);

    snow_mesh.setTexture(&snow_tex);
    // snow_mesh.setSize(0.8, 0.8, 0.8);
    snow_mesh.setPosition(0, 5, 0);
    //lvl.addObjectRef(&snow_mesh);
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

    HateEngine::PhysicalBody rigidBody(HateEngine::PhysicalBody::DynamicBody);
    rigidBody.setPosition(0, 5, 0);
    rigidBody.rotate(48, 22,36);
    //rigidBody.setRotation(0, 0, 0);
    HateEngine::BoxShape   boxShape({1, 1, 1});
    rigidBody.addCollisionShapeRef(&boxShape);
    HateEngine::SphereShape sphereShape(0.5);
    rigidBody.addCollisionShapeRef(&sphereShape);
    rigidBody.bindObj(&mesh1);
    //rigidBody.bindObj(&glmodel);

    HateEngine::PhysicalBody floorBody(HateEngine::PhysicalBody::StaticBody);
    floorBody.setPosition(0, 0, 0);
    HateEngine::BoxShape floorShape({25, 1, 25}, {0, 0, 0}, {0, 0, 0});
    floorBody.addCollisionShapeRef(&floorShape);
    //floorBody.rotate(20, 0, 0);
    floorBody.bindObj(&floor);

    lvl.getPhysEngine()->addObjectRef(&rigidBody);
    lvl.getPhysEngine()->addObjectRef(&floorBody);



    // UI TEST
    HateEngine::WidgetUI ui;
    ui.has_border = true;
    ui.has_title = true;
    ui.position = {50, 0, 1, HateEngine::CoordsUI::TopRight, HateEngine::CoordsUI::Percent};
    ui.size = {50, 50, 1, HateEngine::CoordsUI::TopLeft, HateEngine::CoordsUI::Percent};

    //HateEngine::LabelUI label;
    //label.text = "Hello, World!";
    HateEngine::WidgetUI fps_widget;
    fps_widget.position = {0, 0};
    fps_widget.size = {200, 100, 1.5};
    fps_widget.color.w = 0;
    //fps_widget.has_background = true;
    fps_widget.has_border = true;
    
    fps_widget_ptr = &fps_widget;
        
    fps_label.color = {255, 0, 0};
    fps_label.size = {100, 30};
    fps_label.text = "FPS: 0";
    
    

    fps_widget.addObjectRef(&fps_label);

    lvl.addObjectRef(&fps_widget);
    lvl.addObjectRef(&ui);



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
      //std::cout << "FPS: " << (float)count / del << std::endl;
      fps_label.text = "FPS: " + std::to_string((float)count / del);

      count = 0;
      del = 0.0;
    }
}

void _physics_process(HateEngine::Engine *engine, double delta) {
    //std::cout << "FPS: " << 1.0 / delta << "\n";
    glm::vec3 mesh1_r = mesh1.getGlobalRotationEuler();
    //std::cout << mesh1_r.x << " " << mesh1_r.y << " " << mesh1_r.z << "\n";


    //mesh1.setPosition(rbodyPosVect.z, rbodyPosVect.y, rbodyPosVect.x);

    if (engine->Input.isKeyPressed(GLFW_KEY_ESCAPE))
        engine->Exit();


    /*if (engine->Input.isKeyPressed(GLFW_KEY_UP))
        rbody_body->setLinearVelocity({0, -1, 0});*/

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
        
    if (engine->Input.isKeyPressed(GLFW_KEY_Y))
        engine->setMouseCapture(false);
    if (engine->Input.isKeyPressed(GLFW_KEY_U))
        engine->setMouseCapture(true);

    if (engine->Input.isKeyPressed(GLFW_KEY_P)) {
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        //glfwSetWindowMonitor(engine->getWindow(), monitor, 0, 0, 300, 600, 75);
    }

    glm::vec2 glmodel_rot = engine->Input.getVector(
        GLFW_KEY_LEFT, GLFW_KEY_RIGHT, GLFW_KEY_UP, GLFW_KEY_DOWN
    );
    glmodel.rotate(0, glmodel_rot.x, 0);
    glmodel.offset(0, glmodel_rot.y / 10, 0);



    /*std::cout << "GLOBAL POS: " << glmodel.getGlobalPosition().x << " " <<
        glmodel.getGlobalPosition().y << " " <<
        glmodel.getGlobalPosition().z << '\n';*/



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
    
    if (event.type == HateEngine::Engine::InputEventMouseScroll) {
        if (event.position.y < 0)
            fps_widget_ptr->zoom(-0.1);
        else
            fps_widget_ptr->zoom(0.1);
    }
}
