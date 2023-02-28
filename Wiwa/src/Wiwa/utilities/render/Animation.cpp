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
			//anim->channels.push_back(LoadAnimationNode(animation->mChannels[i]));
			
		}

		animations.push_back(anim);
	}
}