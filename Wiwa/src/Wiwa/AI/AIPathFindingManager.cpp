#include <wipch.h>
#include "AIPathFindingManager.h"
#include "AIMapGeneration.h"
//#include "Wiwa/ecs/systems/System.h"
//#include "Wiwa/ecs/EntityManager.h"
//#include "Wiwa/scene/SceneManager.h"
//#include "Wiwa/utilities/render/CameraManager.h"
//#include <glew.h>

#include <Wiwa/utilities/Log.h>

//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/matrix_decompose.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <vector>
#include <algorithm>

//#include <Wiwa/utilities/json/JSONDocument.h>

uint32_t Wiwa::AIPathFindingManager::m_Width = 0;
uint32_t Wiwa::AIPathFindingManager::m_Height = 0;
int Wiwa::AIPathFindingManager::m_MapSize = 0;
//unsigned char* Wiwa::AIPathFindingManager::m_Map = nullptr;
std::vector<glm::vec2> Wiwa::AIPathFindingManager::m_LastPath = {};
std::vector<unsigned char> Wiwa::AIPathFindingManager::m_MapPathFinding = std::vector<unsigned char>(MAP_TILES_MAX_SIZE);

// PathNode Defintitions

Wiwa::AIPathFindingManager::PathNode::PathNode() : g(-1), h(-1), pos(-1, -1), parent(NULL)
{}

Wiwa::AIPathFindingManager::PathNode::PathNode(int g, int h, const glm::ivec2& pos, const PathNode* parent) : g(g), h(h), pos(pos), parent(parent)
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
	
	// north
	cell = pos;
	cell.y = pos.y + 1;
	
	if (Wiwa::AIPathFindingManager::IsWalkable(cell))
		listToFill.pathList.emplace_back(PathNode(-1, -1, cell, this)); // Pushes an element at the back

	// south
	cell = pos;
	cell.y = pos.y - 1;

	if (Wiwa::AIPathFindingManager::IsWalkable(cell))
		listToFill.pathList.emplace_back(PathNode(-1, -1, cell, this));

	// east
	cell = pos;
	cell.x = pos.x + 1;

	if (Wiwa::AIPathFindingManager::IsWalkable(cell))
		listToFill.pathList.emplace_back(PathNode(-1, -1, cell, this));

	// west
	cell = pos;
	cell.x = pos.x - 1;

	if (Wiwa::AIPathFindingManager::IsWalkable(cell))
		listToFill.pathList.emplace_back(PathNode(-1, -1, cell, this));

	// north-east
	cell = pos;
	cell.y = pos.y + 1;
	cell.x = pos.x + 1;

	if (Wiwa::AIPathFindingManager::IsWalkable(cell))
		listToFill.pathList.emplace_back(PathNode(-1, -1, cell, this)); // Pushes an element at the back

	// south-east
	cell = pos;
	cell.y = pos.y - 1;
	cell.x = pos.x + 1;

	if (Wiwa::AIPathFindingManager::IsWalkable(cell))
		listToFill.pathList.emplace_back(PathNode(-1, -1, cell, this));

	// north-west
	cell = pos;
	cell.y = pos.y + 1;
	cell.x = pos.x - 1;

	if (Wiwa::AIPathFindingManager::IsWalkable(cell))
		listToFill.pathList.emplace_back(PathNode(-1, -1, cell, this)); // Pushes an element at the back

	// south-west
	cell = pos;
	cell.y = pos.y - 1;
	cell.x = pos.x - 1;

	if (Wiwa::AIPathFindingManager::IsWalkable(cell))
		listToFill.pathList.emplace_back(PathNode(-1, -1, cell, this)); // Pushes an element at the back

	return (uint32_t)listToFill.pathList.size();
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

	h = (int)sqrt((fx * fx) + (fy * fy));

	
	return g + h;
}

// PathList Definitions

Wiwa::AIPathFindingManager::PathNode* Wiwa::AIPathFindingManager::PathList::Find(const glm::ivec2& point)
{
	if (pathList.empty())
		return nullptr;

	for (std::list<PathNode>::iterator item = pathList.begin(); item != pathList.end(); item++)
	{
		if ((*item).pos == point)
			return &(*item);
		/*if (pathList.(i).pos == point)
			return &pathList.at(i);*/
	}
	return nullptr;

	/*for (int i = 0; i < pathList.size(); i++)
	{
		if (pathList.at(i).pos == point)
			return &pathList.at(i);
	}*/


	//ListItem<PathNode>* item = list.start;
	//while (item)
	//{
	//	if (item->data.pos == point)
	//		return item;
	//	item = item->next;
	//}
	//return item;
}

Wiwa::AIPathFindingManager::PathNode* Wiwa::AIPathFindingManager::PathList::GetNodeLowestScore()
{
	Wiwa::AIPathFindingManager::PathNode* ret = &(*pathList.rbegin());

	int min = 65535;
	for (std::list<PathNode>::reverse_iterator item = pathList.rbegin(); item != pathList.rend(); item++)
	{
		if (item->Score() < ret->Score() || item->Score() == ret->Score() && item->h < ret->h)
		{
			ret = &(*item);
		}
		/*if (item->Score() < min)
		{
			min = item->Score();
			ret = &(*item);
		}*/
	}
	return ret;
	
	/*auto lowest = std::min_element(pathList.begin(), pathList.end(), [](const PathNode& a, const PathNode& b) {
		return a.Score() < b.Score();
		});

	if (lowest != pathList.end())
	{
		ret = &(*lowest);
	}	*/


}

// AIPathFindingManager Definitions

Wiwa::AIPathFindingManager::AIPathFindingManager()
{
}


bool Wiwa::AIPathFindingManager::CleanUp()
{	
	m_MapPathFinding.clear();
	/*delete[] m_Map;
	m_Map = nullptr;*/
	return false;
}

//void Wiwa::AIPathFindingManager::SetMap(uint32_t width, uint32_t height, unsigned char* data)
//{
//	m_Width = width;
//	m_Height = height;
//
//	if (m_Map != nullptr)
//	{
//		delete[] m_Map;
//		m_Map = nullptr;
//	}
//
//	m_Map = new unsigned char[width * height];
//	memcpy(m_Map, data, width * height * sizeof(unsigned char));
//}

void Wiwa::AIPathFindingManager::SetMap(uint32_t width, uint32_t height, const std::vector<unsigned char>& data)
{
	m_Width = width;
	m_Height = height;

	m_MapPathFinding.clear();
	m_MapPathFinding.assign(width * height, DEFAULT_WALK_CODE);
	m_MapSize = width * height;
	std::copy(data.begin(), data.end(), m_MapPathFinding.begin());
	//memcpy(m_Map, data, width * height * sizeof(unsigned char));
}

int Wiwa::AIPathFindingManager::CreatePath(const glm::ivec2& origin, const glm::ivec2& destination)
{
	size_t ret = -1;
	size_t iterations = 0;

	if (!IsWalkable(origin))
		return ret;

	if (!IsWalkable(destination))
		return ret;

	PathList open;
	PathList closed;

	// Add the origin tile to open
	open.pathList.push_back(PathNode(0, 0, origin, nullptr));

	// Iterate while we have tile in the open list
	while (!open.pathList.empty())
	{
		// L12b: TODO 3: Move the lowest score cell from open list to the closed list
		PathNode* lowest = open.GetNodeLowestScore();
		closed.pathList.push_back(*lowest);
		PathNode* node = &closed.pathList.back();

		//WI_INFO("X = {}", node->pos.x);
		open.pathList.erase(std::remove(open.pathList.begin(), open.pathList.end(), *lowest), open.pathList.end());
		// Using std::remove() algorithm to move the value to be deleted to the end of the vector
		//open.pathList.erase(std::remove(open.pathList.begin(), open.pathList.end(), *lowest), open.pathList.end());


		// L12b: TODO 4: If we just added the destination, we are done!
		if (node && node->pos == destination)
		{
			m_LastPath.clear();

			const PathNode* pathNode = node;

			int i = 0;
			while (pathNode)
			{
				//WI_CORE_INFO(" Last Path value at pos {}: x = {}, y = {}, Walkability Value: {}", i, pathNode->pos.x, pathNode->pos.y, m_Map[(pathNode->pos.x * m_Width) + pathNode->pos.y]);

				glm::vec2 vec = Wiwa::AIMapGeneration::MapToWorld(pathNode->pos.x, pathNode->pos.y);
				m_LastPath.push_back(vec);
				pathNode = pathNode->parent;
				i++;
			}

			//std::reverse(m_LastPath.begin(), m_LastPath.end());
			ret = m_LastPath.size();
			break;
		}

		// L12b: TODO 5: Fill a list of all adjancent nodes
		PathList adjacent;
		node->FindWalkableAdjacents(adjacent);

		// L12b: TODO 6: Iterate adjancent nodes:
		// If it is a better path, Update the parent
		for (auto& item : adjacent.pathList)
		{
			if (closed.Find(item.pos) != nullptr)
				continue;

			// If it is NOT found, calculate its F and add it to the open list
			PathNode* adjacentInOpen = open.Find(item.pos);
			int newMovementCostToAdjacent = node->g + GetDistance(node->pos, item.pos);
			if (newMovementCostToAdjacent < item.g || adjacentInOpen == nullptr)
			{
				item.g = newMovementCostToAdjacent;
				item.h = GetDistance(item.pos, destination);
				item.parent = node;
				//item.CalculateF(destination);
				if (adjacentInOpen == nullptr)
					open.pathList.emplace_back(item);
			}
			else
			{
				//// If it is already in the open list, check if it is a better path (compare G)
				//if (adjacentInOpen->g > item.g)
				//{
				//	item.g = newMovementCostToAdjacent;
				//	item.h = GetDistance(item.pos, destination);
				//	*adjacentInOpen = item;
				//	/*adjacentInOpen->parent = item.parent;
				//	adjacentInOpen->g = newMovementCostToAdjacent;
				//	adjacentInOpen->h = GetDistance(adjacentInOpen->pos, destination);*/
				//	//adjacentInOpen->CalculateF(destination);
				//}
			}
		}
		++iterations;
	}
	
	return ret;
}

const std::vector<glm::vec2> Wiwa::AIPathFindingManager::GetLastPath()
{
	return m_LastPath;
}

bool Wiwa::AIPathFindingManager::CheckBoundaries(const glm::ivec2& pos)
{
	return (pos.x >= 0 && pos.x <= (int)m_Width &&
		pos.y >= 0 && pos.y <= (int)m_Height);
}

bool Wiwa::AIPathFindingManager::IsWalkable(const glm::ivec2& pos)
{
	unsigned char t = GetTileAt(pos);
	return (t != INVALID_WALK_CODE && t > 0);
}

unsigned char Wiwa::AIPathFindingManager::GetTileAt(const glm::ivec2& pos)
{
	if (CheckBoundaries(pos) && (pos.y * m_Width + pos.x) < m_MapSize - 1)
		return m_MapPathFinding[(pos.y * m_Width) + pos.x];

	return INVALID_WALK_CODE;
}

int Wiwa::AIPathFindingManager::GetDistance(const glm::ivec2& node_a, const glm::ivec2& node_b)
{
	int dstX = std::abs(node_a.x - node_b.x);
	int dstY = std::abs(node_a.y - node_b.y);

	if (dstX > dstY)
		return 14 * dstY + 10 * (dstX - dstY);
	return 14 * dstX + 10 * (dstY - dstX);
}
