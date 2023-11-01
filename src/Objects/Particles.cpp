#include "Old3DEngine/Objects/Particles.hpp"

using namespace Old3DEngine;

Particle::Particle(
        const Mesh& mesh, glm::vec3 pos, u_int32_t index,
        float lifetime, bool del_on_time
) : Mesh(mesh) {
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


Particles::Particles(
        const Mesh& mesh, u_int32_t particles_count,
        Particle::ParticleSettings settings
) {
    particlesVector.reserve(particles_count);
    for (u_int32_t i = 0; i < particles_count; ++i) {
        particlesVector.push_back(Particle(mesh, this->position + glm::vec3{0, 2*i, 0}, i));
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
            if (p->lostLifetime <= 0 and p->deleteOnEndOfLife) {
                p->lostLifetime = 0;
                //std::cout << p->index << " -- deleted\n";

                particlesVector.erase(it);
                --it;
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
