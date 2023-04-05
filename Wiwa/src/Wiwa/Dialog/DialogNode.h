#pragma once
#include <vector>
#include <string>

struct DialogNode
{
	// The dialog text lines
	std::uint32_t text1_imgModeID;
	std::uint32_t text2_imgModeID;
	std::uint32_t text3_imgModeID;
	std::uint32_t text4_imgModeID;
	std::uint32_t text5_imgModeID;

	//bool isNodeOccupied = false;

	// The dialog ID of next nodes
	//std::vector<size_t> nodes;

	// The dialog text for the options
	//std::vector<std::string> options;
};