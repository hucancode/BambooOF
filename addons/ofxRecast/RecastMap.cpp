#include "RecastMap.h"
#include "fastlz.h"

int RecastMap::FastLZCompressor::maxCompressedSize(const int bufferSize)
{
	return (int)(bufferSize* 1.05f);
}
dtStatus RecastMap::FastLZCompressor::compress(const unsigned char* buffer, const int bufferSize,
									unsigned char* compressed, const int maxCompressedSize, int* compressedSize)
{
	*compressedSize = fastlz_compress((const void *const)buffer, bufferSize, compressed);
	return DT_SUCCESS;
}
dtStatus RecastMap::FastLZCompressor::decompress(const unsigned char* compressed, const int compressedSize,
									  unsigned char* buffer, const int maxBufferSize, int* bufferSize)
{
	*bufferSize = fastlz_decompress(compressed, compressedSize, buffer, maxBufferSize);
	return *bufferSize < 0 ? DT_FAILURE : DT_SUCCESS;
}

RecastMap::LinearAllocator::LinearAllocator(const int cap) : buffer(0), capacity(0), top(0), high(0)
{
	resize(cap);
}

RecastMap::LinearAllocator::~LinearAllocator()
{
	dtFree(buffer);
}
void RecastMap::LinearAllocator::resize(const int cap)
{
	if (buffer) dtFree(buffer);
	buffer = (unsigned char*)dtAlloc(cap, DT_ALLOC_PERM);
	capacity = cap;
}
void RecastMap::LinearAllocator::reset()
{
	high = dtMax(high, top);
	top = 0;
}
void* RecastMap::LinearAllocator::alloc(const int size)
{
	if (!buffer)
		return 0;
	if (top+size > capacity)
		return 0;
	unsigned char* mem = &buffer[top];
	top += size;
	return mem;
}
void RecastMap::LinearAllocator::free(void* /*ptr*/)
{
	// Empty
}

void RecastMap::MeshProcess::process(struct dtNavMeshCreateParams* params,
						  unsigned char* polyAreas, unsigned short* polyFlags)
{
	// Update poly flags from areas.
	for (int i = 0; i < params->polyCount; ++i)
	{
		if (polyAreas[i] == DT_TILECACHE_WALKABLE_AREA)
		{
			polyAreas[i] = POLY_AREA_GROUND;
		}
		if (polyAreas[i] == POLY_AREA_GROUND)
		{
			polyFlags[i] = POLY_ABILITY_WALK;
		}
		else if(polyAreas[i] == POLY_AREA_WATER)
		{
			polyAreas[i] = POLY_ABILITY_SWIM;
		}
		else
		{
			polyAreas[i] = POLY_ABILITY_ALL;
		}
	}
}

RecastMap::RecastMap()
{
	InitMesh();
	InitCrowd();
}
RecastMap::~RecastMap()
{
	dtFreeNavMesh(m_navMesh);
	m_navMesh = 0;
	dtFreeTileCache(m_tileCache);
}
void RecastMap::InitMesh()
{
	m_geom = 0;
	m_navMesh = 0;
	m_navQuery = dtAllocNavMeshQuery();
	m_crowd = dtAllocCrowd();
	m_tileCache = 0;
	m_maxTiles = 0;
	m_maxPolysPerTile = 0;
	m_talloc = new LinearAllocator(32000);
	m_tcomp = new FastLZCompressor;
	m_tmproc = new MeshProcess;
}
void RecastMap::LoadMesh(const char* filepath)
{
	InputGeometry* geom = new InputGeometry();
	
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
		rcCalcGridSize(bmin, bmax, RECAST_CELL_SIZE, &gw, &gh);
		const int tw = (gw + RECAST_TILE_SIZE-1) / RECAST_TILE_SIZE;
		const int th = (gh + RECAST_TILE_SIZE-1) / RECAST_TILE_SIZE;

		// Max tiles and max polys affect how the tile IDs are caculated.
		// There are 22 bits available for identifying a tile and a polygon.
		int tileBits = rcMin((int)dtIlog2(dtNextPow2(tw*th*RECAST_EXPECTED_LAYERS_PER_TILE)), 14);
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
int RecastMap::RasterizeTileLayers(const int tx, const int ty, const rcConfig& cfg, TileCacheData* tiles, const int maxTiles)
{
	if (!m_geom || !m_geom->getMesh() || !m_geom->getChunkyMesh())
	{
		return 0;
	}
	struct RasterizationContext
	{
		rcHeightfield* solid;
		unsigned char* triareas;
		rcHeightfieldLayerSet* lset;
		rcCompactHeightfield* chf;
		TileCacheData tiles[RECAST_MAX_LAYERS];
		int ntiles;

		RasterizationContext()
		{
			memset(tiles, 0, sizeof(TileCacheData)*RECAST_MAX_LAYERS);
		}
		~RasterizationContext()
		{
			rcFreeHeightField(solid);
			delete [] triareas;
			rcFreeHeightfieldLayerSet(lset);
			rcFreeCompactHeightfield(chf);
			for (int i = 0; i < RECAST_MAX_LAYERS; ++i)
			{
				dtFree(tiles[i].data);
				tiles[i].data = 0;
			}
		}
	};
	RasterizationContext rc;

	const float* verts = m_geom->getMesh()->getVerts();
	const int nverts = m_geom->getMesh()->getVertCount();
	const rcChunkyTriMesh* chunkyMesh = m_geom->getChunkyMesh();

	// Tile bounds.
	const float tcs = cfg.tileSize * cfg.cs;

	rcConfig tcfg;
	memcpy(&tcfg, &cfg, sizeof(tcfg));

	tcfg.bmin[0] = cfg.bmin[0] + tx*tcs;
	tcfg.bmin[1] = cfg.bmin[1];
	tcfg.bmin[2] = cfg.bmin[2] + ty*tcs;
	tcfg.bmax[0] = cfg.bmin[0] + (tx+1)*tcs;
	tcfg.bmax[1] = cfg.bmax[1];
	tcfg.bmax[2] = cfg.bmin[2] + (ty+1)*tcs;
	tcfg.bmin[0] -= tcfg.borderSize*tcfg.cs;
	tcfg.bmin[2] -= tcfg.borderSize*tcfg.cs;
	tcfg.bmax[0] += tcfg.borderSize*tcfg.cs;
	tcfg.bmax[2] += tcfg.borderSize*tcfg.cs;

	// Allocate voxel heightfield where we rasterize our input data to.
	rc.solid = rcAllocHeightfield();
	if (!rc.solid)
	{
		return 0;
	}
	if (!rcCreateHeightfield(*rc.solid, tcfg.width, tcfg.height, tcfg.bmin, tcfg.bmax, tcfg.cs, tcfg.ch))
	{
		return 0;
	}

	// Allocate array that can hold triangle flags.
	// If you have multiple meshes you need to process, allocate
	// and array which can hold the max number of triangles you need to process.
	rc.triareas = new unsigned char[chunkyMesh->maxTrisPerChunk];
	if (!rc.triareas)
	{
		return 0;
	}

	float tbmin[2], tbmax[2];
	tbmin[0] = tcfg.bmin[0];
	tbmin[1] = tcfg.bmin[2];
	tbmax[0] = tcfg.bmax[0];
	tbmax[1] = tcfg.bmax[2];
	int cid[512];// TODO: Make grow when returning too many items.
	const int ncid = rcGetChunksOverlappingRect(chunkyMesh, tbmin, tbmax, cid, 512);
	if (!ncid)
	{
		return 0; // empty
	}

	for (int i = 0; i < ncid; ++i)
	{
		const rcChunkyTriMeshNode& node = chunkyMesh->nodes[cid[i]];
		const int* tris = &chunkyMesh->tris[node.i*3];
		const int ntris = node.n;

		memset(rc.triareas, 0, ntris*sizeof(unsigned char));
		rcMarkWalkableTriangles(tcfg.walkableSlopeAngle,
			verts, nverts, tris, ntris, rc.triareas);

		rcRasterizeTriangles(verts, nverts, tris, rc.triareas, ntris, *rc.solid, tcfg.walkableClimb);
	}

	// Once all geometry is rasterized, we do initial pass of filtering to
	// remove unwanted overhangs caused by the conservative rasterization
	// as well as filter spans where the character cannot possibly stand.
	rcFilterLowHangingWalkableObstacles(tcfg.walkableClimb, *rc.solid);
	rcFilterLedgeSpans(tcfg.walkableHeight, tcfg.walkableClimb, *rc.solid);
	rcFilterWalkableLowHeightSpans(tcfg.walkableHeight, *rc.solid);
	rc.chf = rcAllocCompactHeightfield();
	if (!rc.chf)
	{
		return 0;
	}
	if (!rcBuildCompactHeightfield(tcfg.walkableHeight, tcfg.walkableClimb, *rc.solid, *rc.chf))
	{
		return 0;
	}

	// Erode the walkable area by agent radius.
	if (!rcErodeWalkableArea(tcfg.walkableRadius, *rc.chf))
	{
		return 0;
	}

	// (Optional) Mark areas.
	const ConvexVolume* vols = m_geom->getConvexVolumes();
	for (int i  = 0; i < m_geom->getConvexVolumeCount(); ++i)
	{
		rcMarkConvexPolyArea(vols[i].verts, vols[i].nverts,
			vols[i].hmin, vols[i].hmax,
			(unsigned char)vols[i].area, *rc.chf);
	}

	rc.lset = rcAllocHeightfieldLayerSet();
	if (!rc.lset)
	{
		return 0;
	}
	if (!rcBuildHeightfieldLayers(*rc.chf, tcfg.borderSize, tcfg.walkableHeight, *rc.lset))
	{
		return 0;
	}

	rc.ntiles = 0;
	for (int i = 0; i < rcMin(rc.lset->nlayers, RECAST_MAX_LAYERS); ++i)
	{
		TileCacheData* tile = &rc.tiles[rc.ntiles++];
		const rcHeightfieldLayer* layer = &rc.lset->layers[i];

		// Store header
		dtTileCacheLayerHeader header;
		header.magic = DT_TILECACHE_MAGIC;
		header.version = DT_TILECACHE_VERSION;

		// Tile layer location in the navmesh.
		header.tx = tx;
		header.ty = ty;
		header.tlayer = i;
		dtVcopy(header.bmin, layer->bmin);
		dtVcopy(header.bmax, layer->bmax);

		// Tile info.
		header.width = (unsigned char)layer->width;
		header.height = (unsigned char)layer->height;
		header.minx = (unsigned char)layer->minx;
		header.maxx = (unsigned char)layer->maxx;
		header.miny = (unsigned char)layer->miny;
		header.maxy = (unsigned char)layer->maxy;
		header.hmin = (unsigned short)layer->hmin;
		header.hmax = (unsigned short)layer->hmax;

		dtStatus status = dtBuildTileCacheLayer(m_tcomp, &header, layer->heights, layer->areas, layer->cons,
			&tile->data, &tile->dataSize);
		if (dtStatusFailed(status))
		{
			return 0;
		}
	}

	// Transfer ownsership of tile data from build context to the caller.
	int n = 0;
	for (int i = 0; i < rcMin(rc.ntiles, maxTiles); ++i)
	{
		tiles[n++] = rc.tiles[i];
		rc.tiles[i].data = 0;
		rc.tiles[i].dataSize = 0;
	}

	return n;
}
bool RecastMap::BuildMesh()
{
	dtStatus status;
	if (!m_geom || !m_geom->getMesh()) return false;
	// Init cache
	const float* bmin = m_geom->getMeshBoundsMin();
	const float* bmax = m_geom->getMeshBoundsMax();
	int gw = 0, gh = 0;
	rcCalcGridSize(bmin, bmax, RECAST_CELL_SIZE, &gw, &gh);
	const int ts = RECAST_TILE_SIZE;
	const int tw = (gw + ts-1) / ts;
	const int th = (gh + ts-1) / ts;
	// Generation params.
	rcConfig cfg;
	memset(&cfg, 0, sizeof(cfg));
	cfg.cs = RECAST_CELL_SIZE;
	cfg.ch = RECAST_CELL_HEIGHT;
	cfg.walkableSlopeAngle = RECAST_AGENT_MAX_SLOPE;
	cfg.walkableHeight = (int)ceilf(RECAST_AGENT_HEIGHT / cfg.ch);
	cfg.walkableClimb = (int)floorf(RECAST_AGENT_MAX_CLIMB / cfg.ch);
	cfg.walkableRadius = (int)ceilf(RECAST_AGENT_MIN_RADIUS / cfg.cs);
	cfg.maxEdgeLen = (int)(RECAST_EDGE_MAX_LENGTH / RECAST_CELL_SIZE);
	cfg.maxSimplificationError = RECAST_EDGE_MAX_ERROR;
	cfg.minRegionArea = rcSqr(RECAST_REGION_MIN_SIZE);		// Note: area = size*size
	cfg.mergeRegionArea = rcSqr(RECAST_REGION_MERGE_SIZE);	// Note: area = size*size
	cfg.maxVertsPerPoly = RECAST_VERTEX_PER_POLY;
	cfg.tileSize = RECAST_TILE_SIZE;
	cfg.borderSize = cfg.walkableRadius + 3; // Reserve enough padding.
	cfg.width = cfg.tileSize + cfg.borderSize*2;
	cfg.height = cfg.tileSize + cfg.borderSize*2;
	cfg.detailSampleDist = RECAST_DETAIL_SAMPLE_DISTANCE < 0.9f ? 0 : RECAST_CELL_SIZE * RECAST_DETAIL_SAMPLE_DISTANCE;
	cfg.detailSampleMaxError = RECAST_CELL_HEIGHT * RECAST_DETAIL_SAMPLE_MAX_ERROR;
	rcVcopy(cfg.bmin, bmin);
	rcVcopy(cfg.bmax, bmax);
	// Tile cache params.
	dtTileCacheParams tcparams;
	memset(&tcparams, 0, sizeof(tcparams));
	rcVcopy(tcparams.orig, bmin);
	tcparams.cs = RECAST_CELL_SIZE;
	tcparams.ch = RECAST_CELL_HEIGHT;
	tcparams.width = RECAST_TILE_SIZE;
	tcparams.height = RECAST_TILE_SIZE;
	tcparams.walkableHeight = RECAST_AGENT_HEIGHT;
	tcparams.walkableRadius = RECAST_AGENT_MIN_RADIUS;
	tcparams.walkableClimb = RECAST_AGENT_MAX_CLIMB;
	tcparams.maxSimplificationError = RECAST_EDGE_MAX_ERROR;
	tcparams.maxTiles = tw*th*RECAST_EXPECTED_LAYERS_PER_TILE;
	tcparams.maxObstacles = RECAST_MAX_OBSTACLE;

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
	params.tileWidth = RECAST_TILE_SIZE*RECAST_CELL_SIZE;
	params.tileHeight = RECAST_TILE_SIZE*RECAST_CELL_SIZE;
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
			TileCacheData tiles[RECAST_MAX_LAYERS];
			memset(tiles, 0, sizeof(tiles));
			int n = RasterizeTileLayers(x, y, cfg, tiles, RECAST_MAX_LAYERS);
			for (int i = 0; i < n; ++i)
			{
				status = m_tileCache->addTile(tiles[i].data, tiles[i].dataSize, DT_COMPRESSEDTILE_FREE_DATA, 0);
				if (dtStatusFailed(status))
				{
					dtFree(tiles[i].data);
					tiles[i].data = 0;
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
int RecastMap::AddObstacle(const float* pos)
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
int RecastMap::HitTestObstacle(const float* sp, const float* sq)
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
void RecastMap::RemoveObstacle(const float* sp, const float* sq)
{
	if (!m_tileCache)
		return;
	dtObstacleRef ref = HitTestObstacle(sp, sq);
	m_tileCache->removeObstacle(ref);
}
void RecastMap::ClearAllObstacle()
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
void RecastMap::UpdateMesh(const float dt)
{
	if (!m_navMesh)
		return;
	if (!m_tileCache)
		return;
	m_tileCache->update(dt, m_navMesh);
}

// ===================================================================================


void RecastMap::InitCrowd()
{
	if (m_navMesh && m_crowd)
	{
		m_crowd->init(RECAST_MAX_AGENTS, RECAST_AGENT_MAX_RADIUS, m_navMesh);

		// Make polygons with 'disabled' flag invalid.
		m_crowd->getEditableFilter()->setExcludeFlags(POLY_ABILITY_DISABLED);

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
int RecastMap::AddAgent(const float* p)
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
int RecastMap::HitTestAgent(const float* s, const float* p)
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
void RecastMap::RemoveAgent(const int idx)
{
	if (!m_crowd) return;
	m_crowd->removeAgent(idx);
}


void RecastMap::UpdateCrowd(const float dt)
{
	if (!m_navMesh || !m_crowd) return;
	m_crowd->update(dt, 0);
}