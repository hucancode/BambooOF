#include "NavMesh.h"

NavMesh::NavMesh()
{
	InitMesh();
	//InitCrowd();
}
NavMesh::~NavMesh()
{
	dtFreeNavMesh(m_navMesh);
	m_navMesh = 0;
	dtFreeTileCache(m_tileCache);
}
void NavMesh::InitMesh()
{
	// common setting
	m_cellSize = 0.3f;
	m_cellHeight = 0.2f;
	m_agentHeight = 2.0f;
	m_agentRadius = 0.6f;
	m_agentMaxClimb = 2.5f;
	m_agentMaxSlope = 45.0f;
	m_regionMinSize = 8;
	m_regionMergeSize = 20;
	m_edgeMaxLen = 12.0f;
	m_edgeMaxError = 1.3f;
	m_vertsPerPoly = 6.0f;
	m_detailSampleDist = 6.0f;
	m_detailSampleMaxError = 1.0f;
	//
	m_geom = 0;
	m_navMesh = 0;
	m_navQuery = dtAllocNavMeshQuery();
	m_crowd = dtAllocCrowd();
	m_tileCache = 0;
	m_maxTiles = 0;
	m_maxPolysPerTile = 0;
	m_tileSize = 48;
	m_talloc = new LinearAllocator(32000);
	m_tcomp = new FastLZCompressor;
	m_tmproc = new MeshProcess;
	
}
void NavMesh::LoadMesh(const char* filepath)
{
	InputGeom* geom = new InputGeom();
	geom->loadMesh(filepath);
	m_geom = geom;
	dtFreeTileCache(m_tileCache);
	m_tileCache = 0;

	dtFreeNavMesh(m_navMesh);
	m_navMesh = 0;

	int gridSize = 1;
	if (m_geom)
	{
		const float* bmin = m_geom->getMeshBoundsMin();
		const float* bmax = m_geom->getMeshBoundsMax();
		int gw = 0, gh = 0;
		rcCalcGridSize(bmin, bmax, m_cellSize, &gw, &gh);
		const int ts = (int)m_tileSize;
		const int tw = (gw + ts-1) / ts;
		const int th = (gh + ts-1) / ts;

		// Max tiles and max polys affect how the tile IDs are caculated.
		// There are 22 bits available for identifying a tile and a polygon.
		int tileBits = rcMin((int)dtIlog2(dtNextPow2(tw*th*EXPECTED_LAYERS_PER_TILE)), 14);
		if (tileBits > 14) tileBits = 14;
		int polyBits = 22 - tileBits;
		m_maxTiles = 1 << tileBits;
		m_maxPolysPerTile = 1 << polyBits;
		gridSize = tw*th;
	}
	else
	{
		m_maxTiles = 0;
		m_maxPolysPerTile = 0;
	}
}
bool NavMesh::BuildMesh()
{
	dtStatus status;
	if (!m_geom || !m_geom->getMesh()) return false;
	m_tmproc->init(m_geom);
	// Init cache
	const float* bmin = m_geom->getMeshBoundsMin();
	const float* bmax = m_geom->getMeshBoundsMax();
	int gw = 0, gh = 0;
	rcCalcGridSize(bmin, bmax, m_cellSize, &gw, &gh);
	const int ts = (int)m_tileSize;
	const int tw = (gw + ts-1) / ts;
	const int th = (gh + ts-1) / ts;
	// Generation params.
	rcConfig cfg;
	memset(&cfg, 0, sizeof(cfg));
	cfg.cs = m_cellSize;
	cfg.ch = m_cellHeight;
	cfg.walkableSlopeAngle = m_agentMaxSlope;
	cfg.walkableHeight = (int)ceilf(m_agentHeight / cfg.ch);
	cfg.walkableClimb = (int)floorf(m_agentMaxClimb / cfg.ch);
	cfg.walkableRadius = (int)ceilf(m_agentRadius / cfg.cs);
	cfg.maxEdgeLen = (int)(m_edgeMaxLen / m_cellSize);
	cfg.maxSimplificationError = m_edgeMaxError;
	cfg.minRegionArea = (int)rcSqr(m_regionMinSize);		// Note: area = size*size
	cfg.mergeRegionArea = (int)rcSqr(m_regionMergeSize);	// Note: area = size*size
	cfg.maxVertsPerPoly = (int)m_vertsPerPoly;
	cfg.tileSize = (int)m_tileSize;
	cfg.borderSize = cfg.walkableRadius + 3; // Reserve enough padding.
	cfg.width = cfg.tileSize + cfg.borderSize*2;
	cfg.height = cfg.tileSize + cfg.borderSize*2;
	cfg.detailSampleDist = m_detailSampleDist < 0.9f ? 0 : m_cellSize * m_detailSampleDist;
	cfg.detailSampleMaxError = m_cellHeight * m_detailSampleMaxError;
	rcVcopy(cfg.bmin, bmin);
	rcVcopy(cfg.bmax, bmax);
	// Tile cache params.
	dtTileCacheParams tcparams;
	memset(&tcparams, 0, sizeof(tcparams));
	rcVcopy(tcparams.orig, bmin);
	tcparams.cs = m_cellSize;
	tcparams.ch = m_cellHeight;
	tcparams.width = (int)m_tileSize;
	tcparams.height = (int)m_tileSize;
	tcparams.walkableHeight = m_agentHeight;
	tcparams.walkableRadius = m_agentRadius;
	tcparams.walkableClimb = m_agentMaxClimb;
	tcparams.maxSimplificationError = m_edgeMaxError;
	tcparams.maxTiles = tw*th*EXPECTED_LAYERS_PER_TILE;
	tcparams.maxObstacles = 128;

	dtFreeTileCache(m_tileCache);

	m_tileCache = dtAllocTileCache();
	if (!m_tileCache) return false;
	status = m_tileCache->init(&tcparams, m_talloc, m_tcomp, m_tmproc);
	if (dtStatusFailed(status)) return false;

	dtFreeNavMesh(m_navMesh);

	m_navMesh = dtAllocNavMesh();
	if (!m_navMesh) return false;

	dtNavMeshParams params;
	memset(&params, 0, sizeof(params));
	rcVcopy(params.orig, m_geom->getMeshBoundsMin());
	params.tileWidth = m_tileSize*m_cellSize;
	params.tileHeight = m_tileSize*m_cellSize;
	params.maxTiles = m_maxTiles;
	params.maxPolys = m_maxPolysPerTile;

	status = m_navMesh->init(&params);
	if (dtStatusFailed(status)) return false;

	status = m_navQuery->init(m_navMesh, 2048);
	if (dtStatusFailed(status)) return false;

	for (int y = 0; y < th; ++y)
	{
		for (int x = 0; x < tw; ++x)
		{
			TileCacheData tiles[MAX_LAYERS];
			memset(tiles, 0, sizeof(tiles));
			int n = rasterizeTileLayers(m_geom, x, y, cfg, tiles, MAX_LAYERS);
			for (int i = 0; i < n; ++i)
			{
				TileCacheData* tile = &tiles[i];
				status = m_tileCache->addTile(tile->data, tile->dataSize, DT_COMPRESSEDTILE_FREE_DATA, 0);
				if (dtStatusFailed(status))
				{
					dtFree(tile->data);
					tile->data = 0;
					continue;
				}
			}
		}
	}
	for (int y = 0; y < th; ++y)
		for (int x = 0; x < tw; ++x)
			m_tileCache->buildNavMeshTilesAt(x,y, m_navMesh);
	return true;
}
int NavMesh::AddObstacle(const float* pos)
{
	if (!m_tileCache)
		return -1;
	float p[3];
	dtVcopy(p, pos);
	p[1] -= 0.5f;
	dtObstacleRef result;
	m_tileCache->addObstacle(p, 1.0f, 2.0f, &result);
	return result;
}
int NavMesh::HitTestObstacle(const float* sp, const float* sq)
{
	if (!m_tileCache)
		return 0;
	float tmin = FLT_MAX;
	const dtTileCacheObstacle* obmin = 0;
	for (int i = 0; i < m_tileCache->getObstacleCount(); ++i)
	{
		const dtTileCacheObstacle* ob = m_tileCache->getObstacle(i);
		if (ob->state == DT_OBSTACLE_EMPTY)
			continue;

		float bmin[3], bmax[3], t0,t1;
		m_tileCache->getObstacleBounds(ob, bmin,bmax);

		if (isectSegAABB(sp,sq, bmin,bmax, t0,t1))
		{
			if (t0 < tmin)
			{
				tmin = t0;
				obmin = ob;
			}
		}
	}
	return m_tileCache->getObstacleRef(obmin);
}
void NavMesh::RemoveObstacle(const float* sp, const float* sq)
{
	if (!m_tileCache)
		return;
	dtObstacleRef ref = HitTestObstacle(sp, sq);
	m_tileCache->removeObstacle(ref);
}
void NavMesh::ClearAllObstacle()
{
	if (!m_tileCache)
		return;
	for (int i = 0; i < m_tileCache->getObstacleCount(); ++i)
	{
		const dtTileCacheObstacle* ob = m_tileCache->getObstacle(i);
		if (ob->state == DT_OBSTACLE_EMPTY) continue;
		m_tileCache->removeObstacle(m_tileCache->getObstacleRef(ob));
	}
}
void NavMesh::UpdateMesh(const float dt)
{
	if (!m_navMesh)
		return;
	if (!m_tileCache)
		return;
	m_tileCache->update(dt, m_navMesh);
}

// ===================================================================================


void NavMesh::InitCrowd()
{
	if (m_navMesh && m_crowd)
	{
		m_crowd->init(MAX_AGENTS, 0.6f, m_navMesh);

		// Make polygons with 'disabled' flag invalid.
		m_crowd->getEditableFilter()->setExcludeFlags(SAMPLE_POLYFLAGS_DISABLED);

		// Setup local avoidance params to different qualities.
		dtObstacleAvoidanceParams params;
		// Use mostly default settings, copy from dtCrowd.
		memcpy(&params, m_crowd->getObstacleAvoidanceParams(0), sizeof(dtObstacleAvoidanceParams));

		// Low (11)
		params.velBias = 0.5f;
		params.adaptiveDivs = 5;
		params.adaptiveRings = 2;
		params.adaptiveDepth = 1;
		m_crowd->setObstacleAvoidanceParams(0, &params);

		// Medium (22)
		params.velBias = 0.5f;
		params.adaptiveDivs = 5; 
		params.adaptiveRings = 2;
		params.adaptiveDepth = 2;
		m_crowd->setObstacleAvoidanceParams(1, &params);

		// Good (45)
		params.velBias = 0.5f;
		params.adaptiveDivs = 7;
		params.adaptiveRings = 2;
		params.adaptiveDepth = 3;
		m_crowd->setObstacleAvoidanceParams(2, &params);

		// High (66)
		params.velBias = 0.5f;
		params.adaptiveDivs = 7;
		params.adaptiveRings = 3;
		params.adaptiveDepth = 3;
		m_crowd->setObstacleAvoidanceParams(3, &params);
	}
}
int NavMesh::AddAgent(const float* p)
{
	dtCrowdAgentParams ap;
	memset(&ap, 0, sizeof(ap));
	ap.radius = 0.6f;
	ap.height = 2.0f;
	ap.maxAcceleration = 8.0f;
	ap.maxSpeed = 3.5f;
	ap.collisionQueryRange = ap.radius * 12.0f;
	ap.pathOptimizationRange = ap.radius * 30.0f;
	ap.updateFlags = 0; 
	ap.updateFlags |= DT_CROWD_ANTICIPATE_TURNS;
	ap.updateFlags |= DT_CROWD_OPTIMIZE_VIS;
	ap.updateFlags |= DT_CROWD_OPTIMIZE_TOPO;
	ap.updateFlags |= DT_CROWD_OBSTACLE_AVOIDANCE;
	ap.updateFlags |= DT_CROWD_SEPARATION;
	ap.obstacleAvoidanceType = 3.0f;
	ap.separationWeight = 2.0f;

	return m_crowd->addAgent(p, &ap);;
}
int NavMesh::HitTestAgent(const float* s, const float* p)
{
	int isel = -1;
	float tsel = FLT_MAX;

	for (int i = 0; i < m_crowd->getAgentCount(); ++i)
	{
		const dtCrowdAgent* ag = m_crowd->getAgent(i);
		if (!ag->active) continue;
		float bmin[3], bmax[3];
		// get agent bounds
		const float* p = ag->npos;
		const float r = ag->params.radius;
		const float h = ag->params.height;
		bmin[0] = p[0] - r;
		bmin[1] = p[1];
		bmin[2] = p[2] - r;
		bmax[0] = p[0] + r;
		bmax[1] = p[1] + h;
		bmax[2] = p[2] + r;

		float tmin, tmax;
		if (isectSegAABB(s, p, bmin,bmax, tmin, tmax))
		{
			if (tmin > 0 && tmin < tsel)
			{
				isel = i;
				tsel = tmin;
			} 
		}
	}

	return isel;
}
void NavMesh::RemoveAgent(const int idx)
{
	if (!m_crowd) return;
	m_crowd->removeAgent(idx);
}


void NavMesh::UpdateCrowd(const float dt)
{
	if (!m_navMesh || !m_crowd) return;
	m_crowd->update(dt, 0);
}