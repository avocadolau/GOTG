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
		glm::lowp_uvec2 pos;
		const PathNode* parent; // needed to reconstruct the path in the end

		// Convenient constructors
		PathNode();
		PathNode(int g, int h, const glm::lowp_uvec2& pos, const PathNode* parent);
		PathNode(const PathNode& node);

		// Fills a list (PathList) of all valid adjacent pathnodes
		uint32_t FindWalkableAdjacents(PathList& listToFill) const;
		// Calculates this tile score
		int Score() const;
		// Calculate the F for a specific destination tile
		int CalculateF(const glm::lowp_uvec2& destination);
	};

	// ---------------------------------------------------------------------
	// Helper struct to include a list of path nodes
	// ---------------------------------------------------------------------
	struct PathList
	{
		// Looks for a node in this list and returns it's list node or NULL
		PathNode* Find(const glm::lowp_uvec2& point) const;
		
		// Returns the Pathnode with lowest score in this list or NULL if empty
		PathNode* GetNodeLowestScore() const;

		// The list itself, note they are not pointers!
		std::vector<PathNode> pathList;
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
		void SetMap(uint32_t width, uint32_t height, char32_t* data);

		// Main function to request a path from A to B
		int CreatePath(const glm::lowp_uvec2& origin, const glm::lowp_uvec2& destination);

		// To request all tiles involved in the last generated path
		const std::vector<glm::lowp_uvec2>* GetLastPath() const;

		// Utility: return true if pos is inside the map boundaries
		bool CheckBoundaries(const glm::lowp_uvec2& pos) const;

		// Utility: returns true is the tile is walkable
		bool IsWalkable(const glm::lowp_uvec2& pos) const;

		// Utility: return the walkability value of a tile
		char32_t GetTileAt(const glm::lowp_uvec2& pos) const;

		private:

		// size of the map
		uint32_t m_width;
		uint32_t m_height;
			
		// all map walkability values [0..255]
		char32_t* m_map;

		// we store the created path here
		std::vector<glm::lowp_uvec2> m_lastPath;
		
	};
}

