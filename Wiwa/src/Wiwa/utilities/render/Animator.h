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

        void Update(float dt);

        void UpdateAnimation(float dt);

        void UpdateBlendingAnimation(float dt);

        void Blend(std::string name, float blendTime);

        void PlayAnimation(Animation* pAnimation);

        void PlayAnimationName(std::string name);

        void PlayAnimation();

        void PauseAnimation();

        void BlendTwoAnimations(Animation* baseAnim, Animation* layerAnim, float blendFactor, float deltaTime);

        void CalculateBlendedBoneTransform(Animation* animationBase, const NodeData* node,
            Animation* animationLayer, const NodeData* nodeLayered,
            const float currentTimeBase, const float currentTimeLayered,
            const glm::mat4& parentTransform,
            const float blendFactor);

        void CalculateBoneTransform(const NodeData* node,const glm::mat4 parentTransform);

        void CalculateBoneTransform(Animation* animation, float time,const NodeData* node,const glm::mat4 parentTransform);

        void CalculateBoneFinalTransform();

        void BlendToAnimation(Animation* targetAnim, float blendDuration, float weight);

        static void SaveWiAnimator(Animator* animator, const char* path);

        static Animator* LoadWiAnimator(const char* path);

        void ResetTime() { m_CurrentTime = 0; }

        void SetCurrentAnimation(Animation* anim) { m_CurrentAnimation = anim; }

        void SetTargetAnimation(Animation* anim) { m_TargetAnimation = anim; }

        void SetAnimationSatate(AnimationState state) { m_AnimationState = state; }

        Animation* GetCurrentAnimation() { return m_CurrentAnimation; }

        Animation* GetTargetAnimation() { return m_TargetAnimation; }

        Animation* GetAnimation(std::string name);

        std::vector<glm::mat4> GetFinalBoneMatrices(){return m_FinalBoneMatrices;}

        std::vector<Animation*> m_Animations;
        std::string m_Name;
        unsigned int m_NumAnimations;
        AnimationState m_AnimationState = AnimationState::Paused;

        float m_BlendWeight;
        float m_BlendDuration;
    private:
        std::vector<glm::mat4> m_FinalBoneMatrices;
        //Animator
        Animation* m_CurrentAnimation = nullptr;
        float m_CurrentTime;
        float m_DeltaTime = 0;
        float m_AnimationTime;
        std::string m_RootNodeName;
        //blending
        Animation* m_TargetAnimation = nullptr;
        float m_BlendTime = 0.0f;
    };
}