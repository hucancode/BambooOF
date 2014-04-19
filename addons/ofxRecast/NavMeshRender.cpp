#include "NavMeshRender.h"
// include some opengl header here
NavMeshRender()
{
	glViewport(0, 0, width, height);
	glClearColor(0.3f, 0.3f, 0.32f, 1.0f);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
}
~NavMeshRender()
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
	GLint matrix_v[4];
	glGetDoublev(GL_PROJECTION_MATRIX, matrix_p);
	glGetDoublev(GL_MODELVIEW_MATRIX, matrix_m);
	glGetIntegerv(GL_VIEWPORT, matrix_v);
	float rays[3], raye[3];
	GLdouble x, y, z;
	gluUnProject(screen_pos[0], screen_pos[1], 0.0f, matrix_m, matrix_p, matrix_v, &x, &y, &z);
	rays[0] = (float)x; rays[1] = (float)y; rays[2] = (float)z;
	gluUnProject(screen_pos[0], screen_pos[1], 1.0f, matrix_m, matrix_p, matrix_v, &x, &y, &z);
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
	// ========================================== setup
	glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	{
		GLfloat distance_to_eye = 0.0f;
		distance_to_eye = scrollZoom*2.0f;
		glOrtho(
			-50-distance_to_eye, 50+distance_to_eye, 
			-50-distance_to_eye, 50+distance_to_eye, 
			-1000, 1000);
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	glRotatef(m_RotateX,1,0,0);
	glRotatef(m_RotateY,0,1,0);
	glTranslatef(-m_CamX, -m_CamY, -m_CamZ);
	// ========================================== draw
	duDebugDrawTriMeshSlope(&dd, m_geom->getMesh()->getVerts(), m_geom->getMesh()->getVertCount(),
								m_geom->getMesh()->getTris(), m_geom->getMesh()->getNormals(), m_geom->getMesh()->getTriCount(),
								m_agentMaxSlope, texScale);
	m_geom->drawOffMeshConnections(&dd);
	if (m_tileCache && m_drawMode == DRAWMODE_CACHE_BOUNDS)
		drawTiles(&dd, m_tileCache);
	
	if (m_tileCache)
		drawObstacles(&dd, m_tileCache);
	glDepthMask(GL_FALSE);
	
	// Draw bounds
	const float* bmin = m_geom->getMeshBoundsMin();
	const float* bmax = m_geom->getMeshBoundsMax();
	duDebugDrawBoxWire(&dd, bmin[0],bmin[1],bmin[2], bmax[0],bmax[1],bmax[2], duRGBA(255,255,255,128), 1.0f);
	
	// Tiling grid.
	int gw = 0, gh = 0;
	rcCalcGridSize(bmin, bmax, m_cellSize, &gw, &gh);
	const int tw = (gw + (int)m_tileSize-1) / (int)m_tileSize;
	const int th = (gh + (int)m_tileSize-1) / (int)m_tileSize;
	const float s = m_tileSize*m_cellSize;
	duDebugDrawGridXZ(&dd, bmin[0],bmin[1],bmin[2], tw,th, s, duRGBA(0,0,0,64), 1.0f);
	
	if (m_navMesh && m_navQuery &&
		(m_drawMode == DRAWMODE_NAVMESH ||
		 m_drawMode == DRAWMODE_NAVMESH_TRANS ||
		 m_drawMode == DRAWMODE_NAVMESH_BVTREE ||
		 m_drawMode == DRAWMODE_NAVMESH_NODES ||
		 m_drawMode == DRAWMODE_NAVMESH_PORTALS ||
		 m_drawMode == DRAWMODE_NAVMESH_INVIS))
	{
		if (m_drawMode != DRAWMODE_NAVMESH_INVIS)
			duDebugDrawNavMeshWithClosedList(&dd, *m_navMesh, *m_navQuery, m_navMeshDrawFlags/*|DU_DRAWNAVMESH_COLOR_TILES*/);
		if (m_drawMode == DRAWMODE_NAVMESH_BVTREE)
			duDebugDrawNavMeshBVTree(&dd, *m_navMesh);
		if (m_drawMode == DRAWMODE_NAVMESH_PORTALS)
			duDebugDrawNavMeshPortals(&dd, *m_navMesh);
		if (m_drawMode == DRAWMODE_NAVMESH_NODES)
			duDebugDrawNavMeshNodes(&dd, *m_navQuery);
		duDebugDrawNavMeshPolysWithFlags(&dd, *m_navMesh, SAMPLE_POLYFLAGS_DISABLED, duRGBA(0,0,0,128));
	}

	
	glDepthMask(GL_TRUE);
		
	m_geom->drawConvexVolumes(&dd);
	
	glDepthMask(GL_TRUE);
}