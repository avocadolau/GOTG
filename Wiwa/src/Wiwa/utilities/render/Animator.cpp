#include <wipch.h>

#include "Animator.h"

namespace Wiwa {
	Animator::Animator(Animation* animation) {
		m_CurrentTime = 0;
		
		m_CurrentAnimation = animation;
		m_FinalBoneMatrices.reserve(100);
		
		for (int i = 0; i < 100; i++)
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
	}

	void Animator::UpdateAnimation(float dt)
	{
		m_DeltaTime = dt;
		if (m_CurrentAnimation) 
		{
			m_CurrentTime += m_CurrentAnimation->ticksPerSecond * dt;
			m_CurrentTime = fmodf(m_CurrentTime, m_CurrentAnimation->duration);
			CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
		}
	}

	void Animator::PlayAnimation(Animation* pAnimation)
	{
		m_CurrentAnimation = pAnimation;
		m_CurrentTime = 0;
	}

	void Animator::CalculateBoneTransform(const NodeData* node, glm::mat4 parentTransform)
	{
		std::string nodeName = node->name;
		glm::mat4 nodeTransform = node->transformation;

		//Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

		//if (Bone)
		//{
		//	Bone->
		//}
	}
}


