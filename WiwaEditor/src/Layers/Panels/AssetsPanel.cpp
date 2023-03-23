#include "wipch.h"
#include "AssetsPanel.h"

#include <imgui.h>
#include <imgui_internal.h>
#include <Wiwa/core/Application.h>
#include <Wiwa/core/Resources.h>
#include <Wiwa/utilities/render/Model.h>
#include <Wiwa/utilities/render/Image.h>
#include <Wiwa/utilities/render/shaders/Shader.h>
#include <Wiwa/utilities/json/JSONDocument.h>
#include "MaterialPanel.h"
#include "../EditorLayer.h"
#include "../../Utils/EditorUtils.h"
#include <Wiwa/core/ProjectManager.h>

static const std::filesystem::path s_AssetsPath = "assets";

std::vector<std::function<void()>> AssetsPanel::m_AssetsThreadQueue;
std::mutex AssetsPanel::m_AssetsThreadQueueMutex;

AssetsPanel::AssetsPanel(EditorLayer* instance)
	: Panel("Assets", ICON_FK_FILES_O, instance), m_CurrentPath("assets")
{
	ResourceId folderId = Wiwa::Resources::LoadNative<Wiwa::Image>("resources/icons/folder_icon.png");
	ResourceId fileId = Wiwa::Resources::LoadNative<Wiwa::Image>("resources/icons/file_icon.png");
	ResourceId matId = Wiwa::Resources::LoadNative<Wiwa::Image>("resources/icons/material_icon.png");
	ResourceId modId = Wiwa::Resources::LoadNative<Wiwa::Image>("resources/icons/model_icon.png");
	ResourceId scrptId = Wiwa::Resources::LoadNative<Wiwa::Image>("resources/icons/script_icon.png");
	ResourceId shaderId = Wiwa::Resources::LoadNative<Wiwa::Image>("resources/icons/shader_icon.png");
	ResourceId prefabId = Wiwa::Resources::LoadNative<Wiwa::Image>("resources/icons/prefab_icon.png");
	ResourceId sceneId = Wiwa::Resources::LoadNative<Wiwa::Image>("resources/icons/scene_icon.png");
	

	m_FolderIcon = (ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(folderId)->GetTextureId();
	m_FileIcon = (ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(fileId)->GetTextureId();
	m_MaterialIcon = (ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(matId)->GetTextureId();
	m_ScriptIcon = (ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(scrptId)->GetTextureId();
	m_ModelIcon = (ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(modId)->GetTextureId();
	m_ShaderIcon = (ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(shaderId)->GetTextureId();
	m_PrefabIcon = (ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(prefabId)->GetTextureId();
	m_SceneIcon = (ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(sceneId)->GetTextureId();

	if (!std::filesystem::exists(s_AssetsPath))
	{
		std::filesystem::create_directory(s_AssetsPath);
	}

	watcher = std::make_unique<filewatch::FileWatch<std::filesystem::path>>(s_AssetsPath, OnFolderEvent);


	InitImports(s_AssetsPath);
}
void AssetsPanel::InitImports(const std::filesystem::path& path)
{
	for (auto& p : std::filesystem::directory_iterator(path))
	{
		if (p.is_directory() && !std::filesystem::is_empty(p))
		{
			for (auto& p1 : std::filesystem::directory_iterator(path))
			{
				UpdateImports(p1);
			}
		}
		else
		{
			CheckMeta(p);
			CheckImport(p);
		}
	}
}
void AssetsPanel::UpdateImports(const std::filesystem::directory_entry& path)
{
	if (path.is_directory())
	{
		for (auto& p : std::filesystem::directory_iterator(path))
		{
			UpdateImports(p);
		}
	}
	else
	{
		CheckMeta(path.path());
		CheckImport(path.path());
	}
}
AssetsPanel::~AssetsPanel()
{

}

void AssetsPanel::OnFolderEvent(const std::filesystem::path& path, const filewatch::Event change_type)
{	
	std::filesystem::path assetsPath = "assets";
	assetsPath /= path;

	if (assetsPath.extension() == ".meta")
		return;
	
	if (assetsPath.extension() == ".cs")
	{
		EditorLayer::Get().RegenSol();
	}
	switch (change_type)
	{
	case filewatch::Event::added:
	{
		CheckImport(assetsPath);
	}break;
	case filewatch::Event::removed:
	{
		DeleteFileAssets(assetsPath);
	}break;
	case filewatch::Event::modified:
	{
		CheckImport(assetsPath);
	}break;
	case filewatch::Event::renamed_old:
	{
		DeleteFileAssets(assetsPath);
	}break;
	case filewatch::Event::renamed_new:
	{
		CheckImport(assetsPath);
	}break;
	};
}

void AssetsPanel::DeleteFileAssets(std::filesystem::path& assetsPath)
{
	//Remove the meta file
	std::string metaPath = assetsPath.string();
	metaPath += ".meta";
	std::filesystem::remove(metaPath);
	//Remove from 

	std::string extension;
	std::filesystem::path libraryPath = Wiwa::Resources::_assetToLibPath(assetsPath.string());
	if (ImageExtensionComp(assetsPath))
		extension = ".dds";
	else if (ModelExtensionComp(assetsPath))
		extension = ".wimodel";
	else if (MaterialExtensionComp(assetsPath))
		extension = ".wimaterial";
	else if (ShaderExtensionComp(assetsPath))
		extension = ".wishader";
	else if (assetsPath.extension() == ".wiscene") {
		std::filesystem::path name = assetsPath.filename();
		name.replace_extension();

		Wiwa::ProjectManager::RemoveScene(name.string().c_str());
	}
	else if (assetsPath.extension() == ".wiprefab")
		extension = ".wiprefab";

	libraryPath.replace_extension(extension);
	std::filesystem::remove(libraryPath);
}

void AssetsPanel::CheckImport(const std::filesystem::path& path)
{
	std::string p = path.string();
	Wiwa::Resources::standarizePath(p);
	if (ImageExtensionComp(path) 
		&& (!Wiwa::Resources::CheckImport<Wiwa::Image>(p.c_str())
	|| Wiwa::Resources::CheckMeta(p.c_str()) != Wiwa::Resources::UPDATED))
	{
		Wiwa::ImageSettings settings;
		Wiwa::Resources::LoadMeta<Wiwa::Image>(p.c_str(), &settings);
		Wiwa::Resources::CreateMeta<Wiwa::Image>(p.c_str(), &settings);
		Wiwa::Resources::Import<Wiwa::Image>(p.c_str());
	}
	else if (ModelExtensionComp(path) 
		&& (!Wiwa::Resources::CheckImport<Wiwa::Model>(p.c_str())
		|| Wiwa::Resources::CheckMeta(p.c_str()) != Wiwa::Resources::UPDATED))
	{
		Wiwa::ModelSettings settings;
		Wiwa::Resources::LoadMeta<Wiwa::Model>(p.c_str(), &settings);
		Wiwa::Resources::CreateMeta<Wiwa::Model>(p.c_str(), &settings);
		Wiwa::Resources::Import<Wiwa::Model>(p.c_str(), &settings);
	}
	else if (ShaderExtensionComp(path) 
		&& (!Wiwa::Resources::CheckImport<Wiwa::Shader>(p.c_str())
		|| Wiwa::Resources::CheckMeta(p.c_str()) != Wiwa::Resources::UPDATED))
	{
		if (path.extension() == ".wishader")
		{
			Wiwa::Resources::CreateMeta<Wiwa::Shader>(p.c_str());
			p = p.substr(0, p.size() - 9);
			Wiwa::ResourceId id = Wiwa::Resources::Load<Wiwa::Shader>(p.c_str());
			Wiwa::Resources::Import<Wiwa::Shader>(p.c_str(), Wiwa::Resources::GetResourceById<Wiwa::Shader>(id));
		}
		else
		{
			p = p.substr(0, p.size() - 3);
			Wiwa::ResourceId id = Wiwa::Resources::Load<Wiwa::Shader>(p.c_str());
			Wiwa::Shader* shader = Wiwa::Resources::GetResourceById<Wiwa::Shader>(id);
			if (shader->getState() == Wiwa::Shader::ToCompile)
			{
				Wiwa::Resources::CreateMeta<Wiwa::Shader>(p.c_str());
				Wiwa::Resources::Import<Wiwa::Shader>(p.c_str(), Wiwa::Resources::GetResourceById<Wiwa::Shader>(id));
			}
		}
	}
	else if (MaterialExtensionComp(path) 
		&& (!Wiwa::Resources::CheckImport<Wiwa::Material>(p.c_str())
		|| Wiwa::Resources::CheckMeta(p.c_str()) != Wiwa::Resources::UPDATED))
	{
		Wiwa::Resources::CreateMeta<Wiwa::Material>(p.c_str());
		Wiwa::Resources::Import<Wiwa::Material>(p.c_str());
	}
	else if (path.extension() == ".bnk") {
		std::filesystem::path libfile = Wiwa::Resources::_assetToLibPath(path.string());
		std::filesystem::path libpath = libfile.parent_path();

		Wiwa::FileSystem::CreateDirs(libpath.string().c_str());
		Wiwa::FileSystem::Copy(path.string().c_str(), libfile.string().c_str());
	}
	else if (path.extension() == ".wiscene") {
		std::filesystem::path rpath = Wiwa::Resources::_assetToLibPath(path.string().c_str());
		std::filesystem::path rp = rpath.remove_filename();
		std::filesystem::create_directories(rp);
		Wiwa::FileSystem::Copy(path.string().c_str(), rpath.string().c_str());
	}
	else if (path.extension() == ".wiprefab") {
		std::filesystem::path rpath = Wiwa::Resources::_assetToLibPath(path.string().c_str());
		std::filesystem::path rp = rpath.remove_filename();
		std::filesystem::create_directories(rp);
		Wiwa::FileSystem::Copy(path.string().c_str(), rpath.string().c_str());
	}
}

void AssetsPanel::Draw()
{
	ImGui::Begin(iconName.c_str(), &active);

	if (ImGui::BeginTable("##content_browser", 2, ImGuiTableFlags_Resizable))
	{
		ImGui::TableNextColumn();
		//Folder browser
		if (ImGui::BeginTable("##folder_browser", 1))
		{
			ImGui::TableNextRow();
			ImGui::TableNextColumn();
			for (auto &path : std::filesystem::directory_iterator(s_AssetsPath))
			{
				DisplayNode(path);
			}
			ImGui::EndTable();
		}
		ImGui::TableNextColumn();

		//Back button
		TopBar();



		//Assets display
		float padding = 16.0f * m_ButtonSize;
		float thumbnailSize = 64.0f * m_ButtonSize;
		float cellSize = (thumbnailSize + padding);
		
		float panelWidth = ImGui::GetContentRegionAvail().x;
		int columnCount = (int)(panelWidth / cellSize);
		if (columnCount < 1)
			columnCount = 1;
		if (ImGui::BeginTable("##assets", columnCount))
		{
			int id = 0;
			for (auto& directoryEntry : std::filesystem::directory_iterator(m_CurrentPath))
			{
				if (directoryEntry.path().extension() == ".meta"
				|| directoryEntry.path().extension() == ".json")
					continue;
				ImGui::TableNextColumn();
				ImGui::PushID(id++);

				const auto& path = directoryEntry.path();
				auto relativePath = std::filesystem::relative(directoryEntry.path(), s_AssetsPath);
				std::string filenameString = relativePath.filename().string();
				
				ImTextureID texID = directoryEntry.is_directory() ? m_FolderIcon : m_FileIcon;
				if (ImageExtensionComp(directoryEntry.path()))
				{
					ResourceId pngID = Wiwa::Resources::LoadNative<Wiwa::Image>(path.string().c_str());
					texID = (ImTextureID)(intptr_t)Wiwa::Resources::GetResourceById<Wiwa::Image>(pngID)->GetTextureId();
				}
				else if (ModelExtensionComp(directoryEntry.path()))
					texID = m_ModelIcon;
				else if (MaterialExtensionComp(directoryEntry.path()))
					texID = m_MaterialIcon;
				else if (directoryEntry.path().extension() == ".cs")
					texID = m_ScriptIcon;
				else if (directoryEntry.path().extension() == ".vs"
					|| directoryEntry.path().extension() == ".fs"
					|| directoryEntry.path().extension() == ".gs")
					texID = m_ShaderIcon;
				else if (directoryEntry.path().extension() == ".wiprefab")
					texID = m_PrefabIcon;
				else if (directoryEntry.path().extension() == ".wiscene")
					texID = m_SceneIcon;

				ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
				if (ImGui::ImageButton(texID, { thumbnailSize, thumbnailSize }))
				{
					if (directoryEntry.path().extension() == ".wimaterial")
					{
						ResourceId matId = Wiwa::Resources::Load<Wiwa::Material>(path.string().c_str());
						MaterialChangeEvent event(matId);
						Action<Wiwa::Event&> action = { &EditorLayer::OnEvent, instance };
						action(event);
					}
					m_SelectedEntry = directoryEntry;
				}

				ImGui::PopStyleColor();

				//Drag and drop
				if (ImGui::BeginDragDropSource())
				{
					const wchar_t* itemPath = directoryEntry.path().c_str();
					ImGui::SetDragDropPayload("CONTENT_BROWSER_ITEM", itemPath, (wcslen(itemPath) + 1) * sizeof(wchar_t));
					ImGui::EndDragDropSource();
				}

				//Either opening a file or a folder
				if (ImGui::IsItemHovered() && ImGui::IsMouseDoubleClicked(ImGuiMouseButton_Left))
				{
					if (directoryEntry.is_directory())
						m_CurrentPath /= path.filename();
					else
						Wiwa::Application::Get().OpenDir(path.string().c_str());
				}
				if (ImGui::BeginPopupContextWindow("Assets context window"))
				{
					if (ImGui::MenuItem("Find in explorer"))
					{
						Wiwa::Application::Get().OpenDir(m_CurrentPath.string().c_str());
					}
					if (m_SelectedEntry.exists())
					{
						ImGui::Separator();

						ImGui::TextDisabled(m_SelectedEntry.path().filename().string().c_str());
						if (ImGui::MenuItem("Delete"))
						{
							if (m_SelectedEntry.is_directory())
							{
								if (_rmdir(m_SelectedEntry.path().string().c_str()) != 0)
									WI_ERROR("Can't remove directory at {}", m_SelectedEntry.path().string().c_str());
							}
							else
								remove(m_SelectedEntry.path().string().c_str());
						}
					}
					ImGui::EndPopup();
				}
				ImGui::TextWrapped(filenameString.c_str());

				ImGui::PopID();
			}
			ImGui::EndTable();

			if (ImGui::BeginDragDropTarget())
			{
				if (const ImGuiPayload* payload = ImGui::AcceptDragDropPayload("HIERARCHY_ENTITY"))
				{
					Wiwa::EntityManager& em = Wiwa::SceneManager::getActiveScene()->GetEntityManager();
					const EntityId* id = (EntityId*)payload->Data;
					WI_INFO("Created prefab of Entity {0} at {1}", em.GetEntityName(*id), m_CurrentPath.string().c_str());
					std::filesystem::path file = m_CurrentPath.string();
					file /= em.GetEntityName(*id);
					file += ".wiprefab";
					em.SavePrefab(*id, file.string().c_str());
				}
				ImGui::EndDragDropTarget();
			}
		}
		ImGui::EndTable();
	}

	//Assets context window
	ImGui::End();
}

void AssetsPanel::TopBar()
{
	ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
	if (ImGui::Button(ICON_FK_ARROW_LEFT))
	{
		if (m_CurrentPath != s_AssetsPath)
			m_CurrentPath = m_CurrentPath.parent_path();
	}
	ImGui::PopStyleColor();

	ImGui::SameLine();
	ImGui::AlignTextToFramePadding();
	ImGui::Text(m_CurrentPath.string().c_str());
	ImGui::SameLine();

	if (ImGui::Button("Create"))
		ImGui::OpenPopup("Create");
	int id = 0;
	if (ImGui::BeginPopup("Create"))
	{
		{
			ImGui::PushID(id++);
			ImGui::Text("Folder");
			static char buffer[64] = { 0 };
			ImGui::Text("Folder name");
			ImGui::InputText("##inputfolder", buffer, IM_ARRAYSIZE(buffer));
			ImGui::SameLine();
			if (ImGui::Button("Create"))
			{
				std::filesystem::path path = m_CurrentPath;
				std::filesystem::path dir = buffer;
				path /= dir;
				if (_mkdir(path.string().c_str()) == -1)
					WI_ERROR("Folder can't be created");
				ImGui::CloseCurrentPopup();
			}
			ImGui::Separator();
			ImGui::PopID();
		}
		{
			ImGui::PushID(id++);
			static char buffer[64] = { 0 };
			ImGui::Text("Material");
			ImGui::Text("Material name");
			ImGui::InputText("##inputfolder", buffer, IM_ARRAYSIZE(buffer));
			ImGui::SameLine();
			if (ImGui::Button("Create"))
			{
				std::filesystem::path path = m_CurrentPath;
				std::filesystem::path dir = buffer;
				path /= dir;
				std::string file = path.string() + ".wimaterial";

				Wiwa::Material material;
				Wiwa::Material::SaveMaterial(file.c_str(), &material);
				Wiwa::Resources::CreateMeta<Wiwa::Material>(path.string().c_str());

				ImGui::CloseCurrentPopup();
			}
			ImGui::Separator();
			ImGui::PopID();
		}
		{
			ImGui::PushID(id++);
			static char buffer[64] = { 0 };
			ImGui::Text("Shader");
			ImGui::Text("Shader name");
			ImGui::InputText("##inputfolder", buffer, IM_ARRAYSIZE(buffer));
			ImGui::SameLine();
			if (ImGui::Button("Create"))
			{
				std::filesystem::path path = m_CurrentPath;
				std::filesystem::path dir = buffer;
				path /= dir;
				
				Wiwa::Shader::CreateDefault(path.string().c_str());
				Wiwa::Shader* shader = new Wiwa::Shader();
				shader->setPath(path.string().c_str());
				Wiwa::Resources::Import<Wiwa::Shader>(path.string().c_str(), shader);
				std::string shaderPath = path.string();
				shaderPath += ".wishader";
				Wiwa::Resources::CreateMeta<Wiwa::Shader>(shaderPath.c_str());
				Wiwa::Resources::Load<Wiwa::Shader>(path.string().c_str());
				
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopID();
		}
		{
			ImGui::PushID(id++);
			static char buffer[64] = { 0 };
			ImGui::Text("Script");
			ImGui::Text("Behaviour name");
			ImGui::InputText("##inputfolder", buffer, IM_ARRAYSIZE(buffer));
			ImGui::SameLine();
			if (ImGui::Button("Create"))
			{
				std::filesystem::path path = m_CurrentPath;
				std::filesystem::path dir = buffer;
				path /= dir;
				std::string file = path.string();
				CreateScriptFile(path.string().c_str(), buffer);
				ImGui::CloseCurrentPopup();
			}
			ImGui::PopID();
		}
		ImGui::EndPopup();
	}

	ImGui::SameLine();
	ImGui::PushItemWidth(200.0f);
	ImGui::SliderFloat("##size", &m_ButtonSize, 0.5f, 2.0f);
	ImGui::PopItemWidth();
	ImGui::Separator();
}

void AssetsPanel::CheckMeta(const std::filesystem::path& path)
{
	std::string p = path.string();
	Wiwa::Resources::standarizePath(p);
	std::string metaPath = p;
	metaPath += ".meta";
	if (!std::filesystem::exists(metaPath))
	{
		if (ImageExtensionComp(path))
		{
			Wiwa::ImageSettings settings;
			Wiwa::Resources::CreateMeta<Wiwa::Image>(p.c_str(), &settings);
		}
		else if (ModelExtensionComp(path))
		{
			Wiwa::ModelSettings settings;
			Wiwa::Resources::CreateMeta<Wiwa::Model>(p.c_str(), &settings);
		}
		else if (MaterialExtensionComp(path))
			Wiwa::Resources::CreateMeta<Wiwa::Material>(p.c_str());
	}
}

void AssetsPanel::SubmitToAssetsThread(const std::function<void()> func)
{
	std::scoped_lock<std::mutex> lock(m_AssetsThreadQueueMutex);

	m_AssetsThreadQueue.emplace_back(func);
}

void AssetsPanel::ExecuteAssetsThreadQueue()
{
	for (auto& func : m_AssetsThreadQueue)
		func();

	m_AssetsThreadQueue.clear();
}

void AssetsPanel::OnEvent(Wiwa::Event& e)
{
	Wiwa::EventDispatcher dispatcher(e);
	dispatcher.Dispatch<Wiwa::WindowDropEvent>({&AssetsPanel::OnDragAndDrop, this});
}

bool AssetsPanel::OnDragAndDrop(Wiwa::WindowDropEvent& e)
{
	for (size_t i = 0; i < e.GetCount(); i++)
	{
		std::filesystem::path from = e.GetPaths()[i];
		std::filesystem::path to = m_CurrentPath;
		to /= from.filename();
		if(std::filesystem::is_directory(from))
		{
			if (_mkdir(from.string().c_str()) == -1)
			{
				WI_ERROR("Couldn't create directory at {0}", from.string().c_str());
			}
		}
		else
		{
			std::filesystem::copy_file(from, to);
		}
	}
	return true;
}

void AssetsPanel::DisplayNode(std::filesystem::directory_entry directoryEntry)
{
	const auto &path = directoryEntry.path();

	// ImGui::Text("%s", path.string().c_str());
	// auto relativePath = std::filesystem::relative(directoryEntry.path(), s_EditorPath);
	std::string filenameString = ICON_FK_FOLDER " ";
	filenameString += path.filename().string().c_str();
	static ImGuiTreeNodeFlags base_flags = ImGuiTreeNodeFlags_OpenOnArrow | ImGuiTreeNodeFlags_OpenOnDoubleClick | ImGuiTreeNodeFlags_SpanAvailWidth;
	bool isDir = std::filesystem::is_directory(path);
	if (isDir)
	{
		ImGui::PushStyleColor(ImGuiCol_Button, ImVec4(0, 0, 0, 0));
		ImGui::PushStyleColor(ImGuiCol_ButtonActive, ImVec4(0, 0, 0, 0));
		ImGui::AlignTextToFramePadding();
		if (!directoryEntry.path().empty())
		{
			bool open = ImGui::TreeNodeEx(filenameString.c_str(), base_flags);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				m_CurrentPath = path;
			}
			if (open)
			{
				for (auto &p : std::filesystem::directory_iterator(directoryEntry))
				{
					DisplayNode(p);
				}
				ImGui::TreePop();
			}
		}
		else
		{
			bool open = ImGui::TreeNodeEx(filenameString.c_str(), ImGuiTreeNodeFlags_Leaf | ImGuiTreeNodeFlags_NoTreePushOnOpen);
			if (ImGui::IsItemClicked() && !ImGui::IsItemToggledOpen())
			{
				m_CurrentPath = path;
			}

		}
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
		ImGui::TableNextColumn();
		ImGui::TableNextColumn();
	}
}
