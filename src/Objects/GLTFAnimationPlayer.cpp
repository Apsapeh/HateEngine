#include <HateEngine/Objects/GLTFAnimationPlayer.hpp>
#include "HateEngine/Log.hpp"
#include "HateEngine/Resources/GLTFModel.hpp"
#include <cstdint>

using namespace HateEngine;

GLTFAnimationPlayer::GLTFAnimationPlayer(GLTFModel* model) {
    this->m_model = model;
    this->bindObj(this->m_model);
    for (auto& anim: this->m_model->meshes) {
        std::string name = anim->getName();
        unsigned short pos = (unsigned short) name.find_first_of(']');
        std::string animName = name.substr(1, pos - 1);
        int frame = std::stoi(name.substr(pos + 2, name.find_first_of(']', pos + 1) - pos - 2)) - 1;

        if (animations.count(animName) == 0)
            animations[animName] = GLTFAnimation{};

        if (animations[animName].frames.size() <= frame)
            animations[animName].frames.resize(frame + 1);

        animations[animName].frames[frame].push_back(anim);

        animations[animName].frameCount =
                std::max(animations[animName].frameCount, (uint32_t) (frame + 1));
    }
}

void GLTFAnimationPlayer::Update(float delta) {
    if (playing) {
        this->elapsedTime += delta;
        while (elapsedTime >= frameTime) {
            elapsedTime -= frameTime;
            currentFrame++;
            if (this->currentFrame >= this->currentAnimation->frameCount) {
                if (loop) {
                    this->currentFrame = 0;
                } else {
                    playing = false;
                    return;
                }
            }
        }
    }
}

std::vector<Mesh*>* GLTFAnimationPlayer::getMeshes() {
    return &currentAnimation->frames[currentFrame];
}


bool GLTFAnimationPlayer::play(std::string name) {
    if (animations.count(name) == 1) {
        this->currentAnimation = &animations[name];
        currentAnimationName = name;
        playing = true;
        return true;
    }

    HATE_ERROR_F("Animation not found: %s", name.c_str());
    return false;
}

void GLTFAnimationPlayer::stop() {
    playing = false;
    this->currentAnimationName = "";
    this->currentAnimation = nullptr;
}

void GLTFAnimationPlayer::setFPS(float fps) {
    this->frameTime = 1.0f / fps;
}

void GLTFAnimationPlayer::setLoop(bool loop) {
    this->loop = loop;
}


bool GLTFAnimationPlayer::isPlaying() {
    return playing;
}

std::string GLTFAnimationPlayer::getCurrentAnimation() {
    return currentAnimationName;
}

bool GLTFAnimationPlayer::isLooping() {
    return loop;
}

float GLTFAnimationPlayer::getFPS() {
    return 1.0f / frameTime;
}
