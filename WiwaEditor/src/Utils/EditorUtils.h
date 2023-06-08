#pragma once

#include <imgui.h>
#include <string>
#include <imgui_internal.h>
#include <glm/glm.hpp>
#include <filesystem>
#include <Wiwa/utilities/math/Vector3f.h>
struct Field;

inline static void HelpMarker(const char* desc)
{
	ImGui::TextDisabled("(?)");
	if (ImGui::IsItemHovered(ImGuiHoveredFlags_DelayShort))
	{
		ImGui::BeginTooltip();
		ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
		ImGui::TextUnformatted(desc);
		ImGui::PopTextWrapPos();
		ImGui::EndTooltip();
	}
}

inline void TextCentered(const char* text) {
	ImVec2 windowSize = ImGui::GetWindowSize();
	ImVec2 textSize = ImGui::CalcTextSize(text);

	ImGui::SetCursorPos({ (windowSize.x - textSize.x) * 0.5f, (windowSize.y - textSize.y) * 0.5f });
	ImGui::Text(text);
}
inline void RemoveWordFromLine(std::string& line, const std::string& word)
{
	auto n = line.find(word);
	if (n != std::string::npos)
	{
		line.erase(n, word.length());
	}
}
inline bool ButtonCenteredOnLine(std::string label, float alignment = 0.5f)
{
	ImGuiStyle& style = ImGui::GetStyle();

	float size = ImGui::CalcTextSize(label.c_str()).x + style.FramePadding.x * 2.0f;
	float avail = ImGui::GetContentRegionAvail().x;

	float off = (avail - size) * alignment;
	if (off > 0.0f)
		ImGui::SetCursorPosX(ImGui::GetCursorPosX() + off);

	return ImGui::Button(label.c_str());
}
inline void AssetContainer(const char* label)
{
	ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 20.0f);
	ImGui::InputText("##label", (char*)label, 64, ImGuiInputTextFlags_ReadOnly);
	ImGui::PopStyleVar();
}

void DrawVec3Control(const char* label, unsigned char* data, const Field field, float resetValue = 0.0f, float columnWidth = 100.0f);
void DrawVec3Control(const std::string& label,float* values, float resetValue = 0.0f, float columnWidth = 100.0f);
void DrawVec3Control(const std::string& label, glm::vec3* values, float resetValue = 0.0f, float columnWidth = 100.0f);
void DrawVec2Control(const char* label, unsigned char* data, const Field field, float resetValue = 0.0f, float columnWidth = 100.0f);
void DrawVec2Control(const std::string& label, glm::vec2* values, float resetValue = 0.0f, float columnWidth = 100.0f);
void DrawInt2Control(const char* label, unsigned char* data, const Field field, int resetValue = 0.0f, float columnWidth = 100.0f);
void DrawInt2Control(const std::string& label, glm::ivec2* values, float resetValue = 0.0f, float columnWidth = 100.0f);
void DrawRect2Control(const char* label, unsigned char* data, const Field field, int resetValue = 0.0f, float columnWidth = 100.0f);

bool ImageExtensionComp(const std::filesystem::path file);
bool ModelExtensionComp(const std::filesystem::path file);
bool ShaderExtensionComp(const std::filesystem::path file);
bool MaterialExtensionComp(const std::filesystem::path file);
bool VideoExtensionComp(const std::filesystem::path file);

void CreateScriptFile(const char* filePath, const char* name);

inline void VectorEdit(std::vector<int>& vector)
{
	for (uint32_t i = 0; i < vector.size(); i++)
	{
		ImGui::PushID(i);
		ImGui::InputInt("", &vector[i]);
		ImGui::PopID();
	}
	if (ImGui::Button("+"))
	{
		vector.push_back(0);
	}
	ImGui::SameLine();
	if (ImGui::Button("-"))
	{
		if (vector.size() > 0)
		{
			vector.erase(vector.begin() + vector.size() - 1);
		}
		
	}
}