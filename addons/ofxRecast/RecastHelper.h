#pragma once
#include <stdio.h>
#include <stdarg.h>
#include <float.h>
#include <math.h>
#include <string.h>

#include "Recast.h"
#include "DetourNavMesh.h"
#include "DetourNavMeshBuilder.h"
#include "DetourNavMeshQuery.h"
#include "DetourTileCache.h"
#include "DetourTileCacheBuilder.h"
#include "DetourCrowd.h"
#include "DetourCommon.h"

#include "InputGeometry.h"

#include "GL/glew.h"
#include "DebugDraw.h"

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

struct RasterizationContext
{
	rcHeightfield* solid;
	unsigned char* triareas;
	rcHeightfieldLayerSet* lset;
	rcCompactHeightfield* chf;
	TileCacheData tiles[RECAST_MAX_LAYERS];
	int ntiles;

	RasterizationContext();
	~RasterizationContext();
};

class DebugDrawGL : public duDebugDraw
{
public:
	virtual void depthMask(bool state);
	virtual void texture(bool state);
	virtual void begin(duDebugDrawPrimitives prim, float size = 1.0f);
	virtual void vertex(const float* pos, unsigned int color);
	virtual void vertex(const float x, const float y, const float z, unsigned int color);
	virtual void vertex(const float* pos, unsigned int color, const float* uv);
	virtual void vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v);
	virtual void end();
};