#include "NavMeshRender.h"
NavMeshRender::NavMeshRender(NavMesh* mesh)
{
	m_NavMesh = mesh;
}
NavMeshRender::~NavMeshRender()
{
}
//--------------------------------
// Raycast
// input: model, mvp matrix, screen pos
// output: screen pos hit model or not, if hit, return 3D position
//--------------------------------
bool NavMeshRender::RayCast(float* screen_pos, float* hit_pos)
{
	GLdouble matrix_p[16];
	GLdouble matrix_m[16];
	GLint viewport[4];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix_p);
	glGetDoublev(GL_MODELVIEW_MATRIX, matrix_m);
	glGetIntegerv(GL_VIEWPORT, viewport);
	float rays[3], raye[3];
	GLdouble x, y, z;
	gluUnProject(screen_pos[0], screen_pos[1], 0.0f, matrix_m, matrix_p, viewport, &x, &y, &z);
	rays[0] = (float)x; rays[1] = (float)y; rays[2] = (float)z;
	gluUnProject(screen_pos[0], screen_pos[1], 1.0f, matrix_m, matrix_p, viewport, &x, &y, &z);
	raye[0] = (float)x; raye[1] = (float)y; raye[2] = (float)z;
	float hitt;
	bool hit = m_NavMesh->m_geom->raycastMesh(rays, raye, hitt);
	if (hit)
	{
		hit_pos[0] = rays[0] + (raye[0] - rays[0])*hitt;
		hit_pos[1] = rays[1] + (raye[1] - rays[1])*hitt;
		hit_pos[2] = rays[2] + (raye[2] - rays[2])*hitt;
	}
	return hit;
}
void NavMeshRender::Render()
{
	if (!m_NavMesh->m_geom || !m_NavMesh->m_geom->getMesh())
		return;
	// ========================================== setup
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	DebugDrawGL dd;
	const float texScale = 1.0f / (m_NavMesh->m_cellSize * 10.0f);
	duDebugDrawTriMeshSlope(&dd, 
		m_NavMesh->m_geom->getMesh()->getVerts(), 
		m_NavMesh->m_geom->getMesh()->getVertCount(),
		m_NavMesh->m_geom->getMesh()->getTris(), 
		m_NavMesh->m_geom->getMesh()->getNormals(), 
		m_NavMesh->m_geom->getMesh()->getTriCount(),
		m_NavMesh->m_agentMaxSlope, 
		texScale);
	m_NavMesh->m_geom->drawOffMeshConnections(&dd);
	
	DrawAgents(&dd);
	
	glDepthMask(GL_FALSE);
	// Draw bounds
	const float* bmin = m_NavMesh->m_geom->getMeshBoundsMin();
	const float* bmax = m_NavMesh->m_geom->getMeshBoundsMax();
	duDebugDrawBoxWire(&dd, bmin[0],bmin[1],bmin[2], bmax[0],bmax[1],bmax[2], duRGBA(255,255,255,128), 1.0f);
	
	// Tiling grid.
	int gw = 0, gh = 0;
	rcCalcGridSize(bmin, bmax, m_NavMesh->m_cellSize, &gw, &gh);
	const int tw = (gw + (int)m_NavMesh->m_tileSize-1) / (int)m_NavMesh->m_tileSize;
	const int th = (gh + (int)m_NavMesh->m_tileSize-1) / (int)m_NavMesh->m_tileSize;
	const float s = m_NavMesh->m_tileSize*m_NavMesh->m_cellSize;
	duDebugDrawGridXZ(&dd, bmin[0],bmin[1],bmin[2], tw,th, s, duRGBA(0,0,0,64), 1.0f);

	if(m_NavMesh->m_navMesh && m_NavMesh->m_navQuery)
	{
		duDebugDrawNavMesh(&dd, *(m_NavMesh->m_navMesh), 
			DU_DRAWNAVMESH_COLOR_TILES|DU_DRAWNAVMESH_CLOSEDLIST|DU_DRAWNAVMESH_OFFMESHCONS);
		duDebugDrawNavMeshPolysWithFlags(&dd, *(m_NavMesh->m_navMesh), SAMPLE_POLYFLAGS_DISABLED, duRGBA(0,0,0,128));
	}
	if (m_NavMesh->m_tileCache)
		//DrawTiles(&dd, m_NavMesh->m_tileCache);
	
	if (m_NavMesh->m_tileCache)
		DrawObstacles(&dd, m_NavMesh->m_tileCache);
	
}
void NavMeshRender::DrawTiles(duDebugDraw* dd, dtTileCache* tc)
{
	unsigned int fcol[6];
	float bmin[3], bmax[3];

	for (int i = 0; i < tc->getTileCount(); ++i)
	{
		const dtCompressedTile* tile = tc->getTile(i);
		if (!tile->header) continue;
		
		tc->calcTightTileBounds(tile->header, bmin, bmax);
		
		const unsigned int col = duIntToCol(i,50);
		duCalcBoxColors(fcol, col, col);
		duDebugDrawBox(dd, bmin[0],bmin[1],bmin[2], bmax[0],bmax[1],bmax[2], fcol);
	}
	
	/*for (int i = 0; i < tc->getTileCount(); ++i)
	{
		const dtCompressedTile* tile = tc->getTile(i);
		if (!tile->header) continue;
		
		tc->calcTightTileBounds(tile->header, bmin, bmax);
		
		const unsigned int col = duIntToCol(i,255);
		const float pad = tc->getParams()->cs * 0.1f;
		duDebugDrawBoxWire(dd, bmin[0]-pad,bmin[1]-pad,bmin[2]-pad,
						   bmax[0]+pad,bmax[1]+pad,bmax[2]+pad, col, 2.0f);
	}*/

}

//dtObstacleRef hitTestObstacle(const dtTileCache* tc, const float* sp, const float* sq)
//{
//	float tmin = FLT_MAX;
//	const dtTileCacheObstacle* obmin = 0;
//	for (int i = 0; i < tc->getObstacleCount(); ++i)
//	{
//		const dtTileCacheObstacle* ob = tc->getObstacle(i);
//		if (ob->state == DT_OBSTACLE_EMPTY)
//			continue;
//		
//		float bmin[3], bmax[3], t0,t1;
//		tc->getObstacleBounds(ob, bmin,bmax);
//		
//		if (isectSegAABB(sp,sq, bmin,bmax, t0,t1))
//		{
//			if (t0 < tmin)
//			{
//				tmin = t0;
//				obmin = ob;
//			}
//		}
//	}
//	return tc->getObstacleRef(obmin);
//}
	
void NavMeshRender::DrawObstacles(duDebugDraw* dd, const dtTileCache* tc)
{
	for (int i = 0; i < tc->getObstacleCount(); ++i)
	{
		const dtTileCacheObstacle* ob = tc->getObstacle(i);
		if (ob->state == DT_OBSTACLE_EMPTY) continue;
		float bmin[3], bmax[3];
		tc->getObstacleBounds(ob, bmin,bmax);

		unsigned int col = 0;
		if (ob->state == DT_OBSTACLE_PROCESSING)
			col = duRGBA(255,255,0,128);
		else if (ob->state == DT_OBSTACLE_PROCESSED)
			col = duRGBA(255,192,0,192);
		else if (ob->state == DT_OBSTACLE_REMOVING)
			col = duRGBA(220,0,0,128);

		duDebugDrawCylinder(dd, bmin[0],bmin[1],bmin[2], bmax[0],bmax[1],bmax[2], col);
		duDebugDrawCylinderWire(dd, bmin[0],bmin[1],bmin[2], bmax[0],bmax[1],bmax[2], duDarkenCol(col), 2);
	}
}
void NavMeshRender::DrawAgents(duDebugDraw* dd)
{
	for (int i = 0; i < m_NavMesh->m_crowd->getAgentCount(); ++i)
	{
		const dtCrowdAgent* ag = m_NavMesh->m_crowd->getAgent(i);
		if (!ag->active) continue;
		
		const float height = ag->params.height;
		const float radius = ag->params.radius;
		const float* pos = ag->npos;
		
		unsigned int col = duRGBA(220,220,220,128);
		if (ag->targetState == DT_CROWDAGENT_TARGET_REQUESTING || ag->targetState == DT_CROWDAGENT_TARGET_WAITING_FOR_QUEUE)
			col = duLerpCol(col, duRGBA(128,0,255,128), 32);
		else if (ag->targetState == DT_CROWDAGENT_TARGET_WAITING_FOR_PATH)
			col = duLerpCol(col, duRGBA(128,0,255,128), 128);
		else if (ag->targetState == DT_CROWDAGENT_TARGET_FAILED)
			col = duRGBA(255,32,16,128);
		else if (ag->targetState == DT_CROWDAGENT_TARGET_VELOCITY)
			col = duLerpCol(col, duRGBA(64,255,0,128), 128);
		
		duDebugDrawCylinder(dd, pos[0]-radius, pos[1]+radius*0.1f, pos[2]-radius,
							pos[0]+radius, pos[1]+height, pos[2]+radius, col);
	}
}