#include <wipch.h>

#include "Model.h"
#include <fstream>
#include <string>

#include <glew.h>

#include <Wiwa/core/Application.h>
#include <Wiwa/ecs/EntityManager.h>
#include <Wiwa/scene/SceneManager.h>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

#include <Wiwa/utilities/filesystem/FileSystem.h>
#include <Wiwa/core/Resources.h>
#include <Wiwa/utilities/render/Animation.h>





// For converting between ASSIMP and glm
static inline glm::vec3 vec3_cast(const aiVector3D& v) { return glm::vec3(v.x, v.y, v.z); }
static inline glm::vec2 vec2_cast(const aiVector3D& v) { return glm::vec2(v.x, v.y); } // it's aiVector3D because assimp's texture coordinates use that
static inline glm::quat quat_cast(const aiQuaternion& q) { return glm::quat(q.w, q.x, q.y, q.z); }
static inline glm::mat4 mat4_cast(const aiMatrix4x4& m) { return glm::transpose(glm::make_mat4(&m.a1)); }
static inline glm::mat4 mat4_cast(const aiMatrix3x3& m) { return glm::transpose(glm::make_mat3(&m.a1)); }


namespace Wiwa {
	bool Model::getMeshFromFile(const char* file, ModelSettings* settings, bool gen_buffers)
	{
		unsigned int flags = aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_GenSmoothNormals;//aiProcessPreset_TargetRealtime_Quality | aiProcess_FlipUVs;//aiProcessPreset_TargetRealtime_MaxQuality | aiProcess_FlipUVs;

		if (settings->preTranslatedVertices) {
			//flags |= aiProcess_PreTransformVertices;
		}

		sbyte* file_data = NULL;
		uint32_t file_buf_size = FileSystem::ReadAll(file, &file_data);

		WI_INFO("Loaded with bytes: {}", file_buf_size);

		const aiScene* scene = aiImportFileFromMemory(file_data, file_buf_size, flags, NULL);

		delete[] file_data;

		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			WI_ERROR("Couldn't load mesh file: {0}", file);
			return false;
		}

		WI_CORE_INFO("Loading mesh file at: {0} ...", file);
		
		is_root = true;


		m_ModelPath = file;

		model_name = scene->mRootNode->mName.C_Str();

		std::filesystem::path path = file;
		path.remove_filename();

		std::filesystem::path curr_path = std::filesystem::current_path();

		// Process materials
		if (scene->HasMaterials()) {
			for (unsigned int i = 0; i < scene->mNumMaterials; i++) {
				aiMaterial* mat = scene->mMaterials[i];

				aiString name;
				aiGetMaterialString(mat, AI_MATKEY_NAME, &name);

				aiString texture_diffuse;
				aiGetMaterialString(mat, AI_MATKEY_TEXTURE(aiTextureType_DIFFUSE, 0), &texture_diffuse);

				aiColor4D diffuse(1.0f, 1.0f, 1.0f, 1.0f);
				aiGetMaterialColor(mat, AI_MATKEY_COLOR_DIFFUSE, &diffuse);

				aiColor4D specular(1.0f, 1.0f, 1.0f, 1.0f);
				aiGetMaterialColor(mat, AI_MATKEY_COLOR_SPECULAR, &specular);

				float shininess = 0.1f;
				aiGetMaterialFloat(mat, AI_MATKEY_SHININESS, &shininess);
				
				std::filesystem::path mat_path = path;
				mat_path += name.C_Str();
				mat_path += ".wimaterial";

				bool mat_imported = Resources::Import<Material>(mat_path.string().c_str());

				if (!mat_imported)
				{
					Material material((Shader*)NULL); // Default settings
					size_t id;

					if (texture_diffuse.length > 0)
					{
						std::filesystem::current_path(path);

						std::filesystem::path texture_path = texture_diffuse.C_Str();
						texture_path = std::filesystem::absolute(texture_path);

						std::filesystem::current_path(curr_path);

						texture_path = std::filesystem::relative(texture_path);

						//const char* default_shader = "resources/shaders/light/lit_model_textured";
						//const char* default_shader = "resources/shaders/light/skinned";
						const char* default_shader = "resources/shaders/skinned/debug_bones";

						id = Resources::Load<Shader>(default_shader);
						material.setShader(Resources::GetResourceById<Shader>(id), default_shader);

						bool imported = Resources::Import<Image>(texture_path.string().c_str());

						if (imported) {
							Uniform::SamplerData sdata;
							sdata.resource_id = Resources::Load<Image>(texture_path.string().c_str());
							Image* img = Resources::GetResourceById<Image>(sdata.resource_id);
							sdata.tex_id = img->GetTextureId();
							sdata.tex_path = texture_path.string();				

							material.SetUniformData("u_Tex0", sdata);
						}
					}
					else
					{
						//Set the color of the material
						//id = Resources::Load<Shader>("resources/shaders/light/lit_model_color");
						id = Resources::Load<Shader>("resources/shaders/skinned/debug_bones");
						material.setShader(Resources::GetResourceById<Shader>(id), "resources/shaders/skinned/debug_bones");
						//material.setShader(Resources::GetResourceById<Shader>(id), "resources/shaders/light/lit_model_color");
						material.SetUniformData("u_Color", glm::vec4(diffuse.r, diffuse.g, diffuse.b, diffuse.a));
					}

					Material::SaveMaterial(mat_path.string().c_str(), &material);
				}
				materials.push_back(mat_path.string());
			}
		}

		// Load mesh list
		if (scene->HasMeshes())
		{

			int totalVertices = 0;
			int totalIndices = 0;
			int totalBones = 0;
			//resize the vector that will contain the number of base index
			meshes.resize(scene->mNumMeshes);

			for (unsigned int i = 0; i < scene->mNumMeshes; i++) {

				const aiMesh* pMesh = scene->mMeshes[i];

				int numVertices = pMesh->mNumVertices;
				int numIndices = pMesh->mNumFaces * 3;
				int numBones = pMesh->mNumBones;
				meshes[i] = totalVertices;
				totalVertices += numVertices;
				totalIndices += numIndices;
				totalBones += numBones;

				Model* model = loadmesh(pMesh);
				model->parent = this;
				//Load bones
				if (pMesh->HasBones())
				{
					model->has_bones = true;
					model->bone_data.resize(totalVertices);
					model->LoadMeshBones(i, pMesh);
				}
				if (gen_buffers) {
					model->generateBuffers();
				}
				models.push_back(model);
			}
		}

		//TOOD import animations into an Animation structure

		if (scene->HasAnimations())
		{
			WI_INFO("Model {0} has {1} animations", model_name, scene->mNumAnimations);

			for (unsigned int i = 0; i < scene->mNumAnimations; i++)
			{
			//	animations.push_back(new Animation(scene->mAnimations[i], this));
			}
		}

		//load hierarchy after, animation pourpuses
		model_hierarchy = loadModelHierarchy(scene->mRootNode);
		globalInverseTransform = mat4_cast(scene->mRootNode->mTransformation);
		globalInverseTransform = glm::inverse(globalInverseTransform);

		std::filesystem::path p = file;
		model_hierarchy->name = p.stem().string();


		aiReleaseImport(scene);
		return true;
	}

	void Model::getWiMeshFromFile(const char* file)
	{
		is_root = true;

		File f = FileSystem::OpenIB(file);

		if (f.IsOpen()) {
			m_ModelPath = file;

			size_t model_size;

			// Model size
			f.Read(&model_size, sizeof(size_t));

			models.reserve(model_size);

			//load hierarchy
			model_hierarchy = LoadModelHierarchy(f);

			//load globbal inverse matrix
			f.Read(&globalInverseTransform, sizeof(glm::mat4));

			//load map name to index bone
			size_t bone_index_size;
			f.Read(&bone_index_size, sizeof(size_t));
			for (size_t i = 0; i < bone_index_size; i++)
			{
				std::pair<std::string, BoneInfo> item;
				size_t name_size;
				f.Read(&name_size, sizeof(size_t));
				item.first.resize(name_size);
				f.Read(&item.first[0], name_size);
				f.Read(&item.second.id, sizeof(unsigned int));
				f.Read(&item.second.offsetMatrix, sizeof(glm::mat4));
				f.Read(&item.second.finalTransformation, sizeof(glm::mat4));

				m_BoneInfoMap.insert(item);
			}
			//bone info
			//size_t bones_info_size;
			//f.Read(&bones_info_size, sizeof(size_t));
			//boneInfo.resize(bones_info_size);
			//
			//if (bones_info_size != 0) {
			//	f.Read(&boneInfo[0], bones_info_size * sizeof(BoneInfo));
			//}
			//load animations
			//size_t anim_size;
			//f.Read(&anim_size, sizeof(size_t));
			//animations.resize(anim_size);
			//for (size_t i = 0; i < anim_size; i++)
			//{
			//	animations[i] = LoadWiAnimation(f);
			//}
		

			// Material size
			size_t mat_size;

			f.Read(&mat_size, sizeof(size_t));

			materials.resize(mat_size);

			for (size_t i = 0; i < mat_size; i++) {
				size_t name_size;

				f.Read(&name_size, sizeof(size_t));
				materials[i].resize(name_size);

				f.Read(&materials[i][0], name_size);
			}

			// Children models
			for (size_t i = 0; i < model_size; i++) {
				Model* model = new Model(NULL);

				// Read material index
				f.Read(&model->model_mat, sizeof(size_t));

				// Read vbo
				size_t vbo_size;
				f.Read(&vbo_size, sizeof(size_t));
				model->vbo_data.resize(vbo_size);
				f.Read(&model->vbo_data[0], vbo_size * sizeof(float));

				// Read ebo
				size_t ebo_size;
				f.Read(&ebo_size, sizeof(size_t));
				model->ebo_data.resize(ebo_size);
				f.Read(&model->ebo_data[0], ebo_size * sizeof(int));

				// Read Bones
				size_t bones_size;
				f.Read(&bones_size, sizeof(size_t));
				model->bone_data.resize(bones_size);
				
				if (bones_size != 0)
				f.Read(&model->bone_data[0], bones_size * sizeof(VertexBoneData));

			
				//hold reference to root
				model->parent = this;
	
				model->generateBuffers();
				models.push_back(model);
			}
		}
		f.Close();
	}

	int Model::GetBoneId(const aiBone* pBone)
	{
		int boneid = 0;
		std::string bone_name(pBone->mName.C_Str());
		if (m_BoneInfoMap.find(bone_name) == m_BoneInfoMap.end())
		{
			//allocate an index for a new bone
			boneid = m_BoneInfoMap.size();
			m_BoneInfoMap[bone_name].id = boneid;
		}
		else {
			boneid = m_BoneInfoMap[bone_name].id;
		}
		return boneid;
	}

	Model* Model::loadmesh(const aiMesh* mesh)
	{
		Model* model = new Model(NULL);

		model->model_mat = mesh->mMaterialIndex;

		model->model_name = mesh->mName.C_Str();
		for (unsigned int j = 0; j < mesh->mNumVertices; j++) {
			// Vertices
			model->vbo_data.push_back(mesh->mVertices[j].x);
			model->vbo_data.push_back(mesh->mVertices[j].y);
			model->vbo_data.push_back(mesh->mVertices[j].z);
			// Normals
			model->vbo_data.push_back(mesh->mNormals[j].x);
			model->vbo_data.push_back(mesh->mNormals[j].y);
			model->vbo_data.push_back(mesh->mNormals[j].z);
			// Texture coordinates
			if (mesh->mTextureCoords[0])
			{
				model->vbo_data.push_back(mesh->mTextureCoords[0][j].x);
				model->vbo_data.push_back(mesh->mTextureCoords[0][j].y);
			}
			else {
				model->vbo_data.push_back(0.0f);
				model->vbo_data.push_back(0.0f);
			}
		}
		if (model->vbo_data.empty())
		{
			WI_CORE_ERROR("Error while loading mesh vertex buffer");
		}
		else
		{
			WI_CORE_INFO("Vertex buffer generated correctly");
		}
		// Indices
		for (unsigned int j = 0; j < mesh->mNumFaces; j++) {
			aiFace& face = mesh->mFaces[j];
			for (unsigned int k = 0; k < face.mNumIndices; k++) {
				model->ebo_data.push_back(face.mIndices[k]);
			}
		}
		if (model->ebo_data.empty())
		{
			WI_CORE_ERROR("Error while loading mesh index buffer");
		}
		else
		{
			WI_CORE_INFO("Index buffer generated correctly");
		}
		
		return model;
	}

	ModelHierarchy* Model::loadModelHierarchy(const aiNode* node) //animation
	{
		ModelHierarchy* h = new ModelHierarchy();

		h->name = node->mName.C_Str();

		//DEBUG
		//WI_INFO("Name {0}, num children {1}, num meshes{2}",node->mName.C_Str(),node->mNumChildren, node->mNumMeshes);
		//PrintAssimpNodeMatrix(node);
		// Node transform
		aiVector3D translate, scale, rot;
		//aiQuaternion quat;
		node->mTransformation.Decompose(scale, rot, translate);
		h->translation = { translate.x, translate.y, translate.z };
		h->rotation = { rot.x * 180.0f / PI_F, rot.y * 180.0f / PI_F, rot.z * 180.0f / PI_F };
		h->scale = { scale.x, scale.y, scale.z };

		//Store the transformation matrix for aniamtion
		h->Transformation = mat4_cast(node->mTransformation);
	
		// Node meshes
		for (size_t i = 0; i < node->mNumMeshes; i++) {
			h->meshIndexes.push_back(node->mMeshes[i]);
		}

		// Node children
		for (size_t i = 0; i < node->mNumChildren; i++) {

			h->children.push_back(loadModelHierarchy(node->mChildren[i]));
		}

		return h;
	}

	void Model::SetBoneInfo(const aiNode* pNode, const glm::mat4& parentTransform)
	{
		std::string nodeName(pNode->mName.data);
		aiMatrix4x4 m = pNode->mTransformation;
		glm::mat4 nodeTransformation = glm::make_mat4(m.ToPtr());

		WI_INFO("{0}", nodeName.c_str());

		glm::mat4 globalTransformation = parentTransform * nodeTransformation;

		if (m_BoneInfoMap.find(nodeName) != m_BoneInfoMap.end())
		{
			unsigned int boneIndex = m_BoneInfoMap[nodeName].id;
			boneInfo[boneIndex].finalTransformation = globalTransformation;
		}
		for (unsigned int i = 0; i < pNode->mNumChildren; i++)
		{
			SetBoneInfo(pNode->mChildren[i], globalTransformation);
		}
	}

	void Model::CreateCube()
	{
		vbo_data = {
			1,1,1, 0,0,1, 0.625,0.5,
			-1,1,1, 0,0,1, 0.875,0.5,
			-1,-1,1, 0,0,1, 0.875,0.25,
			1,-1,1, 0,0,1, 0.625,0.25,
			1,-1,-1, 0,-1,0, 0.375,0.25,
			1,-1,1, 0,-1,0, 0.625,0.25,
			-1,-1,1, 0,-1,0, 0.625,0,
			-1,-1,-1, 0,-1,0, 0.375,0,
			-1,-1,-1, -1,0,0, 0.375,1,
			-1,-1,1, -1,0,0, 0.625,1,
			-1,1,1, -1,0,0, 0.625,0.75,
			-1,1,-1, -1,0,0, 0.375,0.75,
			-1,1,-1, 0,0,-1, 0.125,0.5,
			1,1,-1, 0,0,-1, 0.375,0.5,
			1,-1,-1, 0,0,-1, 0.375,0.25,
			-1,-1,-1, 0,0,-1, 0.125,0.25,
			1,1,-1, 1,0,0, 0.375,0.5,
			1,1,1, 1,0,0, 0.625,0.5,
			1,-1,1, 1,0,0, 0.625,0.25,
			1,-1,-1, 1,0,0, 0.375,0.25,
			-1,1,-1, 0,1,0, 0.375,0.75,
			-1,1,1, 0,1,0, 0.625,0.75,
			1,1,1,	0,1,0, 0.625,0.5,
			1,1,-1, 0,1,0, 0.375,0.5
		};

		ebo_data = {
			0,1,2,0,2,3,
			4,5,6,4,6,7,
			8,9,10,8,10,11,
			12,13,14,12,14,15,
			16,17,18,16,18,19,
			20,21,22,20,22,23
		};
		is_root = false;
		generateBuffers();
	}

	void Model::CreatePlane()
	{
		vbo_data = {
			-0.50,-0.50,0.00,0.00,0.00,1.00,0.00,1.00
			,0.50,-0.50,0.00,0.00,0.00,1.00,1.00,1.00,
			0.50,0.50,0.00,0.00,0.00,1.00,1.00,0.00,
			-0.50,0.50,0.00,0.00,0.00,1.00,0.00,
			- 0.50,-0.50,0.00,0.00,0.00,1.00,0.00,
			1.00,0.50,-0.50,0.00,0.00,0.00,1.00,1.00
			,1.00,0.50,0.50,0.00,0.00,0.00,1.00,1.00,
			0.00,-0.50,0.50,0.00,0.00,0.00,1.00,0.00,0.00
		};

		ebo_data = {
			0,1,2,0,2,3
		};

		is_root = false;
		generateBuffers();
	}

	void Model::CreatePyramid()
	{
		vbo_data = {
			0.00f,1.00f,-1.00f,0.67f,0.67f,0.33f,0.25f,0.51f,0.00f,0.00f,1.00f,0.67,0.67,0.33,0.25,0.75,1.00,-0.00,-1.00,0.67,0.67,0.33,0.49,0.75,1.00,-0.00,-1.00,0.67,-0.67,0.33,0.49,0.75,0.00,0.00,1.00,0.67,-0.67,0.33,0.25,0.75,-0.00,-1.00,-1.00,0.67,-0.67,0.33,0.25,0.99,0.00,1.00,-1.00,0.00,0.00,-1.00,0.75,0.51,1.00,-0.00,-1.00,0.00,0.00,-1.00,0.99,0.75,-0.00,-1.00,-1.00,0.00,0.00,-1.00,0.75,0.99,-1.00,0.00,-1.00,0.00,0.00,-1.00,0.51,0.75,-0.00,-1.00,-1.00,-0.67,-0.67,0.33,0.25,0.99,0.00,0.00,1.00,-0.67,-0.67,0.33,0.25,0.75,-1.00,0.00,-1.00,-0.67,-0.67,0.33,0.01,0.75,-1.00,0.00,-1.00,-0.67,0.67,0.33,0.01,0.75,0.00,0.00,1.00,-0.67,0.67,0.33,0.25,0.75,0.00,1.00,-1.00,-0.67,0.67,0.33,0.25,0.51
		};

		ebo_data = {
			0,1,2,3,4,5,6,7,8,6,8,9,10,11,12,13,14,15
		};
		is_root = false;
		generateBuffers();
	}

	void Model::CreateSphere()
	{
		int stack_count = 18;
		int sector_count = 36;
		int radius = 1;

		float x, y, z, xy;
		float sectorStep = 2 * PI_F / sector_count;
		float stackStep = PI_F / stack_count;
		float sectorAngle, stackAngle;

		// Generate vertices
		for (int i = 0; i <= stack_count; ++i)
		{
			stackAngle = (float)PI / 2 - i * stackStep;
			xy = radius * cosf(stackAngle);
			z = radius * sinf(stackAngle);

			for (int j = 0; j <= sector_count; ++j)
			{
				sectorAngle = j * sectorStep;

				x = xy * cosf(sectorAngle);
				y = xy * sinf(sectorAngle);
				vbo_data.push_back(x);
				vbo_data.push_back(y);
				vbo_data.push_back(z);
			}
		}

		// Generate indices
		int k1, k2;
		for (int i = 0; i < stack_count; ++i)
		{
			k1 = i * (sector_count + 1);     // beginning of current stack
			k2 = k1 + sector_count + 1;      // beginning of next stack

			for (int j = 0; j < sector_count; ++j, ++k1, ++k2)
			{
				if (i != 0)
				{
					ebo_data.push_back(k1);
					ebo_data.push_back(k2);
					ebo_data.push_back(k1 + 1);
				}

				if (i != (stack_count - 1))
				{
					ebo_data.push_back(k1 + 1);
					ebo_data.push_back(k2);
					ebo_data.push_back(k2 + 1);
				}
			}
		}
		
		is_root = false;
		generateBuffers();
	}

	void Model::SaveModelHierarchy(File file, ModelHierarchy* h)
	{
		size_t name_len = h->name.size();

		file.Write(&name_len, sizeof(size_t));
		file.Write(h->name.c_str(), name_len);

		file.Write(&h->translation, sizeof(Vector3f));
		file.Write(&h->rotation, sizeof(Vector3f));
		file.Write(&h->scale, sizeof(Vector3f));

		//save transform
		file.Write(&h->Transformation, sizeof(glm::mat4));

		size_t mesh_ind_size = h->meshIndexes.size();

		file.Write(&mesh_ind_size, sizeof(size_t));

		if (mesh_ind_size > 0) {
			file.Write(h->meshIndexes.data(), mesh_ind_size);
		}

		size_t child_size = h->children.size();

		file.Write(&child_size, sizeof(size_t));

		if (child_size > 0) {
			for (size_t i = 0; i < child_size; i++) {
				SaveModelHierarchy(file, h->children[i]);
			}
		}
	}

	ModelHierarchy* Model::LoadModelHierarchy(File file)
	{
		ModelHierarchy* h = new ModelHierarchy();

		size_t name_len;

		file.Read(&name_len, sizeof(size_t));
		h->name.resize(name_len);
		file.Read(&h->name[0], name_len);

		file.Read(&h->translation, sizeof(Vector3f));
		file.Read(&h->rotation, sizeof(Vector3f));
		file.Read(&h->scale, sizeof(Vector3f));

		file.Read(&h->Transformation, sizeof(glm::mat4));

		size_t mesh_ind_size;

		file.Read(&mesh_ind_size, sizeof(size_t));

		if (mesh_ind_size > 0) {
			h->meshIndexes.resize(mesh_ind_size);
			file.Read(&h->meshIndexes[0], mesh_ind_size);
		}

		size_t child_size;

		file.Read(&child_size, sizeof(size_t));

		if (child_size > 0) {
			h->children.reserve(child_size);

			for (size_t i = 0; i < child_size; i++) {
				h->children.push_back(LoadModelHierarchy(file));
			}
		}

		return h;
	}

	//
	//void Model::ReadNodeHeirarchy(float AnimationTime, ModelHierarchy* node, glm::mat4 parentTransform)
	//{
	//	std::string NodeName(node->name.data());
	//	//SetCurrent anim
	//	const Animation* animation = parent->animations[0];

	//	glm::mat4 nodeTransformation(node->Transformation);

	//	const AnimNode* pNodeAnim = FindNodeAnim(animation, NodeName);

	//	if (pNodeAnim) {
	//		
	//		// Interpolate scaling and generate scaling transformation matrix
	//		glm::vec3 Scaling;
	//		CalcInterpolatedScaling(Scaling, AnimationTime, pNodeAnim);
	//		glm::mat4 scalingM = glm::scale(glm::mat4(1.0f), Scaling);

	//		//Interpolate rotation and generate rotation transformation matrix
	//		glm::quat RotationQ;
	//		CalcInterpolatedRotation(RotationQ, AnimationTime, pNodeAnim);
	//		glm::mat4 rotationM = glm::mat4_cast(RotationQ);

	//		// Interpolate translation and generate translation transformation matrix
	//		glm::vec3 Translation;
	//		CalcInterpolatedPosition(Translation, AnimationTime, pNodeAnim);
	//		glm::mat4 translationM = glm::translate(glm::mat4(1.0f), Translation);

	//		// Combine the above transformations
	//		nodeTransformation = translationM * rotationM * scalingM;
	//	}

	//	glm::mat4 GlobalTransformation = parentTransform * nodeTransformation;

	//	if (parent->boneNameToIndexMap.find(NodeName) != parent->boneNameToIndexMap.end()) {
	//		unsigned int BoneIndex = parent->boneNameToIndexMap[NodeName];
	//		parent->boneInfo[BoneIndex].finalTransformation = parent->globalInverseTransform * GlobalTransformation * parent->boneInfo[BoneIndex].offsetMatrix;

	//	}

	//	for (unsigned int i = 0; i < node->children.size(); i++) {
	//		
	//		ReadNodeHeirarchy(AnimationTime, node->children[i], GlobalTransformation);
	//	}
	//}

	//const AnimNode* Model::FindNodeAnim(const Animation* animation, const std::string& NodeName)
	//{
	//	for (unsigned int  i = 0; i < animation->numChannels; i++) {
	//		const AnimNode* nodeAnim = animation->channels[i];

	//		if (std::string(nodeAnim->name.data()) == NodeName) {
	//			return nodeAnim;
	//		}
	//	}

	//	return NULL;
	//}

	//unsigned int Model::FindScaling(float AnimationTime, const AnimNode* NodeAnim)
	//{
	//	assert(NodeAnim->numScalingKeys > 0);

	//	for (unsigned int i = 0; i < NodeAnim->numScalingKeys - 1; i++) {
	//		float t = (float)NodeAnim->scalingKeys[(i + 1)].time;
	//		if (AnimationTime < t) {
	//			return i;
	//		}
	//	}

	//	return 0;
	//}

	//unsigned int Model::FindRotation(float AnimationTime, const AnimNode* NodeAnim)
	//{
	//	assert(NodeAnim->numRotationKeys > 0);

	//	for (unsigned int i = 0; i < NodeAnim->numRotationKeys - 1; i++) {
	//		float t = (float)NodeAnim->rotationKeys[(i + 1)].time;
	//		if (AnimationTime < t) {
	//			return i;
	//		}
	//	}

	//	return 0;
	//}

	//unsigned int Model::FindPosition(float AnimationTime, const AnimNode* NodeAnim)
	//{
	//	for (unsigned int i = 0; i < NodeAnim->numPositionKeys - 1; i++) {
	//		float t = (float)NodeAnim->positionKeys[(i + 1)].time;
	//		if (AnimationTime < t) {
	//			return i;
	//		}
	//	}

	//	return 0;
	//}

	//void Model::CalcInterpolatedScaling(glm::vec3& Out, float AnimationTime, const AnimNode* NodeAnim)
	//{
	//	if (NodeAnim->numScalingKeys == 1) {
	//		Out = NodeAnim->scalingKeys[0].value;
	//		return;
	//	}

	//	unsigned int ScalingIndex = FindScaling(AnimationTime, NodeAnim);
	//	unsigned int NextScalingIndex = (ScalingIndex + 1);
	//	assert(NextScalingIndex < NodeAnim->numScalingKeys);
	//	float DeltaTime = (float)(NodeAnim->scalingKeys[NextScalingIndex].time - NodeAnim->scalingKeys[ScalingIndex].time);
	//	float Factor = (AnimationTime - (float)NodeAnim->scalingKeys[ScalingIndex].time) / DeltaTime;
	//	assert(Factor >= 0.0f && Factor <= 1.0f);
	//	const glm::vec3& Start = NodeAnim->scalingKeys[ScalingIndex].value;
	//	const glm::vec3& End = NodeAnim->scalingKeys[NextScalingIndex].value;
	//	glm::vec3 Delta = End - Start;
	//	Out = Start + Factor * Delta;
	//}

	//void Model::CalcInterpolatedRotation(glm::quat& Out, float AnimationTime, const AnimNode* NodeAnim)
	//{
	//	// we need at least two values to interpolate...
	//	if (NodeAnim->numRotationKeys == 1) {
	//		Out = NodeAnim->rotationKeys[0].value;
	//		return;
	//	}

	//	unsigned int RotationIndex = FindRotation(AnimationTime, NodeAnim);
	//	unsigned int NextRotationIndex = (RotationIndex + 1);
	//	assert(NextRotationIndex < NodeAnim->numRotationKeys);
	//	float DeltaTime = (float)(NodeAnim->rotationKeys[NextRotationIndex].time - NodeAnim->rotationKeys[RotationIndex].time);
	//	float Factor = (AnimationTime - (float)NodeAnim->rotationKeys[RotationIndex].time) / DeltaTime;
	//	assert(Factor >= 0.0f && Factor <= 1.0f);
	//	const glm::quat& StartRotationQ = NodeAnim->rotationKeys[RotationIndex].value;
	//	const glm::quat& EndRotationQ = NodeAnim->rotationKeys[NextRotationIndex].value;
	//	Out = glm::mix(StartRotationQ, EndRotationQ, Factor);
	//	Out = glm::normalize(Out);
	//}

	//void Model::CalcInterpolatedPosition(glm::vec3& Out, float AnimationTime, const AnimNode* NodeAnim)
	//{
	//	if (NodeAnim->numPositionKeys == 1) {
	//		Out = NodeAnim->positionKeys[0].value;
	//		return;
	//	}

	//	unsigned int PositionIndex = FindPosition(AnimationTime, NodeAnim);
	//	unsigned int NextPositionIndex = (PositionIndex + 1);
	//	assert(NextPositionIndex < NodeAnim->numPositionKeys);
	//	float DeltaTime = (float)(NodeAnim->positionKeys[NextPositionIndex].time - NodeAnim->positionKeys[PositionIndex].time);
	//	float Factor = (AnimationTime - (float)NodeAnim->positionKeys[PositionIndex].time) / DeltaTime;
	//	assert(Factor >= 0.0f && Factor <= 1.0f);
	//	const glm::vec3& Start = NodeAnim->positionKeys[PositionIndex].value;
	//	const glm::vec3& End = NodeAnim->positionKeys[NextPositionIndex].value;
	//	glm::vec3 Delta = End - Start;
	//	Out = Start + Factor * Delta;
	//}

	void Model::generateBuffers()
	{
		if (is_root) return;
		WI_CORE_INFO("Generating buffers...");
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);
		glGenVertexArrays(1, &vao);
		//WI_CORE_INFO("Generating buffers DONE");

		if (glGetError() != 0)
		{
			WI_CORE_ERROR("Check error {0}", glewGetErrorString(glGetError()));
		}

		//WI_CORE_INFO("Binding the vertex array ...");
		glBindVertexArray(vao);
		//WI_CORE_INFO("Binding the vertex array DONE");

		if (glGetError() != 0)
		{
			WI_CORE_ERROR("Check error {0}", glewGetErrorString(glGetError()));
		}

		//WI_CORE_INFO("Binding the vertex buffer ...");
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, vbo_data.size() * sizeof(float), vbo_data.data(), GL_STATIC_DRAW);
		//WI_CORE_INFO("Binding the vertex buffer DONE");

		if (glGetError() != 0)
		{
			WI_CORE_ERROR("Check error {0}", glewGetErrorString(glGetError()));
		}

		//WI_CORE_INFO("Binding the index buffer ...");
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, ebo_data.size() * sizeof(int), ebo_data.data(), GL_STATIC_DRAW);

		if (glGetError() != 0)
		{
			WI_CORE_ERROR("Check error {0}", glewGetErrorString(glGetError()));
		}
		glEnableVertexAttribArray(POSITION_DATA);
		glVertexAttribPointer(POSITION_DATA, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(NORMAL_DATA);
		glVertexAttribPointer(NORMAL_DATA, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
		glEnableVertexAttribArray(TEXTURE_DATA);
		glVertexAttribPointer(TEXTURE_DATA, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));	

		if (glGetError() != 0)
		{
			WI_CORE_ERROR("Check error {0}", glewGetErrorString(glGetError()));
		}

		//if there are bones add bone vertex data
		if (!bone_data.empty())
		{
			glGenBuffers(1, &bonevb);
			glBindBuffer(GL_ARRAY_BUFFER, bonevb);
			glBufferData(GL_ARRAY_BUFFER, sizeof(VertexBoneData) * bone_data.size(), bone_data.data(), GL_STATIC_DRAW);
			
			if (glGetError() != 0)
			{
				WI_CORE_ERROR("Check error {0}", glewGetErrorString(glGetError()));
			}
			//bone location
			glEnableVertexAttribArray(BONE_DATA);
			glVertexAttribIPointer(BONE_DATA, MAX_NUM_BONES_PER_VERTEX, GL_INT, sizeof(VertexBoneData), (const GLvoid*)0);
			//weights location
			glEnableVertexAttribArray(WEIGHT_DATA);
			glVertexAttribPointer(WEIGHT_DATA, MAX_NUM_BONES_PER_VERTEX, GL_FLOAT, GL_FALSE, sizeof(VertexBoneData),(const GLvoid*)16);
			if (glGetError() != 0)
			{
				WI_CORE_ERROR("Check error {0}", glewGetErrorString(glGetError()));
			}
			if(bonevb == -1)
			{
				WI_CORE_ERROR("Error generating bone vertex buffer");
			}
		}

		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);

		if (glGetError() != 0)
		{
			WI_CORE_ERROR("Check error {0}", glewGetErrorString(glGetError()));
		}


		for (int i = 0; i < vbo_data.size(); i += 8)
		{
			glm::vec3 vec = { vbo_data[i], vbo_data[i + 1], vbo_data[i + 2] };
			boundingBox.extend(vec);
		}
		bbebo_data = {
			0, 1, 1, 2, 2, 3, 3, 0, // Front
			4, 5, 5, 6, 6, 7, 7, 4, // Back
			0, 4, 1, 5, 2, 6, 3, 7
		};
		bbvbo_data = {
			boundingBox.getMax().x, boundingBox.getMax().y, boundingBox.getMax().z, //TOP
			boundingBox.getMin().x, boundingBox.getMax().y, boundingBox.getMax().z,
			boundingBox.getMin().x, boundingBox.getMax().y, boundingBox.getMin().z,
			boundingBox.getMax().x, boundingBox.getMax().y, boundingBox.getMin().z,

			boundingBox.getMax().x, boundingBox.getMin().y, boundingBox.getMax().z, //Bottom
			boundingBox.getMin().x, boundingBox.getMin().y, boundingBox.getMax().z,
			boundingBox.getMin().x, boundingBox.getMin().y, boundingBox.getMin().z,
			boundingBox.getMax().x, boundingBox.getMin().y, boundingBox.getMin().z,
		};

		glGenBuffers(1, &bbvbo);
		glGenBuffers(1, &bbebo);
		glGenVertexArrays(1, &bbvao);

		glBindVertexArray(bbvao);
		glBindBuffer(GL_ARRAY_BUFFER, bbvbo);
		glBufferData(GL_ARRAY_BUFFER, bbvbo_data.size() * sizeof(float), bbvbo_data.data(), GL_STATIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bbebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, bbebo_data.size() * sizeof(int), bbebo_data.data(), GL_STATIC_DRAW);

		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
		glEnableVertexAttribArray(0);


		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindVertexArray(0);
	}

	Model::Model(const char* file)
		: ebo(0), vbo(0), vao(0)
	{
		is_root = true;
		model_hierarchy = NULL;

		if (file) {
			if (strcmp(file, "cube") == 0)
			{
				CreateCube();
			}
			else if (strcmp(file, "plane") == 0)
			{
				CreatePlane();
			}
			else if (strcmp(file, "pyramid") == 0)
			{
				CreatePyramid();
			}
			else if (strcmp(file, "sphere") == 0)
			{
				CreateSphere();
			}
			else
			{
				ModelSettings settings;
				getMeshFromFile(file, &settings);
			}
		}
		else {
			is_root = false;
		}
	}

	Model::~Model()
	{
		size_t m_size = models.size();

		for (size_t i = 0; i < m_size; i++) {
			delete models[i];
		}

		if (model_hierarchy) {
			delete model_hierarchy;
		}

		glDeleteBuffers(1, &vbo);
		glDeleteBuffers(1, &ebo);
		glDeleteBuffers(1, &bbvbo);
		glDeleteBuffers(1, &bbvbo);

		glDeleteVertexArrays(1, &vao);
		glDeleteVertexArrays(1, &bbvao);
	}

	void Model::Render()
	{
		if(is_root){
			size_t meshCount = models.size();

			for (size_t i = 0; i < meshCount; i++) {
				models[i]->Render();
			}
		}
		else {
			this;
			glBindVertexArray(vao);
			glDrawElements(GL_TRIANGLES, (GLsizei)ebo_data.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		
		}
	}

	void Model::DrawBoudingBox()
	{
		if (is_root) {
			size_t meshCount = models.size();

			for (size_t i = 0; i < meshCount; i++) {
				models[i]->DrawBoudingBox();
			}
		}
		else {
			glBindVertexArray(bbvao);
			glDrawElements(GL_LINES, (GLsizei)bbebo_data.size(), GL_UNSIGNED_INT, 0);
			glBindVertexArray(0);
		}
	}

	void Model::LoadMesh(const char* file, ModelSettings* settings)
	{
		getMeshFromFile(file, settings);
	}

	void Model::LoadWiMesh(const char* file)
	{
		getWiMeshFromFile(file);
	}

	void Model::GetBoneTransforms(float timeInSeconds, std::vector<glm::mat4>& transforms)
	{
		
	//	glm::mat4 identity (1.0f);

	//	//Set current anim
	//	float TicksPerSecond = (float)(parent->animations[0]->ticksPerSecond != 0 ? parent->animations[0]->ticksPerSecond : 25.0f);
	//	float TimeInTicks = timeInSeconds * TicksPerSecond;
	//	float AnimationTimeTicks = fmod(TimeInTicks, (float)parent->animations[0]->duration);
	////	parent->animationTime = AnimationTimeTicks;
	//	
	//	ReadNodeHeirarchy(AnimationTimeTicks, parent->model_hierarchy, identity);
	//	transforms.resize(parent->boneInfo.size());
	//	for (unsigned int i = 0; i < parent->boneInfo.size(); i++)
	//	{
	//		transforms[i] = parent->boneInfo[i].finalTransformation;
	//	}
	}

	void Model::LoadMeshBones(unsigned int meshIndex,const aiMesh* mesh)
	{
		for (int i = 0; i < mesh->mNumBones; i++)
		{
			LoadSingleBone(meshIndex, mesh->mBones[i]);
		}
	}

	void Model::LoadSingleBone(int meshIndex, aiBone* bone)
	{ 		
		int BoneId = -1;
		if (m_BoneInfoMap.find(bone->mName.C_Str()) == m_BoneInfoMap.end())
		{
			BoneInfo newBoneInfo;
			newBoneInfo.id = m_BoneCounter;
			glm::mat4 offset = mat4_cast(bone->mOffsetMatrix);
			newBoneInfo.offsetMatrix = mat4_cast(bone->mOffsetMatrix);
			parent->m_BoneInfoMap[bone->mName.C_Str()] = newBoneInfo;
			BoneId = m_BoneCounter;
			m_BoneCounter++;
		}
		else
		{
			BoneId = parent->m_BoneInfoMap[bone->mName.C_Str()].id;
		}
		/*int BoneId = GetBoneId(bone);
		if (BoneId == boneInfo.size()) {
			glm::mat4 offset = mat4_cast(bone->mOffsetMatrix);
			WI_INFO("bone id {}", BoneId);
			PrintGlmMatrix(offset);
			BoneInfo binfo(offset);
			binfo.id = BoneId;
			m_BoneInfoMap[](binfo);
			m_BoneCounter++;
		}*/

		for (unsigned int i = 0; i < bone->mNumWeights; i++) {
			const aiVertexWeight& vw = bone->mWeights[i];
			unsigned int GlobalVertexID = parent->meshes[meshIndex] + bone->mWeights[i].mVertexId;
			WI_INFO("vertex id {}", GlobalVertexID);
			bone_data[GlobalVertexID].AddBoneData(BoneId, vw.mWeight);
		}
	}

	void Model::PrintAssimpBoneMatrix(const aiBone* mat)
	{
		const aiMatrix4x4 m = mat->mOffsetMatrix;
		WI_INFO("{0} {1} {2} {3}\n", m.a1, m.a2, m.a3, m.a4);
		WI_INFO("{0} {1} {2} {3}\n", m.b1, m.b2, m.b3, m.b4);
		WI_INFO("{0} {1} {2} {3}\n", m.c1, m.c2, m.c3, m.c4);
		WI_INFO("{0} {1} {2} {3}\n", m.d1, m.d2, m.d3, m.d4);
	}
	void Model::PrintAssimpNodeMatrix(const aiNode* mat)
	{
		const aiMatrix4x4 m = mat->mTransformation;
		WI_INFO("{0} {1} {2} {3}\n", m.a1, m.a2, m.a3, m.a4);
		WI_INFO("{0} {1} {2} {3}\n", m.b1, m.b2, m.b3, m.b4);
		WI_INFO("{0} {1} {2} {3}\n", m.c1, m.c2, m.c3, m.c4);
		WI_INFO("{0} {1} {2} {3}\n", m.d1, m.d2, m.d3, m.d4);
	}
	void Model::PrintGlmMatrix(const glm::mat4& mat)
	{
		WI_INFO("{0}_{1}_{2}_{3}", mat[0][0], mat[0][1], mat[0][2], mat[0][3]);
		WI_INFO("{0}_{1}_{2}_{3}", mat[1][0], mat[1][1], mat[1][2], mat[1][3]);
		WI_INFO("{0}_{1}_{2}_{3}", mat[2][0], mat[2][1], mat[2][2], mat[2][3]);
		WI_INFO("{0}_{1}_{2}_{3}", mat[3][0], mat[3][1], mat[3][2], mat[3][3]);
		WI_INFO("\n");
	}

	//void Model::LoadAnimation(const aiAnimation* animation)
	//{
	//	Animation* anim = new Animation();

	//	anim->name = animation->mName.C_Str();
	//	anim->duration = animation->mDuration;
	//	anim->ticksPerSecond = animation->mTicksPerSecond;
	//	anim->numChannels = animation->mNumChannels;

	//	for (unsigned int i = 0; i < anim->numChannels; i++)
	//	{
	//		anim->channels.push_back(LoadAnimationNode(animation->mChannels[i]));
	//	}

	//	animations.push_back(anim);
	//}

	//AnimNode* Model::LoadAnimationNode(const aiNodeAnim* aiNode)
	//{
	//	AnimNode* node = new AnimNode();

	//	node->name = aiNode->mNodeName.C_Str();

	//	node->numPositionKeys = aiNode->mNumPositionKeys;
	//	node->numRotationKeys = aiNode->mNumRotationKeys;
	//	node->numScalingKeys = aiNode->mNumScalingKeys;


	//	for (unsigned int i = 0; i < node->numPositionKeys; i++)
	//	{
	//		VectorKey posKey;

	//		posKey.time = aiNode->mPositionKeys[i].mTime;
	//		posKey.value.x = aiNode->mPositionKeys[i].mValue.x;
	//		posKey.value.y = aiNode->mPositionKeys[i].mValue.y;
	//		posKey.value.z = aiNode->mPositionKeys[i].mValue.z;
	//		node->positionKeys.push_back(posKey);
	//	}
	//	for (unsigned int i = 0; i < node->numRotationKeys; i++)
	//	{
	//		QuatKey quatKey;

	//		quatKey.time = aiNode->mRotationKeys[i].mTime;
	//		quatKey.value.x = aiNode->mRotationKeys[i].mValue.x;
	//		quatKey.value.y = aiNode->mRotationKeys[i].mValue.y;
	//		quatKey.value.z = aiNode->mRotationKeys[i].mValue.z;
	//		quatKey.value.w = aiNode->mRotationKeys[i].mValue.w;
	//		node->rotationKeys.push_back(quatKey);
	//	}
	//	for (unsigned int i = 0; i < node->numScalingKeys; i++)
	//	{
	//		VectorKey scaleKey;

	//		scaleKey.time = aiNode->mScalingKeys[i].mTime;
	//		scaleKey.value.x = aiNode->mScalingKeys[i].mValue.x;
	//		scaleKey.value.y = aiNode->mScalingKeys[i].mValue.y;
	//		scaleKey.value.z = aiNode->mScalingKeys[i].mValue.z;
	//		node->scalingKeys.push_back(scaleKey);
	//	}

	//	return node;
	//}

	//AnimNode* Model::LoadWiAnimNode(File file)
	//{
	//	AnimNode* node = new AnimNode();

	//	size_t name_len;
	//	file.Read(&name_len, sizeof(size_t));
	//	node->name.resize(name_len);
	//	file.Read(&node->name[0], name_len);

	//	file.Read(&node->numPositionKeys, sizeof(unsigned int));
	//	file.Read(&node->numRotationKeys, sizeof(unsigned int));
	//	file.Read(&node->numScalingKeys, sizeof(unsigned int));

	//	// Read position keys
	//	size_t position_key_size;
	//	file.Read(&position_key_size, sizeof(size_t));
	//	node->positionKeys.resize(position_key_size);
	//	file.Read(&node->positionKeys[0], position_key_size* sizeof(VectorKey));
	//	// Read Rotation keys
	//	size_t rotation_key_size;
	//	file.Read(&rotation_key_size, sizeof(size_t));
	//	node->rotationKeys.resize(rotation_key_size);
	//	file.Read(&node->rotationKeys[0], rotation_key_size*sizeof(QuatKey));
	//	// Read scaling keys
	//	size_t scaling_key_size;
	//	file.Read(&scaling_key_size, sizeof(size_t));
	//	node->scalingKeys.resize(scaling_key_size);
	//	file.Read(&node->scalingKeys[0], scaling_key_size* sizeof(VectorKey));
	//	return node;
	//}

	//Animation* Model::LoadWiAnimation(File file)
	//{
	//	Animation* anim = new Animation();

	//	size_t name_len;
	//	file.Read(&name_len, sizeof(size_t));
	//	anim->name.resize(name_len);
	//	file.Read(&anim->name[0], name_len);

	//	file.Read(&anim->duration, sizeof(double));
	//	file.Read(&anim->ticksPerSecond, sizeof(double));
	//	file.Read(&anim->numChannels, sizeof(unsigned int));

	//	size_t channels_size;
	//	file.Read(&channels_size, sizeof(size_t));

	//	for (unsigned int i = 0; i < channels_size; i++)
	//	{
	//		anim->channels.push_back(LoadWiAnimNode(file));
	//	}


	//	return anim;
	//}

	//void Model::SaveWiAnimation(File& file, Animation* anim)
	//{
	//	size_t name_len = anim->name.size();

	//	file.Write(&name_len, sizeof(size_t));
	//	file.Write(anim->name.c_str(), name_len);

	//	file.Write(&anim->duration, sizeof(double));
	//	file.Write(&anim->ticksPerSecond, sizeof(double));
	//	file.Write(&anim->numChannels, sizeof(unsigned int));

	//	size_t channels_size = anim->channels.size();
	//	file.Write(&channels_size, sizeof(size_t));


	//	for (unsigned int i = 0; i < channels_size; i++)
	//	{
	//		SaveWiAnimNode(file, anim->channels[i]);
	//	}
	//}

	//void Model::SaveWiAnimNode(File& file, AnimNode* node)
	//{
	//	if (node == NULL) return;
	//	size_t name_len = node->name.size();
	//	file.Write(&name_len, sizeof(size_t));
	//	file.Write(node->name.c_str(), name_len);

	//	file.Write(&node->numPositionKeys, sizeof(unsigned int));
	//	file.Write(&node->numRotationKeys, sizeof(unsigned int));
	//	file.Write(&node->numScalingKeys, sizeof(unsigned int));


	//	//Position keys
	//	size_t position_size = node->positionKeys.size();
	//	file.Write(&position_size, sizeof(size_t));
	//	file.Write(node->positionKeys.data(), position_size * sizeof(VectorKey));
	//	//Rotation keys
	//	size_t rotation_size = node->rotationKeys.size();
	//	file.Write(&rotation_size, sizeof(size_t));
	//	file.Write(node->rotationKeys.data(), rotation_size * sizeof(QuatKey));
	//	//Scale keys
	//	size_t scale_size = node->scalingKeys.size();
	//	file.Write(&scale_size, sizeof(size_t));
	//	file.Write(node->scalingKeys.data(), scale_size * sizeof(VectorKey));
	//}
 //
	Model* Model::GetModelFromFile(const char* file, ModelSettings* settings)
	{
		Model* model = new Model(NULL);

		if (!model->getMeshFromFile(file, settings, false)) 
		{
			delete model;
			model = nullptr; 
			WI_ERROR("Error loading {0}", file); 
		}

		return model;
	}

	void Model::SaveModel(Model* model, const char* file)
	{
		File f = FileSystem::OpenOB(file);

		if (f.IsOpen()) {
			// Model size
			size_t model_size = model->models.size();
			f.Write(&model_size, sizeof(size_t));

			// Model hierarchy
			SaveModelHierarchy(f, model->model_hierarchy);
			
			//save global inverse matrix
			f.Write(&model->globalInverseTransform, sizeof(glm::mat4));

			//bone to index map			
			size_t bone_index_size = model->m_BoneInfoMap.size();
			f.Write(&bone_index_size, sizeof(size_t));
			std::map<std::string, BoneInfo>::iterator it;
			for (it = model->m_BoneInfoMap.begin(); it != model->m_BoneInfoMap.end();it++)
			{
				size_t name_size = it->first.size();
				f.Write(&name_size, sizeof(size_t));
				f.Write(it->first.c_str(), name_size);
				f.Write(&it->second.id, sizeof(unsigned int));
				f.Write(&it->second.offsetMatrix, sizeof(glm::mat4));
				f.Write(&it->second.finalTransformation, sizeof(glm::mat4));
			}

			//Model Bones info
			//size_t bones_info_size = model->boneInfo.size();
			//f.Write(&bones_info_size, sizeof(size_t));
			//f.Write(model->boneInfo.data(), bones_info_size * sizeof(BoneInfo));

			//Animations
			//size_t anim_size = model->animations.size();
			//f.Write(&anim_size, sizeof(size_t));		
			//for (size_t i = 0; i < anim_size; i++)
			//{
			//	SaveWiAnimation(f,model->animations[i]);
			//}

			// Material size
			size_t mat_size = model->materials.size();

			f.Write(&mat_size, sizeof(size_t));
			for (size_t i = 0; i < mat_size; i++) {
				size_t name_size = model->materials[i].size();

				f.Write(&name_size, sizeof(size_t));
				f.Write(model->materials[i].c_str(), name_size);
			}

			// Model list
			for (size_t i = 0; i < model_size; i++) {
				Model* c_model = model->models[i];

				// Material index
				size_t mat_ind = c_model->getMaterialIndex();
				f.Write(&mat_ind, sizeof(size_t));

				// Model vbo
				size_t vbo_size = c_model->vbo_data.size();
				f.Write(&vbo_size, sizeof(size_t));
				f.Write(c_model->vbo_data.data(), vbo_size * sizeof(float));

				// Model ebo
				size_t ebo_size = c_model->ebo_data.size();
				f.Write(&ebo_size, sizeof(size_t));
				f.Write(c_model->ebo_data.data(), ebo_size * sizeof(int));

				//Model bones
				size_t bones_size = c_model->bone_data.size();
				f.Write(&bones_size, sizeof(size_t));
				f.Write(c_model->bone_data.data(), bones_size * sizeof(VertexBoneData));


			}

		}

		f.Close();
	}
}