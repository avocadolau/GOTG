#pragma once
#include <Wiwa/core/Core.h>

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/utilities/filesystem/FileSystem.h>
//#include <assimp/cimport.h>
//#include <assimp/importer.hpp>
#include "Model.h"


struct Model;
struct BoneInfo;
struct aiAnimation;

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
		glm::quat value;
		QuatKey()
		{
			time = 0; value = { 0,0,0,0 };
		}
		QuatKey(double time, const glm::quat& value)
		{
			this->time = time;
			this->value = value;
		}
	};
	//relation between the aiNode
	//and the animNode is the name
	//to find the node affected by the animation
	// just find the name of the node to apply the animation
	struct AnimNode {
		std::string name;
		unsigned int numPositionKeys = 0;
		unsigned int numRotationKeys = 0;
		unsigned int numScalingKeys = 0;

		std::vector<VectorKey> positionKeys;
		std::vector<QuatKey> rotationKeys;
		std::vector<VectorKey> scalingKeys;

		AnimNode();
	};

	struct NodeData
	{
		glm::mat4 transformation;
		std::string name;
		int childrenCount;
		std::vector<NodeData> children;
	};

	class WI_API Animation
	{
	public: 
		Animation();
		Animation(std::string name) { this->name = name; }
		Animation(aiAnimation* anim, Model* model);
		//Animation(aiAnimation* anim, Model* model)
		//{
		//	//Assimp::Importer importer;
		//	//const aiScene* scene = importer.ReadFile(animationPath, aiProcess_Triangulate);
		//	//assert(scene && scene->mRootNode);
		//	//auto animation = scene->mAnimations[0];
		//	//m_Duration = anim->mDuration;
		//	//m_TicksPerSecond = anim->mTicksPerSecond;
		//	//ReadHeirarchyData(m_RootNode, scene->mRootNode);
		//	//ReadMissingBones(animation, *model);
		//}
		~Animation();

		void LoadAnimationNode(const aiNodeAnim* aiNode);


		std::vector<Animation*> animations;

		std::string name = "";
		std::vector<AnimNode*> channels;
		unsigned int numChannels = 0;
		double duration = 0.0f;
		double ticksPerSecond = 0.0f;


		NodeData GetRootNode() {
			return m_RootNode;
		}

	private: 
		float m_Duration;
		int m_TicksPerSecond;
		//std::vector<Bone> m_Bones;
		NodeData m_RootNode;
		std::map<std::string, BoneInfo> m_BoneInfoMap;
	};
}

