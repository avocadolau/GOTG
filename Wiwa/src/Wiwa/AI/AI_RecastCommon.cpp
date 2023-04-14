#include <wipch.h>
#include "AI_RecastCommon.h"
#include "RecastDebugDraw.h"
#include "DetourDebugDraw.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshQuery.h"
#include "DetourCrowd.h"


dtNavMesh* RecastCommon::loadAll(const char* path)
{
	return nullptr;
}
void RecastCommon::saveAll(const char* path, const dtNavMesh* mesh)
{
}

RecastCommon::RecastCommon() :
	m_navMesh(0),
	m_navQuery(0),
	m_crowd(0),
	m_navMeshDrawFlags(DU_DRAWNAVMESH_OFFMESHCONS | DU_DRAWNAVMESH_CLOSEDLIST),
	m_filterLowHangingObstacles(true),
	m_filterLedgeSpans(true),
	m_filterWalkableLowHeightSpans(true),
	m_ctx(0)
{
	resetCommonSettings();
	m_navQuery = dtAllocNavMeshQuery();
	m_crowd = dtAllocCrowd();

	/*for (int i = 0; i < MAX_TOOLS; i++)
		m_toolStates[i] = 0;*/
}

RecastCommon::~RecastCommon()
{
	dtFreeNavMeshQuery(m_navQuery);
	dtFreeNavMesh(m_navMesh);
	dtFreeCrowd(m_crowd);
	/*delete m_tool;
	for (int i = 0; i < MAX_TOOLS; i++)
		delete m_toolStates[i];*/
}

void RecastCommon::handleDebugMode()
{

}

void RecastCommon::handleRender()
{

}

bool RecastCommon::handleBuild()
{
	return false;
}

void RecastCommon::resetCommonSettings()
{
	m_settings.cellSize = 0.3f;
	m_settings.cellHeight = 0.2f;
	m_settings.agentHeight = 2.0f;
	m_settings.agentRadius = 0.6f;
	m_settings.agentMaxClimb = 0.9f;
	m_settings.agentMaxSlope = 45.0f;
	m_settings.regionMinSize = 8;
	m_settings.regionMergeSize = 20;
	m_settings.edgeMaxLen = 12.0f;
	m_settings.edgeMaxError = 1.3f;
	m_settings.vertsPerPoly = 6.0f;
	m_settings.detailSampleDist = 6.0f;
	m_settings.detailSampleMaxError = 1.0f;
	m_settings.partitionType = 0;
}

void RecastCommon::handleCommonSettings()
{
	return;
}
