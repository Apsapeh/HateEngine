#include "HateEngine/AudioBus.hpp"
#include "HateEngine/Input.hpp"
#include "HateEngine/NavMeshAgent.hpp"
#include "HateEngine/OSDriverInterface.hpp"
#include "HateEngine/Objects/Decal.hpp"
#include "HateEngine/Objects/Light/OmniLight.hpp"
#include "HateEngine/Objects/Light/SpotLight.hpp"
#include "HateEngine/Objects/Physics/CapsuleShape.hpp"
#include "HateEngine/Objects/Physics/CollisionShape.hpp"
#include "HateEngine/Objects/Physics/RigidBody.hpp"
#include "HateEngine/Objects/Physics/StaticBody.hpp"
#include "HateEngine/Objects/Physics/TriggerArea.hpp"
#include "glm/ext/vector_float2.hpp"
#include "glm/ext/vector_float3.hpp"

#include "HateEngine/Objects/Object.hpp"
#include "HateEngine/Objects/Physics/BoxShape.hpp"
#include "glm/fwd.hpp"
#include "glm/geometric.hpp"
#include <HateEngine/Log.hpp>
#include <HateEngine/HateEngine.hpp>
#include <HateEngine/Objects/Camera.hpp>
#include <HateEngine/Objects/CubeMesh.hpp>
#include <HateEngine/Objects/Light/DirectionalLight.hpp>
#include <HateEngine/Objects/Mesh.hpp>
#include <HateEngine/Objects/Particles.hpp>
#include <HateEngine/Objects/BillboardMesh.hpp>
#include <HateEngine/Resources/GLTFModel.hpp>
#include <HateEngine/Resources/Texture.hpp>
#include <HateEngine/Level.hpp>
#include <HateEngine/Resources/HERFile.hpp>
#include <HateEngine/Resources/ObjMapModel.hpp>
#include <cmath>
#include <glm/ext.hpp>
#include <glm/glm.hpp>
#include <glm/gtx/quaternion.hpp>
#include <iostream>
#include <HateEngine/Objects/Physics/SphereShape.hpp>
#include <HateEngine/Objects/Physics/RayCast.hpp>
#include <HateEngine/Resources/UIFont.hpp>

#include <HateEngine/UI/WidgetUI.hpp>
#include <HateEngine/UI/LabelUI.hpp>
#include <HateEngine/UI/ButtonUI.hpp>
#include <HateEngine/UI/CheckboxUI.hpp>
#include <HateEngine/UI/ImageUI.hpp>

#include <HateEngine/Resources/Audio.hpp>
#include <HateEngine/Objects/AudioPlayer.hpp>
#include <HateEngine/AudioServer.hpp>
#include <sys/types.h>

#define WINVER 0x0501
#define _WIN32_WINNT 0x0501


void _process(HateEngine::Engine*, double);
void _physics_process(HateEngine::Engine*, double);
void _input_event(HateEngine::Engine*, const HateEngine::InputEventInfo&);

HateEngine::CubeMesh mesh1;
HateEngine::CubeMesh mesh2;
HateEngine::CubeMesh xAxMesh;
// HateEngine::GLTFModel glmodel("examples/Assets/employee.glb");
// HateEngine::GLTFModel glmodel("examples/Assets/billy-plane-sep.glb");
// HateEngine::GLTFModel glmodel("examples/Assets/ignore/fire.glb");
HateEngine::GLTFModel test_glmodel("examples/Assets/SHOTGUN4.glb");
// HateEngine::GLTFModel* test_glmodel;
HateEngine::GLTFModel playerCapsuleMesh("examples/Assets/capsule.glb");


const int WIDTH = 800;
const int HEIGHT = 600;

// HateEngine::CubeMesh meshes[22500];
HateEngine::Level lvl;
HateEngine::Camera camera(60, 600);
HateEngine::DirectionalLight sun;
HateEngine::SpotLight light;
HateEngine::CubeMesh floor_mesh;

HateEngine::Particles* part;
HateEngine::RayCast* ray;

HateEngine::LabelUI fps_label;
HateEngine::WidgetUI* fps_widget_ptr = nullptr;
HateEngine::CubeMesh test2;
HateEngine::PhysicalBody playerBody(HateEngine::PhysicalBody::DynamicBody);
HateEngine::PhysicalBody rigidBody(HateEngine::PhysicalBody::CharacterBody);
HateEngine::StaticBody floorBody;
HateEngine::BoxShape rigidBodyBoxShape({1, 1, 1});


HateEngine::Particles* cube_part_ptr;

HateEngine::BillboardMesh billboardMesh;

HateEngine::Object head;


// Audio
HateEngine::AudioBus* ambient_bus = nullptr;
HateEngine::AudioBus* music_bus = nullptr;

// Audio UI
HateEngine::LabelUI ambient_bus_volume_label;
HateEngine::LabelUI music_bus_volume_label;


HateEngine::Decal decal;

HateEngine::NavMeshAgent* nav_agent_ptr = nullptr;
HateEngine::CubeMesh nav_agent_cube;

HateEngine::JoystickHandle gamepad;

int main() {
    // int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
    //	LPSTR lpCmdLine, int nCmdShow) {
    rigidBody.setIsRequiredCollisionPoints(true);
    xAxMesh.setSize(1, 0.1, 0.1);
    xAxMesh.offset(0, 6, 0);

    camera.setPosition(0, 0, 0);
    // camera.offset(0, 6, 3);
    // camera.setPosition(0, 25, 0);
    // camera.setRotation(0, 0, 0);
    // camera.setSkyBoxTexture(new HateEngine::Texture("examples/Assets/skybox.jpg",
    // HateEngine::Texture::ClampToEdge)); camera.setSkyBoxEnabled(true);
    // mesh1.setRotation(0, 0, 0);
    mesh1.setSize(1, 1, 1);

    billboardMesh.offset(-4, 4, -4);

    floor_mesh.setPosition(0, 0, 0);
    floor_mesh.setSize(25, 1, 25);
    HATE_INFO("Info message");
    HATE_WARNING("Warning message");
    HATE_ERROR("Error message");
    HATE_DEBUG("Hello");
    // HATE_FATAL("Fatal message");

    // floor.setRotation(20, 0, 0);


    HateEngine::DirectionalLight dirLight;
    // dirLight.rotate(45, 0, 0);
    glm::vec3 dir = dirLight.getDirection();

    light.setVisible(false);
    light.setColor({1.0, 0, 0, 1.0});

    // HATE_FATAL_F("Direction x: %f, y: %f, z: %f", dir.x, dir.y, dir.z);

    // floor.setRotationMatrix(matfloor);
    //  floor.matrix
    //


    // sun.setPosition({1.0, 1.0, 1.0});
    sun.rotate(-45, 45, 0);
    sun.setVisible(true);

    sun.setMaskBit(0, false);
    sun.setMaskBit(1, true);

    mesh2.setPosition(3, 3, 3);

    floor_mesh.render_layers.set(0, false);
    floor_mesh.render_layers.set(1, false);
    // floor_mesh.disableLightShading();


    // test_glmodel.setVisible(false);


    // camera.bindObj(&test_glmodel);

    HATE_DEBUG_F("Camera uuid: %llu", camera.getUUID().getU64());
    // mesh1.bindObj(&test_glmodel);
    HateEngine::Engine game("HateEngine Test", WIDTH, HEIGHT);
    // std::cout << sizeof(game) << "\n";
    // while(true) {}

    game.mainWindow->setMouseMode(HateEngine::OSDriverInterface::Disabled);
    // std::cout << "\n\n\n\n" << glfwGetInputMode(game.window, GLFW_CURSOR) << "\n\n\n\n";
    game.setOneThreadMode(true);
    game.setVSync(true);

    game.onLevelChanged.connect([](HateEngine::Engine* e, HateEngine::Level* lvl,
                                   HateEngine::Level* old) {
        HATE_INFO("Level changed");
        HATE_INFO_F("Old: %p, New: %p", old, lvl);
    });

    game.Input.addKeyToAction("forward", HateEngine::KeyW);
    game.Input.addKeyToAction("forward", HateEngine::KeyNumPad8);
    game.Input.addKeyToAction("forward", HateEngine::GamepadAxisLeftYUp);
    game.Input.addKeyToAction("backward", HateEngine::KeyS);
    game.Input.addKeyToAction("backward", HateEngine::KeyNumPad5);
    game.Input.addKeyToAction("left", HateEngine::KeyA);
    game.Input.addKeyToAction("left", HateEngine::KeyNumPad4);
    game.Input.addKeyToAction("left", HateEngine::GamepadAxisLeftXLeft);
    game.Input.addKeyToAction("right", HateEngine::KeyD);
    game.Input.addKeyToAction("right", HateEngine::KeyNumPad6);
    game.Input.addKeyToAction("up", HateEngine::KeySpace);
    game.Input.addKeyToAction("down", HateEngine::KeyLeftControl);
    game.Input.addKeyToAction("down", HateEngine::KeyLeftSuper);
    // Setting textures for the cube and floor meshes

    std::thread th([&]() {
        std::ostringstream oss;
        oss << std::this_thread::get_id() << std::endl;
        HATE_ERROR_F("Thread id: %s", oss.str().c_str());
        game.callDeferred([]() {
            std::ostringstream oss;
            oss << std::this_thread::get_id() << std::endl;
            HATE_ERROR_F("Deferred thread id: %s", oss.str().c_str());
        });
    });


    // ambient_bus->addSource("examples/Assets/ambient.wav");
    // music_bus->addSource("examples/Assets/soundtrack.mp3");


    HateEngine::HERFile herfile("examples/Assets/test.her", "password");
    HateEngine::Texture tex_floor = herfile["ground.png"].asTexture();
    // HateEngine::GLTFModel her_model = herfile["new_shotgun.glb"].asGLBModel();
    // test_glmodel = &her_model;


    HateEngine::GLTFModel radioModel("examples/Assets/radio.glb");
    radioModel.rotate(-90, 180, 0, true);
    radioModel.setPosition(0, 2, 0);
    radioModel.setScale(0.003, 0.003, 0.003);
    // radioModel.setScale(0.1, 0.1, 0.1);
    // lvl.addObject(&radioModel);


    /*========================> Audio Test <=========================*/
    // Global pointers
    ambient_bus = new HateEngine::AudioBus();
    music_bus = new HateEngine::AudioBus();

    HateEngine::AudioStream ambient = HateEngine::AudioStream("examples/Assets/Ignore/ambient.ogg");
    HateEngine::AudioStream audio2 = HateEngine::AudioStream("examples/Assets/audio2.ogg");

    HateEngine::AudioPlayer ambientPlayer(&ambient, ambient_bus, HateEngine::AudioPlayer::Audio2D);
    HateEngine::AudioPlayer audioPlayer1 =
            herfile["audio.ogg"].asAudioPlayerStream(music_bus, HateEngine::AudioPlayer::Audio3D);
    HateEngine::AudioPlayer audioPlayer2(&audio2, music_bus, HateEngine::AudioPlayer::Audio3D);

    audioPlayer1.setPosition(0, 2, 0);
    playerCapsuleMesh.bindObj(&audioPlayer2);

    // ambientPlayer.play();
    audioPlayer1.play();
    audioPlayer2.play();

    /*===============================================================*/

    camera.bindObj(&decal);
    HateEngine::Texture shadow_circle_texture("examples/Assets/shadow_circle.png");
    decal.setTexture(&shadow_circle_texture);
    decal.getMesh()->setCorrectTransparency(true);
    decal.setRayLength(10);
    decal.setRayDirection({0, -1, 0});

    lvl.addObject(decal.getMesh());
    decal.setPhysEngine(lvl.getPhysEngine());


    HateEngine::TriggerArea trigger;
    trigger.setPosition(0, 0, 0);
    trigger.setOnEnter(
            [](HateEngine::PhysicalBody* body, void* data_p) {
                HATE_INFO_F("Enter UUID: %llu", body->getUUID().getU64());
            },
            nullptr
    );

    trigger.setOnExit(
            [](HateEngine::PhysicalBody* body, void* data_p) {
                HATE_INFO_F("Exit UUID: %llu", body->getUUID().getU64());
            },
            nullptr
    );
    HateEngine::BoxShape box({3, 3, 3});
    trigger.addCollisionShape(&box);


    lvl.getPhysEngine()->addObject(&trigger);

    /*HateEngine::Audio* audio = new HateEngine::Audio("examples/Assets/audio.ogg");
    // HateEngine::Audio audio = herfile["audio.ogg"].asAudio();
    HATE_WARNING("WARNING");
    HateEngine::AudioPlayer audioPlayer(audio, nullptr, HateEngine::AudioPlayer::Audio3D);
    // delete audio;
    audioPlayer.play();*/

    playerCapsuleMesh.bindObj(&test_glmodel);

    // test_glmodel->offset(0.5, -0.5, -2);
    test_glmodel.setScale(0.25, 0.25, 0.25);
    test_glmodel.setRotation(90, 180, 0);


    // HateEngine::Texture tex_floor("examples/Assets/ground.png");
    //    HateEngine::GLTFModel glmodel("examples/Assets/ignore/R.glb");
    // HateEngine::GLTFModel glmodel("examples/Assets/employee.glb");
    HateEngine::Texture tex(
            "examples/Assets/brick.png", HateEngine::Texture::Repeat, HateEngine::Texture::Nearest
    );

    HateEngine::Texture tex2("examples/Assets/UV_test.png");
    HateEngine::CubeMesh uv_test_cube;
    uv_test_cube.setTexture(&tex2);
    uv_test_cube.setPosition(0, 5, 0);
    // lvl.addObject(&uv_test_cube);

    // HateEngine::GLTFModel glmodel2("examples/Assets/ignore/bolg.glb");

    game.setLevel(&lvl);
    // lvl.addObject(&test_glmodel);
    //  HateEngine::CubeMesh test_mesh;
    //  HateEngine::Level level2;
    //  level2.addObjectRef(&test2);
    //  level2.setCameraRef(&camera);
    //  game.setLevelRef(&level2);


    HateEngine::Texture campfire_tex("examples/Assets/campfire.png");
    uv_test_cube.setTexture(&campfire_tex);
    uv_test_cube.setCorrectTransparency(true);
    uv_test_cube.setFaceCulling(false);

    billboardMesh.setTexture(&campfire_tex);
    billboardMesh.setTarget(&camera);
    billboardMesh.setCorrectTransparency(true);
    HATE_INFO_F("BM: %p", &billboardMesh);
    lvl.addObject(&billboardMesh);


    /*const auto desers = std::make_pair(
            std::unordered_map<std::string, HateEngine::ObjMapModel::EntityDeserialzer>(

            ),
            nullptr
    );*/

    HATE_WARNING("Deserializing");
    /*HateEngine::ObjMapModel objmodel(
            "examples/Assets/Ignore/_E1M1.obj", "examples/Assets/Ignore/E1M1.MAP",
            "examples/Assets/Ignore/light.heluv", 16.0, true, 15, 100000000
    );*/
    /*HateEngine::ObjMapModel objmodel(
            "examples/Assets/Ignore/E1M1.obj", "examples/Assets/Ignore/E1M1.MAP",
            "examples/Assets/Ignore/light.heluv", "examples/Assets/Ignore/E1M1.hepvs", 16.0, true,
            true, 15, 1
    );*/

    HateEngine::ObjMapModel objmodel_few_materials(
            "examples/Assets/few_material_test.obj", "examples/Assets/few_material_test.map", "", "",
            16.0, false, false
    );
    objmodel_few_materials.setPosition(5, 5, 5);
    lvl.addObject(&objmodel_few_materials);


    HateEngine::ObjMapModel objmodel(
            "examples/Assets/dev.obj", "examples/Assets/dev.map", "", "", 16.0, true, false, 15, 1
    );
    objmodel.deserializeEntities(
            {{"light",
              [](HE_ENTITY_DE_PARAMS) {
                  HateEngine::OmniLight* light = new HateEngine::OmniLight();
                  light->setPosition(entity.position);
                  light->setColor({10, 0, 0, 1});
                  model->bindObj(light);
                  // model->addEntityObjectToLevel(light);

                  auto lights = static_cast<std::vector<HateEngine::OmniLight*>*>(data);
                  lights->push_back(light);

                  HateEngine::CubeMesh* cube = new HateEngine::CubeMesh();
                  cube->setSize(0.1, 0.1, 0.1);
                  // cube->setPosition(entity.position);
                  // model->bindObj(cube);
                  model->addEntityObjectToLevel(cube);

                  HateEngine::RigidBody* body = new HateEngine::RigidBody();
                  HateEngine::BoxShape* box = new HateEngine::BoxShape({0.1, 0.1, 0.1});
                  body->addCollisionShape(box);
                  box->setFriction(1);
                  box->setBounciness(0.5);
                  body->setPosition(entity.position);
                  body->setMass(7.8);
                  body->bindObj(cube);
                  model->bindObj(body);
                  model->addEntityPhysicalBodyToLevel(body);
              }}},
            new std::vector<HateEngine::OmniLight*>,
            [](void* data) {
                auto lights = static_cast<std::vector<HateEngine::OmniLight*>*>(data);
                for (auto light: *lights) {
                    delete light;
                }
                delete lights;
            }
    );

    objmodel.offset(0, -5, 0);


    // HateEngine::HENFile henfile("examples/Assets/Ignore/E1M1.hen");
    // HateEngine::HENFile henfile = herfile["E1M1.hen"].asHENFile();
    // const auto* nodes = henfile.getNodes();


    // objmodel.setScale(0.0625, 0.0625, 0.0625);
    /*HateEngine::ObjMapModel objmodel =
            herfile.loadObjMap("untitled.obj", "unnamed.map", true, 15, 1);*/
    // objmodel.offset(0, -180, -20);
    // objmodel.offset(-30, -20, 60);

    /*for (auto& m: objmodel.getLOD(0)) {
        glm::vec3 min = m->getAABBMin();
        glm::vec3 max = m->getAABBMax();

        glm::vec3 center = (min + max) / 2 + m->getGlobalPosition();
        glm::vec3 size = max - min;

        HateEngine::CubeMesh cube;
        cube.setSize(size.x, size.y, size.z);
        cube.setPosition(center.x, center.y, center.z);
        // cube.setPosition(m->getGlobalPosition());
        // lvl.addObjectClone(cube);
    }*/

    lvl.addObject(&objmodel);
    // lvl.getPhysEngine()->addObjectRef(objmodel.getStaticBody());


    /*for (int i = 0; i < objmodel.getLODCount(); i++) {
        HateEngine::Mesh* mesh = objmodel.getLOD(i)[0];
        if (mesh->getName() == "entity0_brush632") {
            HATE_WARNING_F("UV SIZE: %d", mesh->getUV()->size());

            //HATE_FATAL_F("LIGHT UV SIZE: %d", mesh->getLightUV()->size());
        }





        for (auto& lod: objmodel.getLOD(i)) {
            //lod->setLightTexture(nullptr);
            //lod->setTexture(mesh->getLightTexture());
            //lod->setUV(*mesh->getLightUV());
            lod->disableLightShading();
            //lod->setTexture(&bri);
            // HATE_INFO_F("LOD name: %s, light_texture: %d", lod->getName().c_str(),
            // lod->getLightTexture() != nullptr);
            //  lod->setVisible(enabled.count(lod->getName()));
        }
    }*/

    // floor_mesh.disableLightShading();

    lvl.setAmbientLightColor(1, 1, 1);
    lvl.setAmbientLightIntensity(0.5);


    /*for (auto& m: objmodel.getMeshes()) {
        std::cout << "POS: " << m->getPosition().x << " " << m->getPosition().y << " "
                  << m->getPosition().z << std::endl;
    }*/
    /*std::cout << "Mesh count: " << objmodel.getMeshes().size() << std::endl;
    std::cout << objmodel.getMeshes()[0]->getVertices()->size() << "\n"; // 108
    std::cout << objmodel.getMeshes()[0]->getIndicies()->size() << "\n";    // 36
    std::cout << objmodel.getMeshes()[0]->getUV()->size() << "\n";  // 72*/
    // exit(0);

    // HATE_FATAL_F("Poly count: %d", poly_count);

    // 17 012 855

    // 477 251 092


    // std::cout << "Poly count: " << poly_count << std::endl;

    // lvl.addObject(&objmodel);


    /*std::cout << glmodel.getGlobalPosition().x << " " << glmodel.getGlobalPosition().y << " "
              << glmodel.getGlobalPosition().z << "\n";*/

    floor_mesh.setTexture(&tex_floor);
    floor_mesh.setUV({
            0, 0, 3, 0, 3, 3, 3, 3, 0, 3, 0, 0, 0, 0, 3, 0, 3, 3, 3, 3, 0, 3, 0, 0,
            0, 0, 3, 0, 3, 3, 3, 3, 0, 3, 0, 0, 0, 0, 3, 0, 3, 3, 3, 3, 0, 3, 0, 0,
            0, 0, 3, 0, 3, 3, 3, 3, 0, 3, 0, 0, 0, 0, 3, 0, 3, 3, 3, 3, 0, 3, 0, 0,
    });
    mesh1.setTexture(&tex);
    // mesh2.setTexture(&tex);
    // mesh1.bindObj(&mesh2);
    // mesh1.bindObj(&glmodel);


    // glmodel.bindObj(&mesh2);
    // glmodel.bindObj(&camera);
    //  mesh1.bindObj(&glmodel);

    lvl.addObject(&mesh1);
    // lvl.addObject(&mesh2);
    // lvl.addObject(&xAxMesh);
    lvl.addObject(&floor_mesh);
    lvl.addObject(&sun);

    /*HateEngine::GLTFModel dance_animation("examples/Assets/ignore/dance.glb");
    HateEngine::GLTFAnimationPlayer anim_player(&dance_animation);
    anim_player.offset(5, 0, 5);
    anim_player.setFPS(60);
    anim_player.setLoop(true);
    anim_player.play("dance_man");
    lvl.addObject(&anim_player);*/


    // lvl.addObject(&dance_animation);
    /* for (auto& mesh : dance_animation.getMeshes()) {
         mesh->disableLightShading();
         mesh->enableCustomMaxLightDist(2500);
     }*/

    // lvl.addObject(&glmodel);

    // print glmodel meshes id


    // glmodel.offset({0, 1, 0});
    //  glmodel.getMeshes()[0]->disableLightShading();

    // light.setPosition({0, 2, 0});
    // l
    light.setVisible(true);
    light.setColor(light.getColor() * 10);
    //  light.color = {10, 10, 10, 1};
    light.setExponent(20);
    light.setRadius(20);

    camera.bindObj(&light);
    lvl.addObject(&light);

    lvl.setFixedProcessLoop([](HateEngine::Engine* engine, double delta) {
        ////TODO: Change engine pointer to struct with Engine*, Level*
        // HateEngine::Engine* _engine = (HateEngine::Engine*) engine;
        //  _engine->getLevel()->ob
        //  std::cout << "Level fixed process loop dealay: " << delta << "\n";
        //_process(engine, delta);
    });

    // game.addObjectRef(&tomato2);
    HateEngine::Particles::ParticleSettings pa_set = {2.0f, 2.0f, false, {0, 0, 0}, {0.1, 0.1, 0.1},
                                                      true};

    HateEngine::Texture snow_tex(
            "examples/Assets/snow.png", HateEngine::Texture::Clamp, HateEngine::Texture::Linear
    );
    HateEngine::CubeMesh snow_mesh;
    snow_mesh.setSize(0.05, 0.05, 0.05);

    snow_mesh.setTexture(&snow_tex);
    // snow_mesh.setSize(0.8, 0.8, 0.8);
    // snow_mesh.setPosition(0, 5, 0);
    // lvl.addObject(&snow_mesh);
    //  tomato.setScale({0.05, 0.05, 0.05});


    /*
        Ёлочка
        float angle = 3.14159f / 2.0f + (3.14159f / 500.0f * p->index);
            float xz_angle =  (360.0f / 500.0f * p->index);
            glm::vec2 xz_pos = {p->getPosition().x, p->getPosition().z};
            float dist_to_center = glm::length(xz_pos);
            //p->data["vel"] = (void*) new glm::vec3(cos(angle) * 30 * xz_pos.x, 5 - 20 *
       dist_to_center, sin(angle) * 30 * xz_pos.y); p->data["vel"] = (void*) new
       glm::vec3(cos(xz_angle) * cos(angle), 5 * sin(angle), sin(xz_angle) * cos(angle));

       Пол сферы
       float theta = glm::linearRand(
                    0.0f, glm::two_pi<float>()
            ); // Угол вокруг Y (горизонтальная плоскость)
            float phi = glm::linearRand(
                    0.0f, glm::pi<float>() / 2.0f
            ); // Угол наклона от вертикали (ограничим верхней половиной)

            // Преобразование вектора из сферических координат в декартовы
            glm::vec3 direction(
                    cos(phi) * cos(theta), // X
                    sin(phi), // Y
                    cos(phi) * sin(theta) // Z
            );
            p->data["vel"] = (void*) new glm::vec3(direction * 5.0f);

    */

    HateEngine::BillboardMesh part_billboard;
    part_billboard.setTexture(&campfire_tex);
    // part_billboard.setTarget(&camera);
    part_billboard.setCorrectTransparency(true);


    HateEngine::Particles cube_part((HateEngine::Mesh) part_billboard, 500, pa_set, 0.001);
    cube_part.calculateFunc = [](HateEngine::Particle* p, double delta) {
        const int PARTICLE_COUNT = 500;
        const float GRAVITY = -9.8f;
        const float MAX_VERTICAL_SPEED = 5.0f; // Максимальная скорость вверх
        const float MAX_HORIZONTAL_SPEED = 5.0f; // Максимальная скорость в стороны
        const float MAX_RADIUS = 1.0f;
        if (p->data.count("vel") == 0) {


            float r = glm::sqrt(glm::linearRand(0.0f, 1.0f)) * 0.25 * 4;

            /*p->rotate(
                    glm::linearRand(0.0f, 360.0f), glm::linearRand(0.0f, 360.0f),
                    glm::linearRand(0.0f, 360.0f)
            );*/

            // Генерируем случайный угол в радианах (от 0 до 2π)
            float angle = glm::linearRand(0.0f, glm::two_pi<float>());

            // Преобразуем из полярных координат в декартовы
            float x = r * glm::cos(angle);
            float z = r * glm::sin(angle);
            glm::vec3 point = glm::vec3(5, x + 5, z);
            glm::vec3 dir = point / glm::length(point);
            p->data["vel"] = (void*) new glm::vec3(dir * 5.0f);
        }

        glm::vec3* vel = (glm::vec3*) (p->data["vel"]);


        // p->data["vel"] = (void*) new glm::vec3(p->getPosition().x * 10, 5, p->getPosition().z *
        // 10);
        vel->y += delta * -9.8;

        //((glm::vec3*) (p->data["vel"]))->y -= delta;

        // p->
        //  if (p->index == 0)
        //  std::cout << ((glm::vec3*)(p->data["vel"]))->y << "\n";
        //  std::cout << float(delta * 9.8) << "\n";
        //  std::cout << ((glm::vec3*)(p->data["vel"]))->y << "\n";
        glm::vec3 off = *vel * glm::vec3(delta);
        p->offset(off);
        // std::cout << off << "\n";
        // p->offset(off);
        // p->offset({0, -0.1 * delta, 0});
        // glm::vec3 scale = p->getScale() * 0.99 * delta;

        // p->setScale(scale);


        // std::cout <<
    };

    cube_part.onParticleDelete = [](HateEngine::Particle* p) { delete (glm::vec3*) p->data["vel"]; };

    cube_part.setPosition(4, 4, 4);

    // cube_part.play();

    cube_part_ptr = &cube_part;
    // lvl.addObject(&cube_part);


    /*for (auto& m: lightmap_model.getLOD(0)) {
        if (m->getName() == "Cube.004") {
            // m->setTexture(&bri);
            m->setLightTexture(&cube3_lightmap);
            std::cout << m->getVertices()->size() << "\n";
            std::cout << m->getLightUV()->size() << "\n";
            std::cout << m->getUV()->size() << "\n";
        }
        m->setTexture(&bri);
        // m->setLightTexture(&cube3_lightmap);
        // std::cout << m->getLightUV()->size() << "\n";
    }*/


    /*cube_part.setPosition(0, 3, 0);
    cube_part.pause = true;*/
    // std::cout << part->getPosition().z << "\n";
    // game.addObjectClone(cube_part);


    // HateEngine::PhysicalBody floorBody(HateEngine::PhysicalBody::StaticBody);
    floorBody.setPosition(0, -0.5, 0);
    HateEngine::BoxShape floorShape({25, 1, 25}, {0, 0, 0}, {0, 0, 0});
    floorBody.addCollisionShape(&floorShape);
    // floorBody.rotate(20, 0, 0);
    floorBody.bindObj(&floor_mesh);

    rigidBody.setPosition(0, 5, 0);
    rigidBody.rotate(48, 22, 36);
    rigidBody.setIsAllowedToSleep(false);
    // rigidBody.setRotation(0, 0, 0);
    rigidBodyBoxShape.setFriction(0);
    rigidBody.addCollisionShape(&rigidBodyBoxShape);
    HateEngine::SphereShape sphereShape(0.5);
    // rigidBody.addCollisionShapeRef(&sphereShape);
    rigidBody.bindObj(&mesh1);
    // rigidBody.setLinearVelocity(0, -9.8, 0);
    //  rigidBody.bindObj(&camera);
    //  rigidBody.bindObj(&glmodel);

    // HateEngine::CubeMesh playerCubeMesh;
    playerCapsuleMesh.rotate(0, 0, 0);

    playerBody.setPosition(0, 3, 0);
    playerBody.rotate(0, 0, 0);
    HateEngine::CapsuleShape capsuleShape(0.5, 2);
    playerBody.addCollisionShape(&capsuleShape);
    playerBody.bindObj(&playerCapsuleMesh);

    lvl.getPhysEngine()->addObject(&rigidBody);
    lvl.getPhysEngine()->addObject(&playerBody);
    lvl.getPhysEngine()->addObject(&floorBody);
    // playerBody.reactRigidBody->setAngularLockAxisFactor({0, 0, 0});
    playerBody.setAngularLockAxisFactor(0, 0, 0);
    // playerBody.reactRigidBody->setLinearVelocity({2, 0.1, 2});
    // capsuleShape.reactCollider->getMaterial().setFrictionCoefficient(0);
    capsuleShape.setFriction(0);


    // lvl.addObject(&playerCapsuleMesh);


    // RayCast test
    ray = new HateEngine::RayCast(lvl.getPhysEngine(), 5.0f);
    // ray->setPosition(0, 0.7, 0);
    // ray->rotate(0, 180, 0, true);
    camera.bindObj(ray);


    // UI TEST
    HateEngine::WidgetUI ui;
    ui.has_border = true;
    ui.has_title = true;
    ui.position = {50, 0, 1, HateEngine::CoordsUI::TopRight, HateEngine::CoordsUI::Percent};
    ui.size = {50, 50, 1, HateEngine::CoordsUI::TopLeft, HateEngine::CoordsUI::Percent};

    // HateEngine::LabelUI label;
    // label.text = "Hello, World!";
    HateEngine::WidgetUI fps_widget;
    fps_widget.position = {0, 0, 1, HateEngine::CoordsUI::TopLeft, HateEngine::CoordsUI::Pixels};
    fps_widget.size = {300, 200, 1, HateEngine::CoordsUI::TopLeft, HateEngine::CoordsUI::Pixels};
    fps_widget.color.w = 0;
    // fps_widget.has_background = true;
    fps_widget.has_border = false;
    fps_widget.is_movable = false;
    fps_widget.is_closable = false;

    HateEngine::UIFont* f = new HateEngine::UIFont("examples/Assets/Comfortaa-Regular.ttf", 36);
    HateEngine::UIFont* f_2 = new HateEngine::UIFont("examples/Assets/NanumGothic-Regular.ttf", 12);

    fps_widget_ptr = &fps_widget;

    fps_label.text_color = {255, 0, 0, 128};
    fps_label.setFont(f);
    fps_label.size = {150, 60};
    fps_label.text = "FPS: 0";

    HateEngine::LabelUI label;
    label.text = "Hello, World!";
    label.setFont(f_2);
    label.position = {
            0, -label.size.y / 2, 1, HateEngine::CoordsUI::Center, HateEngine::CoordsUI::Percent
    };
    // fps_label.text_align = HateEngine::LabelUI::Wrap;

    int a = 0;
    /*HateEngine::ButtonUI button([] (HateEngine::Engine *engine, void* a) {
        *static_cast<int*>(a) += 1;
        std::cout << "a: " << *static_cast<int*>(a) << "\n";
        std::cout << "Hello, World!\n";
    }, &a);*/

    HateEngine::ButtonUI button([](HateEngine::Engine* engine) { std::cout << "Hello, World!\n"; });

    button.position = {100, 60};
    // button.size = {100, 200};
    HateEngine::Texture image_btn(
            "examples/Assets/brick.png", HateEngine::Texture::Repeat, HateEngine::Texture::Nearest
    );
    HateEngine::Texture image_hover_btn(
            "examples/Assets/campfire.png", HateEngine::Texture::Repeat, HateEngine::Texture::Nearest
    );
    HateEngine::Texture image_press_btn(
            "examples/Assets/UV_test.png", HateEngine::Texture::Repeat, HateEngine::Texture::Nearest
    );
    button.color = {255, 0, 0, 255};
    button.text = "Button";
    button.text_color = {255, 255, 255, 255};
    button.roundness = 0;
    button.setNormalTexture(&image_btn);
    button.setHoverTexture(&image_hover_btn);
    button.setPressedTexture(&image_press_btn);

    HateEngine::CheckboxUI checkbox;
    checkbox.position = {-40, -20, 1, HateEngine::CoordsUI::CenterRight, HateEngine::CoordsUI::Pixels};
    checkbox.size = {200, 40, 1, HateEngine::CoordsUI::TopLeft, HateEngine::CoordsUI::Pixels};
    checkbox.text = "Test/Тест";


    HateEngine::Texture image(
            "examples/Assets/brick.png", HateEngine::Texture::Repeat, HateEngine::Texture::Nearest
    );

    HateEngine::ImageUI image_ui(&image);
    // image_ui.setScale(10.0);
    /*image_ui.position = {
            -40, -20, 1, HateEngine::CoordsUI::CenterRight, HateEngine::CoordsUI::Pixels
    };*/
    // image_ui.size = {200, 40, 1, HateEngine::CoordsUI::TopLeft, HateEngine::CoordsUI::Pixels};

    fps_widget.addObjectRef(&fps_label);
    fps_widget.addObjectClone(label);
    fps_widget.addObjectClone(button);
    fps_widget.addObjectRef(&checkbox);
    fps_widget.addObjectClone(image_ui);
    // fps_widget.addObjectRef(&button);


    lvl.addObject(&fps_widget);
    // lvl.addObject(&ui);

    // playerCapsuleMesh.bindObj(&camera);
    head.bindObj(&camera);
    head.bindObj(&uv_test_cube);


    // Audio UI
    HateEngine::WidgetUI audio_widget;
    audio_widget.position = {255, 5, 1, HateEngine::CoordsUI::TopRight, HateEngine::CoordsUI::Pixels};
    audio_widget.size = {250, 55};
    audio_widget.color.w = 0;
    // fps_widget.has_background = true;
    audio_widget.has_border = true;

    audio_widget.addObjectRef(&ambient_bus_volume_label);
    audio_widget.addObjectRef(&music_bus_volume_label);

    ambient_bus_volume_label.size = {150, 20};
    ambient_bus_volume_label.position = {5, 5};

    music_bus_volume_label.size = {150, 20};
    music_bus_volume_label.position = {5, 25};

    HateEngine::ButtonUI inc_ambient_bus_volume_button([](HateEngine::Engine* engine) {
        ambient_bus->setVolume(ambient_bus->getVolume() + 0.1);
    });
    inc_ambient_bus_volume_button.text = "+10";
    inc_ambient_bus_volume_button.size = {40, 15};
    inc_ambient_bus_volume_button.position = {150, 5};

    HateEngine::ButtonUI dec_ambient_bus_volume_button([](HateEngine::Engine* engine) {
        ambient_bus->setVolume(ambient_bus->getVolume() - 0.1);
    });
    dec_ambient_bus_volume_button.text = "-10";
    dec_ambient_bus_volume_button.size = {40, 15};
    dec_ambient_bus_volume_button.position = {200, 5};

    audio_widget.addObjectRef(&inc_ambient_bus_volume_button);
    audio_widget.addObjectRef(&dec_ambient_bus_volume_button);


    HateEngine::ButtonUI inc_music_bus_volume_button([](HateEngine::Engine* engine) {
        music_bus->setVolume(music_bus->getVolume() + 0.1);
    });
    inc_music_bus_volume_button.text = "+10";
    inc_music_bus_volume_button.size = {40, 15};
    inc_music_bus_volume_button.position = {150, 25};

    HateEngine::ButtonUI dec_music_bus_volume_button([](HateEngine::Engine* engine) {
        music_bus->setVolume(music_bus->getVolume() - 0.1);
    });
    dec_music_bus_volume_button.text = "-10";
    dec_music_bus_volume_button.size = {40, 15};
    dec_music_bus_volume_button.position = {200, 25};


    audio_widget.addObjectRef(&inc_music_bus_volume_button);
    audio_widget.addObjectRef(&dec_music_bus_volume_button);


    lvl.addObject(&audio_widget);


    /* ==========================> Navigation Test <========================= */

    /*HateEngine::ObjMapModel nav_obj_map(
            "examples/Assets/NavMeshTest/dungeon.obj", "", "", 1, false, 10000000, 100000000
    );
    lvl.addObject(&nav_obj_map);*/

    /*HateEngine::NavMesh nav_mesh("examples/Assets/NavMeshTest/solo_navmesh.bin");

    HateEngine::NavMeshAgent nav_mesh_agent(&nav_mesh, 2048);
    nav_mesh_agent.computePath({40, 10, 0}, {11, 10, -40});

    nav_agent_ptr = &nav_mesh_agent;
    nav_agent_cube.setPosition(40, 10, 0);
    lvl.addObject(&nav_agent_cube);*/

    /* ==========================> Navigation Test <========================= */

    rigidBody.setIsGravityEnabled(false);
    // rigidBody

    game.setProcessLoop(_process);
    game.setFixedProcessLoop(_physics_process);
    game.setInputEvent(_input_event);
    lvl.setCamera(&camera);
    float xscale, yscale;

    HateEngine::BoxShape wall({1, 10, 10});
    HateEngine::StaticBody wall_body;
    wall_body.addCollisionShape(&wall);
    wall_body.setPosition(10, 3, 0);

    lvl.getPhysEngine()->addObject(&wall_body);
    // glfwGetPrimaryMonitor();
    // glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &xscale, &yscale);
    // std::cout << "Content scale: " << xscale << " " << yscale << "\n";
    //  while (true) {}
    rigidBody.setLinearVelocity(0, -1, 0);

    game.Run();
    // int p = glfwGetPlatform();
    // std::cout << p << " | " << GLFW_PLATFORM_WAYLAND << "\n";
    //  HateEngine::AudioServer::Deinit();
    th.join();
}


int frames_count = 0;
float speed = 1;
double fps_time = 0.0;
double cpu_time = 0;
double gpu_time = 0;

float g = 0;
void _process(HateEngine::Engine* engine, double delta) {
    if (fps_time < 0.5) {
        ++frames_count;
        fps_time += delta;
        cpu_time += engine->getCPUTimeMS();
        gpu_time += engine->getGPUTimeMS();
    } else {
        // std::cout << "FPS: " << (float)count / del << std::endl;
        fps_label.text = "FPS: " + std::to_string((float) frames_count / fps_time);
        HATE_WARNING_F(
                "FPS: %f, CPU: %fms, GPU: %fms", (float) frames_count / fps_time,
                cpu_time / frames_count, gpu_time / frames_count
        );

        frames_count = 0;
        fps_time = 0.0;
        cpu_time = 0;
        gpu_time = 0;
    }

    g += delta * 9.8;
    auto rb_v = rigidBody.getLinearVelocity();
    //    HATE_INFO_F("Velocity: %f | %f | %f", rb_v.x, rb_v.y, rb_v.z);
    // rigidBody.setLinearVelocity(3, -g, 0);
    // HATE_INFO_F("Draw calls: %d", engine->getRenderInterface()->getDrawCalls());

    decal.bake();
    glm::vec3 decal_mesh_pos = decal.getMesh()->getGlobalPosition();
    glm::vec3 camera_pos = camera.getGlobalPosition();

    decal.getMesh()->setScale(glm::vec3{1, 1, 1} / glm::distance(decal_mesh_pos, camera_pos) * 3);


    glm::vec3 cam_rot = camera.getRotationEuler();
    // HATE_INFO_F("Camera rotation: %f | %f | %f", cam_rot.x, cam_rot.y, cam_rot.z);

    glm::vec2 raw_dir =
            // engine->Input.getVector(HateEngine::A, HateEngine::D, HateEngine::W, HateEngine::S);
            engine->Input.getVectorAction("left", "right", "forward", "backward");
    if (raw_dir.x != 0 or raw_dir.y != 0) {
        glm::vec2 dir = raw_dir * glm::vec2(delta * 60) * speed;

        glm::vec3 cam_rot = glm::radians(camera.getRotationEuler());

        // Full free movement with mouse up and down
        camera.offset(
                cos(cam_rot.y) * dir.y * cos(cam_rot.x) * 0.1, sin(cam_rot.x) * 0.1 * dir.y,
                -sin(cam_rot.y) * dir.y * cos(cam_rot.x) * 0.1
        );

        cam_rot.y += glm::pi<float>() * raw_dir.x / 2 * -1;

        camera.offset(cos(cam_rot.y) * fabs(dir.x) * 0.1, 0, -sin(cam_rot.y) * fabs(dir.x) * 0.1);
    }

    if (engine->Input.isActionPressed("down"))
        camera.offset(0, -0.1 * speed * delta * 60, 0);
    if (engine->Input.isActionPressed("up"))
        camera.offset(0, 0.1 * speed * delta * 60, 0);
}

glm::vec3 cam_dir;

void _physics_process(HateEngine::Engine* engine, double delta) {
    // float xaxisvalue = engine->Input.getAnyGamepadAxis(HateEngine::GamepadAxisLeftYUp);
    //  xaxisvalue = engine->Input.getActionAxis("left");
    // HATE_INFO_F("X axis value: %f", xaxisvalue);

    auto rb_points = rigidBody.getCollisionPoints();
    int rb_points_count = 0;
    bool is_floor = false;
    for (auto& point_body: *rb_points) {
        for (auto& point: point_body.second) {
            const glm::vec3 UP = {0, 1, 0};
            const glm::vec3& normal = point.normal;
            // HATE_WARNING_F("Normal: %f | %f | %f", normal.x, normal.y, normal.z);
            float angle = glm::orientedAngle(UP, normal, UP);
            // float angle = glm::angle(UP, normal);
            if (angle < 0.7) {
                is_floor = true;
                // rigidBody.setLinearVelocity(0, 0, 2);
            }
        }
    }
    // HATE_INFO_F("Collision points: %d", rb_points_count);
    // HATE_INFO_F("Is floor: %d", is_floor);


    /*HATE_ERROR_F(
            "W: %d H: %d X: %f Y: %f", engine->getResolution().x, engine->getResolution().y,
            engine->getDisplayScale().x, engine->getDisplayScale().y
    );*/
    glm::vec3 cam_global_pos = camera.getGlobalPosition();

    /*HATE_INFO_F("Camera position: %f | %f | %f", cam_global_pos.x, cam_global_pos.y,
    cam_global_pos.z);*/
    /*HateEngine::RayCastInfo rayCastInfo;
    if (ray->isCollide(&rayCastInfo)) {
        // HATE_DEBUG("Is collide: true");
        // HATE_DEBUG_F("Body UUID: %llu", rayCastInfo.body->getUUID().getU64());
        HATE_INFO_F(
                "x: %f, y: %f, z: %f", rayCastInfo.worldPoint.x, rayCastInfo.worldPoint.y,
                rayCastInfo.worldPoint.z
        );
    }*/
    // HATE_DEBUG_F("Y: %f", rigidBody.getPosition().y);
    //  ray->isCollide();
    //  std::cout << engine->getResolution().x << " | " << engine->getResolution().y << "\n";
    test_glmodel.rotate(1, 0, 0);
    // std::cout << camera.getRotationEuler().x << " " << camera.getRotationEuler().y << " " <<
    // camera.getRotationEuler().z << "\n"; std::cout << "FPS: " << 1.0 / delta << "\n";
    glm::vec3 mesh1_r = mesh1.getGlobalRotationEuler();
    // std::cout << mesh1_r.x << " " << mesh1_r.y << " " << mesh1_r.z << "\n";


    ambient_bus_volume_label.text =
            "Ambient: " + std::to_string(int(std::ceil(ambient_bus->getVolume() * 100)));
    music_bus_volume_label.text =
            "Music: " + std::to_string(int(std::ceil(music_bus->getVolume() * 100)));


    /*glm::vec3 nav_dir =
            nav_agent_ptr->getDirectionByPosition(nav_agent_cube.getGlobalPosition()) * delta * 5;

    nav_agent_cube.offset(nav_dir.x, nav_dir.y, nav_dir.z);*/


    /*glm::vec2 dir = engine->Input.getVector(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S) *
    glm::vec2(delta * 60); glm::vec3 cam_rot = glm::radians(camera.getRotationEuler());

    // Full free movement with mouse up and down
    camera.offset(cos(cam_rot.y) * dir.y * cos(cam_rot.x) * 0.1,
                  sin(cam_rot.x) * 0.1 * dir.y,
                  -sin(cam_rot.y) * dir.y * cos(cam_rot.x) * 0.1
    );

    std::cout << cam_rot.y << " | ";
    cam_rot.y += glm::pi<float>() * dir.x / 2 * -1;

    std::cout << cam_rot.y << "\n";

    camera.offset(cos(cam_rot.y) * fabs(dir.x) * 0.1,
                  0,
                  -sin(cam_rot.y) * fabs(dir.x) * 0.1
    );*/


    // mesh1.setPosition(rbodyPosVect.z, rbodyPosVect.y, rbodyPosVect.x);

    if (engine->Input.isKeyPressed(HateEngine::KeyEscape))
        engine->Exit();


    /*if (engine->Input.isKeyPressed(GLFW_KEY_UP))
        rbody_body->setLinearVelocity({0, -1, 0});*/


    if (engine->Input.isKeyPressed(HateEngine::KeyQ))
        head.rotate(0, 1, 0);

    if (engine->Input.isKeyPressed(HateEngine::KeyE)) {
        std::vector<HateEngine::Mesh> meshes = {};
        meshes.reserve(64535);
        for (int i = 0; i < 64535; ++i) {
            meshes.push_back(HateEngine::Mesh(mesh2));
        }
        HATE_INFO_F("Meshes size: %d", meshes.size());
        HATE_INFO_F("Meshes capacity: %d", meshes.capacity());
        HATE_INFO_F("LAST UUID: %llu\n", meshes.back().getUUID().getU64());
    }

    if (engine->Input.isKeyPressed(HateEngine::KeyT)) {
        cube_part_ptr->reset();
        cube_part_ptr->play();
    }


    if (engine->Input.isKeyPressed(HateEngine::KeyY))
        engine->mainWindow->setMouseMode(HateEngine::OSDriverInterface::Normal);
    if (engine->Input.isKeyPressed(HateEngine::KeyU))
        engine->mainWindow->setMouseMode(HateEngine::OSDriverInterface::Disabled);

    /*if (engine->Input.isKeyPressed(GLFW_KEY_P))
        engine->setFullScreen(!engine->getFullScreen());*/

    glm::vec2 raw_dir =
            engine->Input.getVector(
                    HateEngine::KeyLeft, HateEngine::KeyRight, HateEngine::KeyUp, HateEngine::KeyDown
            ) *
            5;
    // float pb_x = playerBody.reactRigidBody->getLinearVelocity().x;
    // float pb_y = playerBody.reactRigidBody->getLinearVelocity().y;
    // float pb_z = playerBody.reactRigidBody->getLinearVelocity().z;
    // playerBody.reactRigidBody->setLinearVelocity({-raw_dir.y, pb_y, raw_dir.x});
    // playerBody.setLinearVelocity({raw_dir.x, pb_y, raw_dir.y});

    // glmodel.rotate(0, glmodel_rot.x, 0);
    //  glmodel.offset(0, glmodel_rot.y / 10, 0);


    /*std::cout << "GLOBAL POS: " << glmodel.getGlobalPosition().x << " " <<
        glmodel.getGlobalPosition().y << " " <<
        glmodel.getGlobalPosition().z << '\n';*/
}


// Функция для генерации луча
glm::vec3 computeRayDirection(
        double cursorX, double cursorY, int screenWidth, int screenHeight,
        const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix, const glm::vec3& cameraPosition
) {
    // 1. Преобразуем координаты курсора в NDC
    float x = (2.0f * cursorX) / screenWidth - 1.0f;
    float y = 1.0f - (2.0f * cursorY) / screenHeight; // Инверсия оси Y
    float z = 1.0f; // Глубина для "дальнего плана"
    glm::vec3 rayNDC(x, y, z);

    // 2. Преобразуем из NDC в координаты камеры (view space)
    glm::vec4 rayClip(rayNDC.x, rayNDC.y, -1.0f, 1.0f); // -1.0f для задней плоскости
    glm::vec4 rayEye = glm::inverse(projectionMatrix) * rayClip;
    rayEye = glm::vec4(rayEye.x, rayEye.y, -1.0f, 0.0f); // Устанавливаем w = 0 для направления

    // 3. Преобразуем из координат камеры в мировое пространство
    glm::vec4 rayWorld = glm::inverse(viewMatrix) * rayEye;
    glm::vec3 rayDirection = glm::normalize(glm::vec3(rayWorld));

    return rayDirection;
}


float lastX = 0;
float lastY = 0;
float sensitivity = 0.05;
bool is_first_iter = true;
void _input_event(HateEngine::Engine* engine, const HateEngine::InputEventInfo& event) {
    if (event.type == HateEngine::InputEventType::InputEventMouseMove) {
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

        if (engine->mainWindow->getMouseMode() == HateEngine::OSDriverInterface::Disabled) {
            camera.rotate(0, -xoffset, 0, false);
            camera.rotate(-yoffset, 0, 0, true);
        }
        // camera.setRotationMatrix(glm::yawPitchRoll(event.position.x*0.01f,
        // event.position.y*0.01f,0.0f));
    }

    if (event.type == HateEngine::InputEventMouseScroll) {
        if (event.offset.y < 0)
            // fps_widget_ptr->zoom(-0.1);
            speed /= 1.1;
        else
            speed *= 1.1;
        // fps_widget_ptr->zoom(0.1);
    }

    if (event.type == HateEngine::InputEventMouseButton) {
        if (event.button == HateEngine::MouseButtonLeft && event.isPressed) {
            // Координаты курсора
            glm::vec3 rayDirection =
                    camera.getProjectRayFromScreen(event.position, engine->getResolution());

            // Вывод направления луча
            /*std::cout << "Ray Direction: (" << rayDirection.x << ", " << rayDirection.y << ", "
                      << rayDirection.z << ")" << std::endl;*/


            HateEngine::RayCast ray = HateEngine::RayCast(engine->getLevel()->getPhysEngine(), 50);
            ray.setPosition(camera.getGlobalPosition());
            ray.enableCustomDirection(rayDirection);

            std::vector<HateEngine::RayCastInfo> hits =
                    ray.getSortedByDistanceAllCollisions(camera.getGlobalPosition());

            HateEngine::RayCastInfo rayCastInfo;
            // if (ray.isCollide(&rayCastInfo)) {
            if (not hits.empty()) {
                rayCastInfo = hits[0];
                HATE_INFO_F(
                        "x: %f, y: %f, z: %f", rayCastInfo.worldPoint.x, rayCastInfo.worldPoint.y,
                        rayCastInfo.worldPoint.z
                );

                HateEngine::CubeMesh cube;
                // cube.setSize(0.1, 0.1, 0.1);
                cube.setPosition(rayCastInfo.worldPoint);
                // lvl.addObjectClone(cube);
            }
        }
    }

    if (event.type == HateEngine::InputEventKey) {
        if (event.key == HateEngine::KeyI && event.isPressed) {
            engine->mainWindow->setWindowMode(HateEngine::OSDriverInterface::OSWindow::Window);
        }
        if (event.key == HateEngine::KeyO && event.isPressed) {
            engine->mainWindow->setWindowMode(HateEngine::OSDriverInterface::OSWindow::Fullscreen);
        }
        if (event.key == HateEngine::KeyP && event.isPressed) {
            engine->mainWindow->setWindowMode(HateEngine::OSDriverInterface::OSWindow::WindowedFullscreen
            );
        }


        if (event.key == HateEngine::KeyM and event.isPressed) {
            glm::vec3 velocity = rigidBody.getLinearVelocity();
            velocity.x = 1;
            rigidBody.setLinearVelocity(velocity);
        }

        if (event.key == HateEngine::KeyJ and event.isPressed) {
            // lvl.getPhysEngine()->removeObject(floorBody.getUUID());
            // floorBody.setIsActive(false);
            // rigidBody.setIsActive(!rigidBody.getIsActive());
            HateEngine::CollisionShape* shape =
                    (HateEngine::CollisionShape*) rigidBody.getShapes()->begin()->second;
            // shape->setCollisionMask(0);
            // shape->setCollisionCategoryRaw(-1);
            rigidBodyBoxShape.setCollisionMask(0);
            rigidBodyBoxShape.setCollisionCategoryRaw(0);
        }

        if (event.key == HateEngine::KeyK and event.isPressed) {
            rigidBody.setLinearVelocity({0, 4, 0});
        }

        if (event.key == HateEngine::KeyNumPadAdd && event.isPressed) {
            lvl.setAmbientLightIntensity(lvl.getAmbientLightIntensity() + 0.1);
        }
        if (event.key == HateEngine::KeyNumPadSubtract && event.isPressed) {
            lvl.setAmbientLightIntensity(lvl.getAmbientLightIntensity() - 0.1);
        }
    }
}
