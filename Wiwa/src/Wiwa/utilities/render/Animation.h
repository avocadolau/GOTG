#pragma once

#include <Wiwa/core/Core.h>
#include <glm/glm.hpp>
#include <chrono>
#include <vector>
#include <map>
#include <stdint.h>
#include <Wiwa/core/Resources.h>

#include <stdint.h>

#define MAX_FRAMES 60

namespace Wiwa {

    class WI_API AnimationParticles
    {
    public:
        float speed = 1.0f;
        std::vector<glm::vec4> frames;
        bool loop = true;
        // Allows the animation to keep going back and forth
        bool pingpong = false;
        bool mustFlip = false;

    private:
        float currentFrame = 0.0f;
        int totalFrames = 0;
        int loopCount = 0;
        int pingpongDirection = 1;
        std::chrono::time_point<std::chrono::high_resolution_clock> lastUpdate;

    public:

        void PushBack(const glm::vec4& rect)
        {
            frames.push_back(rect);
            totalFrames++;
        }

        void Reset()
        {
            currentFrame = 0;
            loopCount = 0;
            lastUpdate = std::chrono::high_resolution_clock::now();
        }

        bool HasFinished()
        {
            return !loop && !pingpong && loopCount > 0;
        }

        void Update()
        {
            std::chrono::time_point<std::chrono::high_resolution_clock> now = std::chrono::high_resolution_clock::now();
            float elapsedSeconds = std::chrono::duration<float>(now - lastUpdate).count();
            lastUpdate = now;

            currentFrame += speed * elapsedSeconds;
            if (currentFrame >= totalFrames)
            {
                currentFrame = (loop || pingpong) ? 0.0f : totalFrames - 1;
                ++loopCount;

                if (pingpong)
                    pingpongDirection = -pingpongDirection;
            }
        }


        glm::vec4& GetCurrentFrame()
        {
            int actualFrame = static_cast<int>(currentFrame);
            if (pingpongDirection == -1)
                actualFrame = totalFrames - static_cast<int>(currentFrame) - 1;

            return frames[actualFrame];
        }
    };

}
