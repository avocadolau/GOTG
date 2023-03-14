#include "wipch.h"
#include "Animation.h"
#include <assimp/anim.h>

namespace Wiwa {
	Animation::Animation()
	{
		m_Duration = 0;
		m_TicksPerSecond = 0;
		m_Name = "new animation";
	}
	Animation::Animation(const aiAnimation* animation, Model* model)
	{
		m_Duration = animation->mDuration;
		m_TicksPerSecond = animation->mTicksPerSecond;
		m_NumChannels = animation->mNumChannels;
		m_Name = animation->mName.C_Str();
		ReadHeirarchyData(m_RootNode, model->getModelHierarchy());
		ReadMissingBones(animation, *model);
	}

	Animation::Animation(const char* filePath, Model* model)
	{
		m_Duration = 0;
		m_TicksPerSecond = 0;
		m_Name = "new animation";
	}

	Animation::~Animation()
	{
	}

	Bone* Animation::FindBone(const std::string& name)
	{
		auto iter = std::find_if(m_Bones.begin(), m_Bones.end(),
			[&](const Bone& Bone)
			{
				return Bone.GetBoneName() == name;
			}
		);
		if (iter == m_Bones.end()) return nullptr;
		else return &(*iter);
	}
	void Animation::ReadMissingBones(const aiAnimation* animation, Model& model)
	{
		int size = animation->mNumChannels;

		auto& boneInfoMap = model.GetBoneInfoMap();//getting m_BoneInfoMap from Model class
		int& boneCount = model.GetBoneCount(); //getting the m_BoneCounter from Model class

		//reading channels(bones engaged in an animation and their keyframes)
		for (int i = 0; i < size; i++)
		{
			auto channel = animation->mChannels[i];
			std::string boneName = channel->mNodeName.data;

			if (boneInfoMap.find(boneName) == boneInfoMap.end())
			{
				boneInfoMap[boneName].id = boneCount;
				boneCount++;
			}
			m_Bones.push_back(Bone(channel->mNodeName.data, boneInfoMap[channel->mNodeName.data].id, channel));
		}

		m_BoneInfoMap = boneInfoMap;
	}

	void Animation::ReadHeirarchyData(NodeData& dest, const ModelHierarchy* root)
	{
		assert(root);

		dest.name = root->name.data();
		dest.transformation = root->Transformation;
		dest.childrenCount = root->children.size();

		for (int i = 0; i < root->children.size(); i++)
		{
			NodeData newData;
			ReadHeirarchyData(newData, root->children[i]);
			dest.children.push_back(newData);
		}
	}
	//Animation::Animation()
	//{
	//	name = "new animation";
	//}
	//AnimNode::AnimNode()
	//{
	//}
	//
	//Animation::Animation(aiAnimation* animation, Model* model)
	//{
	//	Animation* anim = new Animation();
	//	m_Duration = animation->mDuration;
	//	m_TicksPerSecond = animation->mTicksPerSecond;
	//	ticksPerSecond = animation->mTicksPerSecond;
	//	numChannels = animation->mNumChannels;

	//	for (unsigned int i = 0; i < animation->mNumChannels; i++)
	//	{
	//		anim->channels.push_back(LoadAnimationNode(animation->mChannels[i]));
	//		
	//	}

	//	animations.push_back(anim);
	//}

	void Animation::LoadAnimation(const aiAnimation* animation)
	{
		m_Duration = animation->mDuration;
		m_TicksPerSecond = animation->mTicksPerSecond;
		m_TicksPerSecond = animation->mTicksPerSecond;
		m_NumChannels = animation->mNumChannels;

		for (unsigned int i = 0; i < animation->mNumChannels; i++)
		{
			Bone bone;
			//m_Bones.push_back(bone.LoadAnimationNode(animation->mChannels[i]));
		}
	}
	void Animation::SaveWiAnimation(File& file, Animation* animation)
	{
		size_t name_len = animation->m_Name.size();

		file.Write(&name_len, sizeof(size_t));
		file.Write(animation->m_Name.c_str(), name_len);

		file.Write(&animation->m_Duration, sizeof(double));
		file.Write(&animation->m_TicksPerSecond, sizeof(double));
		file.Write(&animation->m_NumChannels, sizeof(unsigned int));

		size_t channels_size = animation->m_Bones.size();
		file.Write(&channels_size, sizeof(size_t));


		for (unsigned int i = 0; i < channels_size; i++)
		{
			animation->m_Bones[i].SaveWiAnimNode(file, animation->m_Bones[i]);
		}
	}
	
	Animation* Animation::LoadWiAnimation(File file)
	{
		Animation* anim = new Animation();

		size_t name_len;
		file.Read(&name_len, sizeof(size_t));
		anim->m_Name.resize(name_len);
		file.Read(&anim->m_Name[0], name_len);

		file.Read(&anim->m_Duration, sizeof(double));
		file.Read(&anim->m_TicksPerSecond, sizeof(double));
		file.Read(&anim->m_NumChannels, sizeof(unsigned int));

		size_t channels_size;
		file.Read(&channels_size, sizeof(size_t));

		for (unsigned int i = 0; i < channels_size; i++)
		{
		//	anim->m_Bones.push_back(LoadWiAnimNode(file));
		}


		return anim;
	}

	
}