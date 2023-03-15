#pragma once

#include <wipch.h>
#include <Wiwa/core/Core.h>

#include <Wiwa/ecs/components/AgentAI.h>
//#include <Wiwa/ecs/components/ColliderCube.h>
//#include <Wiwa/ecs/components/ColliderCylinder.h>
//#include <Wiwa/ecs/components/ColliderSphere.h>
//#include <Wiwa/ecs/components/ColliderCapsule.h>
//#include <Wiwa/ecs/components/RayCast.h>
//#include "Wiwa/ecs/components/Transform3D.h"
//#include <Wiwa/utilities/render/shaders/Shader.h>
//#include <Wiwa/utilities/render/Uniforms.h>
//#include <Wiwa/core/Resources.h>

//#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>

#include <vector>
//#include <bitset>
//#include <algorithm>

#define DEFAULT_PATH_LENGTH 50

#define DEFAULT_WALK_CODE 1
#define INVALID_WALK_CODE 255

namespace Wiwa {	

	class WI_API AIPathFindingManager
	{

	public:

		// Data from the map
		struct MapData
		{
			float startingPosition;
			int width;
			int	height;
			int	tileWidth;
			int	tileHeight;
		};


		// ---------------------------------------------------------------------
		// Pathnode: Helper struct to represent a node in the path creation
		// ---------------------------------------------------------------------

		struct PathList;

		struct PathNode
		{
			int g;
			int h;
			glm::ivec2 pos;
			const PathNode* parent; // needed to reconstruct the path in the end

			// Convenient constructors
			PathNode();
			PathNode(int g, int h, const glm::ivec2& pos, const PathNode* parent);
			PathNode(const PathNode& node);

			bool operator==(const PathNode& other) const;

			// Fills a list (PathList) of all valid adjacent pathnodes
			uint32_t FindWalkableAdjacents(Wiwa::AIPathFindingManager::PathList& listToFill) const;
			// Calculates this tile score
			int Score() const;
			// Calculate the F for a specific destination tile
			int CalculateF(const glm::ivec2& destination);
		};

		// ---------------------------------------------------------------------
		// Helper struct to include a list of path nodes
		// ---------------------------------------------------------------------

		struct PathList
		{
			// Looks for a node in this list and returns it's list node or NULL
			PathNode* Find(const glm::ivec2& point);

			// Returns the Pathnode with lowest score in this list or NULL if empty
			PathNode* GetNodeLowestScore();

			// The list itself, note they are not pointers!
			std::vector<PathNode> pathList;
		};

	private:

		AIPathFindingManager();
		
	public:

		// Called before quitting
		static bool CleanUp();

		// Sets up the walkability map
		static void SetMap(uint32_t width, uint32_t height, unsigned char* data); // set map for each agent as well?

		// Main function to request a path from A to B
		static int CreatePath(const glm::ivec2& origin, const glm::ivec2& destination);

		// To request all tiles involved in the last generated path
		static const std::vector<glm::ivec2>* GetLastPath();

		// Utility: return true if pos is inside the map boundaries
		static bool CheckBoundaries(const glm::ivec2& pos);

		// Utility: returns true if the tile is walkable
		static bool IsWalkable(const glm::ivec2& pos);

		// Utility: return the walkability value of a tile
		static unsigned char GetTileAt(const glm::ivec2& pos);

		/*
		// Map Related functions
		*/

		// Generate the grid of the map, stores it inside map data and set the navigation values to walkable or non walkable
		static bool CreateWalkabilityMap(int width, int height, float tileWidth, float tileHeight, float startPos);

		// Space translations for the pathfinding
		static glm::ivec2 MapToWorld(int x, int y);
		static glm::ivec2 WorldToMap(int x, int y);


		static MapData& GetMapData()
		{
			return m_mapData;
		}

		private:

		// total size of the map in world coords if i am not mistaken
		static uint32_t m_width;
		static uint32_t m_height;
			
		// all map walkability values [0..255]
		static unsigned char* m_map;

		// we store the created path here
		static std::vector<glm::ivec2> m_lastPath;

		// data of the map
		static MapData m_mapData;
		
	};
}

