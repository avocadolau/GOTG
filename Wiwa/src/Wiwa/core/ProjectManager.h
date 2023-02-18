#pragma once
#include <Wiwa/core/Core.h>

#include <filesystem>
#include <vector>
#include <Wiwa/scene/SceneManager.h>

namespace Wiwa
{
	class WI_API ProjectManager
	{
	public:
		enum class Target
		{
			None = 0,
			Windows
		};
	private:
		static std::filesystem::path m_CurrentProject;

		static std::string m_Name;
		static std::string m_Version;
		static std::string m_Company;
		static Target m_Target;
	public:
		static void CreateProject(const char* file);
		static void OpenProject(const char* file);
		static bool SaveProject();
		static void SaveProjectAs(const char* file);

		static void SetProjectName(const char* name) { m_Name = name; }
		static void SetProjectVersion(const char* version) { m_Version = version; }
		static void SetProjectCompany(const char* company) { m_Company = company; }
		static void SetProjectTarget(Target target) { m_Target = target; }

		static const char* GetProjectName() { return m_Name.c_str(); }
		static const char* GetProjectVersion() { return m_Version.c_str(); }
		static const char* GetProjectCompany() { return m_Company.c_str(); }
		static Target GetProjectTarget() { return m_Target; }

		static void SaveScene(const char* file);
		static void LoadScene(const char* file);
	private:
		ProjectManager();
	};
}