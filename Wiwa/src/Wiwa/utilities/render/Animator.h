#include <Wiwa/core/Core.h>

#include <Wiwa/utilities/render/Animation.h>







namespace Wiwa {




	class WI_API Animator {
		


	public:
		Animator(Animation* animation);

		void UpdateAnimation(float dt);

		void PlayAnimation(Animation* pAnimation);

		void CalculateBoneTransform(const NodeData* node, glm::mat4 parentTransform);
	private:

		Wiwa::Animation* m_CurrentAnimation;

		std::vector <glm::mat4> m_FinalBoneMatrices;

		float m_CurrentTime = 0;
		float m_DeltaTime = 0;
	};
}