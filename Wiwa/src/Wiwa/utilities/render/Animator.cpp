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


