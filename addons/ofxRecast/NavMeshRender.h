#pragma once
#include "GL/glew.h"
#include "DebugDraw.h"
#include "DetourDebugDraw.h"
#include "RecastDebugDraw.h"
#include "NavMesh.h"
/*------------------------------
This renderer has no transform matrix included.
That's mean you need to use your framework to setup the camera.
Just call function Render() after you bind your camera.
------------------------------*/
class NavMeshRender
{
private:
	NavMesh* m_NavMesh;
public:
	NavMeshRender(NavMesh* mesh);
	~NavMeshRender();
	void Render();
private:
	void DrawTiles(duDebugDraw* dd, dtTileCache* tc);
	void DrawObstacles(duDebugDraw* dd, const dtTileCache* tc);
	void DrawAgents(duDebugDraw* dd);
	dtObstacleRef HitTestObstacle(const float* sp, const float* sq);
};