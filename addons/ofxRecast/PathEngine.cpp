#include "PathEngine.h"
#include "RecastAlloc.h"
#include "RecastAssert.h"
#include "SDL.h"
#include "SDL_opengl.h"

	void SetupCamera()
	{
		glEnable(GL_DEPTH_TEST);
		glMatrixMode(GL_PROJECTION);
		glLoadIdentity();
		gluOrtho2D(-1.0,1.0,-1.0,1.0);// i'm not sure about these 1.0 values
		glViewport(0,0,800,600);
		glMatrixMode(GL_MODELVIEW);
		glLoadIdentity();
	}
	
	
	