//
// Copyright (c) 2009-2010 Mikko Mononen memon@inside.org
//
// This software is provided 'as-is', without any express or implied
// warranty.  In no event will the authors be held liable for any damages
// arising from the use of this software.
// Permission is granted to anyone to use this software for any purpose,
// including commercial applications, and to alter it and redistribute it
// freely, subject to the following restrictions:
// 1. The origin of this software must not be misrepresented; you must not
//    claim that you wrote the original software. If you use this software
//    in a product, an acknowledgment in the product documentation would be
//    appreciated but is not required.
// 2. Altered source versions must be plainly marked as such, and must not be
//    misrepresented as being the original software.
// 3. This notice may not be removed or altered from any source distribution.
//

#define _USE_MATH_DEFINES
#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "Recast.h"
#include "InputGeom.h"
#include "DebugDraw.h"
#include "RecastDebugDraw.h"
#include "DetourNavMesh.h"


struct BoundsItem
{
	float bmin[2];
	float bmax[2];
	int i;
};

static int compareItemX(const void* va, const void* vb)
{
	const BoundsItem* a = (const BoundsItem*)va;
	const BoundsItem* b = (const BoundsItem*)vb;
	if (a->bmin[0] < b->bmin[0])
		return -1;
	if (a->bmin[0] > b->bmin[0])
		return 1;
	return 0;
}

static int compareItemY(const void* va, const void* vb)
{
	const BoundsItem* a = (const BoundsItem*)va;
	const BoundsItem* b = (const BoundsItem*)vb;
	if (a->bmin[1] < b->bmin[1])
		return -1;
	if (a->bmin[1] > b->bmin[1])
		return 1;
	return 0;
}

static void calcExtends(const BoundsItem* items, const int /*nitems*/,
						const int imin, const int imax,
						float* bmin, float* bmax)
{
	bmin[0] = items[imin].bmin[0];
	bmin[1] = items[imin].bmin[1];
	
	bmax[0] = items[imin].bmax[0];
	bmax[1] = items[imin].bmax[1];
	
	for (int i = imin+1; i < imax; ++i)
	{
		const BoundsItem& it = items[i];
		if (it.bmin[0] < bmin[0]) bmin[0] = it.bmin[0];
		if (it.bmin[1] < bmin[1]) bmin[1] = it.bmin[1];
		
		if (it.bmax[0] > bmax[0]) bmax[0] = it.bmax[0];
		if (it.bmax[1] > bmax[1]) bmax[1] = it.bmax[1];
	}
}

inline int longestAxis(float x, float y)
{
	return y > x ? 1 : 0;
}

static void subdivide(BoundsItem* items, int nitems, int imin, int imax, int trisPerChunk,
					  int& curNode, rcChunkyTriMeshNode* nodes, const int maxNodes,
					  int& curTri, int* outTris, const int* inTris)
{
	int inum = imax - imin;
	int icur = curNode;
	
	if (curNode > maxNodes)
		return;

	rcChunkyTriMeshNode& node = nodes[curNode++];
	
	if (inum <= trisPerChunk)
	{
		// Leaf
		calcExtends(items, nitems, imin, imax, node.bmin, node.bmax);
		
		// Copy triangles.
		node.i = curTri;
		node.n = inum;
		
		for (int i = imin; i < imax; ++i)
		{
			const int* src = &inTris[items[i].i*3];
			int* dst = &outTris[curTri*3];
			curTri++;
			dst[0] = src[0];
			dst[1] = src[1];
			dst[2] = src[2];
		}
	}
	else
	{
		// Split
		calcExtends(items, nitems, imin, imax, node.bmin, node.bmax);
		
		int	axis = longestAxis(node.bmax[0] - node.bmin[0],
							   node.bmax[1] - node.bmin[1]);
		
		if (axis == 0)
		{
			// Sort along x-axis
			qsort(items+imin, inum, sizeof(BoundsItem), compareItemX);
		}
		else if (axis == 1)
		{
			// Sort along y-axis
			qsort(items+imin, inum, sizeof(BoundsItem), compareItemY);
		}
		
		int isplit = imin+inum/2;
		
		// Left
		subdivide(items, nitems, imin, isplit, trisPerChunk, curNode, nodes, maxNodes, curTri, outTris, inTris);
		// Right
		subdivide(items, nitems, isplit, imax, trisPerChunk, curNode, nodes, maxNodes, curTri, outTris, inTris);
		
		int iescape = curNode - icur;
		// Negative index means escape.
		node.i = -iescape;
	}
}

bool rcCreateChunkyTriMesh(const float* verts, const int* tris, int ntris,
						   int trisPerChunk, rcChunkyTriMesh* cm)
{
	int nchunks = (ntris + trisPerChunk-1) / trisPerChunk;

	cm->nodes = new rcChunkyTriMeshNode[nchunks*4];
	if (!cm->nodes)
		return false;
		
	cm->tris = new int[ntris*3];
	if (!cm->tris)
		return false;
		
	cm->ntris = ntris;

	// Build tree
	BoundsItem* items = new BoundsItem[ntris];
	if (!items)
		return false;

	for (int i = 0; i < ntris; i++)
	{
		const int* t = &tris[i*3];
		BoundsItem& it = items[i];
		it.i = i;
		// Calc triangle XZ bounds.
		it.bmin[0] = it.bmax[0] = verts[t[0]*3+0];
		it.bmin[1] = it.bmax[1] = verts[t[0]*3+2];
		for (int j = 1; j < 3; ++j)
		{
			const float* v = &verts[t[j]*3];
			if (v[0] < it.bmin[0]) it.bmin[0] = v[0]; 
			if (v[2] < it.bmin[1]) it.bmin[1] = v[2]; 

			if (v[0] > it.bmax[0]) it.bmax[0] = v[0]; 
			if (v[2] > it.bmax[1]) it.bmax[1] = v[2]; 
		}
	}

	int curTri = 0;
	int curNode = 0;
	subdivide(items, ntris, 0, ntris, trisPerChunk, curNode, cm->nodes, nchunks*4, curTri, cm->tris, tris);
	
	delete [] items;
	
	cm->nnodes = curNode;
	
	// Calc max tris per node.
	cm->maxTrisPerChunk = 0;
	for (int i = 0; i < cm->nnodes; ++i)
	{
		rcChunkyTriMeshNode& node = cm->nodes[i];
		const bool isLeaf = node.i >= 0;
		if (!isLeaf) continue;
		if (node.n > cm->maxTrisPerChunk)
			cm->maxTrisPerChunk = node.n;
	}
	 
	return true;
}


inline bool checkOverlapRect(const float amin[2], const float amax[2],
							 const float bmin[2], const float bmax[2])
{
	bool overlap = true;
	overlap = (amin[0] > bmax[0] || amax[0] < bmin[0]) ? false : overlap;
	overlap = (amin[1] > bmax[1] || amax[1] < bmin[1]) ? false : overlap;
	return overlap;
}

int rcGetChunksOverlappingRect(const rcChunkyTriMesh* cm,
							   float bmin[2], float bmax[2],
							   int* ids, const int maxIds)
{
	// Traverse tree
	int i = 0;
	int n = 0;
	while (i < cm->nnodes)
	{
		const rcChunkyTriMeshNode* node = &cm->nodes[i];
		const bool overlap = checkOverlapRect(bmin, bmax, node->bmin, node->bmax);
		const bool isLeafNode = node->i >= 0;
		
		if (isLeafNode && overlap)
		{
			if (n < maxIds)
			{
				ids[n] = i;
				n++;
			}
		}
		
		if (overlap || isLeafNode)
			i++;
		else
		{
			const int escapeIndex = -node->i;
			i += escapeIndex;
		}
	}
	
	return n;
}



static bool checkOverlapSegment(const float p[2], const float q[2],
								const float bmin[2], const float bmax[2])
{
	static const float EPSILON = 1e-6f;

	float tmin = 0;
	float tmax = 1;
	float d[2];
	d[0] = q[0] - p[0];
	d[1] = q[1] - p[1];
	
	for (int i = 0; i < 2; i++)
	{
		if (fabsf(d[i]) < EPSILON)
		{
			// Ray is parallel to slab. No hit if origin not within slab
			if (p[i] < bmin[i] || p[i] > bmax[i])
				return false;
		}
		else
		{
			// Compute intersection t value of ray with near and far plane of slab
			float ood = 1.0f / d[i];
			float t1 = (bmin[i] - p[i]) * ood;
			float t2 = (bmax[i] - p[i]) * ood;
			if (t1 > t2) { float tmp = t1; t1 = t2; t2 = tmp; }
			if (t1 > tmin) tmin = t1;
			if (t2 < tmax) tmax = t2;
			if (tmin > tmax) return false;
		}
	}
	return true;
}

int rcGetChunksOverlappingSegment(const rcChunkyTriMesh* cm,
								  float p[2], float q[2],
								  int* ids, const int maxIds)
{
	// Traverse tree
	int i = 0;
	int n = 0;
	while (i < cm->nnodes)
	{
		const rcChunkyTriMeshNode* node = &cm->nodes[i];
		const bool overlap = checkOverlapSegment(p, q, node->bmin, node->bmax);
		const bool isLeafNode = node->i >= 0;
		
		if (isLeafNode && overlap)
		{
			if (n < maxIds)
			{
				ids[n] = i;
				n++;
			}
		}
		
		if (overlap || isLeafNode)
			i++;
		else
		{
			const int escapeIndex = -node->i;
			i += escapeIndex;
		}
	}
	
	return n;
}

rcMeshLoaderObj::rcMeshLoaderObj() :
	m_scale(1.0f),
	m_verts(0),
	m_tris(0),
	m_normals(0),
	m_vertCount(0),
	m_triCount(0)
{
}

rcMeshLoaderObj::~rcMeshLoaderObj()
{
	delete [] m_verts;
	delete [] m_normals;
	delete [] m_tris;
}
		
void rcMeshLoaderObj::addVertex(float x, float y, float z, int& cap)
{
	if (m_vertCount+1 > cap)
	{
		cap = !cap ? 8 : cap*2;
		float* nv = new float[cap*3];
		if (m_vertCount)
			memcpy(nv, m_verts, m_vertCount*3*sizeof(float));
		delete [] m_verts;
		m_verts = nv;
	}
	float* dst = &m_verts[m_vertCount*3];
	*dst++ = x*m_scale;
	*dst++ = y*m_scale;
	*dst++ = z*m_scale;
	m_vertCount++;
}

void rcMeshLoaderObj::addTriangle(int a, int b, int c, int& cap)
{
	if (m_triCount+1 > cap)
	{
		cap = !cap ? 8 : cap*2;
		int* nv = new int[cap*3];
		if (m_triCount)
			memcpy(nv, m_tris, m_triCount*3*sizeof(int));
		delete [] m_tris;
		m_tris = nv;
	}
	int* dst = &m_tris[m_triCount*3];
	*dst++ = a;
	*dst++ = b;
	*dst++ = c;
	m_triCount++;
}

static char* parseRow(char* buf, char* bufEnd, char* row, int len)
{
	bool cont = false;
	bool start = true;
	bool done = false;
	int n = 0;
	while (!done && buf < bufEnd)
	{
		char c = *buf;
		buf++;
		// multirow
		switch (c)
		{
			case '\\':
				cont = true; // multirow
				break;
			case '\n':
				if (start) break;
				done = true;
				break;
			case '\r':
				break;
			case '\t':
			case ' ':
				if (start) break;
			default:
				start = false;
				cont = false;
				row[n++] = c;
				if (n >= len-1)
					done = true;
				break;
		}
	}
	row[n] = '\0';
	return buf;
}

static int parseFace(char* row, int* data, int n, int vcnt)
{
	int j = 0;
	while (*row != '\0')
	{
		// Skip initial white space
		while (*row != '\0' && (*row == ' ' || *row == '\t'))
			row++;
		char* s = row;
		// Find vertex delimiter and terminated the string there for conversion.
		while (*row != '\0' && *row != ' ' && *row != '\t')
		{
			if (*row == '/') *row = '\0';
			row++;
		}
		if (*s == '\0')
			continue;
		int vi = atoi(s);
		data[j++] = vi < 0 ? vi+vcnt : vi-1;
		if (j >= n) return j;
	}
	return j;
}

bool rcMeshLoaderObj::load(const char* filename)
{
	char* buf = 0;
	FILE* fp = fopen(filename, "rb");
	if (!fp)
		return false;
	fseek(fp, 0, SEEK_END);
	int bufSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	buf = new char[bufSize];
	if (!buf)
	{
		fclose(fp);
		return false;
	}
	fread(buf, bufSize, 1, fp);
	fclose(fp);

	char* src = buf;
	char* srcEnd = buf + bufSize;
	char row[512];
	int face[32];
	float x,y,z;
	int nv;
	int vcap = 0;
	int tcap = 0;
	
	while (src < srcEnd)
	{
		// Parse one row
		row[0] = '\0';
		src = parseRow(src, srcEnd, row, sizeof(row)/sizeof(char));
		// Skip comments
		if (row[0] == '#') continue;
		if (row[0] == 'v' && row[1] != 'n' && row[1] != 't')
		{
			// Vertex pos
			sscanf(row+1, "%f %f %f", &x, &y, &z);
			addVertex(x, y, z, vcap);
		}
		if (row[0] == 'f')
		{
			// Faces
			nv = parseFace(row+1, face, 32, m_vertCount);
			for (int i = 2; i < nv; ++i)
			{
				const int a = face[0];
				const int b = face[i-1];
				const int c = face[i];
				if (a < 0 || a >= m_vertCount || b < 0 || b >= m_vertCount || c < 0 || c >= m_vertCount)
					continue;
				addTriangle(a, b, c, tcap);
			}
		}
	}

	delete [] buf;

	// Calculate normals.
	m_normals = new float[m_triCount*3];
	for (int i = 0; i < m_triCount*3; i += 3)
	{
		const float* v0 = &m_verts[m_tris[i]*3];
		const float* v1 = &m_verts[m_tris[i+1]*3];
		const float* v2 = &m_verts[m_tris[i+2]*3];
		float e0[3], e1[3];
		for (int j = 0; j < 3; ++j)
		{
			e0[j] = v1[j] - v0[j];
			e1[j] = v2[j] - v0[j];
		}
		float* n = &m_normals[i];
		n[0] = e0[1]*e1[2] - e0[2]*e1[1];
		n[1] = e0[2]*e1[0] - e0[0]*e1[2];
		n[2] = e0[0]*e1[1] - e0[1]*e1[0];
		float d = sqrtf(n[0]*n[0] + n[1]*n[1] + n[2]*n[2]);
		if (d > 0)
		{
			d = 1.0f/d;
			n[0] *= d;
			n[1] *= d;
			n[2] *= d;
		}
	}
	
	strncpy(m_filename, filename, sizeof(m_filename));
	m_filename[sizeof(m_filename)-1] = '\0';
	
	return true;
}


static bool intersectSegmentTriangle(const float* sp, const float* sq,
									 const float* a, const float* b, const float* c,
									 float &t)
{
	float v, w;
	float ab[3], ac[3], qp[3], ap[3], norm[3], e[3];
	rcVsub(ab, b, a);
	rcVsub(ac, c, a);
	rcVsub(qp, sp, sq);
	
	// Compute triangle normal. Can be precalculated or cached if
	// intersecting multiple segments against the same triangle
	rcVcross(norm, ab, ac);
	
	// Compute denominator d. If d <= 0, segment is parallel to or points
	// away from triangle, so exit early
	float d = rcVdot(qp, norm);
	if (d <= 0.0f) return false;
	
	// Compute intersection t value of pq with plane of triangle. A ray
	// intersects iff 0 <= t. Segment intersects iff 0 <= t <= 1. Delay
	// dividing by d until intersection has been found to pierce triangle
	rcVsub(ap, sp, a);
	t = rcVdot(ap, norm);
	if (t < 0.0f) return false;
	if (t > d) return false; // For segment; exclude this code line for a ray test
	
	// Compute barycentric coordinate components and test if within bounds
	rcVcross(e, qp, ap);
	v = rcVdot(ac, e);
	if (v < 0.0f || v > d) return false;
	w = -rcVdot(ab, e);
	if (w < 0.0f || v + w > d) return false;
	
	// Segment/ray intersects triangle. Perform delayed division
	t /= d;
	
	return true;
}

InputGeom::InputGeom() :
	m_chunkyMesh(0),
	m_mesh(0),
	m_offMeshConCount(0),
	m_volumeCount(0)
{
}

InputGeom::~InputGeom()
{
	delete m_chunkyMesh;
	delete m_mesh;
}
		
bool InputGeom::loadMesh(const char* filepath)
{
	if (m_mesh)
	{
		delete m_chunkyMesh;
		m_chunkyMesh = 0;
		delete m_mesh;
		m_mesh = 0;
	}
	m_offMeshConCount = 0;
	m_volumeCount = 0;
	
	m_mesh = new rcMeshLoaderObj;
	if (!m_mesh)
	{
		return false;
	}
	if (!m_mesh->load(filepath))
	{
		return false;
	}

	rcCalcBounds(m_mesh->getVerts(), m_mesh->getVertCount(), m_meshBMin, m_meshBMax);

	m_chunkyMesh = new rcChunkyTriMesh;
	if (!m_chunkyMesh)
	{
		return false;
	}
	if (!rcCreateChunkyTriMesh(m_mesh->getVerts(), m_mesh->getTris(), m_mesh->getTriCount(), 256, m_chunkyMesh))
	{
		return false;
	}		

	return true;
}

bool InputGeom::load(const char* filePath)
{
	char* buf = 0;
	FILE* fp = fopen(filePath, "rb");
	if (!fp)
		return false;
	fseek(fp, 0, SEEK_END);
	int bufSize = ftell(fp);
	fseek(fp, 0, SEEK_SET);
	buf = new char[bufSize];
	if (!buf)
	{
		fclose(fp);
		return false;
	}
	fread(buf, bufSize, 1, fp);
	fclose(fp);
	
	m_offMeshConCount = 0;
	m_volumeCount = 0;
	delete m_mesh;
	m_mesh = 0;

	char* src = buf;
	char* srcEnd = buf + bufSize;
	char row[512];
	while (src < srcEnd)
	{
		// Parse one row
		row[0] = '\0';
		src = parseRow(src, srcEnd, row, sizeof(row)/sizeof(char));
		if (row[0] == 'f')
		{
			// File name.
			const char* name = row+1;
			// Skip white spaces
			while (*name && isspace(*name))
				name++;
			if (*name)
			{
				if (!loadMesh(name))
				{
					delete [] buf;
					return false;
				}
			}
		}
		else if (row[0] == 'c')
		{
			// Off-mesh connection
			if (m_offMeshConCount < MAX_OFFMESH_CONNECTIONS)
			{
				float* v = &m_offMeshConVerts[m_offMeshConCount*3*2];
				int bidir, area = 0, flags = 0;
				float rad;
				sscanf(row+1, "%f %f %f  %f %f %f %f %d %d %d",
					   &v[0], &v[1], &v[2], &v[3], &v[4], &v[5], &rad, &bidir, &area, &flags);
				m_offMeshConRads[m_offMeshConCount] = rad;
				m_offMeshConDirs[m_offMeshConCount] = (unsigned char)bidir;
				m_offMeshConAreas[m_offMeshConCount] = (unsigned char)area;
				m_offMeshConFlags[m_offMeshConCount] = (unsigned short)flags;
				m_offMeshConCount++;
			}
		}
		else if (row[0] == 'v')
		{
			// Convex volumes
			if (m_volumeCount < MAX_VOLUMES)
			{
				ConvexVolume* vol = &m_volumes[m_volumeCount++];
				sscanf(row+1, "%d %d %f %f", &vol->nverts, &vol->area, &vol->hmin, &vol->hmax);
				for (int i = 0; i < vol->nverts; ++i)
				{
					row[0] = '\0';
					src = parseRow(src, srcEnd, row, sizeof(row)/sizeof(char));
					sscanf(row, "%f %f %f", &vol->verts[i*3+0], &vol->verts[i*3+1], &vol->verts[i*3+2]);
				}
			}
		}
	}
	
	delete [] buf;
	
	return true;
}

bool InputGeom::save(const char* filepath)
{
	if (!m_mesh) return false;
	
	FILE* fp = fopen(filepath, "w");
	if (!fp) return false;
	
	// Store mesh filename.
	fprintf(fp, "f %s\n", m_mesh->getFileName());
	
	// Store off-mesh links.
	for (int i = 0; i < m_offMeshConCount; ++i)
	{
		const float* v = &m_offMeshConVerts[i*3*2];
		const float rad = m_offMeshConRads[i];
		const int bidir = m_offMeshConDirs[i];
		const int area = m_offMeshConAreas[i];
		const int flags = m_offMeshConFlags[i];
		fprintf(fp, "c %f %f %f  %f %f %f  %f %d %d %d\n",
				v[0], v[1], v[2], v[3], v[4], v[5], rad, bidir, area, flags);
	}

	// Convex volumes
	for (int i = 0; i < m_volumeCount; ++i)
	{
		ConvexVolume* vol = &m_volumes[i];
		fprintf(fp, "v %d %d %f %f\n", vol->nverts, vol->area, vol->hmin, vol->hmax);
		for (int j = 0; j < vol->nverts; ++j)
			fprintf(fp, "%f %f %f\n", vol->verts[j*3+0], vol->verts[j*3+1], vol->verts[j*3+2]);
	}
	
	fclose(fp);
	
	return true;
}

static bool isectSegAABB(const float* sp, const float* sq,
						 const float* amin, const float* amax,
						 float& tmin, float& tmax)
{
	static const float EPS = 1e-6f;
	
	float d[3];
	d[0] = sq[0] - sp[0];
	d[1] = sq[1] - sp[1];
	d[2] = sq[2] - sp[2];
	tmin = 0.0;
	tmax = 1.0f;
	
	for (int i = 0; i < 3; i++)
	{
		if (fabsf(d[i]) < EPS)
		{
			if (sp[i] < amin[i] || sp[i] > amax[i])
				return false;
		}
		else
		{
			const float ood = 1.0f / d[i];
			float t1 = (amin[i] - sp[i]) * ood;
			float t2 = (amax[i] - sp[i]) * ood;
			if (t1 > t2) { float tmp = t1; t1 = t2; t2 = tmp; }
			if (t1 > tmin) tmin = t1;
			if (t2 < tmax) tmax = t2;
			if (tmin > tmax) 
				return false;
		}
	}
	
	return true;
}


bool InputGeom::raycastMesh(float* src, float* dst, float& tmin)
{
	float dir[3];
	rcVsub(dir, dst, src);

	// Prune hit ray.
	float btmin, btmax;
	if (!isectSegAABB(src, dst, m_meshBMin, m_meshBMax, btmin, btmax))
		return false;
	float p[2], q[2];
	p[0] = src[0] + (dst[0]-src[0])*btmin;
	p[1] = src[2] + (dst[2]-src[2])*btmin;
	q[0] = src[0] + (dst[0]-src[0])*btmax;
	q[1] = src[2] + (dst[2]-src[2])*btmax;
	
	int cid[512];
	const int ncid = rcGetChunksOverlappingSegment(m_chunkyMesh, p, q, cid, 512);
	if (!ncid)
		return false;
	
	tmin = 1.0f;
	bool hit = false;
	const float* verts = m_mesh->getVerts();
	
	for (int i = 0; i < ncid; ++i)
	{
		const rcChunkyTriMeshNode& node = m_chunkyMesh->nodes[cid[i]];
		const int* tris = &m_chunkyMesh->tris[node.i*3];
		const int ntris = node.n;

		for (int j = 0; j < ntris*3; j += 3)
		{
			float t = 1;
			if (intersectSegmentTriangle(src, dst,
										 &verts[tris[j]*3],
										 &verts[tris[j+1]*3],
										 &verts[tris[j+2]*3], t))
			{
				if (t < tmin)
					tmin = t;
				hit = true;
			}
		}
	}
	
	return hit;
}

void InputGeom::addOffMeshConnection(const float* spos, const float* epos, const float rad,
									 unsigned char bidir, unsigned char area, unsigned short flags)
{
	if (m_offMeshConCount >= MAX_OFFMESH_CONNECTIONS) return;
	float* v = &m_offMeshConVerts[m_offMeshConCount*3*2];
	m_offMeshConRads[m_offMeshConCount] = rad;
	m_offMeshConDirs[m_offMeshConCount] = bidir;
	m_offMeshConAreas[m_offMeshConCount] = area;
	m_offMeshConFlags[m_offMeshConCount] = flags;
	m_offMeshConId[m_offMeshConCount] = 1000 + m_offMeshConCount;
	rcVcopy(&v[0], spos);
	rcVcopy(&v[3], epos);
	m_offMeshConCount++;
}

void InputGeom::deleteOffMeshConnection(int i)
{
	m_offMeshConCount--;
	float* src = &m_offMeshConVerts[m_offMeshConCount*3*2];
	float* dst = &m_offMeshConVerts[i*3*2];
	rcVcopy(&dst[0], &src[0]);
	rcVcopy(&dst[3], &src[3]);
	m_offMeshConRads[i] = m_offMeshConRads[m_offMeshConCount];
	m_offMeshConDirs[i] = m_offMeshConDirs[m_offMeshConCount];
	m_offMeshConAreas[i] = m_offMeshConAreas[m_offMeshConCount];
	m_offMeshConFlags[i] = m_offMeshConFlags[m_offMeshConCount];
}

void InputGeom::drawOffMeshConnections(duDebugDraw* dd, bool hilight)
{
	unsigned int conColor = duRGBA(192,0,128,192);
	unsigned int baseColor = duRGBA(0,0,0,64);
	dd->depthMask(false);

	dd->begin(DU_DRAW_LINES, 2.0f);
	for (int i = 0; i < m_offMeshConCount; ++i)
	{
		float* v = &m_offMeshConVerts[i*3*2];

		dd->vertex(v[0],v[1],v[2], baseColor);
		dd->vertex(v[0],v[1]+0.2f,v[2], baseColor);
		
		dd->vertex(v[3],v[4],v[5], baseColor);
		dd->vertex(v[3],v[4]+0.2f,v[5], baseColor);
		
		duAppendCircle(dd, v[0],v[1]+0.1f,v[2], m_offMeshConRads[i], baseColor);
		duAppendCircle(dd, v[3],v[4]+0.1f,v[5], m_offMeshConRads[i], baseColor);

		if (hilight)
		{
			duAppendArc(dd, v[0],v[1],v[2], v[3],v[4],v[5], 0.25f,
						(m_offMeshConDirs[i]&1) ? 0.6f : 0.0f, 0.6f, conColor);
		}
	}	
	dd->end();

	dd->depthMask(true);
}

void InputGeom::addConvexVolume(const float* verts, const int nverts,
								const float minh, const float maxh, unsigned char area)
{
	if (m_volumeCount >= MAX_VOLUMES) return;
	ConvexVolume* vol = &m_volumes[m_volumeCount++];
	memset(vol, 0, sizeof(ConvexVolume));
	memcpy(vol->verts, verts, sizeof(float)*3*nverts);
	vol->hmin = minh;
	vol->hmax = maxh;
	vol->nverts = nverts;
	vol->area = area;
}

void InputGeom::deleteConvexVolume(int i)
{
	m_volumeCount--;
	m_volumes[i] = m_volumes[m_volumeCount];
}

void InputGeom::drawConvexVolumes(struct duDebugDraw* dd, bool /*hilight*/)
{
	dd->depthMask(false);

	dd->begin(DU_DRAW_TRIS);
	
	for (int i = 0; i < m_volumeCount; ++i)
	{
		const ConvexVolume* vol = &m_volumes[i];
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
	for (int i = 0; i < m_volumeCount; ++i)
	{
		const ConvexVolume* vol = &m_volumes[i];
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
	for (int i = 0; i < m_volumeCount; ++i)
	{
		const ConvexVolume* vol = &m_volumes[i];
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
