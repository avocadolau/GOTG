#pragma once

#include <wipch.h>
#include <Wiwa/core/Core.h>

#include <Wiwa/ecs/components/AgentAI.h>
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

		struct PathList;

		struct PathNode
		{
			int g; // Distance from current node and parent
			int h; // Distance between current node and destination
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
			//std::vector<PathNode> pathList;
			std::list<PathNode> pathList;
		};

	private:

		AIPathFindingManager();
		
	public:

		// Called before quitting
		static bool CleanUp();

		// Sets up the walkability map
		static void SetMap(uint32_t width, uint32_t height, unsigned char* data);

		// Main function to request a path from A to B
		static int CreatePath(const glm::ivec2& origin, const glm::ivec2& destination);

		// To request all tiles involved in the last generated path
		static const std::vector<glm::vec2> GetLastPath();

		// Utility: return true if pos is inside the map boundaries
		static bool CheckBoundaries(const glm::ivec2& pos);

		// Utility: returns true if the tile is walkable
		static bool IsWalkable(const glm::ivec2& pos);

		// Utility: return the walkability value of a tile
		static unsigned char GetTileAt(const glm::ivec2& pos);

		private:

		// size of the map
		static uint32_t m_Width;
		static uint32_t m_Height;
			
		// all map walkability values [0..255]
		static unsigned char* m_Map;

		// we store the created path here
		static std::vector<glm::vec2> m_LastPath;
	};
}

