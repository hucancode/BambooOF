#include "CollisionEditTool.h"
#include "ofApp.h"
void CollisionEditTool::enter()
{
	parent->structure->collision->open();
}
void CollisionEditTool::leave()
{
	parent->structure->collision->close();
}
void CollisionEditTool::draw()
{
	// nothing to draw
}
void CollisionEditTool::mouseDown(int x, int y)
{
	bool swallow = parent->structure->collision->pick(x, y);
	if (!swallow)
	{
		parent->structure->image->pick(x, y);
	}
}
void CollisionEditTool::mouseDrag(int x, int y)
{
	parent->structure->collision->drag(x, y);
	parent->structure->image->drag(x, y);
}
void CollisionEditTool::mouseMove(int x, int y)
{

}
void CollisionEditTool::mouseUp(int x, int y)
{
	parent->structure->collision->release();
	parent->structure->image->release();
}