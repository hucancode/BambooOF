#pragma once
#include "GL/glew.h"
#include "DebugDraw.h"
#include "DetourDebugDraw.h"
#include "RecastDebugDraw.h"
#include "NavMesh.h"
class NavMeshRender
{
private:
	NavMesh* m_NavMesh;
public:
	NavMeshRender(NavMesh* mesh);
	~NavMeshRender();
	bool RayCast(float* screen_pos, float* hit_pos);
	void Render();
private:
	void DrawTiles(duDebugDraw* dd, dtTileCache* tc);
	void DrawObstacles(duDebugDraw* dd, const dtTileCache* tc);
	void DrawAgents(duDebugDraw* dd);
};