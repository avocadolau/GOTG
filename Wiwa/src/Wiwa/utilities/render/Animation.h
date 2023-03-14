#pragma once
#include <Wiwa/core/Core.h>

#include <string>
#include <vector>
#include <map>
#include <glm/glm.hpp>
#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/utilities/filesystem/FileSystem.h>
#include <Wiwa/utilities/render/Bone.h>
#include <Wiwa/utilities/render/Model.h>

struct aiAnimation;
struct BoneInfo;
class ModelHierarchy;

namespace Wiwa {
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
		Animation(const aiAnimation* animation, Model* model);
		Animation(const char* filePath, Model* model);
		~Animation();

		Bone* FindBone(const std::string& name);

		inline float GetTicksPerSecond() { return m_TicksPerSecond; }

		inline float GetDuration() { return m_Duration; }

		inline const NodeData& GetRootNode() { return m_RootNode; }

		inline const std::map<std::string, BoneInfo>& GetBoneIDMap()
		{
			return m_BoneInfoMap;
		}

		void LoadAnimation(const aiAnimation* animation);
		static Animation* LoadWiAnimation(File file);
		static void SaveWiAnimation(File& file, Animation* animation);
		void SaveNodeData(File& file, NodeData* node);
		NodeData* LoadNodeData(File& file);

		float m_Duration = 0;
		int m_TicksPerSecond = 0;
		int m_NumChannels = 0;
		std::string m_Name;
	private:
		void ReadMissingBones(const aiAnimation* animation, Model& model);

		void ReadHeirarchyData(NodeData& dest, const ModelHierarchy* root);

		std::vector<Bone*> m_Bones;
		NodeData m_RootNode;
		std::map<std::string, BoneInfo> m_BoneInfoMap;
		unsigned int m_BoneCount;
	};
	//struct VectorKey
	//{
	//	//time of this key when the transformation should happen
	//	double time;
	//	glm::vec3 value;
	//	VectorKey() 
	//	{
	//		time = 0; value = { 0,0,0 };
	//	}
	//	VectorKey(double time, const glm::vec3 value)
	//	{
	//		this->time = time;
	//		this->value = value;
	//	}
	//};
	//struct QuatKey {
	//	//time of this key when the transformation should happen
	//	double time;
	//	glm::quat value;
	//	QuatKey()
	//	{
	//		time = 0; value = { 0,0,0,0 };
	//	}
	//	QuatKey(double time, const glm::quat& value)
	//	{
	//		this->time = time;
	//		this->value = value;
	//	}
	//};
	////relation between the aiNode
	////and the animNode is the name
	////to find the node affected by the animation
	//// just find the name of the node to apply the animation
	//struct AnimNode {
	//	std::string name;
	//	unsigned int numPositionKeys = 0;
	//	unsigned int numRotationKeys = 0;
	//	unsigned int numScalingKeys = 0;

	//	std::vector<VectorKey> positionKeys;
	//	std::vector<QuatKey> rotationKeys;
	//	std::vector<VectorKey> scalingKeys;

	//	AnimNode();
	//};

	//struct NodeData
	//{
	//	glm::mat4 transformation;
	//	std::string name;
	//	int childrenCount;
	//	std::vector<NodeData> children;
	//};

	//class WI_API Animation
	//{
	//public: 
	//	   Animation();
	//	Animation(std::string name) { this->name = name; }
	//	Animation(aiAnimation* anim, Model* model);
	//	~Animation();		

	//	void LoadAnimation(const aiAnimation* animation);
	//	AnimNode* LoadAnimationNode(const aiNodeAnim* aiAnimNode);
	//	AnimNode* LoadWiAnimNode(File file);
	//	Animation* LoadWiAnimation(File file);
	//	static void SaveWiAnimation(File& file, Animation* anim);
	//	static void SaveWiAnimNode(File& file, AnimNode* node);

	//	std::vector<Animation*> animations;

	//	std::string name = "";
	//	std::vector<AnimNode*> channels;
	//	unsigned int numChannels = 0;
	//	double duration = 0.0f;
	//	double ticksPerSecond = 0.0f;


	//	NodeData GetRootNode() 
	//	{
	//		return m_RootNode;
	//	}

	//private: 
	//	float m_Duration;
	//	int m_TicksPerSecond;
	//	//std::vector<Bone> m_Bones;
	//	NodeData m_RootNode;
	//	//std::map<std::string, BoneInfo> m_BoneInfoMap;
	//};

}

