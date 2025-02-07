#include "HateEngine/AudioBus.hpp"
#include "HateEngine/Input.hpp"
#include "HateEngine/NavMeshAgent.hpp"
#include "HateEngine/Objects/Decal.hpp"
#include "HateEngine/Objects/Light/OmniLight.hpp"
#include "HateEngine/Objects/Light/SpotLight.hpp"
#include "HateEngine/Objects/Physics/CapsuleShape.hpp"
#include "HateEngine/Objects/Physics/CollisionShape.hpp"
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
void _input_event(HateEngine::Engine*, HateEngine::Engine::InputEventInfo);

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

HateEngine::Particles* part;
HateEngine::RayCast* ray;

HateEngine::LabelUI fps_label;
HateEngine::WidgetUI* fps_widget_ptr = nullptr;
HateEngine::CubeMesh test2;
HateEngine::PhysicalBody playerBody(HateEngine::PhysicalBody::DynamicBody);
HateEngine::PhysicalBody rigidBody(HateEngine::PhysicalBody::DynamicBody);
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


int main() {
//int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
//	LPSTR lpCmdLine, int nCmdShow) {
    xAxMesh.setSize(1, 0.1, 0.1);
    xAxMesh.offset(0, 6, 0);

    camera.setPosition(0, 6, 3);
    // camera.offset(0, 6, 3);
    // camera.setPosition(0, 25, 0);
    camera.setRotation(0, 0, 0);
    // camera.setSkyBoxTexture(new HateEngine::Texture("examples/Assets/skybox.jpg",
    // HateEngine::Texture::ClampToEdge)); camera.setSkyBoxEnabled(true);
    mesh1.setRotation(0, 0, 0);
    mesh1.setSize(1, 1, 1);

    billboardMesh.offset(-4, 4, -4);

    HateEngine::CubeMesh floor;
    floor.setPosition(0, 0, 0);
    floor.setSize(25, 1, 25);
    HATE_INFO("Info message");
    HATE_WARNING("Warning message");
    HATE_ERROR("Error message");
    HATE_DEBUG("Hello");
    // HATE_FATAL("Fatal message");

    // floor.setRotation(20, 0, 0);


    HateEngine::DirectionalLight dirLight;
    // dirLight.rotate(45, 0, 0);
    glm::vec3 dir = dirLight.getDirection();

    light.setVisible(true);
    light.setColor({1.0, 0, 0, 1.0});

    // HATE_FATAL_F("Direction x: %f, y: %f, z: %f", dir.x, dir.y, dir.z);

    // floor.setRotationMatrix(matfloor);
    //  floor.matrix
    //


    // sun.setPosition({1.0, 1.0, 1.0});
    sun.rotate(-45, 45, 0);
    sun.setVisible(false);

    mesh2.setPosition(3, 3, 3);


    // test_glmodel.setVisible(false);


    // camera.bindObj(&test_glmodel);

    HATE_DEBUG_F("Camera uuid: %llu", camera.getUUID().getU64());
    // mesh1.bindObj(&test_glmodel);
    HateEngine::Engine game("HateEngine Test", WIDTH, HEIGHT);
    // std::cout << sizeof(game) << "\n";
    // while(true) {}

    game.setMouseCapture(true);
    // std::cout << "\n\n\n\n" << glfwGetInputMode(game.window, GLFW_CURSOR) << "\n\n\n\n";
    game.setOneThreadMode(false);
    game.setVSync(false);

    game.onLevelChanged.connect([](HateEngine::Engine* e, HateEngine::Level* lvl, HateEngine::Level* old) {
        HATE_INFO("Level changed");
        HATE_INFO_F("Old: %p, New: %p", old, lvl);
    });

    game.Input.addKeyToAction("forward", HateEngine::KeyW);
    game.Input.addKeyToAction("forward", HateEngine::KeyNumPad8);
    game.Input.addKeyToAction("backward", HateEngine::KeyS);
    game.Input.addKeyToAction("backward", HateEngine::KeyNumPad5);
    game.Input.addKeyToAction("left", HateEngine::KeyA);
    game.Input.addKeyToAction("left", HateEngine::KeyNumPad4);
    game.Input.addKeyToAction("right", HateEngine::KeyD);
    game.Input.addKeyToAction("right", HateEngine::KeyNumPad6);
    game.Input.addKeyToAction("up", HateEngine::KeySpace);
    game.Input.addKeyToAction("down", HateEngine::KeyLeftControl);
    game.Input.addKeyToAction("down", HateEngine::KeyLeftSuper);
    // Setting textures for the cube and floor meshes


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
    // lvl.addObjectRef(&radioModel);


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

    ambientPlayer.play();
    audioPlayer1.play();
    audioPlayer2.play();

    /*===============================================================*/



    // playerCapsuleMesh.getLOD(0)[0]->setColor(glm::vec4(1, 0, 0, 1));

    HateEngine::CubeMesh colorCube;
    colorCube.offset(3, 3, 3);
    // colorCube.setUV({0, 0,0.19731724, 0,0.19731724, 0.19731724,0.3946345, 0.19731724,0.19731724,
    // 0.19731724,0.19731724, 0,0, 0.3946345,0, 0.19731724,0.19731724, 0.19731724,0.5919517,
    // 0,0.5919517, 0.19731724,0.3946345, 0.19731724,0, 0.3946345,0.19731724, 0.3946345,0.19731724,
    // 0.5919517,0.3946345, 0.3946345,0.19731724, 0.3946345,0.19731724, 0.19731724,0.5919517,
    // 0,0.789269, 0,0.789269, 0.19731724,0.19731724, 0.789269,0, 0.789269,0, 0.5919517,0.5919517,
    // 0.19731724,0.5919517, 0.3946345,0.3946345, 0.3946345,0.19731724, 0.5919517,0.19731724,
    // 0.3946345,0.3946345, 0.3946345,0.789269, 0.19731724,0.789269, 0,0.9865862, 0,0.19731724,
    // 0.789269,0.19731724, 0.9865862,0, 0.9865862,});
    colorCube.setLightUV({
            0,          0, 0.16666667, 0, 0.16666667, 1, 0.16666667, 1, 0,          1,
            0,          0, 0.16666667, 0, 0.33333334, 0, 0.33333334, 1, 0.33333334, 1,
            0.16666667, 1, 0.16666667, 0, 0.33333334, 0, 0.33333334, 1, 0.5,        1,
            0.5,        1, 0.5,        0, 0.33333334, 0, 0.5,        0, 0.6666667,  0,
            0.6666667,  1, 0.6666667,  1, 0.5,        1, 0.5,        0, 0.6666667,  0,
            0.8333334,  0, 0.8333334,  1, 0.8333334,  1, 0.6666667,  1, 0.6666667,  0,
            0.8333334,  1, 0.8333334,  0, 1,          0, 1,          0, 1,          1,
            0.8333334,  1,
    });


    HateEngine::Texture cube_lightmap(
            "examples/Assets/cube_lightmap.png", HateEngine::Texture::Repeat,
            HateEngine::Texture::Nearest
    );
    colorCube.setLightTexture(&cube_lightmap);
    // colorCube.disableLightShading();
    colorCube.setSize(3, 3, 3);

    HateEngine::Texture bri("examples/Assets/brick.png");
    // HATE_FATAL_F("BRI loaded: %d", bri.isLoaded());
    colorCube.setTexture(&bri);


    // colorCube.setColor({0.4, 0.1, 0.1});
    //  colorCube.disableLightShading();

    // lvl.addObjectRef(&colorCube);

    


    camera.bindObj(&decal);
    decal.setRayLength(10);

    // lvl.addObjectRef(decal.getMesh());
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
    trigger.addCollisionShapeRef(&box);


    lvl.getPhysEngine()->addObjectRef(&trigger);

    /*HateEngine::Audio* audio = new HateEngine::Audio("examples/Assets/audio.ogg");
    // HateEngine::Audio audio = herfile["audio.ogg"].asAudio();
    HATE_WARNING("WARNING");
    HateEngine::AudioPlayer audioPlayer(audio, nullptr, HateEngine::AudioPlayer::Audio3D);
    // delete audio;
    audioPlayer.play();*/

    playerCapsuleMesh.bindObj(&test_glmodel);

    // test_glmodel->offset(0.5, -0.5, -2);
    test_glmodel.setScale(0.25, 0.25, 0.25);
    test_glmodel.setRotation(0, 180, 0);


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
    // lvl.addObjectRef(&uv_test_cube);

    // HateEngine::GLTFModel glmodel2("examples/Assets/ignore/bolg.glb");

    game.setLevelRef(&lvl);
    // lvl.addObjectRef(&test_glmodel);
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
    // lvl.addObjectRef(&billboardMesh);

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
    HateEngine::ObjMapModel objmodel(
            "examples/Assets/Ignore/E1M1.obj", "examples/Assets/Ignore/E1M1.MAP",
            "examples/Assets/Ignore/light.heluv","examples/Assets/Ignore/E1M1.hepvs", 16.0, true, 15, 1
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
                  cube->setPosition(entity.position);
                  model->bindObj(cube);
                  //model->addEntityObjectToLevel(cube);
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

    lvl.addObjectRef(&objmodel);


    std::unordered_set<std::string> enabled = {
            "entity0_brush0",    "entity0_brush1",    "entity0_brush2",    "entity0_brush3",
            "entity0_brush4",    "entity0_brush5",    "entity0_brush6",    "entity0_brush8",
            "entity0_brush9",    "entity0_brush10",   "entity0_brush11",   "entity0_brush12",
            "entity0_brush13",   "entity0_brush15",   "entity0_brush16",   "entity0_brush17",
            "entity0_brush18",   "entity0_brush19",   "entity0_brush20",   "entity0_brush21",
            "entity0_brush22",   "entity0_brush23",   "entity0_brush24",   "entity0_brush25",
            "entity0_brush27",   "entity0_brush28",   "entity0_brush29",   "entity0_brush30",
            "entity0_brush31",   "entity0_brush32",   "entity0_brush33",   "entity0_brush34",
            "entity0_brush36",   "entity0_brush38",   "entity0_brush39",   "entity0_brush40",
            "entity0_brush41",   "entity0_brush42",   "entity0_brush43",   "entity0_brush44",
            "entity0_brush45",   "entity0_brush46",   "entity0_brush47",   "entity0_brush49",
            "entity0_brush50",   "entity0_brush51",   "entity0_brush52",   "entity0_brush53",
            "entity0_brush54",   "entity0_brush55",   "entity0_brush56",   "entity0_brush60",
            "entity0_brush61",   "entity0_brush62",   "entity0_brush63",   "entity0_brush64",
            "entity0_brush65",   "entity0_brush66",   "entity0_brush67",   "entity0_brush68",
            "entity0_brush69",   "entity0_brush70",   "entity0_brush71",   "entity0_brush72",
            "entity0_brush73",   "entity0_brush74",   "entity0_brush75",   "entity0_brush76",
            "entity0_brush77",   "entity0_brush78",   "entity0_brush79",   "entity0_brush80",
            "entity0_brush81",   "entity0_brush82",   "entity0_brush83",   "entity0_brush84",
            "entity0_brush85",   "entity0_brush86",   "entity0_brush87",   "entity0_brush88",
            "entity0_brush95",   "entity0_brush120",  "entity0_brush121",  "entity0_brush131",
            "entity0_brush132",  "entity0_brush134",  "entity0_brush135",  "entity0_brush136",
            "entity0_brush137",  "entity0_brush141",  "entity0_brush395",  "entity0_brush397",
            "entity0_brush398",  "entity0_brush399",  "entity0_brush401",  "entity0_brush402",
            "entity0_brush403",  "entity0_brush404",  "entity0_brush405",  "entity0_brush406",
            "entity0_brush407",  "entity0_brush408",  "entity0_brush409",  "entity0_brush410",
            "entity0_brush411",  "entity0_brush412",  "entity0_brush413",  "entity0_brush414",
            "entity0_brush1062", "entity0_brush1073", "entity0_brush1075", "entity0_brush1084",
            "entity0_brush1085", "entity0_brush1086", "entity0_brush1087", "entity0_brush1088",
            "entity0_brush1089", "entity0_brush1107", "entity0_brush1109", "entity0_brush1112",
            "entity14_brush0",   "entity14_brush1",   "entity14_brush2",   "entity15_brush0",
            "entity15_brush1",   "entity15_brush2",   "entity310_brush0",  "entity338_brush0",
            "entity339_brush0",  "entity352_brush0",  "entity353_brush0",


    };

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

    floor.disableLightShading();

    lvl.setAmbientLightColor(255, 255, 255);
    lvl.setAmbientLightIntensity(1);


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

    // lvl.addObjectRef(&objmodel);


    /*std::cout << glmodel.getGlobalPosition().x << " " << glmodel.getGlobalPosition().y << " "
              << glmodel.getGlobalPosition().z << "\n";*/

    floor.setTexture(&tex_floor);
    floor.setUV({
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

    lvl.addObjectRef(&mesh1);
    // lvl.addObjectRef(&mesh2);
    // lvl.addObjectRef(&xAxMesh);
    lvl.addObjectRef(&floor);
    lvl.addObjectRef(&sun);

    /*HateEngine::GLTFModel dance_animation("examples/Assets/ignore/dance.glb");
    HateEngine::GLTFAnimationPlayer anim_player(&dance_animation);
    anim_player.offset(5, 0, 5);
    anim_player.setFPS(60);
    anim_player.setLoop(true);
    anim_player.play("dance_man");
    lvl.addObjectRef(&anim_player);*/


    // lvl.addObjectRef(&dance_animation);
    /* for (auto& mesh : dance_animation.getMeshes()) {
         mesh->disableLightShading();
         mesh->enableCustomMaxLightDist(2500);
     }*/

    // lvl.addObjectRef(&glmodel);

    // print glmodel meshes id


    // glmodel.offset({0, 1, 0});
    //  glmodel.getMeshes()[0]->disableLightShading();

    // light.setPosition({0, 2, 0});
    // l
    light.setColor(light.getColor() * 10);
    lvl.addObjectRef(&light);
    // light.color = {10, 10, 10, 1};
    light.setExponent(20);

    camera.bindObj(&light);

    lvl.setFixedProcessLoop([](void* engine, double delta) {
        ////TODO: Change engine pointer to struct with Engine*, Level*
        HateEngine::Engine* _engine = (HateEngine::Engine*) engine;
        // _engine->getLevel()->ob
        // std::cout << "Level fixed process loop dealay: " << delta << "\n";
        //_process(engine, delta);
    });

    // game.addObjectRef(&tomato2);
    HateEngine::Particle::ParticleSettings pa_set = {6.0f, 6.0f, false, {0, 0, 0}, {4, 0, 4}};

    HateEngine::Texture snow_tex(
            "examples/Assets/snow.png", HateEngine::Texture::Clamp, HateEngine::Texture::Linear
    );
    HateEngine::CubeMesh snow_mesh;
    snow_mesh.setSize(0.05, 0.05, 0.05);

    snow_mesh.setTexture(&snow_tex);
    // snow_mesh.setSize(0.8, 0.8, 0.8);
    // snow_mesh.setPosition(0, 5, 0);
    // lvl.addObjectRef(&snow_mesh);
    //  tomato.setScale({0.05, 0.05, 0.05});
    HateEngine::Particles cube_part((HateEngine::Mesh) snow_mesh, 100, pa_set);
    cube_part.calculateFunc = [](HateEngine::Particle* p, double delta) {
        if (p->data.count("vel") == 0)
            p->data["vel"] = (void*) new glm::vec3(0, 0, 0);
        //((glm::vec3*)(p->data["vel"]))->y += delta * -9.8;
        ((glm::vec3*) (p->data["vel"]))->y -= delta * 9.8;
        // if (p->index == 0)
        // std::cout << ((glm::vec3*)(p->data["vel"]))->y << "\n";
        // std::cout << float(delta * 9.8) << "\n";
        // std::cout << ((glm::vec3*)(p->data["vel"]))->y << "\n";
        glm::vec3 off = *((glm::vec3*) (p->data["vel"])) * glm::vec3(delta);
        // std::cout << off << "\n";
        // p->offset(off);
        p->offset({0, -0.1 * delta, 0});
        glm::vec3 scale = p->getScale() * 0.99;

        p->setScale(scale);


        // std::cout <<
    };

    cube_part.onParticleDelete = [](HateEngine::Particle* p) {
        delete (glm::vec3*) p->data["vel"];
    };

    cube_part.setPosition(4, 4, 4);

    // cube_part.play();

    cube_part_ptr = &cube_part;
    // lvl.addObjectRef(&cube_part);


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
    floorBody.addCollisionShapeRef(&floorShape);
    // floorBody.rotate(20, 0, 0);
    floorBody.bindObj(&floor);

    rigidBody.setPosition(0, 5, 0);
    rigidBody.rotate(48, 22, 36);
    // rigidBody.setRotation(0, 0, 0);
    rigidBody.addCollisionShapeRef(&rigidBodyBoxShape);
    HateEngine::SphereShape sphereShape(0.5);
    // rigidBody.addCollisionShapeRef(&sphereShape);
    rigidBody.bindObj(&mesh1);
    // rigidBody.bindObj(&camera);
    // rigidBody.bindObj(&glmodel);

    // HateEngine::CubeMesh playerCubeMesh;
    playerCapsuleMesh.rotate(0, 0, 0);

    playerBody.setPosition(0, 3, 0);
    playerBody.rotate(0, 0, 0);
    HateEngine::CapsuleShape capsuleShape(0.5, 2);
    playerBody.addCollisionShapeRef(&capsuleShape);
    playerBody.bindObj(&playerCapsuleMesh);

    lvl.getPhysEngine()->addObjectRef(&rigidBody);
    lvl.getPhysEngine()->addObjectRef(&playerBody);
    lvl.getPhysEngine()->addObjectRef(&floorBody);
    //playerBody.reactRigidBody->setAngularLockAxisFactor({0, 0, 0});
    playerBody.setAngularLockAxisFactor(0, 0, 0);
    // playerBody.reactRigidBody->setLinearVelocity({2, 0.1, 2});
    //capsuleShape.reactCollider->getMaterial().setFrictionCoefficient(0);
    capsuleShape.setFriction(0);


    lvl.addObjectRef(&playerCapsuleMesh);


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
            "examples/Assets/campfire.png", HateEngine::Texture::Repeat,
            HateEngine::Texture::Nearest
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
    checkbox.position = {
            -40, -20, 1, HateEngine::CoordsUI::CenterRight, HateEngine::CoordsUI::Pixels
    };
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


    lvl.addObjectRef(&fps_widget);
    // lvl.addObjectRef(&ui);

    // playerCapsuleMesh.bindObj(&camera);
    head.bindObj(&camera);
    head.bindObj(&uv_test_cube);


    // Audio UI
    HateEngine::WidgetUI audio_widget;
    audio_widget.position = {
            255, 5, 1, HateEngine::CoordsUI::TopRight, HateEngine::CoordsUI::Pixels
    };
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


    lvl.addObjectRef(&audio_widget);


    /* ==========================> Navigation Test <========================= */

    /*HateEngine::ObjMapModel nav_obj_map(
            "examples/Assets/NavMeshTest/dungeon.obj", "", "", 1, false, 10000000, 100000000
    );
    lvl.addObjectRef(&nav_obj_map);*/

    /*HateEngine::NavMesh nav_mesh("examples/Assets/NavMeshTest/solo_navmesh.bin");

    HateEngine::NavMeshAgent nav_mesh_agent(&nav_mesh, 2048);
    nav_mesh_agent.computePath({40, 10, 0}, {11, 10, -40});

    nav_agent_ptr = &nav_mesh_agent;
    nav_agent_cube.setPosition(40, 10, 0);
    lvl.addObjectRef(&nav_agent_cube);*/

    /* ==========================> Navigation Test <========================= */


    game.setProcessLoop(_process);
    game.setFixedProcessLoop(_physics_process);
    game.setInputEvent(_input_event);
    lvl.setCameraRef(&camera);
    float xscale, yscale;
    // glfwGetPrimaryMonitor();
    //glfwGetMonitorContentScale(glfwGetPrimaryMonitor(), &xscale, &yscale);
    // std::cout << "Content scale: " << xscale << " " << yscale << "\n";
    //  while (true) {}
    game.Run();
    //int p = glfwGetPlatform();
    //std::cout << p << " | " << GLFW_PLATFORM_WAYLAND << "\n";
    // HateEngine::AudioServer::Deinit();
}


int frames_count = 0;
float speed = 1;
double fps_time = 0.0;
void _process(HateEngine::Engine* engine, double delta) {
    if (fps_time < 0.5) {
        ++frames_count;
        fps_time += delta;
    } else {
        // std::cout << "FPS: " << (float)count / del << std::endl;
        fps_label.text = "FPS: " + std::to_string((float) frames_count / fps_time);
        // HATE_INFO_F("FPS: %f", (float) frames_count / fps_time);

        frames_count = 0;
        fps_time = 0.0;
    }

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
    /*HATE_ERROR_F(
            "W: %d H: %d X: %f Y: %f", engine->getResolution().x, engine->getResolution().y,
            engine->getDisplayScale().x, engine->getDisplayScale().y
    );*/
    glm::vec3 cam_global_pos = camera.getGlobalPosition();

    /*HATE_INFO_F("Camera position: %f | %f | %f", cam_global_pos.x, cam_global_pos.y,
    cam_global_pos.z);*/
    HateEngine::RayCastInfo rayCastInfo;
    if (ray->isCollide(&rayCastInfo)) {
        // HATE_DEBUG("Is collide: true");
        // HATE_DEBUG_F("Body UUID: %llu", rayCastInfo.body->getUUID().getU64());
        HATE_INFO_F(
                "x: %f, y: %f, z: %f", rayCastInfo.worldPoint.x, rayCastInfo.worldPoint.y,
                rayCastInfo.worldPoint.z
        );
    }
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
        engine->setMouseCapture(false);
    if (engine->Input.isKeyPressed(HateEngine::KeyU))
        engine->setMouseCapture(true);

    /*if (engine->Input.isKeyPressed(GLFW_KEY_P))
        engine->setFullScreen(!engine->getFullScreen());*/

    glm::vec2 raw_dir = engine->Input.getVector(
                                HateEngine::KeyLeft, HateEngine::KeyRight, HateEngine::KeyUp,
                                HateEngine::KeyDown
                        ) *
                        5;
    //float pb_x = playerBody.reactRigidBody->getLinearVelocity().x;
    //float pb_y = playerBody.reactRigidBody->getLinearVelocity().y;
    //float pb_z = playerBody.reactRigidBody->getLinearVelocity().z;
    //playerBody.reactRigidBody->setLinearVelocity({-raw_dir.y, pb_y, raw_dir.x});
    //playerBody.setLinearVelocity({raw_dir.x, pb_y, raw_dir.y});

    // glmodel.rotate(0, glmodel_rot.x, 0);
    //  glmodel.offset(0, glmodel_rot.y / 10, 0);


    /*std::cout << "GLOBAL POS: " << glmodel.getGlobalPosition().x << " " <<
        glmodel.getGlobalPosition().y << " " <<
        glmodel.getGlobalPosition().z << '\n';*/
}


// Функция для генерации луча
glm::vec3 computeRayDirection(
        double cursorX, double cursorY, int screenWidth, int screenHeight,
        const glm::mat4& projectionMatrix, const glm::mat4& viewMatrix,
        const glm::vec3& cameraPosition
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
void _input_event(HateEngine::Engine* engine, HateEngine::Engine::InputEventInfo event) {
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

        if (engine->getMouseCapture()) {
            camera.rotate(0, -xoffset, 0, false);
            camera.rotate(-yoffset, 0, 0, true);
        }
        // camera.setRotationMatrix(glm::yawPitchRoll(event.position.x*0.01f,
        // event.position.y*0.01f,0.0f));
    }

    if (event.type == HateEngine::Engine::InputEventMouseScroll) {
        if (event.position.y < 0)
            // fps_widget_ptr->zoom(-0.1);
            speed /= 1.1;
        else
            speed *= 1.1;
        // fps_widget_ptr->zoom(0.1);
    }

    if (event.type == HateEngine::Engine::InputEventMouseButton) {
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
                lvl.addObjectClone(cube);
            }
        }
    }

    if (event.type == HateEngine::Engine::InputEventKey) {
        if (event.key == HateEngine::KeyP && event.isPressed) {
            engine->setFullScreen(!engine->getFullScreen());
            HATE_WARNING("Toggled fullscreen")
        }

        if (event.key == HateEngine::KeyJ and event.isPressed) {
            // lvl.getPhysEngine()->removeObject(floorBody.getUUID());
            // floorBody.setIsActive(false);
            // rigidBody.setIsActive(!rigidBody.getIsActive());
            HateEngine::CollisionShape* shape =
                    (HateEngine::CollisionShape*) rigidBody.getShapes()->begin()->second.shape;
            // shape->setCollisionMask(0);
            // shape->setCollisionCategoryRaw(-1);
            rigidBodyBoxShape.setCollisionMask(0);
            rigidBodyBoxShape.setCollisionCategoryRaw(0);
        }

        if (event.key == HateEngine::KeyNumPadAdd && event.isPressed) {
            lvl.setAmbientLightIntensity(lvl.getAmbientLightIntensity() + 0.1);
        }
        if (event.key == HateEngine::KeyNumPadSubtract && event.isPressed) {
            lvl.setAmbientLightIntensity(lvl.getAmbientLightIntensity() - 0.1);
        }
    }
}
