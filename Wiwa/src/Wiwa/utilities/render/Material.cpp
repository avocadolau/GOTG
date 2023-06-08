#include "wipch.h"
#include <Wiwa/core/Resources.h>

#include <Wiwa/utilities/json/JSONDocument.h>
#include <Wiwa/utilities/render/shaders/Shader.h>
#include <Wiwa/utilities/render/shaders/Uniform.h>
#include <Wiwa/utilities/render/shaders/Shader.h>

#include "rapidjson.h"
#include "glew.h"

namespace Wiwa {
	Material::Material()
	{

	}

	Material::Material(Shader* shader)
	{
		if (!shader)
		{
			size_t id = Wiwa::Resources::Load<Shader>("resources/shaders/model_color");
			setShader(Resources::GetResourceById<Shader>(id), "resources/shaders/model_color");
		}
		else
			setShader(shader, shader->getPath());

	}

	Material::Material(const char* file)
	{
		Load(file);
	}

	Material::~Material()
	{
		if (m_Shader)
			m_Shader->DeleteRef(this);
		m_Uniforms.clear();
	}

	void Material::Load(const char* path)
	{
		JSONDocument matFile(path);
		if (!matFile.IsObject())
			return;
		if (matFile.HasMember("shader"))
			m_ShaderPath = matFile["shader"].as_string();

		size_t shaderId = Resources::Load<Shader>(m_ShaderPath.c_str());
		Shader* shader = Resources::GetResourceById<Shader>(shaderId);


		setShader(shader, m_ShaderPath.c_str());

		if (matFile.HasMember("uniforms"))
		{
			JSONValue uniforms = matFile["uniforms"];
			rapidjson::Value* val = uniforms.getValue();
			for (rapidjson::Value::MemberIterator p = val->MemberBegin(); p != val->MemberEnd(); ++p)
			{
				Uniform* uniform = getUniform(p->name.GetString());
				if (!uniform)
				{
					continue;
				}
				const char* name = uniform->name.c_str();
				switch (uniform->getType())
				{
				case Wiwa::UniformType::Bool:
					uniform->setData(p->value.GetBool(), uniform->getType());
					break;
				case Wiwa::UniformType::Int:
					uniform->setData(p->value.GetInt(), uniform->getType());
					break;
				case Wiwa::UniformType::Uint:
					uniform->setData(p->value.GetUint(), uniform->getType());
					break;
				case Wiwa::UniformType::Float:
					uniform->setData(p->value.GetFloat(), uniform->getType());
					break;
				case Wiwa::UniformType::fVec2:
				{
					glm::vec2 data;
					data.x = p->value["x"].GetFloat();
					data.y = p->value["y"].GetFloat();
					uniform->setData(data, uniform->getType());
				}break;
				case Wiwa::UniformType::fVec3:
				{
					glm::vec3 data;
					data.x = p->value["x"].GetFloat();
					data.y = p->value["y"].GetFloat();
					data.z = p->value["z"].GetFloat();
					uniform->setData(data, uniform->getType());
				}break;
				case Wiwa::UniformType::fVec4:
				{
					glm::vec4 data;
					data.x = p->value["r"].GetFloat();
					data.y = p->value["g"].GetFloat();
					data.z = p->value["b"].GetFloat();
					data.w = p->value["a"].GetFloat();
					uniform->setData(data, uniform->getType());
				}break;
				/*	case Wiwa::UniformType::Mat2:
					{
						Wiwa::JSONValue mat = uniforms.AddMemberObject(name));

						Wiwa::JSONValue vec1 = mat.AddMemberObject("vec1");
						vec1.AddMember("x", uniform->getData<glm::mat2>()[0].x);
						vec1.AddMember("y", uniform->getData<glm::mat2>()[0].y);

						Wiwa::JSONValue vec2 = mat.AddMemberObject("vec2");
						vec2.AddMember("x", uniform->getData<glm::mat2>()[1].x);
						vec2.AddMember("y", uniform->getData<glm::mat2>()[1].y);
					}break;
					case Wiwa::UniformType::Mat3:
					{
						Wiwa::JSONValue mat = uniforms.AddMemberObject(name);

						Wiwa::JSONValue vec1 = mat.AddMemberObject("vec1");
						vec1.AddMember("x", uniform->getData<glm::mat3>()[0].x);
						vec1.AddMember("y", uniform->getData<glm::mat3>()[0].y);
						vec1.AddMember("z", uniform->getData<glm::mat3>()[0].z);

						Wiwa::JSONValue vec2 = mat.AddMemberObject("vec2");
						vec2.AddMember("x", uniform->getData<glm::mat3>()[1].x);
						vec2.AddMember("y", uniform->getData<glm::mat3>()[1].y);
						vec2.AddMember("z", uniform->getData<glm::mat3>()[1].z);

						Wiwa::JSONValue vec3 = mat.AddMemberObject("vec3");
						vec3.AddMember("x", uniform.getData<glm::mat3>()[2].x);
						vec3.AddMember("y", uniform.getData<glm::mat3>()[2].y);
						vec3.AddMember("z", uniform.getData<glm::mat3>()[2].z);
					}break;
					case Wiwa::UniformType::Mat4:
					{
						Wiwa::JSONValue mat = uniforms.AddMemberObject(name);

						Wiwa::JSONValue vec1 = mat.AddMemberObject("vec1");
						vec1.AddMember("x", uniform->getData<glm::mat4>()[0].x);
						vec1.AddMember("y", uniform->getData<glm::mat4>()[0].y);
						vec1.AddMember("z", uniform->getData<glm::mat4>()[0].z);
						vec1.AddMember("w", uniform->getData<glm::mat4>()[0].w);

						Wiwa::JSONValue vec2 = mat.AddMemberObject("vec2");
						vec2.AddMember("x", uniform->getData<glm::mat4>()[1].x);
						vec2.AddMember("y", uniform->getData<glm::mat4>()[1].y);
						vec2.AddMember("z", uniform->getData<glm::mat4>()[1].z);
						vec2.AddMember("w", uniform->getData<glm::mat4>()[1].w);

						Wiwa::JSONValue vec3 = mat.AddMemberObject("vec3");
						vec3.AddMember("x", uniform->getData<glm::mat4>()[2].x);
						vec3.AddMember("y", uniform->getData<glm::mat4>()[2].y);
						vec3.AddMember("z", uniform->getData<glm::mat4>()[2].z);
						vec3.AddMember("w", uniform->getData<glm::mat4>()[2].w);

						Wiwa::JSONValue vec4 = mat.AddMemberObject("vec4");
						vec4.AddMember("x", uniform->getData<glm::mat4>()[2].x);
						vec4.AddMember("y", uniform->getData<glm::mat4>()[2].y);
						vec4.AddMember("z", uniform->getData<glm::mat4>()[2].z);
						vec4.AddMember("w", uniform->getData<glm::mat4>()[2].w);
					}break;*/
				case Wiwa::UniformType::Time:
				{

				}break;
				case Wiwa::UniformType::Sampler2D:
				{
					Uniform::SamplerData sdata;
					std::string path = p->value.GetString();
					memcpy(sdata.tex_path, &path[0], path.size() + 1);
					if (sdata.tex_path != "") {
						sdata.resource_id = Resources::Load<Image>(sdata.tex_path);

						Image* img = Resources::GetResourceById<Image>(sdata.resource_id);
						sdata.tex_id = img->GetTextureId();
						
						uniform->setData(sdata, uniform->getType());
						
					}
					else
					{
						sdata.tex_id = -1;
						sdata.resource_id = -1;
						uniform->setData(sdata, uniform->getType());
					}
				}break;
				default:
					break;
				}
			}
		}
		m_MaterialPath = path;
	}

	void Material::Save(const char* path)
	{
		std::string file = path;
		Resources::SetAssetPath(file);
		Wiwa::JSONDocument doc;
		Wiwa::Resources::standarizePath(m_ShaderPath);
		doc.AddMember("shader", m_ShaderPath.c_str());
		Wiwa::JSONValue uniforms = doc.AddMemberObject("uniforms");
		for (Wiwa::Uniform uniform : m_Uniforms)
		{
			const char* name = uniform.name.c_str();
			switch (uniform.getType())
			{
			case Wiwa::UniformType::Bool:
				uniforms.AddMember(name, uniform.getData<bool>());
				break;
			case Wiwa::UniformType::Int:
				uniforms.AddMember(name, uniform.getData<int>());
				break;
			case Wiwa::UniformType::Uint:
				uniforms.AddMember(name, uniform.getData<unsigned int>());
				break;
			case Wiwa::UniformType::Float:
				uniforms.AddMember(name, uniform.getData<float>());
				break;
			case Wiwa::UniformType::fVec2:
			{
				Wiwa::JSONValue vec = uniforms.AddMemberObject(name);
				vec.AddMember("x", uniform.getData<glm::vec2>().x);
				vec.AddMember("y", uniform.getData<glm::vec2>().y);
			}break;
			case Wiwa::UniformType::fVec3:
			{
				Wiwa::JSONValue vec = uniforms.AddMemberObject(name);
				vec.AddMember("x", uniform.getData<glm::vec3>().x);
				vec.AddMember("y", uniform.getData<glm::vec3>().y);
				vec.AddMember("z", uniform.getData<glm::vec3>().z);
			}break;
			case Wiwa::UniformType::fVec4:
			{
				Wiwa::JSONValue vec = uniforms.AddMemberObject(name);
				vec.AddMember("r", uniform.getData<glm::vec4>().r);
				vec.AddMember("g", uniform.getData<glm::vec4>().g);
				vec.AddMember("b", uniform.getData<glm::vec4>().b);
				vec.AddMember("a", uniform.getData<glm::vec4>().a);
			}break;
			case Wiwa::UniformType::Mat2:
			{
				Wiwa::JSONValue mat = uniforms.AddMemberObject(name);

				Wiwa::JSONValue vec1 = mat.AddMemberObject("vec1");
				vec1.AddMember("x", uniform.getData<glm::mat2>()[0].x);
				vec1.AddMember("y", uniform.getData<glm::mat2>()[0].y);

				Wiwa::JSONValue vec2 = mat.AddMemberObject("vec2");
				vec2.AddMember("x", uniform.getData<glm::mat2>()[1].x);
				vec2.AddMember("y", uniform.getData<glm::mat2>()[1].y);
			}break;
			case Wiwa::UniformType::Mat3:
			{
				Wiwa::JSONValue mat = uniforms.AddMemberObject(name);

				Wiwa::JSONValue vec1 = mat.AddMemberObject("vec1");
				vec1.AddMember("x", uniform.getData<glm::mat3>()[0].x);
				vec1.AddMember("y", uniform.getData<glm::mat3>()[0].y);
				vec1.AddMember("z", uniform.getData<glm::mat3>()[0].z);

				Wiwa::JSONValue vec2 = mat.AddMemberObject("vec2");
				vec2.AddMember("x", uniform.getData<glm::mat3>()[1].x);
				vec2.AddMember("y", uniform.getData<glm::mat3>()[1].y);
				vec2.AddMember("z", uniform.getData<glm::mat3>()[1].z);

				Wiwa::JSONValue vec3 = mat.AddMemberObject("vec3");
				vec3.AddMember("x", uniform.getData<glm::mat3>()[2].x);
				vec3.AddMember("y", uniform.getData<glm::mat3>()[2].y);
				vec3.AddMember("z", uniform.getData<glm::mat3>()[2].z);
			}break;
			case Wiwa::UniformType::Mat4:
			{
				Wiwa::JSONValue mat = uniforms.AddMemberObject(name);

				Wiwa::JSONValue vec1 = mat.AddMemberObject("vec1");
				vec1.AddMember("x", uniform.getData<glm::mat4>()[0].x);
				vec1.AddMember("y", uniform.getData<glm::mat4>()[0].y);
				vec1.AddMember("z", uniform.getData<glm::mat4>()[0].z);
				vec1.AddMember("w", uniform.getData<glm::mat4>()[0].w);

				Wiwa::JSONValue vec2 = mat.AddMemberObject("vec2");
				vec2.AddMember("x", uniform.getData<glm::mat4>()[1].x);
				vec2.AddMember("y", uniform.getData<glm::mat4>()[1].y);
				vec2.AddMember("z", uniform.getData<glm::mat4>()[1].z);
				vec2.AddMember("w", uniform.getData<glm::mat4>()[1].w);

				Wiwa::JSONValue vec3 = mat.AddMemberObject("vec3");
				vec3.AddMember("x", uniform.getData<glm::mat4>()[2].x);
				vec3.AddMember("y", uniform.getData<glm::mat4>()[2].y);
				vec3.AddMember("z", uniform.getData<glm::mat4>()[2].z);
				vec3.AddMember("w", uniform.getData<glm::mat4>()[2].w);

				Wiwa::JSONValue vec4 = mat.AddMemberObject("vec4");
				vec4.AddMember("x", uniform.getData<glm::mat4>()[2].x);
				vec4.AddMember("y", uniform.getData<glm::mat4>()[2].y);
				vec4.AddMember("z", uniform.getData<glm::mat4>()[2].z);
				vec4.AddMember("w", uniform.getData<glm::mat4>()[2].w);
			}break;
			case Wiwa::UniformType::Time:

				break;
			case Wiwa::UniformType::Sampler2D:
			{
				Uniform::SamplerData* sdata = uniform.getPtrData<Uniform::SamplerData>();

				std::string string = sdata->tex_path;
				Wiwa::Resources::standarizePath(string);
				uniforms.AddMember(name, string.c_str());
			}break;
			default:
				break;
			}
		}
		doc.save_file(file.c_str());
		m_MaterialPath = path;
		Resources::Import<Material>(file.c_str());
	}

	void Material::SaveLib(const char* path)
	{
		std::string file = path;

		Wiwa::JSONDocument doc;
		Wiwa::Resources::standarizePath(m_ShaderPath);
		doc.AddMember("shader", m_ShaderPath.c_str());
		Wiwa::JSONValue uniforms = doc.AddMemberObject("uniforms");
		for (Wiwa::Uniform uniform : m_Uniforms)
		{
			const char* name = uniform.name.c_str();
			switch (uniform.getType())
			{
			case Wiwa::UniformType::Bool:
				uniforms.AddMember(name, uniform.getData<bool>());
				break;
			case Wiwa::UniformType::Int:
				uniforms.AddMember(name, uniform.getData<int>());
				break;
			case Wiwa::UniformType::Uint:
				uniforms.AddMember(name, uniform.getData<unsigned int>());
				break;
			case Wiwa::UniformType::Float:
				uniforms.AddMember(name, uniform.getData<float>());
				break;
			case Wiwa::UniformType::fVec2:
			{
				Wiwa::JSONValue vec = uniforms.AddMemberObject(name);
				vec.AddMember("x", uniform.getData<glm::vec2>().x);
				vec.AddMember("y", uniform.getData<glm::vec2>().y);
			}break;
			case Wiwa::UniformType::fVec3:
			{
				Wiwa::JSONValue vec = uniforms.AddMemberObject(name);
				vec.AddMember("x", uniform.getData<glm::vec3>().x);
				vec.AddMember("y", uniform.getData<glm::vec3>().y);
				vec.AddMember("z", uniform.getData<glm::vec3>().z);
			}break;
			case Wiwa::UniformType::fVec4:
			{
				Wiwa::JSONValue vec = uniforms.AddMemberObject(name);
				vec.AddMember("r", uniform.getData<glm::vec4>().r);
				vec.AddMember("g", uniform.getData<glm::vec4>().g);
				vec.AddMember("b", uniform.getData<glm::vec4>().b);
				vec.AddMember("a", uniform.getData<glm::vec4>().a);
			}break;
			case Wiwa::UniformType::Mat2:
			{
				Wiwa::JSONValue mat = uniforms.AddMemberObject(name);

				Wiwa::JSONValue vec1 = mat.AddMemberObject("vec1");
				vec1.AddMember("x", uniform.getData<glm::mat2>()[0].x);
				vec1.AddMember("y", uniform.getData<glm::mat2>()[0].y);

				Wiwa::JSONValue vec2 = mat.AddMemberObject("vec2");
				vec2.AddMember("x", uniform.getData<glm::mat2>()[1].x);
				vec2.AddMember("y", uniform.getData<glm::mat2>()[1].y);
			}break;
			case Wiwa::UniformType::Mat3:
			{
				Wiwa::JSONValue mat = uniforms.AddMemberObject(name);

				Wiwa::JSONValue vec1 = mat.AddMemberObject("vec1");
				vec1.AddMember("x", uniform.getData<glm::mat3>()[0].x);
				vec1.AddMember("y", uniform.getData<glm::mat3>()[0].y);
				vec1.AddMember("z", uniform.getData<glm::mat3>()[0].z);

				Wiwa::JSONValue vec2 = mat.AddMemberObject("vec2");
				vec2.AddMember("x", uniform.getData<glm::mat3>()[1].x);
				vec2.AddMember("y", uniform.getData<glm::mat3>()[1].y);
				vec2.AddMember("z", uniform.getData<glm::mat3>()[1].z);

				Wiwa::JSONValue vec3 = mat.AddMemberObject("vec3");
				vec3.AddMember("x", uniform.getData<glm::mat3>()[2].x);
				vec3.AddMember("y", uniform.getData<glm::mat3>()[2].y);
				vec3.AddMember("z", uniform.getData<glm::mat3>()[2].z);
			}break;
			case Wiwa::UniformType::Mat4:
			{
				Wiwa::JSONValue mat = uniforms.AddMemberObject(name);

				Wiwa::JSONValue vec1 = mat.AddMemberObject("vec1");
				vec1.AddMember("x", uniform.getData<glm::mat4>()[0].x);
				vec1.AddMember("y", uniform.getData<glm::mat4>()[0].y);
				vec1.AddMember("z", uniform.getData<glm::mat4>()[0].z);
				vec1.AddMember("w", uniform.getData<glm::mat4>()[0].w);

				Wiwa::JSONValue vec2 = mat.AddMemberObject("vec2");
				vec2.AddMember("x", uniform.getData<glm::mat4>()[1].x);
				vec2.AddMember("y", uniform.getData<glm::mat4>()[1].y);
				vec2.AddMember("z", uniform.getData<glm::mat4>()[1].z);
				vec2.AddMember("w", uniform.getData<glm::mat4>()[1].w);

				Wiwa::JSONValue vec3 = mat.AddMemberObject("vec3");
				vec3.AddMember("x", uniform.getData<glm::mat4>()[2].x);
				vec3.AddMember("y", uniform.getData<glm::mat4>()[2].y);
				vec3.AddMember("z", uniform.getData<glm::mat4>()[2].z);
				vec3.AddMember("w", uniform.getData<glm::mat4>()[2].w);

				Wiwa::JSONValue vec4 = mat.AddMemberObject("vec4");
				vec4.AddMember("x", uniform.getData<glm::mat4>()[2].x);
				vec4.AddMember("y", uniform.getData<glm::mat4>()[2].y);
				vec4.AddMember("z", uniform.getData<glm::mat4>()[2].z);
				vec4.AddMember("w", uniform.getData<glm::mat4>()[2].w);
			}break;
			case Wiwa::UniformType::Time:

				break;
			case Wiwa::UniformType::Sampler2D:
			{
				Uniform::SamplerData* sdata = uniform.getPtrData<Uniform::SamplerData>();

				std::string string = sdata->tex_path;
				Wiwa::Resources::standarizePath(string);
				uniforms.AddMember(name, string.c_str());
			}break;
			default:
				break;
			}
		}
		doc.save_file(file.c_str());
		m_MaterialPath = file;
	}

	void Material::SaveMaterial(const char* path, Material* mat)
	{
		mat->Save(path);
	}

	Material* Material::LoadMaterial(const char* path)
	{
		return new Material(path);
	}

	Material* Material::LoadMaterialData(const char* path)
	{
		Material* mat = new Material((Shader*)0);

		JSONDocument matFile(path);
		if (!matFile.IsObject())
			return nullptr;
		if (matFile.HasMember("shader")) {
			std::string shader_path = matFile["shader"].get<const char*>();

			size_t shaderId = Resources::Load<Shader>(shader_path.c_str());
			Shader* shader = Resources::GetResourceById<Shader>(shaderId);

			mat->setShader(shader, shader_path.c_str());
		}

		if (matFile.HasMember("uniforms"))
		{
			JSONValue uniforms = matFile["uniforms"];
			rapidjson::Value* val = uniforms.getValue();
			for (rapidjson::Value::MemberIterator p = val->MemberBegin(); p != val->MemberEnd(); ++p)
			{
				Uniform* uniform = mat->getUniform(p->name.GetString());
				if (!uniform)
				{
					continue;
				}
				const char* name = uniform->name.c_str();
				switch (uniform->getType())
				{
				case Wiwa::UniformType::Bool:
					uniform->setData(p->value.GetBool(), uniform->getType());
					break;
				case Wiwa::UniformType::Int:
					uniform->setData(p->value.GetInt(), uniform->getType());
					break;
				case Wiwa::UniformType::Uint:
					uniform->setData(p->value.GetUint(), uniform->getType());
					break;
				case Wiwa::UniformType::Float:
					uniform->setData(p->value.GetFloat(), uniform->getType());
					break;
				case Wiwa::UniformType::fVec2:
				{
					glm::vec2 data;
					data.x = p->value["x"].GetFloat();
					data.y = p->value["y"].GetFloat();
					uniform->setData(data, uniform->getType());
				}break;
				case Wiwa::UniformType::fVec3:
				{
					glm::vec3 data;
					data.x = p->value["x"].GetFloat();
					data.y = p->value["y"].GetFloat();
					data.z = p->value["z"].GetFloat();
					uniform->setData(data, uniform->getType());
				}break;
				case Wiwa::UniformType::fVec4:
				{
					glm::vec4 data;
					data.x = p->value["r"].GetFloat();
					data.y = p->value["g"].GetFloat();
					data.z = p->value["b"].GetFloat();
					data.w = p->value["a"].GetFloat();
					uniform->setData(data, uniform->getType());
				}break;
				/*	case Wiwa::UniformType::Mat2:
					{
						Wiwa::JSONValue mat = uniforms.AddMemberObject(name));

						Wiwa::JSONValue vec1 = mat.AddMemberObject("vec1");
						vec1.AddMember("x", uniform->getData<glm::mat2>()[0].x);
						vec1.AddMember("y", uniform->getData<glm::mat2>()[0].y);

						Wiwa::JSONValue vec2 = mat.AddMemberObject("vec2");
						vec2.AddMember("x", uniform->getData<glm::mat2>()[1].x);
						vec2.AddMember("y", uniform->getData<glm::mat2>()[1].y);
					}break;
					case Wiwa::UniformType::Mat3:
					{
						Wiwa::JSONValue mat = uniforms.AddMemberObject(name);

						Wiwa::JSONValue vec1 = mat.AddMemberObject("vec1");
						vec1.AddMember("x", uniform->getData<glm::mat3>()[0].x);
						vec1.AddMember("y", uniform->getData<glm::mat3>()[0].y);
						vec1.AddMember("z", uniform->getData<glm::mat3>()[0].z);

						Wiwa::JSONValue vec2 = mat.AddMemberObject("vec2");
						vec2.AddMember("x", uniform->getData<glm::mat3>()[1].x);
						vec2.AddMember("y", uniform->getData<glm::mat3>()[1].y);
						vec2.AddMember("z", uniform->getData<glm::mat3>()[1].z);

						Wiwa::JSONValue vec3 = mat.AddMemberObject("vec3");
						vec3.AddMember("x", uniform.getData<glm::mat3>()[2].x);
						vec3.AddMember("y", uniform.getData<glm::mat3>()[2].y);
						vec3.AddMember("z", uniform.getData<glm::mat3>()[2].z);
					}break;
					case Wiwa::UniformType::Mat4:
					{
						Wiwa::JSONValue mat = uniforms.AddMemberObject(name);

						Wiwa::JSONValue vec1 = mat.AddMemberObject("vec1");
						vec1.AddMember("x", uniform->getData<glm::mat4>()[0].x);
						vec1.AddMember("y", uniform->getData<glm::mat4>()[0].y);
						vec1.AddMember("z", uniform->getData<glm::mat4>()[0].z);
						vec1.AddMember("w", uniform->getData<glm::mat4>()[0].w);

						Wiwa::JSONValue vec2 = mat.AddMemberObject("vec2");
						vec2.AddMember("x", uniform->getData<glm::mat4>()[1].x);
						vec2.AddMember("y", uniform->getData<glm::mat4>()[1].y);
						vec2.AddMember("z", uniform->getData<glm::mat4>()[1].z);
						vec2.AddMember("w", uniform->getData<glm::mat4>()[1].w);

						Wiwa::JSONValue vec3 = mat.AddMemberObject("vec3");
						vec3.AddMember("x", uniform->getData<glm::mat4>()[2].x);
						vec3.AddMember("y", uniform->getData<glm::mat4>()[2].y);
						vec3.AddMember("z", uniform->getData<glm::mat4>()[2].z);
						vec3.AddMember("w", uniform->getData<glm::mat4>()[2].w);

						Wiwa::JSONValue vec4 = mat.AddMemberObject("vec4");
						vec4.AddMember("x", uniform->getData<glm::mat4>()[2].x);
						vec4.AddMember("y", uniform->getData<glm::mat4>()[2].y);
						vec4.AddMember("z", uniform->getData<glm::mat4>()[2].z);
						vec4.AddMember("w", uniform->getData<glm::mat4>()[2].w);
					}break;*/
				case Wiwa::UniformType::Time:
				{

				}break;
				case Wiwa::UniformType::Sampler2D:
				{
					Uniform::SamplerData sdata;
					std::string path = p->value.GetString();
					memcpy(sdata.tex_path, &path[0], path.size() + 1);

					uniform->setData(sdata, uniform->getType());
				}break;
				default:
					break;
				}
			}
		}
		mat->m_MaterialPath = path;

		return mat;
	}
	void Material::Refresh()
	{
		std::vector<UniformField>& fields = m_Shader->getUniforms();
		for (UniformField& field : fields)
		{

			Uniform* uniform = getUniform(field.name.c_str());
			if (uniform)
			{
				if (uniform->getType() != field.type)
				{
					m_Uniforms.erase(m_Uniforms.begin() + getUniformIndex(uniform->name.c_str()));
					m_Uniforms.emplace_back(field.name, field.type);
				}
			}
			else
			{
				m_Uniforms.emplace_back(field.name, field.type);
			}
		}

		for (size_t i = 0; i < m_Uniforms.size(); i++)
		{
			size_t j = 0;
			for (; j < fields.size(); j++)
			{
				if (m_Uniforms[i].name == fields[j].name
					&& m_Uniforms[i].getType() == fields[j].type)
					break;
			}
			if (j == fields.size())
			{
				m_Uniforms.erase(m_Uniforms.begin() + i);
				i--;
			}
		}
	}

	void Material::setShader(Shader* shader, const char* path)
	{
		m_Uniforms.clear();
		if (m_Shader)
			m_Shader->DeleteRef(this);

		m_Shader = shader;
		m_Shader->AddRef(this);
		m_ShaderPath = path;
		Refresh();
	}

	Wiwa::Uniform* Material::getUniform(const char* name)
	{
		for (size_t i = 0; i < m_Uniforms.size(); i++)
		{
			if (m_Uniforms[i].name == name)
			{
				return &m_Uniforms[i];
			}
		}
		return nullptr;
	}

	void Material::Bind(int id)
	{
		Bind(m_Shader->getID(), id);
	}

	void Material::Bind(uint32_t program_id, int id)
	{
		int texureId = id;
		for (uint32_t i = 0; i < m_Uniforms.size(); i++)
		{
			m_Uniforms[i].sendToShader(program_id, texureId);
		}
	}

	void Material::UnBind(int id)
	{
		int texureId = id;
		for (size_t i = 0; i < m_Uniforms.size(); i++)
		{
			if (m_Uniforms[i].getType() == UniformType::Sampler2D)
			{
				GL(ActiveTexture(texureId));
				GL(BindTexture(GL_TEXTURE_2D, 0));
				texureId++;
			}
		}

		m_Shader->UnBind();
	}

}