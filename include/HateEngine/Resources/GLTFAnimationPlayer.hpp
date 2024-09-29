#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include "../Objects/Mesh.hpp"
#include "../Objects/Model.hpp"
#include "../Objects/Object.hpp"
#include "HateEngine/Resources/GLTFModel.hpp"
#include "Texture.hpp"

namespace HateEngine {
    struct GLTFAnimation {
        uint32_t frameCount = 0;
        std::vector<std::vector<Mesh*>> frames = {};
    };

    class GLTFAnimationPlayer : public Object {
        GLTFModel* m_model = nullptr;
        std::unordered_map<std::string, GLTFAnimation> animations;
        GLTFAnimation* currentAnimation = nullptr;
        uint32_t currentFrame = 0;
        float frameTime = 1.0f / 30.0f;
        float elapsedTime = 0.0f;
        bool loop = false;
        bool playing = false;
        std::string currentAnimationName = "";

    public:
        GLTFAnimationPlayer(GLTFModel* model);

        void Update(float deltaTime);

        std::vector<Mesh*>* getMeshes();

        bool play(std::string animationName);
        void stop();
        void setFPS(float fps);
        void setLoop(bool loop);
        void setFrame(uint32_t frame);

        bool isPlaying();
        std::string getCurrentAnimation();
        bool isLooping();
        float getFPS();
        uint32_t getCurrentFrame();
        uint32_t getFrameCount();
    };
} // namespace HateEngine
