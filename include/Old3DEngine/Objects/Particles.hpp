#pragma once
#include "Object.hpp"
#include "Mesh.hpp"
#include <vector>
#include <cstdint>

namespace Old3DEngine {
    class Particles;


    class Particle : public Mesh {
        friend class Particles;

        float lifetime = 0;
        bool deleteOnEndOfLife = true;
        float lostLifetime = 0;
        u_int32_t index;

    public:
        struct ParticleSettings {
            float min_lifetime = 0.0;
            float max_lifetime = 1.0;
            float delete_on_end_of_life = true;
        };

        Particle(
                const Mesh& mesh, glm::vec3 pos, u_int32_t index,
                float lifetime = 1.0, bool del_on_time = true
        );
        Particle(const Particle& particle);
        Particle& operator=(Particle &&other) {
            this->~Particle();
            return *new(this) Particle(other);
        }
    };


    class Particles : public Object {
        friend Particle;
    public:
        std::vector<Particle> particlesVector;
        void (*calculateFunc)(Particle*, double) = [] (Particle* p, double d) {
            p->offset(0, -9.8f * (float)d, 0);
        };

    public:
        Particles(
                const Mesh& mesh, u_int32_t particles_count,
                Particle::ParticleSettings settings
        );

        void update(double delta);

        void setPosition(float x, float y, float z);
        void setPosition(glm::vec3);
    };
}