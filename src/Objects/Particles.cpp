#include "Old3DEngine/Objects/Particles.hpp"
#include <random>



using namespace Old3DEngine;

Particle::Particle(
        uint32_t index, const Mesh& mesh, glm::vec3 pos,
        float lifetime, bool del_on_time
) : Mesh(mesh) {
    //std::cout << pos.x << " | " << pos.y << " | " << pos.z << "\n";
    this->position = pos;
    this->index = index;
    this->lifetime = lifetime;
    this->lostLifetime = lifetime;
    this->deleteOnEndOfLife = del_on_time;
}

Particle::Particle(const Particle &particle) : Mesh(particle) {
    lifetime = particle.lifetime;
    deleteOnEndOfLife = particle.deleteOnEndOfLife;
    lostLifetime = particle.lostLifetime;
    index = particle.index;
}


// Pretty this function
Particles::Particles(
        const Mesh& mesh, uint32_t particles_count,
        Particle::ParticleSettings settings
) {
    std::random_device rand_dev;
    std::mt19937 gen(rand_dev());
    std::uniform_real_distribution<float> life_dist(settings.min_lifetime, settings.max_lifetime);
    std::uniform_real_distribution<float> posX_dist(settings.min_offset.x, settings.max_offset.x);
    std::uniform_real_distribution<float> posY_dist(settings.min_offset.y, settings.max_offset.y);
    std::uniform_real_distribution<float> posZ_dist(settings.min_offset.z, settings.max_offset.z);
    this->set = settings;

    particlesVector.reserve(particles_count);
    for (uint32_t i = 0; i < particles_count; ++i) {
        particlesVector.push_back(
            Particle(
                i, mesh,
                this->position + glm::vec3{posX_dist(gen), posY_dist(gen), posZ_dist(gen)},
                life_dist(gen),
                settings.delete_on_end_of_life
            )
        );
    }
}

//#include <iostream>
void Particles::update(double delta) {
    for (auto it = particlesVector.begin(); it < particlesVector.end(); ++it) {
        Particle* p = &*it;
        //std::cout << p->index << " -- " << p->lostLifetime << "\n";
        calculateFunc(p, delta);
        if (p->lostLifetime >= 0) {
            p->lostLifetime -= (float)delta;
            if (p->lostLifetime <= 0 and p->deleteOnEndOfLife and p->visible == true) {
                p->lostLifetime = 0;
                #ifdef _OLD3D_DEBUG
                std::cout << p->index << " -- deleted\n";
                #endif

                //p->visible = false;

                //particlesVector.erase(it);

                std::random_device rand_dev;
                std::mt19937 gen(rand_dev());
                std::uniform_real_distribution<float> life_dist(set.min_lifetime, set.max_lifetime);
                std::uniform_real_distribution<float> posX_dist(set.min_offset.x, set.max_offset.x);
                std::uniform_real_distribution<float> posY_dist(set.min_offset.y, set.max_offset.y);
                std::uniform_real_distribution<float> posZ_dist(set.min_offset.z, set.max_offset.z);

                p->position = this->position + glm::vec3{posX_dist(gen), posY_dist(gen), posZ_dist(gen)};
                //std::cout << "POS: " << p->position.x << " | " << p->position.y << " | " << p->position.z << "\n";
                p->lostLifetime = p->lifetime;
                //--it;
            }
        }
    }
}

void Particles::setPosition(float x, float y, float z) {
    setPosition({x, y, z});
}

void Particles::setPosition(glm::vec3 vec) {
    Object::setPosition(vec);
    for (Particle &p : particlesVector) {
        p.offset(vec);
    }
}

Particle::ParticleSettings::ParticleSettings(float min_lifetime, float max_liftime, bool del_on_end,
                                             glm::vec3 min_offset, glm::vec3 max_offset) {
    this->min_lifetime = min_lifetime;
    this->max_lifetime = max_liftime;
    this->delete_on_end_of_life = del_on_end;
    this->min_offset = min_offset;
    this->max_offset = max_offset;
}

Particle::ParticleSettings::ParticleSettings() {

}

