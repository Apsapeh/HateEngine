#pragma once
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "Mesh.hpp"
#include "Object.hpp"

namespace HateEngine {
    class Particles;

    class Particle : public Mesh {
        friend class Particles;

    public:
        float lifetime = 0;
        bool deleteOnEndOfLife = true;
        float lostLifetime = 0;
        uint32_t index;

    public:
        struct ParticleSettings {
            ParticleSettings(
                    float min_lifetime, float max_liftime, bool del_on_end, glm::vec3 min_offset,
                    glm::vec3 max_offset
            );

            ParticleSettings();

            float min_lifetime = 0.0;
            float max_lifetime = 1.0;
            bool delete_on_end_of_life = true;
            glm::vec3 min_offset = {0, 0, 0};
            glm::vec3 max_offset = {0, 0, 0};
        };

        std::unordered_map<std::string, void*> data;

        Particle(
                uint32_t index, const Mesh& mesh, glm::vec3 pos, float lifetime = 1.0,
                bool del_on_time = true
        );
        Particle(const Particle& particle);
        Particle& operator=(Particle&& other) {
            this->~Particle();
            return *new (this) Particle(other);
        }
    };

    class Particles : public Object {
        friend Particle;

    public:
        std::vector<Particle> particlesVector;
        Particle::ParticleSettings set;
        bool pause = true;
        void (*calculateFunc)(Particle*, double) = [](Particle* p, double d) {
            // p->offset(0, -9.8f * (float)d, 0);
            // std::cout <<
        };

    public:
        Particles(const Mesh& mesh, uint32_t particles_count, Particle::ParticleSettings settings);

        void update(double delta);

        void setPosition(float x, float y, float z);
        void setPosition(glm::vec3);
    };
} // namespace HateEngine
