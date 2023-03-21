#include <Wiwa/core/Core.h>

#include <Wiwa/utilities/render/Animation.h>
#include <Wiwa/utilities/render/Bone.h>

namespace Wiwa {
    enum class AnimationState {
        Playing,
        Paused,
        Blending
    };
    class WI_API Animator {
    public:
        Animator();

        Animator(Animation* animation);

        ~Animator();

        void UpdateAnimation(float dt);

        void PlayAnimation(Animation* pAnimation);

        void PlayAnimationName(std::string name);

        void PlayAnimationIndex(unsigned int index);

        void CalculateBoneTransform(const NodeData* node,const glm::mat4 parentTransform);

        void CalculateBoneFinalTransform();

        void BlendToAnimation(Animation* targetAnim, float blendDuration, float weight);

        static void SaveWiAnimator(Animator* animator, const char* path);

        static Animator* LoadWiAnimator(const char* path);

        void ResetTime() { m_CurrentTime = 0; }

        void SetCurrentAnimation(Animation* anim) { m_CurrentAnimation = anim; }

        Animation* GetCurrentAnimation() { return m_CurrentAnimation; }

        std::vector<glm::mat4> GetFinalBoneMatrices(){return m_FinalBoneMatrices;}

        std::vector<Animation*> m_Animations;
        std::string m_Name;
        unsigned int m_NumAnimations;
        AnimationState m_AnimationState = AnimationState::Paused;
    private:
        std::vector<glm::mat4> m_FinalBoneMatrices;
        //Animator
        Animation* m_CurrentAnimation;
        float m_CurrentTime;
        float m_DeltaTime = 0;
        float m_AnimationTime;
        std::string m_RootNodeName;
        //blending
        Animation* m_TargetAnimation ;
        float m_BlendDuration;
        float m_BlendTime = 0.0f;
        float m_BlendWeight;
    };
}