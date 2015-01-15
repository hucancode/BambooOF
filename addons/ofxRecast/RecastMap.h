#pragma once
#include <string.h>
#include "Recast.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"
#include "DetourCrowd.h"
#include "DetourCommon.h"
#include "DetourTileCacheBuilder.h"
#include "DetourTileCache.h"
#include "DebugDrawGL.h"

#define RECAST_TILE_SIZE 48
#define RECAST_CELL_SIZE 0.3f
#define RECAST_CELL_HEIGHT 0.2f
#define RECAST_AGENT_MAX_RADIUS 0.6f
#define RECAST_AGENT_MIN_RADIUS 0.6f
#define RECAST_AGENT_HEIGHT 2.0f
#define RECAST_AGENT_MAX_SLOPE 45.0f
#define RECAST_AGENT_MAX_CLIMB 2.5f
#define RECAST_REGION_MIN_SIZE 8
#define RECAST_REGION_MERGE_SIZE 20
#define RECAST_EDGE_MAX_LENGTH 12.0f
#define RECAST_EDGE_MAX_ERROR 1.3f
#define RECAST_VERTEX_PER_POLY 6
#define RECAST_DETAIL_SAMPLE_DISTANCE 6.0f
#define RECAST_DETAIL_SAMPLE_MAX_ERROR 1.0f
#define RECAST_MAX_AGENTS 128
#define RECAST_MAX_LAYERS 32
#define RECAST_EXPECTED_LAYERS_PER_TILE 4

enum SamplePolyAreas
{
	SAMPLE_POLYAREA_GROUND,
	SAMPLE_POLYAREA_WATER,
	SAMPLE_POLYAREA_ROAD,
	SAMPLE_POLYAREA_DOOR,
	SAMPLE_POLYAREA_GRASS,
	SAMPLE_POLYAREA_JUMP,
};
enum SamplePolyFlags
{
	SAMPLE_POLYFLAGS_WALK		= 0x01,		// Ability to walk (ground, grass, road)
	SAMPLE_POLYFLAGS_SWIM		= 0x02,		// Ability to swim (water).
	SAMPLE_POLYFLAGS_DOOR		= 0x04,		// Ability to move through doors.
	SAMPLE_POLYFLAGS_JUMP		= 0x08,		// Ability to jump.
	SAMPLE_POLYFLAGS_DISABLED	= 0x10,		// Disabled polygon
	SAMPLE_POLYFLAGS_ALL		= 0xffff	// All abilities.
};

class RecastMap
{
	struct FastLZCompressor : public dtTileCacheCompressor
	{
		virtual int maxCompressedSize(const int bufferSize);

		virtual dtStatus compress(const unsigned char* buffer, const int bufferSize,
			unsigned char* compressed, const int maxCompressedSize, int* compressedSize);

		virtual dtStatus decompress(const unsigned char* compressed, const int compressedSize,
			unsigned char* buffer, const int maxBufferSize, int* bufferSize);
	};

	struct LinearAllocator : public dtTileCacheAlloc
	{
		unsigned char* buffer;
		int capacity;
		int top;
		int high;

		LinearAllocator(const int cap);
		~LinearAllocator();
		void resize(const int cap);
		virtual void reset();
		virtual void* alloc(const int size);
		virtual void free(void* /*ptr*/);
	};

	struct MeshProcess : public dtTileCacheMeshProcess
	{
		virtual void process(struct dtNavMeshCreateParams* params,
			unsigned char* polyAreas, unsigned short* polyFlags);
	};
	struct TileCacheData
	{
		unsigned char* data;
		int dataSize;
	};
	friend class RecastMapRender;
private:
	InputGeometry*		m_geom;
	dtNavMesh*			m_navMesh;
	dtNavMeshQuery*		m_navQuery;
	dtCrowd*			m_crowd;
	dtTileCache*		m_tileCache;
private:
	LinearAllocator*	m_talloc;
	FastLZCompressor*	m_tcomp;
	MeshProcess*		m_tmproc;
	int					m_maxTiles;
	int					m_maxPolysPerTile;
public:
	InputGeometry*		GetGeometry() { return m_geom;}
	dtCrowd*			GetCrowd(){ return m_crowd;}
	dtNavMesh*			GetMesh(){ return m_navMesh;}
	dtNavMeshQuery*		GetQuery(){ return m_navQuery;}
	dtTileCache*		GetTileCache() { return m_tileCache;}
public:
	RecastMap();
	~RecastMap();
	// navigation mesh
private:
	int					RasterizeTileLayers(InputGeometry* geom, const int tx, const int ty, 
							const rcConfig& cfg, TileCacheData* tiles, const int maxTiles);
public:
	void				InitMesh();
	void				LoadMesh(const char* filepath);
	bool				BuildMesh();
	void				UpdateMesh(const float dt);
public:
	// temp obstacle
	int					HitTestObstacle(const float* sp, const float* sq);
	int					AddObstacle(const float* pos);
	void				RemoveObstacle(const float* sp, const float* sq);
	void				ClearAllObstacle();
public:
	// crowd control
	void				InitCrowd();
	int					AddAgent(const float* p);
	int					HitTestAgent(const float* s, const float* p);
	void				RemoveAgent(const int idx);
	void				UpdateCrowd(const float dt);
};