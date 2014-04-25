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
	/*
	float m_CamX, m_CamY, m_CamZ;
	float m_RotateX, m_RotateY;
	float m_Zoom;
	*/
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