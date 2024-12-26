#include <HateEngine/Log.hpp>
#include <HateEngine/Objects/Particles.hpp>

#include <mutex>
#include <random>
#include <string>

using namespace HateEngine;

// ==========> ParticleSettings <==========
Particle::ParticleSettings::ParticleSettings(
        float min_lifetime, float max_liftime, bool del_on_end, glm::vec3 min_offset,
        glm::vec3 max_offset
) {
    this->min_lifetime = min_lifetime;
    this->max_lifetime = max_liftime;
    this->delete_on_end_of_life = del_on_end;
    this->min_offset = min_offset;
    this->max_offset = max_offset;
}

Particle::ParticleSettings::ParticleSettings() {
}


// ===============> Particle <===============
Particle::Particle(uint32_t index, const Mesh& mesh, float lifetime, bool del_on_time) :
    Mesh(mesh) {
    // std::cout << pos.x << " | " << pos.y << " | " << pos.z << "\n";
    this->index = index;
    this->lifetime = lifetime;
    this->lostLifetime = lifetime;
    this->deleteOnEndOfLife = del_on_time;
}

Particle::Particle(const Particle& particle) : Mesh(particle) {
    lifetime = particle.lifetime;
    deleteOnEndOfLife = particle.deleteOnEndOfLife;
    lostLifetime = particle.lostLifetime;
    index = particle.index;
}

Particle::Particle() {
}


// ===============> Particles <===============
Particles::Particles(
        const Mesh& mesh, uint32_t particles_count, Particle::ParticleSettings settings,
        float spawn_delay
) :
    rd(),
    mesh(mesh) {
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
}

Particles::~Particles() {
    for (Particle& p: particlesVector)
        onParticleDelete(&p);
}

// #include <iostream>
void Particles::Update(double delta) {
    if (this->is_pause)
        return;

    std::lock_guard<std::mutex> lock(this->particlesMutex);

    elapsedTime += delta;
    if (particlesVector.size() < this->maxParticles) {
        if (this->spawnDelay <= 0) {
            for (uint32_t i = 0; i < this->maxParticles - particlesVector.size(); ++i) {
                Particle particle(i, mesh, life_dist(gen), this->set.delete_on_end_of_life);
                particle.setPosition({posX_dist(gen), posY_dist(gen), posZ_dist(gen)});
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
                particle.setPosition({posX_dist(gen), posY_dist(gen), posZ_dist(gen)});
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
                    p->position = glm::vec3{posX_dist(gen), posY_dist(gen), posZ_dist(gen)};
                    p->scale = {1, 1, 1};
                }
            }
        }
    }
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
