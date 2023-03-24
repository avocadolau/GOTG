#pragma once

#include <wipch.h>
#include <Wiwa/core/Core.h>

//#include <Wiwa/ecs/components/AgentAI.h>
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

//#include <vector>
//#include <bitset>
//#include <algorithm>


namespace Wiwa {

	class WI_API AIMapGeneration
	{

	public:

		// Data from the map
		struct MapData
		{
			float startingPosition;
			int width;
			int	height;
			float tileWidth; // Cannot be less than 1!
			float tileHeight;// Cannot be less than 1!
		};


	private:

		AIMapGeneration();

	public:

		

		/*
		// Map Related functions
		*/

		// Generate the grid of the map, stores it inside map data and set the navigation values to walkable or non walkable
		static bool CreateWalkabilityMap(int width, int height, float tileWidth, float tileHeight, float startPos);

		// Space translations for the pathfinding
		static glm::vec2 MapToWorld(int x, int y);
		static glm::ivec2 WorldToMap(float x, float y);


		static MapData& GetMapData()
		{
			return m_mapData;
		}

		static unsigned char* GetMap()
		{
			return m_map;
		}

	private:

		// data of the map
		static MapData m_mapData;

		static unsigned char* m_map;

	};
}