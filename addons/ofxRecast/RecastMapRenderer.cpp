#include "RecastMapRenderer.h"
RecastMapRenderer::RecastMapRenderer(RecastMap* mesh)
{
	m_RecastMap = mesh;
}
RecastMapRenderer::~RecastMapRenderer()
{
}
void RecastMapRenderer::Render()
{
	if (!m_RecastMap->GetGeometry() || !m_RecastMap->GetGeometry()->getMesh())
		return;
	// ========================================== setup
	glDepthFunc(GL_LESS);
	glDepthMask(GL_TRUE);
	DebugDrawGL dd;
	const float texScale = 1.0f / (RECAST_CELL_SIZE * 10.0f);
	duDebugDrawTriMeshSlope(&dd, 
		m_RecastMap->GetGeometry()->getMesh()->getVerts(), 
		m_RecastMap->GetGeometry()->getMesh()->getVertCount(),
		m_RecastMap->GetGeometry()->getMesh()->getTris(), 
		m_RecastMap->GetGeometry()->getMesh()->getNormals(), 
		m_RecastMap->GetGeometry()->getMesh()->getTriCount(),
		RECAST_AGENT_MAX_SLOPE, 
		texScale);
	
	DrawAgents(&dd);
	
	glDepthMask(GL_FALSE);
	// Draw bounds
	const float* bmin = m_RecastMap->GetGeometry()->getMeshBoundsMin();
	const float* bmax = m_RecastMap->GetGeometry()->getMeshBoundsMax();
	duDebugDrawBoxWire(&dd, bmin[0],bmin[1],bmin[2], bmax[0],bmax[1],bmax[2], duRGBA(255,255,255,128), 1.0f);
	
	// Tiling grid.
	int gw = 0, gh = 0;
	rcCalcGridSize(bmin, bmax, RECAST_CELL_SIZE, &gw, &gh);
	const int tw = (gw + RECAST_TILE_SIZE - 1) / RECAST_TILE_SIZE;
	const int th = (gh + RECAST_TILE_SIZE - 1) / RECAST_TILE_SIZE;
	const float s = RECAST_TILE_SIZE*RECAST_CELL_SIZE;
	duDebugDrawGridXZ(&dd, bmin[0],bmin[1],bmin[2], tw,th, s, duRGBA(0,0,0,64), 1.0f);

	if(m_RecastMap->GetMesh() && m_RecastMap->GetQuery())
	{
		duDebugDrawNavMesh(&dd, *(m_RecastMap->GetMesh()), 
			DU_DRAWNAVMESH_COLOR_TILES|DU_DRAWNAVMESH_CLOSEDLIST|DU_DRAWNAVMESH_OFFMESHCONS);
		duDebugDrawNavMeshPolysWithFlags(&dd, *(m_RecastMap->GetMesh()), POLY_ABILITY_DISABLED, duRGBA(0,0,0,128));
	}
	if (m_RecastMap->GetTileCache())
		//DrawTiles(&dd, m_RecastMap->GetTileCache());
	
	if (m_RecastMap->GetTileCache())
		DrawObstacles(&dd, m_RecastMap->GetTileCache());

	if (m_RecastMap->GetGeometry())
		DrawConvexVolumes(&dd, m_RecastMap->GetGeometry());
}

void RecastMapRenderer::DrawTiles(duDebugDraw* dd, dtTileCache* tc)
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
	return;
	for (int i = 0; i < tc->getTileCount(); ++i)
	{
		const dtCompressedTile* tile = tc->getTile(i);
		if (!tile->header) continue;
		
		tc->calcTightTileBounds(tile->header, bmin, bmax);
		
		const unsigned int col = duIntToCol(i,255);
		const float pad = tc->getParams()->cs * 0.1f;
		duDebugDrawBoxWire(dd, bmin[0]-pad,bmin[1]-pad,bmin[2]-pad,
						   bmax[0]+pad,bmax[1]+pad,bmax[2]+pad, col, 2.0f);
	}

}
void RecastMapRenderer::DrawConvexVolumes(struct duDebugDraw* dd, InputGeometry* geometry)
{
	dd->depthMask(false);

	dd->begin(DU_DRAW_TRIS);
	
	for (int i = 0; i < geometry->getConvexVolumeCount(); ++i)
	{
		const ConvexVolume* vol = &geometry->getConvexVolumes()[i];
		unsigned int col = duIntToCol(vol->area, 32);
		for (int j = 0, k = vol->nverts-1; j < vol->nverts; k = j++)
		{
			const float* va = &vol->verts[k*3];
			const float* vb = &vol->verts[j*3];

			dd->vertex(vol->verts[0],vol->hmax,vol->verts[2], col);
			dd->vertex(vb[0],vol->hmax,vb[2], col);
			dd->vertex(va[0],vol->hmax,va[2], col);
			
			dd->vertex(va[0],vol->hmin,va[2], duDarkenCol(col));
			dd->vertex(va[0],vol->hmax,va[2], col);
			dd->vertex(vb[0],vol->hmax,vb[2], col);

			dd->vertex(va[0],vol->hmin,va[2], duDarkenCol(col));
			dd->vertex(vb[0],vol->hmax,vb[2], col);
			dd->vertex(vb[0],vol->hmin,vb[2], duDarkenCol(col));
		}
	}
	
	dd->end();

	dd->begin(DU_DRAW_LINES, 2.0f);
	for (int i = 0; i < geometry->getConvexVolumeCount(); ++i)
	{
		const ConvexVolume* vol = &geometry->getConvexVolumes()[i];
		unsigned int col = duIntToCol(vol->area, 220);
		for (int j = 0, k = vol->nverts-1; j < vol->nverts; k = j++)
		{
			const float* va = &vol->verts[k*3];
			const float* vb = &vol->verts[j*3];
			dd->vertex(va[0],vol->hmin,va[2], duDarkenCol(col));
			dd->vertex(vb[0],vol->hmin,vb[2], duDarkenCol(col));
			dd->vertex(va[0],vol->hmax,va[2], col);
			dd->vertex(vb[0],vol->hmax,vb[2], col);
			dd->vertex(va[0],vol->hmin,va[2], duDarkenCol(col));
			dd->vertex(va[0],vol->hmax,va[2], col);
		}
	}
	dd->end();

	dd->begin(DU_DRAW_POINTS, 3.0f);
	for (int i = 0; i < geometry->getConvexVolumeCount(); ++i)
	{
		const ConvexVolume* vol = &geometry->getConvexVolumes()[i];
		unsigned int col = duDarkenCol(duIntToCol(vol->area, 255));
		for (int j = 0; j < vol->nverts; ++j)
		{
			dd->vertex(vol->verts[j*3+0],vol->verts[j*3+1]+0.1f,vol->verts[j*3+2], col);
			dd->vertex(vol->verts[j*3+0],vol->hmin,vol->verts[j*3+2], col);
			dd->vertex(vol->verts[j*3+0],vol->hmax,vol->verts[j*3+2], col);
		}
	}
	dd->end();
	
	
	dd->depthMask(true);
}

/*----------------------------------------
return obstacle that hit by mouse's ray
sp, sq = 2 points form the ray, calculated by framework
------------------------------------------*/
dtObstacleRef RecastMapRenderer::HitTestObstacle(const float* sp, const float* sq)
{
	float tmin = FLT_MAX;
	const dtTileCacheObstacle* obmin = 0;
	for (int i = 0; i < m_RecastMap->GetTileCache()->getObstacleCount(); ++i)
	{
		const dtTileCacheObstacle* ob = m_RecastMap->GetTileCache()->getObstacle(i);
		if (ob->state == DT_OBSTACLE_EMPTY)
			continue;
		
		float bmin[3], bmax[3], t0,t1;
		m_RecastMap->GetTileCache()->getObstacleBounds(ob, bmin,bmax);
		
		if (isectSegAABB(sp,sq, bmin,bmax, t0,t1))
		{
			if (t0 < tmin)
			{
				tmin = t0;
				obmin = ob;
			}
		}
	}
	return m_RecastMap->GetTileCache()->getObstacleRef(obmin);
}

void RecastMapRenderer::DrawObstacles(duDebugDraw* dd, const dtTileCache* tc)
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
void RecastMapRenderer::DrawAgents(duDebugDraw* dd)
{
	for (int i = 0; i < m_RecastMap->GetCrowd()->getAgentCount(); ++i)
	{
		const dtCrowdAgent* ag = m_RecastMap->GetCrowd()->getAgent(i);
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