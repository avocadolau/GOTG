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


uint32_t Wiwa::AIPathFindingManager::m_width = 0;
uint32_t Wiwa::AIPathFindingManager::m_height = 0;
unsigned char* Wiwa::AIPathFindingManager::m_map = nullptr;
std::vector<glm::ivec2> Wiwa::AIPathFindingManager::m_lastPath = {};


// PathNode Defintitions

Wiwa::AIPathFindingManager::PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

Wiwa::AIPathFindingManager::PathNode::PathNode(int g, int h, const const glm::ivec2& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
{}

Wiwa::AIPathFindingManager::PathNode::PathNode(const PathNode& node) : g(node.g), h(node.h), pos(node.pos), parent(node.parent)
{}

bool Wiwa::AIPathFindingManager::PathNode::operator==(const PathNode& other) const
{
	return (pos == other.pos && g == other.g && h == other.h && parent == other.parent);
}

uint32_t Wiwa::AIPathFindingManager::PathNode::FindWalkableAdjacents(Wiwa::AIPathFindingManager::PathList& listToFill) const
{
	glm::ivec2 cell;
	uint32_t before = listToFill.pathList.size();

	// north
	cell = pos;
	cell.y = pos.y + 1;
	// AIPathFindingManager* test;
	// test sexo->IsWalkable(cell)
	if (Wiwa::AIPathFindingManager::IsWalkable(cell))
		listToFill.pathList.push_back(PathNode(-1, -1, cell, this)); // Pushes an element at the back

	// south
	cell = pos;
	cell.y = pos.y - 1;
	if (Wiwa::AIPathFindingManager::IsWalkable(cell))
		listToFill.pathList.push_back(PathNode(-1, -1, cell, this));

	// east
	cell = pos;
	cell.x = pos.x + 1;
	if (Wiwa::AIPathFindingManager::IsWalkable(cell))
		listToFill.pathList.push_back(PathNode(-1, -1, cell, this));

	// west
	cell = pos;
	cell.x = pos.x - 1;
	if (Wiwa::AIPathFindingManager::IsWalkable(cell))
		listToFill.pathList.push_back(PathNode(-1, -1, cell, this));

	return listToFill.pathList.size();
}


int Wiwa::AIPathFindingManager::PathNode::Score() const
{
	
	return g + h;
	
}

int Wiwa::AIPathFindingManager::PathNode::CalculateF(const glm::ivec2& destination)
{
	g = parent->g + 1;	

	int fx = pos.x - destination.x;
	int fy = pos.y - destination.y;

	h = sqrtf((fx * fx) + (fy * fy));

	
	return g + h;
}

// PathList Definitions

Wiwa::AIPathFindingManager::PathNode* Wiwa::AIPathFindingManager::PathList::Find(const glm::ivec2& point)
{
	for (int i = 0; i < pathList.size(); i++)
	{
		if (pathList.at(i).pos == point)
			return &pathList.at(i);
	}

	return nullptr;
}

Wiwa::AIPathFindingManager::PathNode* Wiwa::AIPathFindingManager::PathList::GetNodeLowestScore()
{
	PathNode ret = PathNode();
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

// AIPathFindingManager Definitions

Wiwa::AIPathFindingManager::AIPathFindingManager()
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
		open.pathList.push_back(PathNode(0, 0, origin, nullptr));

		// Iterate while we have tile in the open list
		while (open.pathList.size() > 0)
		{
			// L12b: TODO 3: Move the lowest score cell from open list to the closed list
			PathNode* lowest = open.GetNodeLowestScore();
			closed.pathList.push_back(*lowest);
			PathNode* node = lowest;

			// Using std::remove() algorithm to move the value to be deleted to the end of the vector
			open.pathList.erase(std::remove(open.pathList.begin(), open.pathList.end(), *lowest), open.pathList.end());

			

			// L12b: TODO 4: If we just added the destination, we are done!
			if (node->pos == destination)
			{
				m_lastPath.clear();

				// Backtrack to create the final path
				// Use the Pathnode::parent and Flip() the path when you are finish
				const PathNode* pathNode = node;

				while (pathNode)
				{
					m_lastPath.push_back(pathNode->pos);
					pathNode = pathNode->parent;
				}

				std::reverse(m_lastPath.begin(), m_lastPath.end());
				ret = m_lastPath.size();
				//LOG("Created path of %d steps in %d iterations", ret, iterations);
				break;
			}

			// L12b: TODO 5: Fill a list of all adjancent nodes
			PathList adjacent;
			node->FindWalkableAdjacents(adjacent);

			// L12b: TODO 6: Iterate adjancent nodes:
			// If it is a better path, Update the parent
			for (int i = 0; i < adjacent.pathList.size(); i++)
			{
				adjacent.pathList.at(i);

				if (closed.Find(adjacent.pathList.at(i).pos) != NULL)
					continue;

				PathNode* adjacentInOpen = open.Find(adjacent.pathList.at(i).pos);

				if (adjacentInOpen == NULL)
				{
					adjacent.pathList.at(i).CalculateF(destination);
					open.pathList.push_back(adjacent.pathList.at(i));
				}
				else
				{
					if (adjacentInOpen->g > adjacent.pathList.at(i).g + 1)
					{
						adjacentInOpen->parent = adjacent.pathList.at(i).parent;
						adjacentInOpen->CalculateF(destination);
					}
				}

			}

			++iterations;
		}
	}

	return ret;
}

const std::vector<glm::ivec2>* Wiwa::AIPathFindingManager::GetLastPath()
{
	return &m_lastPath;
}

bool Wiwa::AIPathFindingManager::CheckBoundaries(const glm::ivec2& pos)
{
	return (pos.x >= 0 && pos.x <= (int)m_width &&
		pos.y >= 0 && pos.y <= (int)m_height);
}

bool Wiwa::AIPathFindingManager::IsWalkable(const glm::ivec2& pos)
{
	unsigned char t = GetTileAt(pos);
	return t != INVALID_WALK_CODE && t > 0;
}

unsigned char Wiwa::AIPathFindingManager::GetTileAt(const glm::ivec2& pos)
{
	if (CheckBoundaries(pos))
		return m_map[(pos.y * m_width) + pos.x];

	return INVALID_WALK_CODE;
}
