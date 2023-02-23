#pragma once
#include <Wiwa/core/Core.h>

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>

#include <Wiwa/utilities/filesystem/FileSystem.h>

namespace Wiwa {

	struct VectorKey
	{
		//time of this key when the transformation should happen
		double time;
		glm::vec3 value;
		VectorKey() 
		{
			time = 0; value = { 0,0,0 };
		}
		VectorKey(double time, const glm::vec3 value)
		{
			this->time = time;
			this->value = value;
		}
	};
	struct QuatKey {
		//time of this key when the transformation should happen
		double time;
		glm::vec4 value;
		QuatKey()
		{
			time = 0; value = { 0,0,0,0 };
		}
		QuatKey(double time, const glm::vec4& value)
		{
			this->time = time;
			this->value = value;
		}
	};
	//realition between the aiNode
	//and the animNode is the name
	//to find the node affected by the animation
	// just find the name of the node to apply the animation
	struct AnimNode {
		std::string name;
		unsigned int numPositionKeys;
		unsigned int numRotationKeys;
		unsigned int numScalingKeys;

		std::vector<VectorKey> positionKeys;
		std::vector<QuatKey> rotationKeys;
		std::vector<VectorKey> scalingKeys;
	};

	class WI_API Animation
	{
	public:
		Animation();
		Animation(std::string name) { this->name = name; }
		
		std::string name;
		//animation key node tree 
		std::vector<AnimNode*> channels;
		unsigned int numChannels;
		double duration;
		double ticksPerSecond;
	};
}

