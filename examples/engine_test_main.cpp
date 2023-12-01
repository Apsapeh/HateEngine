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
#include <Old3DEngine/Objects/Particles.hpp>

#include <reactphysics3d/reactphysics3d.h>

using namespace reactphysics3d;



// Define these only in *one* .cc file.
#define TINYGLTF_IMPLEMENTATION
#define STB_IMAGE_WRITE_IMPLEMENTATION
// #define TINYGLTF_NOEXCEPTION // optional. disable exception handling.
#include <tiny_gltf.h>

using namespace tinygltf;



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






    Model model;
    TinyGLTF loader;
    std::string err;
    std::string warn;

    //bool ret = loader.LoadASCIIFromFile(&model, &err, &warn, argv[1]);
    bool ret = loader.LoadBinaryFromFile(&model, &err, &warn, "examples/Assets/tomat.glb"); // for binary glTF(.glb)

    if (!warn.empty()) {
        printf("Warn: %s\n", warn.c_str());
    }

    if (!err.empty()) {
        printf("Err: %s\n", err.c_str());
    }

    if (!ret) {
        printf("Failed to parse glTF\n");
        return -1;
    }

    std::vector<std::vector<float>> i_v;
    std::vector<std::vector<uint32_t>> ix_v;
    std::vector<std::vector<float>> in_v;
    std::vector<std::vector<float>> uv_v;
    // Итерируем по всем mesh-ам модели
    for (const auto& mesh : model.meshes) {
        // Итерируем по всем primitive-ам в mesh
        for (const auto& primitive : mesh.primitives) {
            // Получаем доступ к атрибутам вершин
            const auto& attributes = primitive.attributes;

            // Получаем данные о вершинах
            const auto& position_accessor = model.accessors[attributes.find("POSITION")->second];
            const auto& position_view = model.bufferViews[position_accessor.bufferView];
            const auto& position_buffer = model.buffers[position_view.buffer];

            // Получаем указатель на данные о вершинах
            const float* positions = reinterpret_cast<const float*>(&(position_buffer.data[position_view.byteOffset + position_accessor.byteOffset]));

            // Получаем количество вершин
            int num_vertices = static_cast<int>(position_accessor.count);

            // Выводим координаты вершин
            std::vector<float> v;

            for (int i = 0; i < num_vertices; ++i) {
                float x = positions[i * 3];
                float y = positions[i * 3 + 1];
                float z = positions[i * 3 + 2];
                v.push_back(x);
                v.push_back(y);
                v.push_back(z);

                //std::cout << "Vertex " << i << ": (" << x << ", " << y << ", " << z << ")" << std::endl;
            }
            i_v.push_back(v);

            int mode = primitive.mode;


            const auto& indices_accessor = model.accessors[primitive.indices];
            const auto& indices_view = model.bufferViews[indices_accessor.bufferView];
            const auto& indices_buffer = model.buffers[indices_view.buffer];

            // Получаем указатель на данные о индексах
            const uint32_t* indices = reinterpret_cast<const uint32_t*>(&(indices_buffer.data[indices_view.byteOffset + indices_accessor.byteOffset]));

            // Получаем количество индексов
            int num_indices = static_cast<int>(indices_accessor.count);

            std::vector<uint32_t> ix;
            // Выводим индексы вершин
            for (int i = 0; i < num_indices; ++i) {
                ix.push_back(indices[i]);
                //std::cout << "Index " << i << ": " << indices[i] << std::endl;

            }
            ix_v.push_back(ix);
            std::cout << mode << '\n';

            std::vector<float> n;

            const auto& normal_accessor = model.accessors[attributes.find("NORMAL")->second];
            const auto& normal_view = model.bufferViews[normal_accessor.bufferView];
            const auto& normal_buffer = model.buffers[normal_view.buffer];

            // Получаем указатель на данные о нормалях
            const float* normals = reinterpret_cast<const float*>(&(normal_buffer.data[normal_view.byteOffset + normal_accessor.byteOffset]));

            // Получаем количество нормалей
            int num_normals = static_cast<int>(normal_accessor.count);

            // Выводим нормали
            for (int i = 0; i < num_normals; ++i) {
                float nx = normals[i * 3];
                float ny = normals[i * 3 + 1];
                float nz = normals[i * 3 + 2];

                n.push_back(nx);
                n.push_back(ny);
                n.push_back(nz);

                //std::cout << "Normal " << i << ": (" << nx << ", " << ny << ", " << nz << ")" << std::endl;
            }
            in_v.push_back(n);


            /*GLuint textureID = loadTexture(model.images[model.textures[model.materials[model.meshes[0].primitives[0].material].values["baseColorTexture"].TextureIndex()].source].uri);
            const auto& material_index = primitive.material;
            const auto& material = model.materials[material_index];
            // Проверяем наличие атрибута "baseColorTexture"
            if (material.values.find("baseColorTexture") != material.values.end()) {
                const auto& texture_index = material.values["baseColorTexture"].TextureIndex();
                const auto& texture = model.textures[texture_index];

                // Выводим информацию о текстуре
                std::cout << "Texture Index: " << texture_index << std::endl;
                std::cout << "Texture Path: " << model.images[texture.source].uri << std::endl;
                // Другие свойства текстуры, такие как сжатие, формат, могут быть получены из model.images[texture.source]

            } else {
                std::cout << "Mesh does not have a base color texture." << std::endl;
            }*/

            // Проверяем наличие атрибута "TEXCOORD_0" (первый набор текстурных координат)
            if (attributes.find("TEXCOORD_0") != attributes.end()) {
                const auto& texcoord_accessor = model.accessors[attributes.find("TEXCOORD_0")->second];
                const auto& texcoord_view = model.bufferViews[texcoord_accessor.bufferView];
                const auto& texcoord_buffer = model.buffers[texcoord_view.buffer];

                // Получаем указатель на данные о текстурных координатах
                const float* texcoords = reinterpret_cast<const float*>(&(texcoord_buffer.data[texcoord_view.byteOffset + texcoord_accessor.byteOffset]));

                // Получаем количество текстурных координат
                int num_texcoords = static_cast<int>(texcoord_accessor.count);

                std::vector<float> uv;
                // Выводим текстурные координаты
                for (int i = 0; i < num_texcoords; ++i) {
                    float u = texcoords[i * 2];
                    float v = texcoords[i * 2 + 1];
                    uv.push_back(u);
                    uv.push_back(v);

                    //std::cout << "Texcoord " << i << ": (" << u << ", " << v << ")" << std::endl;
                }
                uv_v.push_back(uv);
            }
            else {
                std::cout << "Mesh does not have texture coordinates." << std::endl;
            }
        }
    }

    std::vector<Old3DEngine::Texture> textures;
    for (auto m : model.textures) {
        int textureIndex = m.source;
        const tinygltf::Image& image = model.images[textureIndex];

        // Access image data
        const std::vector<unsigned char>& imageData = image.image;

        Old3DEngine::Texture::TexType t_form;
        if (image.component == 3)
            t_form = Old3DEngine::Texture::TexType::RGB;
        else if (image.component == 4)
            t_form = Old3DEngine::Texture::TexType::RGBA;
        else {
            Old3DEngine::Error::throwWarning("Неизвестный формат");
            continue;
        }
        std::cout << "Format: " << image.image.size() << "\n";


        textures.push_back(Old3DEngine::Texture(
            image.image, image.width, image.height, t_form,
            Old3DEngine::Texture::Repeat, Old3DEngine::Texture::Linear
        ));
    }

    /*std::vector<float> tomat_v, tomat_n;
    std::vector<uint32_t> tomat_i;
    tomat_v.insert(tomat_v.end(), i_v[0].begin(), i_v[0].end());
    tomat_v.insert(tomat_v.end(), i_v[1].begin(), i_v[1].end());
    tomat_n.insert(tomat_n.end(), in_v[0].begin(), in_v[0].end());
    tomat_n.insert(tomat_n.end(), in_v[1].begin(), in_v[1].end());

    tomat_i.insert(tomat_i.end(), ix_v[0].begin(), ix_v[0].end());

    for (auto i : ix_v[1]) {
        tomat_i.push_back(i + i_v[0].size());
    }
    //tomat_i.insert(tomat_i.end(), ix_v[1].begin(), ix_v[1].end());


    Old3DEngine::Mesh tomato(tomat_v, tomat_i, tomat_n);*/

    Old3DEngine::Mesh tomato_leaf(i_v[0], ix_v[0], in_v[0]);
    Old3DEngine::Mesh tomato(i_v[1], ix_v[1], in_v[1]);
    tomato.setTexture(&textures[0]);
    tomato.setUV(uv_v[1]);
    tomato_leaf.setTexture(&textures[0]);
    tomato_leaf.setUV(uv_v[0]);
    tomato.setScale(0.1, 0.1, 0.1);
    tomato_leaf.setScale(tomato.getScale());







    Quaternion qufloor = floor_body->getTransform().getOrientation();
    glm::quat qefloor(qufloor.w, qufloor.z, qufloor.y, qufloor.x);
    glm::mat4 matfloor = glm::toMat4(qefloor);
    floor.setRotationMatrix(matfloor);
    //floor.matrix


    sun.setPosition({1.0, 1.0, 1.0});

    mesh2.setPosition(0, 1, 10);


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
    game.addObjectRef(&mesh1);
    game.addObjectRef(&mesh2);
    game.addObjectRef(&xAxMesh);
    game.addObjectClone(floor);
    game.addObjectRef(&sun);
    game.addObjectRef(&tomato);
    game.addObjectRef(&tomato_leaf);
    //game.addObjectRef(&tomato2);
    std::cout << sizeof(Old3DEngine::Particles) << "\n";
    Old3DEngine::Particle::ParticleSettings pa_set = {
            6.0f, 6.0f, true,
            {0, 4, 0}, {4, 4, 4}
    };

    Old3DEngine::Texture snow_tex("examples/Assets/snow.png", Old3DEngine::Texture::Clamp, Old3DEngine::Texture::Linear);
    Old3DEngine::CubeMesh snow_mesh;
    snow_mesh.setSize(0.01, 0.01, 0.01);

    snow_mesh.setTexture(&snow_tex);
    //snow_mesh.setSize(0.8, 0.8, 0.8);
    snow_mesh.setPosition(0, 5, 0);
    game.addObjectRef(&snow_mesh);
    Old3DEngine::Particles cube_part((Old3DEngine::Mesh)tomato, 1000, pa_set);
    cube_part.calculateFunc =  [] (Old3DEngine::Particle* p, double delta) {
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
    };

    cube_part.setPosition(0, 3, 0);
    cube_part.pause = false;
    //std::cout << part->getPosition().z << "\n";
    game.addObjectClone(cube_part);


    Old3DEngine::PhysicalBody rigidBody;
    Old3DEngine::PhysEngine* ph_w = game.getPhysEngine();
    ph_w->addObjectRef(&rigidBody);




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
    if (count < 100) {
        ++count;
        del += delta;
    }
    else {
        std::cout << "FPS: " << (float)count / del << std::endl;
        count = 0;
        del = 0.0;
    }
}

float Matrix4ToEuler(glm::mat4 InMatrix4) {
    bool bad = InMatrix4[0][0] == 0 and InMatrix4[0][2] == 0;
    if (not bad)
        return atan2(InMatrix4[0][0], -InMatrix4[0][2]);
    else
        return atan2(InMatrix4[1][0], -InMatrix4[1][2]);
}

void _physics_process(Old3DEngine::Engine* engine, double delta) {
    //std::cout << "EOUU\n";
    //part->update(delta);
    //std::cout << "Process delta: " << (float) delta << "\n";
    physicsWorld->update((float) delta / 1.0f);

    decimal dec;
    Vector3 vec;
    rbody_body->getTransform().getOrientation().getRotationAngleAxis(dec, vec);
    Quaternion qu = rbody_body->getTransform().getOrientation();
    //glm::quat qe(qu.w, qu.z, qu.y, qu.x);
    glm::quat qe(qu.w, -qu.z, -qu.y, -qu.x);

    //glm::quat qe(dec, vec.z, vec.y, vec.x);
    glm::mat4 mat = glm::toMat4(qe);
    mesh1.setRotationMatrix(mat);
    //exit(0);



    glm::vec3 result = glm::degrees(glm::eulerAngles(qe));
    //std::cout << qu.x << " | " << qu.y << " | " << qu.z << " | " << qu.w <<  "\n";

    Vector3 rbodyPosVect = rbody_body->getTransform().getPosition();
    //std::cout << "Rbody Y pos: " << rbodyPosVect.y << "\n";
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


    if (engine->Input.isKeyPressed(GLFW_KEY_F11)) {
        GLFWmonitor *monitor = glfwGetPrimaryMonitor();
        glfwSetWindowMonitor(engine->getWindow(), monitor, 0, 0, 3440, 1440, 144);
    }

    glm::vec2 dir = engine->Input.getVector(GLFW_KEY_A, GLFW_KEY_D, GLFW_KEY_W, GLFW_KEY_S);
    glm::vec3 cam_rot = glm::radians(camera.getRotationEuler());
    /*glm::vec3 direction;
    direction.x = cos(glm::radians(camera.getRotationEuler().y)) * cos(glm::radians(camera.getRotationEuler().x));
    direction.y = sin(glm::radians(camera.getRotationEuler().x));
    direction.z = sin(glm::radians(camera.getRotationEuler().y)) * cos(glm::radians(camera.getRotationEuler().x));
    glm::vec3 cameraFront = glm::normalize(direction);
    camera.setPosition(camera.getPosition() + glm::vec3 {
        dir.y * 0.1 * direction.x / cos(glm::radians(camera.getRotationEuler().x)) + dir.x * 0.1 * direction.z * -1 / cos(glm::radians(
                camera.getRotationEuler().x)),
        direction.y * 0.1 * dir.y,
        dir.y * 0.1 * direction.z + dir.x * 0.1 * direction.x
    });*/
    //camera.offset(cos(cam_rot.y) * dir.y * 0.1 * dir.x, 0, sin(cam_rot.y) * dir.y * dir.x * 0.1);
    //std::cout << dir.x << " | " << dir.y << "\n";
    //std::cout << cos(cam_rot.y) * dir.y * 0.1 << "\n";
    //std::cout << cos(M_PI) << "\n";
    //cam_rot.y = Matrix4ToEuler(camera.getRotationMatrix());
    //std::cout << cos(cam_rot.y) * dir.y << '\n';
    //camera.offset(cos(cam_rot.y), 0, 0);
    //std::cout << cam_rot.y << '\n';
    camera.offset(cos(cam_rot.y) * dir.y * 0.1,
                  0,
                  -sin(cam_rot.y) * dir.y * 0.1);
    //std::cout << camera.getPosition().z << '\n';

    //std::cout << "CAM POS:" << camera.getPosition().z << "\n";

    //std::cout << "Fixed FPS: " << 1.0 / delta <<  delta<< std::endl;
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

        //camera.setRotation(30, -90, 0);
        /*if (abs(camera.getRotationEuler().x) < 90) {
            float rotX = camera.getRotationEuler().x + yoffset;
            //std::cout << rotX << "\n";
            if (rotX > 90)
                yoffset -= rotX - 90;
            if (rotX < -90)
                yoffset -= rotX + 90;
        }
        if (abs(camera.getRotationEuler().x) >= 90) {
            yoffset = 0;
        }*/

        //std::cout << camera.getRotationEuler().y << "\n";
        camera.rotate(0, -xoffset, 0);
        camera.rotate(-yoffset, 0, 0, false);
    }
    float posit[4] = {camera.getPosition().x, camera.getPosition().y,camera.getPosition().z,1.0};
    //glLightfv(GL_LIGHT0, GL_POSITION, posit);
    //sun.setPosition(camera.getPosition());
}
