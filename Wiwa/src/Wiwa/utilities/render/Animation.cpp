#include "wipch.h"
#include "Animation.h"
#include <assimp/anim.h>

namespace Wiwa {
	Animation::Animation()
	{
		name = "new animation";
	}
	AnimNode::AnimNode()
	{
	}
	
	Animation::Animation(aiAnimation* animation, Model* model)
	{
		Animation* anim = new Animation();
		m_Duration = animation->mDuration;
		m_TicksPerSecond = animation->mTicksPerSecond;
		ticksPerSecond = animation->mTicksPerSecond;
		numChannels = animation->mNumChannels;

		for (unsigned int i = 0; i < animation->mNumChannels; i++)
		{
			anim->channels.push_back(LoadAnimationNode(animation->mChannels[i]));
			
		}

		animations.push_back(anim);
	}

	void Animation::LoadAnimation(const aiAnimation* animation)
	{
		Animation* anim = new Animation();
		m_Duration = animation->mDuration;
		m_TicksPerSecond = animation->mTicksPerSecond;
		ticksPerSecond = animation->mTicksPerSecond;
		numChannels = animation->mNumChannels;

		for (unsigned int i = 0; i < animation->mNumChannels; i++)
		{
			anim->channels.push_back(LoadAnimationNode(animation->mChannels[i]));

		}

		animations.push_back(anim);
	}

	AnimNode* Animation::LoadAnimationNode(const aiNodeAnim* aiAnimNode)
	{
		AnimNode* node = new AnimNode();

		node->name = aiAnimNode->mNodeName.C_Str();

		node->numPositionKeys = aiAnimNode->mNumPositionKeys;
		node->numRotationKeys = aiAnimNode->mNumRotationKeys;
		node->numScalingKeys = aiAnimNode->mNumScalingKeys;


		for (unsigned int i = 0; i < node->numPositionKeys; i++)
		{
			VectorKey posKey;

			posKey.time = aiAnimNode->mPositionKeys[i].mTime;
			posKey.value.x = aiAnimNode->mPositionKeys[i].mValue.x;
			posKey.value.y = aiAnimNode->mPositionKeys[i].mValue.y;
			posKey.value.z = aiAnimNode->mPositionKeys[i].mValue.z;
			node->positionKeys.push_back(posKey);
		}
		for (unsigned int i = 0; i < node->numRotationKeys; i++)
		{
			QuatKey quatKey;

			quatKey.time = aiAnimNode->mRotationKeys[i].mTime;
			quatKey.value.x = aiAnimNode->mRotationKeys[i].mValue.x;
			quatKey.value.y = aiAnimNode->mRotationKeys[i].mValue.y;
			quatKey.value.z = aiAnimNode->mRotationKeys[i].mValue.z;
			quatKey.value.w = aiAnimNode->mRotationKeys[i].mValue.w;
			node->rotationKeys.push_back(quatKey);
		}
		for (unsigned int i = 0; i < node->numScalingKeys; i++)
		{
			VectorKey scaleKey;

			scaleKey.time = aiAnimNode->mScalingKeys[i].mTime;
			scaleKey.value.x = aiAnimNode->mScalingKeys[i].mValue.x;
			scaleKey.value.y = aiAnimNode->mScalingKeys[i].mValue.y;
			scaleKey.value.z = aiAnimNode->mScalingKeys[i].mValue.z;
			node->scalingKeys.push_back(scaleKey);
		}

		return node;
	}

	AnimNode* Animation::LoadWiAnimNode(File file)
	{
		AnimNode* node = new AnimNode();

		size_t name_len;
		file.Read(&name_len, sizeof(size_t));
		node->name.resize(name_len);
		file.Read(&node->name[0], name_len);

		file.Read(&node->numPositionKeys, sizeof(unsigned int));
		file.Read(&node->numRotationKeys, sizeof(unsigned int));
		file.Read(&node->numScalingKeys, sizeof(unsigned int));

		// Read position keys
		size_t position_key_size;
		file.Read(&position_key_size, sizeof(size_t));
		node->positionKeys.resize(position_key_size);
		file.Read(&node->positionKeys[0], position_key_size * sizeof(VectorKey));
		// Read Rotation keys
		size_t rotation_key_size;
		file.Read(&rotation_key_size, sizeof(size_t));
		node->rotationKeys.resize(rotation_key_size);
		file.Read(&node->rotationKeys[0], rotation_key_size * sizeof(QuatKey));
		// Read scaling keys
		size_t scaling_key_size;
		file.Read(&scaling_key_size, sizeof(size_t));
		node->scalingKeys.resize(scaling_key_size);
		file.Read(&node->scalingKeys[0], scaling_key_size * sizeof(VectorKey));
		return node;
	}

	Animation* Animation::LoadWiAnimation(File file)
	{
		Animation* anim = new Animation();

		size_t name_len;
		file.Read(&name_len, sizeof(size_t));
		anim->name.resize(name_len);
		file.Read(&anim->name[0], name_len);

		file.Read(&anim->duration, sizeof(double));
		file.Read(&anim->ticksPerSecond, sizeof(double));
		file.Read(&anim->numChannels, sizeof(unsigned int));

		size_t channels_size;
		file.Read(&channels_size, sizeof(size_t));

		for (unsigned int i = 0; i < channels_size; i++)
		{
			anim->channels.push_back(LoadWiAnimNode(file));
		}


		return anim;
	}

	void Animation::SaveWiAnimation(File& file, Animation* anim)
	{
		size_t name_len = anim->name.size();

		file.Write(&name_len, sizeof(size_t));
		file.Write(anim->name.c_str(), name_len);

		file.Write(&anim->duration, sizeof(double));
		file.Write(&anim->ticksPerSecond, sizeof(double));
		file.Write(&anim->numChannels, sizeof(unsigned int));

		size_t channels_size = anim->channels.size();
		file.Write(&channels_size, sizeof(size_t));


		for (unsigned int i = 0; i < channels_size; i++)
		{
			SaveWiAnimNode(file, anim->channels[i]);
		}
	}
	void Animation::SaveWiAnimNode(File& file, AnimNode* node)
	{
		if (node == NULL) return;
		size_t name_len = node->name.size();
		file.Write(&name_len, sizeof(size_t));
		file.Write(node->name.c_str(), name_len);

		file.Write(&node->numPositionKeys, sizeof(unsigned int));
		file.Write(&node->numRotationKeys, sizeof(unsigned int));
		file.Write(&node->numScalingKeys, sizeof(unsigned int));


		//Position keys
		size_t position_size = node->positionKeys.size();
		file.Write(&position_size, sizeof(size_t));
		file.Write(node->positionKeys.data(), position_size * sizeof(VectorKey));
		//Rotation keys
		size_t rotation_size = node->rotationKeys.size();
		file.Write(&rotation_size, sizeof(size_t));
		file.Write(node->rotationKeys.data(), rotation_size * sizeof(QuatKey));
		//Scale keys
		size_t scale_size = node->scalingKeys.size();
		file.Write(&scale_size, sizeof(size_t));
		file.Write(node->scalingKeys.data(), scale_size * sizeof(VectorKey));
	}
}