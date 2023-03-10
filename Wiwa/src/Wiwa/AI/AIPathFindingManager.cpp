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

uint32_t Wiwa::PathNode::FindWalkableAdjacents(Wiwa::PathList & listToFill) const
{
	glm::ivec2 cell;
	uint32_t before = listToFill.pathList.size();

	// north
	cell = pos;
	cell.y = pos.y + 1;
	if (IsWalkable(cell))
		listToFill.pathList.push_back(Wiwa::PathNode(-1, -1, cell, this)); // Pushes an element at the back

	// south
	cell = pos;
	cell.y = pos.y - 1;
	if (IsWalkable(cell))
		listToFill.pathList.push_back(Wiwa::PathNode(-1, -1, cell, this));

	// east
	cell = pos;
	cell.x = pos.x + 1;
	if (IsWalkable(cell))
		listToFill.pathList.push_back(Wiwa::PathNode(-1, -1, cell, this));

	// west
	cell = pos;
	cell.x = pos.x - 1;
	if (IsWalkable(cell))
		listToFill.pathList.push_back(Wiwa::PathNode(-1, -1, cell, this));

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
	Wiwa::PathNode ret = Wiwa::PathNode();
	int min = 65535;

	for (int i = pathList.size(); i > 0; i--)
	{
		if (pathList.at(i).Score() < min)
		{
			min = pathList.at(i).Score();
			ret = pathList.at(i);
		}
			
	}

	return &ret;

}

Wiwa::AIPathFindingManager::AIPathFindingManager()
{
}

Wiwa::AIPathFindingManager::~AIPathFindingManager()
{

}

bool Wiwa::AIPathFindingManager::CleanUp()
{	
	delete m_map;
	m_map = nullptr;
	return false;
}

void Wiwa::AIPathFindingManager::SetMap(uint32_t width, uint32_t height, unsigned char* data)
{
	m_width = width;
	m_height = height;

	
	m_map = new unsigned char[width * height];
	memcpy(m_map, data, width * height);
}

int Wiwa::AIPathFindingManager::CreatePath(const glm::ivec2& origin, const glm::ivec2& destination)
{
	int ret = -1;
	int iterations = 0;

	// L12b: TODO 1: if origin or destination are not walkable, return -1
	if (IsWalkable(origin) && IsWalkable(destination))
	{
		// L12b: TODO 2: Create two lists: open, close
		PathList open;
		PathList closed;

		// Add the origin tile to open
		open.pathList.push_back(Wiwa::PathNode(0, 0, origin, nullptr));

		// Iterate while we have tile in the open list
		while (open.pathList.size() > 0)
		{
			// L12b: TODO 3: Move the lowest score cell from open list to the closed list
			Wiwa::PathNode* lowest = open.GetNodeLowestScore();			
			closed.pathList.push_back(*lowest);
			Wiwa::PathNode* node = lowest;

			for (int i = 0, i < open.pathList.size(); i++)
			{
				if (open.pathList.at(i) == lowest)
				{

				}
			}
			

			// L12b: TODO 4: If we just added the destination, we are done!
			if (node->data.pos == destination)
			{
				lastPath.Clear();

				// Backtrack to create the final path
				// Use the Pathnode::parent and Flip() the path when you are finish
				const PathNode* pathNode = &node->data;

				while (pathNode)
				{
					lastPath.PushBack(pathNode->pos);
					pathNode = pathNode->parent;
				}

				lastPath.Flip();
				ret = lastPath.Count();
				//LOG("Created path of %d steps in %d iterations", ret, iterations);
				break;
			}

			// L12b: TODO 5: Fill a list of all adjancent nodes
			PathList adjacent;
			node->data.FindWalkableAdjacents(adjacent);

			// L12b: TODO 6: Iterate adjancent nodes:
			// If it is a better path, Update the parent
			ListItem<PathNode>* item = adjacent.list.start;
			for (; item; item = item->next)
			{
				// ignore nodes in the closed list
				if (closed.Find(item->data.pos) != NULL)
					continue;

				// If it is NOT found, calculate its F and add it to the open list
				ListItem<PathNode>* adjacentInOpen = open.Find(item->data.pos);
				if (adjacentInOpen == NULL)
				{
					item->data.CalculateF(destination);
					open.list.add(item->data);
				}
				else
				{
					// If it is already in the open list, check if it is a better path (compare G)
					if (adjacentInOpen->data.g > item->data.g + 1)
					{
						adjacentInOpen->data.parent = item->data.parent;
						adjacentInOpen->data.CalculateF(destination);
					}
				}
			}

			++iterations;
		}
	}

	return ret;
}
