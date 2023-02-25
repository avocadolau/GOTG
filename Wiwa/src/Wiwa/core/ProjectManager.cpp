#include <wipch.h>
#include "ProjectManager.h"
#include <Wiwa/utilities/json/JSONDocument.h>
#include <Wiwa/core/Application.h>
#include <Wiwa/scene/Scene.h>

namespace Wiwa
{
	std::filesystem::path ProjectManager::m_CurrentProject = "";
	std::string ProjectManager::m_Name;
	std::string ProjectManager::m_Version;
	std::string ProjectManager::m_Company;
	ProjectManager::Target ProjectManager::m_Target;

	void ProjectManager::CreateProject(const char* file)
	{
		m_Name = "New project";
		m_Version = "0.1";
		m_Company = "My company";
		m_Target = Target::Windows;

		Wiwa::JSONDocument doc;
		doc.AddMember("name", m_Name.c_str());
		doc.AddMember("version", m_Version.c_str());
		doc.AddMember("company", m_Company.c_str());
		doc.AddMember("target", (int)m_Target);
		
		std::string path = file;
		path += ".wiproject";
		doc.save_file(path.c_str());
		m_CurrentProject = path.c_str();
	}
	void ProjectManager::OpenProject(const char* file)
	{
		Wiwa::JSONDocument doc(file);
		m_CurrentProject = file;
		if (doc.HasMember("name"))
			m_Name = doc["name"].as_string();
		if (doc.HasMember("version"))
			m_Version = doc["version"].as_string();
		if (doc.HasMember("company"))
			m_Company = doc["company"].get<const char*>();

		doc.save_file(file);
	}

	bool ProjectManager::SaveProject()
	{
		if (m_CurrentProject.empty())
		{
			return false;
		}
		Wiwa::JSONDocument doc(m_CurrentProject.string().c_str());
		doc.AddMember("name", m_Name.c_str());
		doc.AddMember("version", m_Version.c_str());
		doc.AddMember("company", m_Company.c_str());
		doc.save_file(m_CurrentProject.string().c_str());

		return true;
	}

	void ProjectManager::SaveProjectAs(const char* file)
	{
		std::string path = file;
		path += ".wiproject";
		Wiwa::JSONDocument doc(path.c_str());
		m_CurrentProject = path;
		doc.AddMember("name", m_Name.c_str());
		doc.AddMember("version", m_Version.c_str());
		doc.AddMember("company", m_Company.c_str());
		doc.save_file(path.c_str());
	}

	void ProjectManager::SaveScene(const char* file)
	{
	}

	void ProjectManager::LoadScene(const char* file)
	{
	}
}