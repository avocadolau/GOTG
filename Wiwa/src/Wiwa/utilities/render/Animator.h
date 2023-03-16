#include <Wiwa/core/Core.h>

#include <Wiwa/utilities/render/Animation.h>
#include <Wiwa/utilities/render/Bone.h>

namespace Wiwa {

    class WI_API Animator {
    public:
        Animator();

        Animator(Animation* animation);

        ~Animator();

        void UpdateAnimation(float dt);

        void PlayAnimation(Animation* pAnimation);
        void PlayAnimationName(std::string name);
        void PlayAnimationIndex(unsigned int index);

        void CalculateBoneTransform(const NodeData* node, glm::mat4 parentTransform);


        static void SaveWiAnimator(Animator* animator, const char* path);
        static Animator* LoadWiAnimator(const char* path);

        Animation* GetCurrentAnimation() { return m_CurrentAnimation; }
        std::vector<glm::mat4> GetFinalBoneMatrices()
        {
            return m_FinalBoneMatrices;
        }


        std::vector<Animation*> m_Animations;
        std::string m_Name;
        unsigned int m_NumAnimations;
    private:
        std::vector<glm::mat4> m_FinalBoneMatrices;
        Animation* m_CurrentAnimation;
        float m_CurrentTime;
        float m_DeltaTime = 0;
    };
}