#include <wipch.h>
#include "AIPathFindingManager.h"
//#include "Wiwa/ecs/systems/System.h"
//#include "Wiwa/ecs/EntityManager.h"
//#include "Wiwa/scene/SceneManager.h"
//#include "Wiwa/utilities/render/CameraManager.h"
//#include <glew.h>

#include <Wiwa/utilities/Log.h>
//#include "Wiwa/utilities/time/Time.h"

//#include <glm/gtc/quaternion.hpp>
//#include <glm/gtx/matrix_decompose.hpp>
//#include <glm/gtc/type_ptr.hpp>
//#include <vector>

//#include <Wiwa/utilities/json/JSONDocument.h>


unsigned char* Wiwa::AIPathFindingManager::m_map = nullptr;
std::vector<glm::ivec2> Wiwa::AIPathFindingManager::m_lastPath = {};
Wiwa::AIPathFindingManager::MapData Wiwa::AIPathFindingManager::m_mapData = MapData();

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

	
	
	//WI_INFO(" IsWalkable N {}", Wiwa::AIPathFindingManager::IsWalkable(cell));

	if (Wiwa::AIPathFindingManager::IsWalkable(cell))
		listToFill.pathList.push_back(PathNode(-1, -1, cell, this)); // Pushes an element at the back

	// south
	cell = pos;
	cell.y = pos.y - 1;

	//WI_INFO(" IsWalkable S {}", Wiwa::AIPathFindingManager::IsWalkable(cell));

	if (Wiwa::AIPathFindingManager::IsWalkable(cell))
		listToFill.pathList.push_back(PathNode(-1, -1, cell, this));

	// east
	cell = pos;
	cell.x = pos.x + 1;

	//WI_INFO(" IsWalkable E {}", Wiwa::AIPathFindingManager::IsWalkable(cell));

	if (Wiwa::AIPathFindingManager::IsWalkable(cell))
		listToFill.pathList.push_back(PathNode(-1, -1, cell, this));

	// west
	cell = pos;
	cell.x = pos.x - 1;

	//WI_INFO(" IsWalkable W {}", Wiwa::AIPathFindingManager::IsWalkable(cell));

	if (Wiwa::AIPathFindingManager::IsWalkable(cell))
		listToFill.pathList.push_back(PathNode(-1, -1, cell, this));

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
	for (int i = 0; i < pathList.size(); i++)
	{
		if (pathList.at(i).pos == point)
			return &pathList.at(i);
	}

	return nullptr;
}

Wiwa::AIPathFindingManager::PathNode* Wiwa::AIPathFindingManager::PathList::GetNodeLowestScore()
{
	//PathNode ret = PathNode();
	int min = 65535;

	//for (int i = pathList.size()-1; i > 0; --i)
	//{
	//	if (pathList.at(i).Score() < min)
	//	{
	//		min = pathList.at(i).Score();
	//		ret = pathList.at(i);
	//	}			
	//}

	std::vector<PathNode>::iterator ret = pathList.end();
	
	for (std::vector<PathNode>::iterator it = pathList.begin(); it != pathList.end(); ++it)
	{
		if (it->Score() < min)
		{
			min = it->Score();
			ret = it;
		}
	}

	return &(*ret);
	//return ret;

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

int Wiwa::AIPathFindingManager::CreatePath(const glm::ivec2& origin, const glm::ivec2& destination)
{
	size_t ret = -1;
	size_t iterations = 0;

	// they are not defined, IsWalkable returns NULL
	bool ori = IsWalkable(origin);
	bool desti = IsWalkable(destination);

	// L12b: TODO 1: if origin or destination are not walkable, return -1
	if (ori && desti)
	{
		// L12b: TODO 2: Create two lists: open, close
		PathList open;
		PathList closed;

		// Add the origin tile to open
		open.pathList.push_back(PathNode(0, 0, origin, nullptr));	

		iterations = open.pathList.size();
		
		// Iterate while we have tile in the open list
		while (open.pathList.size() > 0)
		{
			// L12b: TODO 3: Move the lowest score cell from open list to the closed list
			PathNode* lowest = open.GetNodeLowestScore();
			closed.pathList.push_back(*lowest);
			PathNode* node = &closed.pathList.back();

			WI_INFO("X = {}", node->pos.x);
			open.pathList.erase(std::remove(open.pathList.begin(), open.pathList.end(), *lowest), open.pathList.end());
			// Using std::remove() algorithm to move the value to be deleted to the end of the vector
			//open.pathList.erase(std::remove(open.pathList.begin(), open.pathList.end(), *lowest), open.pathList.end());

			

			// L12b: TODO 4: If we just added the destination, we are done!
			if (node && node->pos == destination)
			{
				m_lastPath.clear();

				// Backtrack to create the final path
				// Use the Pathnode::parent and Flip() the path when you are finish
				const PathNode* pathNode = node;

				WI_INFO("pathnode = {}", pathNode->pos.x);
				while (pathNode)
				{
					//m_lastPath.push_back(pathNode->pos);
					//pathNode = pathNode->parent;
				}

				std::reverse(m_lastPath.begin(), m_lastPath.end());
				ret = m_lastPath.size();
				//LOG("Created path of %d steps in %d iterations", ret, iterations);
				WI_INFO(" Path of {} steps and {} iterations", ret,iterations);
				break;
			}

			// L12b: TODO 5: Fill a list of all adjancent nodes
			PathList adjacent;
			uint32_t yes = node->FindWalkableAdjacents(adjacent);


			// L12b: TODO 6: Iterate adjancent nodes:
			// If it is a better path, Update the parent
			for (int i = 0; i < adjacent.pathList.size(); i++)
			{
				
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
			WI_INFO(" Path of {} steps and {} iterations", ret, iterations);

			iterations--;
		}
	}

	return (int)ret;
}

const std::vector<glm::ivec2>* Wiwa::AIPathFindingManager::GetLastPath()
{
	return &m_lastPath;
}

bool Wiwa::AIPathFindingManager::CheckBoundaries(const glm::ivec2& pos)
{
	bool ret = false;

	if ((pos.x >= 0) && (pos.x <= (int)m_mapData.width) &&
		(pos.y >= 0) && (pos.y <= (int)m_mapData.height))
	{
		ret = true;
	}

	return ret;
}

bool Wiwa::AIPathFindingManager::IsWalkable(const glm::ivec2& pos)
{
	unsigned char t = GetTileAt(pos);

	if (t != INVALID_WALK_CODE && t > 0) {
		//WI_INFO(" IsWalkable True");
		return true;
	}
	else {
		//WI_INFO(" IsWalkable False");
		return false;
	}
}

unsigned char Wiwa::AIPathFindingManager::GetTileAt(const glm::ivec2& pos)
{
	if (CheckBoundaries(pos))
	{
		//WI_INFO(" m_map at {} is equal to {}", pos.y + pos.x, m_map[pos.y + pos.x]);
		return m_map[pos.y + pos.x]; // returns the walk code of the tile
	}	

	//WI_INFO(" Invalid Code", pos.y + pos.x, m_map[pos.y + pos.x]);

	return INVALID_WALK_CODE;
}

bool Wiwa::AIPathFindingManager::CreateWalkabilityMap(int width, int height, float tileWidth, float tileHeight, float startPos)
{
	bool ret = false;

	m_mapData.startingPosition = startPos;
	m_mapData.width = width;
	m_mapData.height = height;
	m_mapData.tileHeight = (int)tileHeight;
	m_mapData.tileWidth = (int)tileWidth;
	
	m_map = new unsigned char[width * height]; // string that store tiles, this can vary as we want
	memset(m_map, 1, width * height);

	for (int z = 0; z < m_mapData.height; z++)
	{
		for (int x = 0; x < m_mapData.width; x++)
		{
			// if collision then invalid walking code
			
			// if not collision then default walking code
			m_map[z + x] = DEFAULT_WALK_CODE;
		}
	}
	
	ret = true;

	return ret;
}

glm::ivec2 Wiwa::AIPathFindingManager::MapToWorld(int x, int y)
{
	glm::ivec2 ret;

	ret.x = x * m_mapData.tileWidth;
	ret.y = y * m_mapData.tileHeight;

	return ret;
}

glm::ivec2 Wiwa::AIPathFindingManager::WorldToMap(int x, int y)
{
	glm::ivec2 ret;

	ret.x = x / m_mapData.tileWidth;
	ret.y = y / m_mapData.tileHeight;

	return ret;
}
