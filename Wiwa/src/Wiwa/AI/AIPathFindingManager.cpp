#include <wipch.h>
#include "AIPathFindingManager.h"
//#include "Wiwa/ecs/systems/System.h"
//#include "Wiwa/ecs/EntityManager.h"
//#include "Wiwa/scene/SceneManager.h"
//#include "Wiwa/utilities/render/CameraManager.h"
//#include <glew.h>

//#include <Wiwa/utilities/Log.h>
//#include "Wiwa/utilities/time/Time.h"

//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/matrix_decompose.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <vector>

//#include <Wiwa/utilities/json/JSONDocument.h>


// PathNode Defintitions

Wiwa::PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

Wiwa::PathNode::PathNode(int g, int h, const const glm::ivec2& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

Wiwa::PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

uint32_t Wiwa::PathNode::FindWalkableAdjacents(PathList & listToFill) const
{
	glm::ivec2 cell;
	uint32_t before = listToFill.pathList.size();

	// north
	cell = pos;
	cell.y = pos.y + 1;
	if (IsWalkable(cell))
		listToFill.pathList.push_back(PathNode(-1, -1, cell, this)); // Pushes an element at the back

	// south
	cell = pos;
	cell.y = pos.y - 1;
	if (IsWalkable(cell))
		listToFill.pathList.push_back(PathNode(-1, -1, cell, this));

	// east
	cell = pos;
	cell.x = pos.x + 1;
	if (IsWalkable(cell))
		listToFill.pathList.push_back(PathNode(-1, -1, cell, this));

	// west
	cell = pos;
	cell.x = pos.x - 1;
	if (IsWalkable(cell))
		listToFill.pathList.push_back(PathNode(-1, -1, cell, this));

	return listToFill.pathList.size();
}

int Wiwa::PathNode::Score() const
{
	
	return g + h;
	
}

int Wiwa::PathNode::CalculateF(const glm::ivec2& destination)
{
	g = parent->g + 1;	

	int fx = pos.x - destination.x;
	int fy = pos.y - destination.y;

	h = sqrtf((fx * fx) + (fy * fy));

	
	return g + h;
}

// PathList Definitions

Wiwa::PathNode* Wiwa::PathList::Find(const glm::ivec2& point) 
{
	for (int i = 0; i < pathList.size(); i++)
	{
		if (pathList.at(i).pos == point)
			return &pathList.at(i);
	}

	return nullptr;
}

Wiwa::PathNode* Wiwa::PathList::GetNodeLowestScore()
{
	Wiwa::PathNode ret = PathNode();
	int min = 65535;

	for (int i = 0; i < pathList.size(); i++)
	{
		if (pathList.at(i).Score() < min)
			ret = pathList.at(i);
	}

	return &ret;

}
