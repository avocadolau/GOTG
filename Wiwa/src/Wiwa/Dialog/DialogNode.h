#pragma once
#include <vector>
#include <string>

#include <Wiwa/core/Renderer2D.h>

struct DialogNode
{
	// The dialog text lines
	Wiwa::Renderer2D::InstanceData text1_imgModeID;
	Wiwa::Renderer2D::InstanceData text2_imgModeID;
	Wiwa::Renderer2D::InstanceData text3_imgModeID;
	Wiwa::Renderer2D::InstanceData text4_imgModeID;
	Wiwa::Renderer2D::InstanceData text5_imgModeID;

	//bool isNodeOccupied = false;

	// The dialog ID of next nodes
	//std::vector<size_t> nodes;

	// The dialog text for the options
	//std::vector<std::string> options;
};