#include <HateEngine/Log.hpp>
#include <HateEngine/Objects/Particles.hpp>

#include <cstdint>
#include <mutex>
#include <random>
#include <string>
#include <sys/types.h>
#include "HateEngine/Objects/CubeMesh.hpp"
#include "HateEngine/Objects/Mesh.hpp"
#include <HateEngine/Objects/Camera.hpp>

using namespace HateEngine;

// ===============> Particle <===============
Particle::Particle(uint32_t index, const Mesh& mesh, float lifetime, bool del_on_time) :
    Mesh(mesh) {
    // std::cout << pos.x << " | " << pos.y << " | " << pos.z << "\n";
    this->index = index;
    this->lifetime = lifetime;
    this->lostLifetime = lifetime;
    this->deleteOnEndOfLife = del_on_time;

    std::cout << this->getVertices()->size() << "\n";
}

/*Particle::Particle(const Particle& particle) {
    lifetime = particle.lifetime;
    deleteOnEndOfLife = particle.deleteOnEndOfLife;
    lostLifetime = particle.lostLifetime;
    index = particle.index;
}*/

Particle::Particle() {
}


// ==========> ParticleSettings <==========
Particles::ParticleSettings::ParticleSettings(
        float min_lifetime, float max_liftime, bool del_on_end, glm::vec3 min_offset,
        glm::vec3 max_offset, bool is_billboard
) {
    this->min_lifetime = min_lifetime;
    this->max_lifetime = max_liftime;
    this->delete_on_end_of_life = del_on_end;
    this->min_offset = min_offset;
    this->max_offset = max_offset;
    this->is_billboard = is_billboard;
}

Particles::ParticleSettings::ParticleSettings() {
}


// ===============> Particles <===============
Particles::Particles(
        const Mesh& mesh, uint32_t particles_count, ParticleSettings settings, float spawn_delay
) : rd(), mesh(mesh) {
    this->maxParticles = particles_count;
    this->spawnDelay = spawn_delay;
    this->set = settings;
    this->gen = std::mt19937(rd());
    this->life_dist =
            std::uniform_real_distribution<float>(settings.min_lifetime, settings.max_lifetime);
    this->posX_dist =
            std::uniform_real_distribution<float>(settings.min_offset.x, settings.max_offset.x);
    this->posY_dist =
            std::uniform_real_distribution<float>(settings.min_offset.y, settings.max_offset.y);
    this->posZ_dist =
            std::uniform_real_distribution<float>(settings.min_offset.z, settings.max_offset.z);
    particlesVector.reserve(particles_count);
    // bindObj(&this->mesh);
}

Particles::~Particles() {
    for (Particle& p: particlesVector)
        onParticleDelete(&p);
}

// #include <iostream>
void Particles::Update(double delta) {
    if (this->is_pause)
        return;
    elapsedTime += delta;

    std::lock_guard<std::mutex> lock(this->particlesMutex);

    /*if (elapsedTime < 0.033)
        return;*/

    if (particlesVector.size() < this->maxParticles) {
        // HATE_WARNING_F("Particles update: %f", delta);
        if (this->spawnDelay <= 0) {
            for (uint32_t i = 0; i < this->maxParticles - particlesVector.size(); ++i) {
                Particle particle(i, mesh, life_dist(gen), this->set.delete_on_end_of_life);
                particle.setPosition(
                        glm::vec3{posX_dist(gen), posY_dist(gen), posZ_dist(gen)} -
                        set.max_offset / 2.0f
                );
                particlesVector.push_back(std::move(particle));
                bindObj(&particlesVector[particlesVector.size() - 1]);
            }
        } else {
            while (particlesVector.size() < this->maxParticles and elapsedTime >= this->spawnDelay
            ) {
                Particle particle(
                        particlesVector.size(), mesh, life_dist(gen),
                        this->set.delete_on_end_of_life
                );
                particle.setPosition(
                        glm::vec3{posX_dist(gen), posY_dist(gen), posZ_dist(gen)} -
                        set.max_offset / 2.0f
                );
                particlesVector.push_back(std::move(particle));
                bindObj(&particlesVector[particlesVector.size() - 1]);
                elapsedTime -= this->spawnDelay;
            }
        }
    }

    for (auto it = particlesVector.begin(); it < particlesVector.end(); ++it) {
        Particle* p = &*it;
        // std::cout << p->index << " -- " << p->lostLifetime << "\n";
        calculateFunc(p, delta);
        if (p->lostLifetime >= 0) {
            p->lostLifetime -= (float) delta;
            if (p->isAlive and p->lostLifetime <= 0) {
                if (p->deleteOnEndOfLife) {
                    p->visible = false;
                    unbindObj(p->getUUID());
                } else {
                    p->lostLifetime = p->lifetime;
                    p->position = glm::vec3{posX_dist(gen), posY_dist(gen), posZ_dist(gen)} -
                                  set.max_offset / 2.0f;
                    p->scale = {1, 1, 1};
                    onParticleDelete(p);
                    p->data.clear();
                }
            }
        }
    }

    // elapsedTime = 0;

    if (this->particlesVector.size() == 0)
        return;

    std::vector<float> vertices;
    vertices.reserve(this->particlesVector.size() * this->particlesVector[0].getVertices()->size());
    std::vector<uint32_t> indicies;
    indicies.reserve(this->particlesVector.size() * this->particlesVector[0].getIndicies()->size());
    std::vector<float> normals;
    normals.reserve(this->particlesVector.size() * this->particlesVector[0].getNormals()->size());

    std::vector<float> UV;
    UV.reserve(this->particlesVector.size() * this->particlesVector[0].getUV()->size());

    for (auto& obj: this->particlesVector) {
        // auto v = *obj.getVertices();
        // vertices.insert(vertices.end(), v.begin(), v.end());
        glm::vec3 offset = obj.getGlobalPosition();
        glm::mat4 rot = obj.getGlobalRotationMatrix();

        auto v = obj.getVertices();
        auto vv = (glm::vec3*) v->data();
        auto vv_size = v->size() / 3;
        for (uint32_t i = 0; i < vv_size; ++i) {
            glm::vec4 result = rot * glm::vec4(vv[i], 1.0f);
            // vv[i] = glm::vec3{result.x, result.y, result.z};
            // vv[i] = vv[i] + offset;
            vertices.push_back(vv[i].x + offset.x);
            vertices.push_back(vv[i].y + offset.y);
            vertices.push_back(vv[i].z + offset.z);
        }

        auto i = *obj.getIndicies();
        uint32_t size = indicies.size();
        for (auto& ind: i) {
            indicies.push_back(ind + size);
        }
        // indicies.insert(indicies.end(), i.begin(), i.end());
        auto n = *obj.getNormals();
        normals.insert(normals.end(), n.begin(), n.end());

        auto uv = *obj.getUV();
        UV.insert(UV.end(), uv.begin(), uv.end());


        // auto v = *obj.getVertices();
        // auto vv = (glm::vec3*)v.data();
        // auto vv_size = v.size() / 3;
        // for (uint32_t i = 0; i < vv_size; ++i) {
        //     glm::vec4 result = rot * glm::vec4(vv[i], 1.0f);
        //     //vv[i] = glm::vec3{result.x, result.y, result.z};
        //     vv[i] = vv[i] + offset;
        // }
        // vertices.insert(vertices.end(), v.begin(), v.end());


        // auto i = *obj.getIndicies();
        // for (auto& ind: i) {
        //     ind = ind + indicies.size();
        // }
        // indicies.insert(indicies.end(), i.begin(), i.end());
        // auto n = *obj.getNormals();
        // normals.inert(normals.end(), n.begin(), n.end());
    }
    this->all_particles_in_one.setVertices(vertices);
    this->all_particles_in_one.setIndicies(indicies);
    this->all_particles_in_one.setNormals(normals);
    this->all_particles_in_one.setUV(UV);

    // this->all_particles_in_one.render(renderer);
}

void Particles::play() {
    this->is_pause = false;
}

void Particles::pause() {
    this->is_pause = true;
}

void Particles::reset() {
    std::lock_guard<std::mutex> lock(this->particlesMutex);
    for (Particle& p: particlesVector)
        onParticleDelete(&p);
    particlesVector.clear();
    bindedObjects.clear();
    this->is_pause = true;
    this->elapsedTime = 0;
}

std::vector<Particle>* Particles::getParticles() {
    return &particlesVector;
}

void Particles::_process(class Engine* engine, double delta) {
    Update(delta);
}

void Particles::render(RenderInterface* renderer) {
    std::lock_guard<std::mutex> lock(this->particlesMutex);
    glm::vec3 cam_pos = renderer->getCamera()->getGlobalPosition();
    for (auto& obj: this->particlesVector) {
        if (set.is_billboard) {
            obj.lookAt(cam_pos);
            obj.setCorrectTransparency(true);
        }
        // HATE_INFO_F("%f, %f, %f, %d", obj.getGlobalPosition().x, obj.getGlobalPosition().y,
        // obj.getGlobalPosition().z, obj.getVertices()->size());
        obj.visible = true;
        // obj.render(renderer);
        // renderer->renderMesh(&obj);
    }

    if (this->particlesVector.size() == 0)
        return;

    this->all_particles_in_one.setCorrectTransparency(true);
    this->all_particles_in_one.setTexture(this->mesh.getTexture());
    renderer->renderMesh(&this->all_particles_in_one);

    /*std::vector<float> vertices;
    vertices.reserve(this->particlesVector.size() * this->particlesVector[0].getVertices()->size());
    std::vector<uint32_t> indicies;
    indicies.reserve(this->particlesVector.size() * this->particlesVector[0].getIndicies()->size());
    std::vector<float> normals;
    normals.reserve(this->particlesVector.size() * this->particlesVector[0].getNormals()->size());
    for (auto& obj: this->particlesVector) {
        //auto v = *obj.getVertices();
        //vertices.insert(vertices.end(), v.begin(), v.end());
        glm::vec3 offset = obj.getGlobalPosition();
        glm::mat4 rot = obj.getGlobalRotationMatrix();


        auto v = *obj.getVertices();
        auto vv = (glm::vec3*)v.data();
        auto vv_size = v.size() / 3;
        for (uint32_t i = 0; i < vv_size; ++i) {
            glm::vec4 result = rot * glm::vec4(vv[i], 1.0f);
            //vv[i] = glm::vec3{result.x, result.y, result.z};
            vv[i] = vv[i] + offset;
        }
        vertices.insert(vertices.end(), v.begin(), v.end());


        auto i = *obj.getIndicies();
        for (auto& ind: i) {
            ind = ind + indicies.size();
        }
        indicies.insert(indicies.end(), i.begin(), i.end());
        auto n = *obj.getNormals();
        normals.insert(normals.end(), n.begin(), n.end());
    }
    Mesh _mesh(vertices, indicies, normals);
    renderer->renderMesh(&_mesh);

    CubeMesh math_mesh;

    math_mesh.offset(2, 2, 2);
    math_mesh.rotate(0, 30, 0);
    glm::vec3 offset = math_mesh.getGlobalPosition();
    glm::mat4 rot = math_mesh.getRotationMatrix();

    CubeMesh mesh;
    auto v = *mesh.getVertices();
    auto vv = (glm::vec3*)v.data();
    auto vv_size = v.size() / 3;
    for (uint32_t i = 0; i < vv_size; ++i) {
        glm::vec4 result = rot * glm::vec4(vv[i], 1.0f);
        vv[i] = glm::vec3{result.x, result.y, result.z};
        vv[i] = vv[i] + offset;
    }
    mesh.setVertices(v);
    renderer->renderMesh(&mesh);*/
}
