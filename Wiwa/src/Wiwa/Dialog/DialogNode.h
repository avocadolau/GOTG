#pragma once
#include <vector>
#include <string>

#include <Wiwa/core/Renderer2D.h>
#include <Wiwa/utilities/render/Text.h>

struct DialogNode
{
	// The dialog text lines
	Wiwa::Renderer2D::InstanceData text1_imgModeID;
	Wiwa::Renderer2D::InstanceData text2_imgModeID;
	Wiwa::Renderer2D::InstanceData text3_imgModeID;

	Wiwa::Text* text1_imgMode;
	Wiwa::Text* text2_imgMode;
	Wiwa::Text* text3_imgMode;

	//bool isNodeOccupied = false;

	// The dialog ID of next nodes
	//std::vector<size_t> nodes;

	// The dialog text for the options
	//std::vector<std::string> options;
};