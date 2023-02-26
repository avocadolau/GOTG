#pragma once

#include <Wiwa/core/Core.h>

#include <vector>
#include <map>
#include <Wiwa/utilities/math/Math.h>
#include <Wiwa/utilities/math/AABB.h>
#include <Wiwa/utilities/render/Animation.h>

#include <Wiwa/utilities/filesystem/FileSystem.h>

#define MAX_NUM_BONES_PER_VERTEX 10
#define MAX_NUM_TEXTCOORDS_PER_ANIM 200 // ???? no idea, obviously needs more but I dunno


struct aiScene;
struct aiMesh;
struct aiNode;
struct aiBone;
struct aiAnimation;
struct aiNodeAnim;
struct aiAnimMesh;

namespace Wiwa {
	struct ModelSettings;

	struct ModelHierarchy {
		std::string name;

		// Index in model list
		std::vector<unsigned int> meshIndexes;

		// Model children
		std::vector<ModelHierarchy*> children;

		// Transformation
		Vector3f translation;
		Vector3f rotation;
		Vector3f scale;

		glm::mat4 Transformation;

		~ModelHierarchy() {
			size_t len = children.size();

			for (size_t i = 0; i < len; i++) {
				delete children[i];
			}

			children.clear();
			meshIndexes.clear();
		}
	};

	struct Armature {
		std::string name;
		glm::mat4 offsetMatrix;
		glm::mat4 transform;

		std::vector<Armature*> bones;
		~Armature()
		{
			size_t len = bones.size();

			for (size_t i = 0; i < len; i++) {
				delete bones[i];
			}

			bones.clear();
		}

	};
	struct VertexWeight {
		unsigned int vertexId;
		float weight;
		
	};
	struct BoneInfo {
		std::string name;
		glm::mat4 offsetMatrix;
		glm::mat4 finalTransformation;

		BoneInfo()
		{
			offsetMatrix = glm::mat4();
			finalTransformation = glm::mat4();
		}
		BoneInfo(glm::mat4& offset)
		{
			offsetMatrix = offset;
			finalTransformation = glm::mat4();
		}
	};

	struct AnimInfo {
		std::vector<float> Bitangents;
		std::vector<float> color; // 4 floats!
		std::vector<float> Normals;
		unsigned int numVertices;
		std::vector<float> Tangents;
		std::vector<float> TextCoords;
		std::vector<float> Vertices;


	};

	//Structure to save what bones affect each vertex for then apply transformations
	struct VertexBoneData
	{
		unsigned int BoneIDs[MAX_NUM_BONES_PER_VERTEX] = { 0 };
		float Weights[MAX_NUM_BONES_PER_VERTEX] = { 0 };
		VertexBoneData()
		{
		}

		void AddBoneData(unsigned int BoneID, float weight)
		{
			for (unsigned int i = 0; i < sizeof(BoneIDs)/sizeof(unsigned int); i++)
			{
				// weight has to be greater than 0 if the bone influences that vertex
				// if 0 then we assume its empty
				if (Weights[i] == 0.0)
				{
					BoneIDs[i] = BoneID;
					Weights[i] = weight;
					//DEBUG
					//WI_INFO("bone {0} weight {1} index {2}\n", BoneID, weight, i);
					return;
				}
			}

			//we should never reach here, more bones than we have spce for
			assert(0,"BONE INDEX TO VERTEX OUT OF SIZE");
		}
	};
	class WI_API Model
	{
	private:
		std::string m_ModelPath;
		glm::mat4 globalInverseTransform;
	protected:
		bool is_root = false;
		bool has_bones = false;

		std::string model_name;
		unsigned int model_mat;

		std::vector<float> vbo_data;
		std::vector<int> ebo_data;
		std::vector<int> bbebo_data;
		std::vector<float> bbvbo_data;
		std::vector<VertexBoneData> bone_data;

		std::vector<Model*> models;
		std::vector<std::string> materials;
		

		//stores offest matrix and final bone transformation
		std::vector<BoneInfo> boneInfo;
		std::vector<int> meshBaseVertex;
		std::map<std::string, unsigned int> boneNameToIndexMap;

		Animation* currentAnimation;

		ModelHierarchy* model_hierarchy;
		Model* parent;

		void generateBuffers();

	private:
		unsigned int vao, vbo, ebo, bbvao, bbvbo, bbebo, bonevb;

		bool getMeshFromFile(const char* file, ModelSettings* settings, bool gen_buffers=true);
		void getWiMeshFromFile(const char* file);
		int getBoneId(const aiBone* pBone);

		Model* loadmesh(const aiMesh* mesh);
		ModelHierarchy* loadModelHierarchy(const aiNode* node, const glm::mat4& parentMatrix);
		
		void SetBoneInfo(const aiNode* pNode, const glm::mat4& parentTransform);

		void LoadMeshBones(unsigned int index, const aiMesh* mesh, Model* root);
		void LoadSingleBone(int meshIndex, aiBone* bone, Model* root);
		void PrintAssimpMatrix(const aiBone* b);

		void LoadAnimation(const aiAnimation* animation);
		AnimNode* LoadAnimationNode(const aiNodeAnim* aiAnimNode);
		AnimNode* LoadWiAnimNode(File file);
		Animation* LoadWiAnimation(File file);
		static void SaveWiAnimation(File& file, Animation* anim);
		static void SaveWiAnimNode(File& file, AnimNode* node);
		
		void CreateCube();
		void CreatePlane();
		void CreatePyramid();
		void CreateSphere();

		static void SaveModelHierarchy(File file, ModelHierarchy* h);
		static ModelHierarchy* LoadModelHierarchy(File file);
		void ReadNodeHeirarchy(float timeInSeconds, ModelHierarchy* root, glm::mat4 parentTransform);
		const AnimNode* FindNodeAnim(const Animation* animation, const std::string& NodeName);
		////animation
		unsigned int FindScaling(float AnimationTime, const AnimNode* pNodeAnim);
		unsigned int FindRotation(float AnimationTime, const AnimNode* pNodeAnim);
		unsigned int FindPosition(float AnimationTime, const AnimNode* pNodeAnim);
		void CalcInterpolatedScaling(glm::vec3& Out, float AnimationTime, const AnimNode* NodeAnim);
		void CalcInterpolatedRotation(glm::quat& Out, float AnimationTime, const AnimNode* pNodeAnim);
		void CalcInterpolatedPosition(glm::vec3& Out, float AnimationTime, const AnimNode* pNodeAnim);
		
	public:
		Model(const char* file);
		~Model();

		void Render();

		void DrawBoudingBox();

		unsigned int getVAO() { return vao; }

		const char* getModelPath() { return m_ModelPath.c_str(); }

		bool IsRoot() const { return is_root; }

		Model* getModelAt(size_t index) { return models[index]; }

		std::string getMaterialAt(size_t index) { return materials[index]; }

		size_t getMaterialCount() { return materials.size(); }

		unsigned int getMaterialIndex() { return model_mat; }

		const ModelHierarchy* getModelHierarchy() { return model_hierarchy; }

		std::string getModelName() { return model_name; }

		void SetCurrentAnimation(Animation* newAnimation) { currentAnimation = newAnimation; }
		Animation* GetCurrentAnimation() { return currentAnimation; }

		void LoadMesh(const char* file, ModelSettings* settings);
		void LoadWiMesh(const char* file);

		void GetBoneTransforms(float timeInSeconds,std::vector<glm::mat4>& transforms);

		void IsRoot(bool root) { is_root = root; }

		static Model* GetModelFromFile(const char* file, ModelSettings* settings);
		static void SaveModel(Model* model, const char* file);
	public:

		// for the moment i'll put the anim here, i'll change it
		std::vector<Animation*> animations;

		bool showNormals = false;
		Math::AABB boundingBox;
	};
}