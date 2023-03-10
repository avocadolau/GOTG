#pragma once

#include <wipch.h>
#include <Wiwa/core/Core.h>

//#include <Wiwa/ecs/components/RigidBody.h>
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
#define INVALID_WALK_CODE 255

namespace Wiwa {

	//enum CollisionType {
	//	CT_ENTER,
	//	CT_LOOP,
	//	CT_LEAVE,
	//	CT_NONE
	//};
	//
	//struct CollisionData {
	//	size_t entityA;
	//	size_t entityB;
	//	CollisionType collisionType;
	//};

	// ---------------------------------------------------------------------
	// Pathnode: Helper struct to represent a node in the path creation
	// ---------------------------------------------------------------------
	struct PathNode
	{
		int g;
		int h;
		glm::ivec2 pos;
		const Wiwa::PathNode* parent; // needed to reconstruct the path in the end

		// Convenient constructors
		PathNode();
		PathNode(int g, int h, const glm::ivec2& pos, const Wiwa::PathNode* parent);
		PathNode(const Wiwa::PathNode& node);

		// Fills a list (PathList) of all valid adjacent pathnodes
		uint32_t FindWalkableAdjacents(Wiwa::PathList& listToFill) const;
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
		Wiwa::PathNode* Wiwa::PathList::Find(const glm::ivec2& point);
		
		// Returns the Pathnode with lowest score in this list or NULL if empty
		Wiwa::PathNode* GetNodeLowestScore();

		// The list itself, note they are not pointers!
		std::vector<Wiwa::PathNode> pathList;
	};

	class WI_API AIPathFindingManager
	{

		public:

		AIPathFindingManager();

		// Destructor
		~AIPathFindingManager();

		// Called before quitting
		bool CleanUp();

		// Sets up the walkability map
		void SetMap(uint32_t width, uint32_t height, unsigned char* data);

		// Main function to request a path from A to B
		int CreatePath(const glm::ivec2& origin, const glm::ivec2& destination);

		// To request all tiles involved in the last generated path
		const std::vector<glm::ivec2>* GetLastPath() const;

		// Utility: return true if pos is inside the map boundaries
		bool CheckBoundaries(const glm::ivec2& pos) const;

		// Utility: returns true is the tile is walkable
		bool IsWalkable(const glm::ivec2& pos) const;

		// Utility: return the walkability value of a tile
		unsigned char GetTileAt(const glm::ivec2& pos) const;

		private:

		// size of the map
		uint32_t m_width = 0;
		uint32_t m_height = 0;
			
		// all map walkability values [0..255]
		unsigned char* m_map;

		// we store the created path here
		std::vector<glm::ivec2> m_lastPath;
		
	};
}

