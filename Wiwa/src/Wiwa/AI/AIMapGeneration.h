#pragma once

#include <wipch.h>
#include <Wiwa/core/Core.h>

//#include <btBulletDynamicsCommon.h>
#include <glm/glm.hpp>
#include <Wiwa/ecs/components/MapAI.h>
//#include <vector>
//#include <bitset>
#include <algorithm>

namespace Wiwa {

	class WI_API AIMapGeneration
	{

	public:

		// Data from the map
		struct MapData
		{
			glm::vec3 startingPosition;
			int width;
			int	height;
			float tileWidth; // Cannot be less than 1!
			float tileHeight;// Cannot be less than 1!
		};


	private:

		static void LoadMapData(size_t id);
		static void SaveMapData(size_t id);
		static void CreateNewEntityAndSaveData();
	public:

		/*
		// Map Related functions
		*/

		// Generate the grid of the map, stores it inside map data and set the navigation values to walkable or non walkable
		static bool CreateWalkabilityMap(int width, int height, float tileWidth, float tileHeight, glm::vec3 startPos);
		static bool BakeMap();
		static void SetPositionUnWalkable(glm::ivec2 world_pos);
		static bool DebugDrawMap();
		// Space translations for the pathfinding
		static glm::vec2 MapToWorld(int x, int y);
		static glm::ivec2 WorldToMap(float x, float y);

		static bool OnSave();
		static bool OnLoad();
		static bool ClearMap();

		static MapData& GetMapData()
		{
			return m_MapData;
		}

		/*static unsigned char* GetMap()
		{
			return m_Map;
		}*/

		static std::vector<unsigned char>& GetMap()
		{
			return m_Map;
		}

	private:

		// data of the map
		static MapData m_MapData;

		//static unsigned char* m_Map;
		static std::vector <unsigned char> m_Map;
	};
}