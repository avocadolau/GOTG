#pragma once
#include "resources_impl.h"

namespace Wiwa {
	//--SPECIALIZATION FOR MODEL
	template<>
	inline void Resources::LoadMeta<Model>(const char* file, ModelSettings* settings)
	{
		std::filesystem::path filePath = file;
		filePath += ".meta";
		if (!std::filesystem::exists(filePath))
			return;
		JSONDocument doc(filePath.string().c_str());
		if (!doc.HasMember("modelImportSettings"))
			return;
		settings->preTranslatedVertices = doc["modelImportSettings"]["pre_translated_vertices"].get<bool>();
	}
	template<>
	inline void Resources::CreateMeta<Model>(const char* file, ModelSettings* settings)
	{
		std::filesystem::path filePath = file;
		filePath += ".meta";
		JSONDocument doc;
		doc.AddMember("fileFormatVersion", 1);
		doc.AddMember("file", file);
		doc.AddMember("folderAsset", false);
		std::time_t time = to_time_t(std::filesystem::last_write_time(file));
		doc.AddMember("timeCreated", time);
		JSONValue modelSettignsObject = doc.AddMemberObject("modelImportSettings");
		modelSettignsObject.AddMember("pre_translated_vertices", settings->preTranslatedVertices);
		doc.save_file(filePath.string().c_str());
	}
	template<>
	inline ResourceId Resources::LoadNative<Model>(const char* file)
	{
		ResourceId position = getResourcePosition(WRT_MODEL, file);
		size_t size = m_Resources[WRT_MODEL].size();

		ResourceId resourceId;

		if (position == size) {
			Model* model = new Model(file);

			PushResource(WRT_MODEL, file, model, true);

			resourceId = size;
		}
		else {
			resourceId = position;
		}

		return resourceId;
	}
	template<>
	inline ResourceId Resources::Load<Model>(const char* file)
	{
		std::filesystem::path file_path = _assetToLibPath(file);
		file_path.replace_extension(".wimodel");
		ResourceId position = getResourcePosition(WRT_MODEL, file_path.string().c_str());
		size_t size = m_Resources[WRT_MODEL].size();

		ResourceId resourceId;

		if (position == size) {
			Model* model = new Model(NULL);
			
			model->LoadWiMesh(file_path.string().c_str(), m_GenBuffersOnLoad);

			PushResource(WRT_MODEL, file_path.string().c_str(), model);

			resourceId = size;
		}
		else {
			resourceId = position;
		}

		return resourceId;
	}
	template<>
	inline Model* Resources::GetResourceById<Model>(ResourceId id)
	{
		Model* model = NULL;

		if (id >= 0 && id < m_Resources[WRT_MODEL].size()) {
			model = static_cast<Model*>(m_Resources[WRT_MODEL][id]->resource);
		}

		return model;
	}
	template<>
	inline bool Resources::Import<Model>(const char* file, ModelSettings* settings)
	{
		if (!_file_exists(file)) return false;

		std::filesystem::path import_path = file;
		import_path = _import_path_impl(import_path, ".wimodel");
		

		_import_model_impl(file, import_path.string().c_str(), settings);

		WI_CORE_INFO("Model at {} imported succesfully!", import_path.string().c_str());

		return true;
	}

	template<>
	inline bool Resources::CheckImport<Model>(const char* file)
	{
		return _check_import_impl(file, ".wimodel");
	}

	template<>
	inline const char* Resources::getResourcePathById<Model>(size_t id)
	{
		return getPathById(WRT_MODEL, id);
	}

	template<>
	inline void Resources::UnloadResourcesOf<Model>() {
		std::vector<Resource*>& rvec = m_Resources[WRT_MODEL];
		size_t count = rvec.size();

		for (size_t i = 0; i < count; i++) {
			if (!rvec[i]->isNative) {
				Model* mat = (Model*)rvec[i]->resource;

				delete mat;

				rvec.erase(rvec.begin() + i);
				i--;
				count--;
			}
		}
	}
}