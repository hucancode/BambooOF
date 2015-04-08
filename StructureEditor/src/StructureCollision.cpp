#include "StructureCollision.h"
#include "OrthoCamera.h"
StructureCollision::StructureCollision()
{
	opening = true;
	pivotTop = new StructurePivot();
	pivotRight = new StructurePivot();
	pivotBottom = new StructurePivot();
	pivotLeft = new StructurePivot();
	pivotTop->name = "T";
	pivotTop->x = 600;
	pivotTop->y = 300;
	pivotTop->disabled = true;
	pivotRight->name = "R";
	pivotRight->x = 600;
	pivotRight->y = 300;
	pivotBottom->name = "B";
	pivotBottom->x = 600;
	pivotBottom->y = 300;
	pivotLeft->name = "L";
	pivotLeft->x = 600;
	pivotLeft->y = 300;
}
StructureCollision::~StructureCollision()
{
}
void StructureCollision::open()
{
	opening = true;
}
void StructureCollision::close()
{
	opening = false;
}
void StructureCollision::draw()
{
	bool dragging = pivotLeft->isDragged() || pivotRight->isDragged() || pivotBottom->isDragged();
	// base line
	{
		ofLine(pivotTop->x, pivotTop->y, pivotRight->x, pivotRight->y);
		ofLine(pivotRight->x, pivotRight->y, pivotBottom->x, pivotBottom->y);
		ofLine(pivotBottom->x, pivotBottom->y, pivotLeft->x, pivotLeft->y);
		ofLine(pivotLeft->x, pivotLeft->y, pivotTop->x, pivotTop->y);
	}
	// 3d collision preview
	if (!dragging && opening)
	{
		orthoCamera cam;
		{
			static float ratio = 1.0 / cos(VIEW_ANGLE);
			ofVec3f origin(-pivotBottom->x, -pivotBottom->y*ratio);
			cam.setGlobalPosition(origin + ofVec3f(0.0f, 1.0f, 1.0f));
			cam.lookAt(origin);
		}
		cam.begin();
		{
			ofSetColor(255, 255, 255, 30);
			mesh.drawFaces();
			ofSetHexColor(0x000000);
			mesh.drawWireframe();
		}
		cam.end();
	}
	// pivot
	if(opening)
	{
		pivotTop->draw();
		pivotRight->draw();
		pivotBottom->draw();
		pivotLeft->draw();
	}
	// anchors
	if (!dragging && !opening)
	{
		{
			ofFill();
			ofSetHexColor(0xCC00FF);
			vector<ofVec2f>::iterator it = anchors.begin();
			for (; it != anchors.end(); it++)
			{
				ofVec2f item = *it;
				ofCircle(item.x, item.y, PIVOT_RADIUS / 2);
			}
		}
		// anchor slices
		{
			const int min_x = 0;
			const int min_y = 0;
			const int max_x = ofGetWindowWidth();
			const int max_y = ofGetWindowHeight();

			int px = min_x;
			int x = min_x;
			int y = min_y;
			for (int i = 0; i < anchors.size(); i++)
			{
				x = anchors[i].x;
				y = anchors[i].y;
				ofSetHexColor(0x0000FF);
				ofLine(x, y, px, y);
				ofSetHexColor(0x00FF00);
				ofLine(x, min_y, x, max_y);
				px = x;
			}
			if (anchors.size() > 0)
			{
				ofSetHexColor(0x0000FF);
				ofLine(px, pivotRight->y, max_x, pivotRight->y);
			}
		}
	}
	
	ofSetHexColor(0xFFFFFF);
}
bool StructureCollision::pick(int x, int y)
{
	
	pivotRight->mouseDown(x, y);
	if(pivotRight->isDragged()) return true;
	pivotBottom->mouseDown(x, y);
	if(pivotBottom->isDragged()) return true;
	pivotLeft->mouseDown(x, y);
	if(pivotLeft->isDragged()) return true;
	pivotTop->mouseDown(x, y);
	if (pivotTop->isDragged())
	{
		pivotRight->forceDrag(x, y);
		pivotBottom->forceDrag(x, y);
		pivotLeft->forceDrag(x, y);
		return true;
	}
	return false;
}
void StructureCollision::drag(int x, int y)
{
	pivotLeft->mouseDrag(x, y);
	pivotRight->mouseDrag(x, y);
	pivotBottom->mouseDrag(x, y);
	if(pivotLeft->isDragged() || pivotRight->isDragged() || pivotBottom->isDragged())
	{
		ofVec2f ab = ofVec2f(pivotLeft->x, pivotLeft->y) - ofVec2f(pivotBottom->x, pivotBottom->y);
		ofVec2f cd = ofVec2f(pivotRight->x, pivotRight->y) + ab;
		pivotTop->x = cd.x;
		pivotTop->y = cd.y;
	}
}
void StructureCollision::release()
{
	if(!(pivotLeft->isDragged() || pivotRight->isDragged() || pivotBottom->isDragged()))
	{
		return;
	}
	pivotTop->mouseUp();
	pivotLeft->mouseUp();
	pivotRight->mouseUp();
	pivotBottom->mouseUp();
	indentify();
	anchors.clear();
	slice();
	buildMesh();
}
void StructureCollision::indentify()
{
	//
	if(pivotLeft->x > pivotRight->x)
	{
		swap(pivotLeft, pivotRight);
	}
	if(pivotLeft->x > pivotBottom->x)
	{
		swap(pivotLeft, pivotBottom);
	}
	if(pivotLeft->x > pivotTop->x)
	{
		swap(pivotLeft, pivotTop);
	}
	// 
	if(pivotRight->x < pivotBottom->x)
	{
		swap(pivotRight, pivotBottom);
	}
	if(pivotRight->x < pivotTop->x)
	{
		swap(pivotRight, pivotTop);
	}
	//
	if(pivotBottom->y < pivotTop->y)
	{
		swap(pivotBottom, pivotTop);
	}
	pivotTop->disabled = true;
	pivotTop->name = "T";
	pivotBottom->disabled = false;
	pivotBottom->name = "B";
	pivotLeft->disabled = false;
	pivotLeft->name = "L";
	pivotRight->disabled = false;
	pivotRight->name = "R";
}
void StructureCollision::slice()
{
	ofVec2f tracer;
	ofVec2f origin;
	ofVec2f hypo;
	bool going_down = pivotRight->y > pivotLeft->y;
	if(going_down)
	{
		origin.x = pivotTop->x;
		origin.y = pivotTop->y;
		tracer.x = pivotLeft->x - pivotTop->x;
		tracer.y = pivotLeft->y - pivotTop->y;
		hypo.x = pivotRight->x - pivotTop->x;
		hypo.y = pivotRight->y - pivotTop->y;
	}
	else
	{
		origin.x = pivotTop->x;
		origin.y = pivotTop->y;
		tracer.x = pivotRight->x - pivotTop->x;
		tracer.y = pivotRight->y - pivotTop->y;
		hypo.x = pivotLeft->x - pivotTop->x;
		hypo.y = pivotLeft->y - pivotTop->y;
	}
	if(tracer.y == 0 || hypo.y == 0)
	{
		return;
	}
	int child_num = ceil(hypo.y/tracer.y);
	ofVec2f anchor_unit = hypo*(tracer.y/hypo.y);
	for(int i = 1; i <= child_num; i++)
	{
		ofVec2f anchor = anchor_unit*i;
		if(anchor.y > hypo.y)
		{
			anchor = hypo;
		}
		anchor += origin;
		anchors.push_back(anchor);
	}
	struct local
	{
		static bool compare(ofVec2f a, ofVec2f b)
		{
			return a.x < b.x;
		}
	};
	sort(anchors.begin(), anchors.end(), local::compare);
	if(!going_down)
	{
		int size = anchors.size()-1;
		for(int i = 0; i < size; i++)
		{
			anchors[i].x = anchors[i+1].x;
		}
	}
	if(anchors.size() > 0)
	{
		anchors.pop_back();
	}
}

void StructureCollision::buildMesh()
{
	static float ratio = 1.0 / cos(VIEW_ANGLE);
	mesh.clear();
	vector<ofVec2f> pivots;
	{// parallelgram(whatever, i dont remember the word)
		ofVec2f a = ofVec2f(0, 0); 
		ofVec2f b = ofVec2f(pivotRight->x - pivotBottom->x, pivotRight->y - pivotBottom->y);
		ofVec2f c = ofVec2f(pivotTop->x - pivotBottom->x, pivotTop->y - pivotBottom->y);
		ofVec2f d = ofVec2f(pivotLeft->x - pivotBottom->x, pivotLeft->y - pivotBottom->y);
		a.y *= ratio;
		b.y *= ratio;
		c.y *= ratio;
		d.y *= ratio;
		pivots.push_back(a);
		pivots.push_back(b);
		pivots.push_back(c);
		pivots.push_back(d);
	}
	{// other shapes go here

	}
	// bottom
	for (int i = 0; i < pivots.size(); i++)
	{
		mesh.addVertex(ofVec3f(pivots[i]));
		if (i > 1)
		{
			mesh.addTriangle(0, i - 1, i);
		}
	}
	// top
	for (int i = 0; i < pivots.size(); i++)
	{
		mesh.addVertex(ofVec3f(pivots[i].x, pivots[i].y, 100.0f));
		if (i > 1)
		{
			mesh.addTriangle(0, i - 1, i);
		}
	}
	// side
	for (int i = 0; i < pivots.size(); i++)
	{
		int j = i + pivots.size();
		int pi;
		int pj;
		if (i > 0)
		{
			pi = i - 1;
			pj = j - 1;
		}
		else
		{
			pi = pivots.size() - 1;
			pj = pi + pivots.size();
		}
		mesh.addTriangle(pi, pj, j);
		mesh.addTriangle(pi, j, i);
	}
}