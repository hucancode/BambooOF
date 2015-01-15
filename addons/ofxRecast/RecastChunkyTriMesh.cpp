#include <math.h>
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include "RecastChunkyTriMesh.h"


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
	struct local
	{
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
	};
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
		src = local::parseRow(src, srcEnd, row, sizeof(row)/sizeof(char));
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
