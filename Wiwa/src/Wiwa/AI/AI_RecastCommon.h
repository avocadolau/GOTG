#pragma once
#include "Recast.h"
#include "AI_BuildSettings.h"
#include "AI_DebugDrawer.h"
#include "AI_BuildContext.h"

class RecastCommon
{
protected:
	class dtNavMesh* m_navMesh;
	class dtNavMeshQuery* m_navQuery;
	class dtCrowd* m_crowd;

	unsigned char m_navMeshDrawFlags;

	BuildSettings m_settings;

	bool m_filterLowHangingObstacles;
	bool m_filterLedgeSpans;
	bool m_filterWalkableLowHeightSpans;

	/*SampleTool* m_tool;
	SampleToolState* m_toolStates[MAX_TOOLS];*/

	BuildContext* m_ctx;

	DebugDrawGL m_dd;

	dtNavMesh* loadAll(const char* path);
	void saveAll(const char* path, const dtNavMesh* mesh);

public:
	RecastCommon();
	virtual ~RecastCommon();

	void setContext(BuildContext* ctx) { m_ctx = ctx; }

	/*	void setTool(SampleTool* tool);
		SampleToolState* getToolState(int type) { return m_toolStates[type]; }
		void setToolState(int type, SampleToolState* s) { m_toolStates[type] = s; }*/

	DebugDrawGL& getDebugDraw() { return m_dd; }

	//virtual void handleSettings();
	//virtual void handleTools();
	virtual void handleDebugMode();
	//virtual void handleClick(const float* s, const float* p, bool shift);
	//virtual void handleToggle();
	//virtual void handleStep();
	virtual void handleRender();
	//virtual void handleRenderOverlay(double* proj, double* model, int* view);
	//virtual void handleMeshChanged(class InputGeom* geom);
	virtual bool handleBuild();
	//virtual void handleUpdate(const float dt);
	//virtual void collectSettings(struct BuildSettings& settings);

	virtual class dtNavMesh* getNavMesh() { return m_navMesh; }
	virtual class dtNavMeshQuery* getNavMeshQuery() { return m_navQuery; }
	virtual class dtCrowd* getCrowd() { return m_crowd; }
	virtual float getAgentRadius() { return m_settings.agentRadius; }
	virtual float getAgentHeight() { return m_settings.agentHeight; }
	virtual float getAgentClimb() { return m_settings.agentMaxClimb; }

	unsigned char getNavMeshDrawFlags() const { return m_navMeshDrawFlags; }
	void setNavMeshDrawFlags(unsigned char flags) { m_navMeshDrawFlags = flags; }

	//void updateToolStates(const float dt);
	//void initToolStates(RecastCommon* sample);
	//void resetToolStates();
	//void renderToolStates();
	//void renderOverlayToolStates(double* proj, double* model, int* view);

	void resetCommonSettings();
	void handleCommonSettings();

private:
	// Explicitly disabled copy constructor and copy assignment operator.
	RecastCommon(const RecastCommon&);
	RecastCommon& operator=(const RecastCommon&);
};