#include <Wiwa/core/Core.h>

#include <Wiwa/utilities/render/Animation.h>
#include <Wiwa/utilities/render/Bone.h>

namespace Wiwa {

    class WI_API Animator {
        Animator::Animator(Animation* animation);

        void Animator::UpdateAnimation(float dt);

        void Animator::PlayAnimation(Animation* pAnimation);

        void Animator::CalculateBoneTransform(const NodeData* node, glm::mat4 parentTransform);

        std::vector<glm::mat4> GetFinalBoneMatrices()
        {
            return m_FinalBoneMatrices;
        }

    private:
        std::vector<glm::mat4> m_FinalBoneMatrices;
        Animation* m_CurrentAnimation;
        float m_CurrentTime;
        float m_DeltaTime;
    };
}