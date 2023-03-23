#include <wipch.h>
#include "resources_impl.h"

#include "../vendor/stb/stb_image.h"

// Resource implementations
#include "image_impl_inl.h"
#include "material_impl_inl.h"
#include "model_impl_inl.h"
#include "shader_impl_inl.h"
#include "animation_impl_in.h"

namespace Wiwa {
	std::vector<Resources::Resource*> Resources::m_Resources[Resources::WRT_LAST];

	Resources::Resources()
	{
	}

	void Resources::PushResource(ResourceType rt, const char* file, void* rsc, bool isNative)
	{
		Resource* resource = new Resource();
		resource->isNative = isNative;
		resource->filePath = file;
		resource->resource = rsc;

		m_Resources[rt].push_back(resource);

		std::string message = "Loaded resource \"" + resource->filePath + "\" successfully.";

		WI_CORE_INFO(message.c_str());
	}

	ResourceId Resources::getResourcePosition(ResourceType rt, const char* file)
	{
		size_t size = m_Resources[rt].size();

		ResourceId resourceId = size;

		for (size_t i = 0; i < size; i++) {
			if (m_Resources[rt][i]->filePath == file) {
				resourceId = i;
				break;
			}
		}

		return resourceId;
	}

	std::string Resources::_assetToLibPath(std::string path)
	{
		standarizePath(path);
		size_t a_ind = path.find("assets");

		if (a_ind != path.npos) {
			path.replace(a_ind, 6, "library");
		}
		else //resources path impl
		{
			if (path.find("library") != path.npos)
				return path;
			std::string res_path = "library/";
			res_path += path;
			path = res_path;
		}
		return path;
	}

	std::filesystem::path Resources::_import_path_impl(const std::filesystem::path& path, const char* extension)
	{
		std::string pathFile = _assetToLibPath(path.string().c_str());
		std::filesystem::path importPath = pathFile;
		_preparePath(importPath.parent_path().string());
		importPath.replace_extension(extension);
		return importPath;
	}

	bool Resources::_preparePath(std::string path)
	{
		if (!std::filesystem::exists(path)) {
			if (!std::filesystem::create_directories(path)) {
				WI_ERROR("Couldn't create directory {}", path.c_str());
				return false;
			}
		}

		return true;
	}

	void Resources::_toLower(std::string& path)
	{
		size_t len = path.size();

		for (size_t i = 0; i < len; i++) {
			if (path[i] >= 'A' && path[i] <= 'Z') path[i] -= ('Z' - 'z');
		}
	}

	void Resources::UnloadAllResources()
	{
		UnloadResourcesOf<Image>();
		UnloadResourcesOf<Material>();
		//UnloadResourcesOf<Shader>();
		UnloadResourcesOf<Model>();
		UnloadResourcesOf<Animation>();
	}
	
	Resources::MetaResult Resources::CheckMeta(const char* filename)
	{
		std::string metaPath = filename;
		metaPath += ".meta";

		if (!std::filesystem::exists(filename))
		{
			std::filesystem::remove(metaPath);
			//TODO: Delete the library
			return DELETED;
		}

		JSONDocument metaFile;
		if (!metaFile.load_file(metaPath.c_str()))
			return NOTFOUND;

		if (metaFile.HasMember("timeCreated"))
		{
			time_t metaTime = metaFile["timeCreated"].get<time_t>();
			time_t fileTime = to_time_t(std::filesystem::last_write_time(filename));

			if (metaTime < fileTime)
				return TOUPDATE;
		}
		return UPDATED;
	}

	void Resources::UpdateMeta(const char* filename)
	{
		std::filesystem::path metaPath = filename;
		metaPath.replace_extension(".meta");

		if (!std::filesystem::exists(filename))
			return;
		JSONDocument metaFile;
		if (!metaFile.load_file(metaPath.string().c_str()))
			return;

		if (metaFile.HasMember("timeCreated"))
		{
			time_t fileTime = to_time_t(std::filesystem::last_write_time(filename));
			metaFile["timeCreated"] = fileTime;
		}
	}

	bool Resources::_file_exists(const char* path)
	{
		return Wiwa::FileSystem::Exists(path);
	}

	void Resources::_import_image_impl(const char* origin, const char* destination)
	{
		int w, h, ch;

		// STBI_rgb_alpha loads image as 32 bpp (4 channels), ch = image origin channels
		unsigned char* image = stbi_load(origin, &w, &h, &ch, STBI_rgb_alpha);
		if (w != h)
		{
			WI_ERROR("Image at {0} needs to be square in order to be imported", origin);
			return;
		}

		Image::raw_to_dds_file(destination, image, w, h, 32);

		stbi_image_free(image);
	}

	void Resources::_import_model_impl(const char* origin, const char* destination, ModelSettings* settings)
	{
		Model* model = Model::GetModelFromFile(origin, settings);

		Model::SaveModel(model, destination);

		delete model;
	}

	void Resources::_import_animation_impl(const char* origin, const char* destination)
	{
		Animation* animation = Animation::GetAnimationFromFile(origin);

		Animation::SaveWiAnimation(animation, destination);

		delete animation;
	}
	
	bool Resources::_check_import_impl(const char* file, const char* extension)
	{
		std::string fileStr = _assetToLibPath(file);
		std::filesystem::path fileFS = fileStr;
		fileFS.replace_extension(extension);
		return std::filesystem::exists(fileFS);
	}

	void Resources::SetAssetPath(std::string& path)
	{
		_toLower(path);

		size_t a_ind = path.find("library");

		if (a_ind != path.npos) {
			path.replace(a_ind, 7, "assets");
		}
	}

	void Resources::Clear()
	{
		for (int i = 0; i < WRT_LAST; i++) {
			size_t rsize = m_Resources[i].size();

			for (size_t j = 0; j < rsize; j++) {
				delete m_Resources[i][j]->resource;
				delete m_Resources[i][j];
				j--;
			}
		}
	}

}