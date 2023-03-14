#include <wipch.h>

#include "Animator.h"

namespace Wiwa {
	Animator::Animator()
	{
		m_CurrentTime = 0.0;
		m_CurrentAnimation = nullptr;

		m_FinalBoneMatrices.reserve(100);

		for (int i = 0; i < 100; i++)
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
	}
	Animator::Animator(Animation* animation) {
		m_CurrentTime = 0.0;
		m_CurrentAnimation = animation;

		m_FinalBoneMatrices.reserve(100);

		for (int i = 0; i < 100; i++)
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
	}

	void Animator::UpdateAnimation(float dt)
	{
		if (m_CurrentAnimation)
		{
			m_DeltaTime = dt;
			m_CurrentTime += m_CurrentAnimation->GetTicksPerSecond() * dt;
			m_CurrentTime = fmod(m_CurrentTime, m_CurrentAnimation->GetDuration());
			CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
		}
	}

	void Animator::PlayAnimation(Animation* pAnimation)
	{
		m_CurrentAnimation = pAnimation;	
		m_CurrentTime = 0;
	}

	void Animator::SaveWiAnimator(File& file,Animator* animator)
	{
		size_t name_len = animator->m_Name.size();

		file.Write(&name_len, sizeof(size_t));
		file.Write(animator->m_Name.c_str(), name_len);

		file.Write(&animator->m_NumAnimations, sizeof(unsigned int));

		for (auto& animation : animator->animations)
		{
			animation->SaveWiAnimation(file, animation);
		}
	}

	Animator* Animator::LoadWiAnimator(File& file)
	{
		Animator* anim = new Animator();

		return anim;
	}

	void Animator::PlayAnimationName(std::string name)
	{
		for (auto& animation : animations)
		{
			if (animation->m_Name == name)
			{
				m_CurrentAnimation = animation;
				m_CurrentTime = 0;
				return;
			}
		}
	}

	void Animator::PlayAnimationIndex(unsigned int index)
	{
		if (index > animations.size()) return;
		m_CurrentAnimation = animations[index];
	}


	void Animator::CalculateBoneTransform(const NodeData* node, glm::mat4 parentTransform)
	{
		std::string nodeName = node->name;
		glm::mat4 nodeTransform = node->transformation;

		Bone* Bone = m_CurrentAnimation->FindBone(nodeName);

		if (Bone)
		{
			Bone->Update(m_CurrentTime);
			nodeTransform = Bone->GetLocalTransform();
		}

		glm::mat4 globalTransformation = parentTransform * nodeTransform;

		auto boneInfoMap = m_CurrentAnimation->GetBoneIDMap();
		if (boneInfoMap.find(nodeName) != boneInfoMap.end())
		{
			int index = boneInfoMap[nodeName].id;
			glm::mat4 offset = boneInfoMap[nodeName].offsetMatrix;
			m_FinalBoneMatrices[index] = globalTransformation * offset;
		}

		for (int i = 0; i < node->childrenCount; i++)
			CalculateBoneTransform(&node->children[i], globalTransformation);
	}
}


