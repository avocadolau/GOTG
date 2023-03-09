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
		glm::vec2 pos;
		const PathNode* parent; // needed to reconstruct the path in the end

		// Convenient constructors
		PathNode();
		PathNode(int g, int h, const glm::vec2& pos, const PathNode* parent);
		PathNode(const PathNode& node);

		// Fills a list (PathList) of all valid adjacent pathnodes
		uint32_t FindWalkableAdjacents(PathList& list_to_fill) const;
		// Calculates this tile score
		int Score() const;
		// Calculate the F for a specific destination tile
		int CalculateF(const glm::vec2& destination);
	};

	// ---------------------------------------------------------------------
	// Helper struct to include a list of path nodes
	// ---------------------------------------------------------------------
	struct PathList
	{
		// Looks for a node in this list and returns it's list node or NULL
		std::_Vector_iterator<PathNode>* Find(const glm::vec2& point) const;
		
		// Returns the Pathnode with lowest score in this list or NULL if empty
		std::_Vector_iterator<PathNode>* GetNodeLowestScore() const;

		// The list itself, note they are not pointers!
		std::vector<PathNode> pathList;
	};

	class WI_API AIMapManager
	{



	};
}

