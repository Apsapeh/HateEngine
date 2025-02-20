#pragma once
#include <cstdint>
#include <string>
#include <vector>
#include <unordered_map>
#include "Object.hpp"
#include "../Resources/GLTFModel.hpp"
#include "LODMesh.hpp"
#include "../Render/RenderInterface.hpp"
#include "Interfaces/Renderable3DInterface.hpp"

namespace HateEngine {
    struct GLTFAnimation {
        uint32_t frameCount = 0;
        std::vector<std::vector<LODMesh*>> frames = {};
    };

    class GLTFAnimationPlayer : public Object, public Renderable3DInterface {
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

        std::vector<LODMesh*>* getMeshes();

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

        void render(RenderInterface* renderer) override;
    };
} // namespace HateEngine
