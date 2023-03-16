#include <wipch.h>

#include "Animator.h"
#include <Wiwa/utilities/json/JSONDocument.h>
#include <Wiwa/utilities/json/JSONValue.h>
namespace Wiwa {
	Animator::Animator()
	{
		m_Name = "New Animator";
		m_CurrentTime = 0.0;
		m_CurrentAnimation = nullptr;

		m_FinalBoneMatrices.reserve(100);

		for (int i = 0; i < 100; i++)
			m_FinalBoneMatrices.push_back(glm::mat4(1.0f));
	}
	Animator::Animator(Animation* animation) {
		m_Name = "New Animator";
		m_CurrentTime = 0.0;
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
			float TicksPerSecond = (float)(m_CurrentAnimation->m_TicksPerSecond != 0 ? m_CurrentAnimation->m_TicksPerSecond : 25.0f);
			float TimeInTicks = dt * TicksPerSecond;
			float AnimationTimeTicks = fmod(TimeInTicks, (float)m_CurrentAnimation->m_Duration);
			m_CurrentTime = AnimationTimeTicks;
			CalculateBoneTransform(&m_CurrentAnimation->GetRootNode(), glm::mat4(1.0f));
		}
	}

	void Animator::PlayAnimation(Animation* pAnimation)
	{
		m_CurrentAnimation = pAnimation;	
		m_CurrentTime = 0;
	}

	void Animator::SaveWiAnimator(Animator* animator, const char* path)
	{
		//size_t name_len = animator->m_Name.size();

		//file.Write(&name_len, sizeof(size_t));
		//file.Write(animator->m_Name.c_str(), name_len);

		//size_t animations_size = animator->animations.size();
		//file.Write(&animations_size, sizeof(size_t));
		// 
		// 
		//std::filesystem::path filePath = path;
		//std::string name = filePath.filename().string();
		//name += "Animator";
		//filePath.replace_filename(name);
		//filePath.replace_extension("animator");
		//JSONDocument doc(filePath.string().c_str());
		//JSONValue animations = doc.AddMemberObject("Animations");



		//for (auto& animation : animator->animations)
		//{
		//	animation->SaveWiAnimation(animation,path);
		//}
	}

	Animator* Animator::LoadWiAnimator(File& file)
	{
		Animator* animator = new Animator();
		/*size_t name_size;
		file.Read(&name_size, sizeof(size_t));
		animator->m_Name.resize(name_size);
		file.Read(&animator->m_Name[0], name_size);
		
		size_t animations_size;
		file.Read(&animations_size, sizeof(size_t));*/

		//for (int i = 0; i < animations_size; i++)
		//{
		//	animator->animations.push_back(Animation::LoadWiAnimation(file));
		//}

		return animator;
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
			m_FinalBoneMatrices[index] = globalTransformation * boneInfoMap[nodeName].offsetMatrix;
		}

		for (int i = 0; i < node->childrenCount; i++)
			CalculateBoneTransform(&node->children[i], globalTransformation);
	}
}


