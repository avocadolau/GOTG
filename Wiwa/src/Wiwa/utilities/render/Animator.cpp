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

		// Update the time for the current animation
		m_AnimationTime += dt;
		if (m_AnimationTime > m_CurrentAnimation->GetDuration()) {
			m_AnimationTime = fmod(m_AnimationTime, m_CurrentAnimation->GetDuration());
		}

		// Update the blend time if blending
		if (m_AnimationState == AnimationState::Blending) {
			m_BlendTime += dt;
			if (m_BlendTime >= m_BlendDuration) {
				// If the blend is complete, switch to the target animation
				m_CurrentAnimation = m_TargetAnimation;
				m_AnimationTime = 0.0f;
				m_BlendTime = 0.0f;
				m_AnimationState = AnimationState::Playing;
			}
			else {
				// Otherwise, blend between the source and target animations
				float blendFactor = m_BlendTime / m_BlendDuration;
				for (unsigned int i = 0; i < m_FinalBoneMatrices.size(); i++) {
					glm::mat4 sourceTransform = m_CurrentAnimation->GetBoneTransform(i);
					glm::mat4 targetTransform = m_TargetAnimation->GetBoneTransform(i);
					m_FinalBoneMatrices[i] = glm::mix(sourceTransform, targetTransform, blendFactor) * m_BlendWeight
						+ sourceTransform * (1.0f - m_BlendWeight);
				}
			}
		}
		else {
			// If not blending, update the bone transforms for the current animation
			for (unsigned int i = 0; i < m_FinalBoneMatrices.size(); i++) {
				m_FinalBoneMatrices[i] = m_CurrentAnimation->GetBoneTransform(i);
			}
		}
	}

	void Animator::PlayAnimation(Animation* pAnimation)
	{
		m_CurrentAnimation = pAnimation;	
		m_CurrentTime = 0;
	}

	Animator::~Animator()
	{
		m_Animations.clear();
		delete m_CurrentAnimation;
	}

	void Animator::SaveWiAnimator(Animator* animator, const char* path)
	{
		std::filesystem::path filePath = path;
		std::string name = filePath.filename().string();
		filePath.replace_filename(name);
		filePath.replace_extension();
		filePath += ".wianimator";
		JSONDocument doc;
		doc.AddMember("fileFormatVersion", 1);
		doc.AddMember("file", path);
		doc.AddMember("folderAsset", false);
		JSONValue animations = doc.AddMemberArray("animations");
		
		for (auto& anim : animator->m_Animations)
		{
			animations.PushBack(anim->m_SavePath.c_str());
		}

		doc.save_file(filePath.string().c_str());
	}

	Animator* Animator::LoadWiAnimator(const char* path)
	{
		std::filesystem::path filePath = path;
		if (!std::filesystem::exists(filePath))
			return nullptr;
		Animator* animator = new Animator();
		JSONDocument doc(filePath.string().c_str());
		if (doc.HasMember("animations"))
		{
			JSONValue animations = doc["animations"];

			for (size_t i = 0; i < animations.Size(); i++)
			{
				std::string path = animations[i].as_string();
				if(!FileSystem::Exists(path.c_str()))continue;
				animator->m_Animations.push_back( Animation::LoadWiAnimation(path.c_str()));	
			}
		}
		return animator;
	}

	void Animator::PlayAnimationName(std::string name)
	{
		for (auto& animation : m_Animations)
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
		if (index > m_Animations.size()) return;
		m_CurrentAnimation = m_Animations[index];
	}

	void Animator::BlendToAnimation(Animation* targetAnim, float blendDuration, float weight)
	{
		// Store the current animation as the source animation
		Animation& sourceAnim = *m_CurrentAnimation;

		// Set the target animation and blend duration
		m_TargetAnimation = targetAnim;
		m_BlendDuration = blendDuration;
		m_BlendTime = 0.0f;
		m_BlendWeight = weight;

		// Set the source animation as the starting pose for blending
		std::map<std::string, BoneInfo> t = sourceAnim.GetBoneIDMap();
		std::map<std::string, BoneInfo>::iterator it;
		int counter = 0;
		for (it = t.begin(); it != t.end(); it++)
		{
			m_FinalBoneMatrices[counter] = it->second.finalTransformation;
			counter++;
		}
		// Set the animation state to blending
		m_AnimationState = AnimationState::Blending;
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


