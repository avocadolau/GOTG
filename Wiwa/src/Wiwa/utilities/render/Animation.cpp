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
	
	Animation::Animation(aiAnimation* anim, Model* model)
	{
		m_Duration = anim->mDuration;
		m_TicksPerSecond = anim->mTicksPerSecond;
	}
}