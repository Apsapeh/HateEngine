#pragma once
#include <cstdint>
#include <iostream>
#include <unordered_map>
#include <vector>
#include "Mesh.hpp"
#include "Object.hpp"
#include <random>

namespace HateEngine {
    class Particles;

    class Particle : public Mesh {
        friend class Particles;

        float prev_spawn_time = 0;
        bool isAlive = true;

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

        Particle(uint32_t index, const Mesh& mesh, float lifetime = 1.0, bool del_on_time = true);
        Particle(const Particle& particle);
        Particle();
    };


    class Particles : public Object {
        friend Particle;

        std::random_device rd;
        std::mt19937 gen;
        std::uniform_real_distribution<float> life_dist;
        std::uniform_real_distribution<float> posX_dist;
        std::uniform_real_distribution<float> posY_dist;
        std::uniform_real_distribution<float> posZ_dist;

        float elapsedTime = 0;
        const Mesh mesh;
        std::mutex particlesMutex;
        std::vector<Particle> particlesVector;

        Particle::ParticleSettings set;
        bool is_pause = true;
        uint32_t maxParticles = 0;
        float spawnDelay = 0;

    public:
        void (*calculateFunc)(Particle*, double) = [](Particle* p, double d) {
            p->offset(0, -9.8f * (float) d, 0);
        };
        void (*onParticleDelete)(Particle*) = [](Particle* p) {};

    public:
        Particles(
                const Mesh& mesh, uint32_t particles_count, Particle::ParticleSettings settings,
                float spawn_delay = 0.1
        );
        ~Particles();

        void Update(double delta);
        void play();
        void pause();
        void reset();

        std::vector<Particle>* getParticles();
    };
} // namespace HateEngine
