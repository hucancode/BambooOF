#include "RecastHelper.h"
#include "fastlz.h"

RasterizationContext::RasterizationContext() :
	solid(0),triareas(0),lset(0),chf(0),ntiles(0)
{
	memset(tiles, 0, sizeof(TileCacheData)*RECAST_MAX_LAYERS);
}

RasterizationContext::~RasterizationContext()
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


int FastLZCompressor::maxCompressedSize(const int bufferSize)
{
	return (int)(bufferSize* 1.05f);
}
dtStatus FastLZCompressor::compress(const unsigned char* buffer, const int bufferSize,
									unsigned char* compressed, const int maxCompressedSize, int* compressedSize)
{
	*compressedSize = fastlz_compress((const void *const)buffer, bufferSize, compressed);
	return DT_SUCCESS;
}
dtStatus FastLZCompressor::decompress(const unsigned char* compressed, const int compressedSize,
									  unsigned char* buffer, const int maxBufferSize, int* bufferSize)
{
	*bufferSize = fastlz_decompress(compressed, compressedSize, buffer, maxBufferSize);
	return *bufferSize < 0 ? DT_FAILURE : DT_SUCCESS;
}


LinearAllocator::LinearAllocator(const int cap) : buffer(0), capacity(0), top(0), high(0)
{
	resize(cap);
}

LinearAllocator::~LinearAllocator()
{
	dtFree(buffer);
}
void LinearAllocator::resize(const int cap)
{
	if (buffer) dtFree(buffer);
	buffer = (unsigned char*)dtAlloc(cap, DT_ALLOC_PERM);
	capacity = cap;
}
void LinearAllocator::reset()
{
	high = dtMax(high, top);
	top = 0;
}
void* LinearAllocator::alloc(const int size)
{
	if (!buffer)
		return 0;
	if (top+size > capacity)
		return 0;
	unsigned char* mem = &buffer[top];
	top += size;
	return mem;
}
void LinearAllocator::free(void* /*ptr*/)
{
	// Empty
}

void MeshProcess::process(struct dtNavMeshCreateParams* params,
						  unsigned char* polyAreas, unsigned short* polyFlags)
{
	// hu: Honestly i don't know what this mesh process do
	return;
	// Update poly flags from areas.
	for (int i = 0; i < params->polyCount; ++i)
	{
		if (polyAreas[i] == DT_TILECACHE_WALKABLE_AREA)
		{
			polyAreas[i] = SAMPLE_POLYFLAGS_WALK;
		}
		else
		{
			polyAreas[i] = SAMPLE_POLYFLAGS_DISABLED;
		}
	}
}

void DebugDrawGL::depthMask(bool state)
{
	glDepthMask(state ? GL_TRUE : GL_FALSE);
}
void DebugDrawGL::texture(bool state)
{
	struct GLCheckerTexture
	{
		GLuint texture_id;
		GLCheckerTexture()
		{
			// Create checker pattern.
			const unsigned int col0 = duRGBA(215,215,215,255);
			const unsigned int col1 = duRGBA(255,255,255,255);
			static const int TSIZE = 64;
			unsigned int data[TSIZE*TSIZE];

			glGenTextures(1, &texture_id);
			glBindTexture(GL_TEXTURE_2D, texture_id);

			int level = 0;
			int size = TSIZE;
			while (size > 0)
			{
				for (int y = 0; y < size; ++y)
					for (int x = 0; x < size; ++x)
						data[x+y*size] = (x==0 || y==0) ? col0 : col1;
				glTexImage2D(GL_TEXTURE_2D, level, GL_RGBA, size,size, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
				size /= 2;
				level++;
			}

			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
		}
		~GLCheckerTexture()
		{
			if (texture_id != 0)
				glDeleteTextures(1, &texture_id);
		}
	};
	static GLCheckerTexture checker_texture;
	if (state)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, checker_texture.texture_id);
	}
	else
	{
		glDisable(GL_TEXTURE_2D);
	}
}

void DebugDrawGL::begin(duDebugDrawPrimitives prim, float size)
{
	switch (prim)
	{
	case DU_DRAW_POINTS:
		glPointSize(size);
		glBegin(GL_POINTS);
		break;
	case DU_DRAW_LINES:
		glLineWidth(size);
		glBegin(GL_LINES);
		break;
	case DU_DRAW_TRIS:
		glBegin(GL_TRIANGLES);
		break;
	case DU_DRAW_QUADS:
		glBegin(GL_QUADS);
		break;
	};
}

void DebugDrawGL::vertex(const float* pos, unsigned int color)
{
	glColor4ubv((GLubyte*)&color);
	glVertex3fv(pos);
}

void DebugDrawGL::vertex(const float x, const float y, const float z, unsigned int color)
{
	glColor4ubv((GLubyte*)&color);
	glVertex3f(x,y,z);
}

void DebugDrawGL::vertex(const float* pos, unsigned int color, const float* uv)
{
	glColor4ubv((GLubyte*)&color);
	glTexCoord2fv(uv);
	glVertex3fv(pos);
}

void DebugDrawGL::vertex(const float x, const float y, const float z, unsigned int color, const float u, const float v)
{
	glColor4ubv((GLubyte*)&color);
	glTexCoord2f(u,v);
	glVertex3f(x,y,z);
}

void DebugDrawGL::end()
{
	glEnd();
	glLineWidth(1.0f);
	glPointSize(1.0f);
}