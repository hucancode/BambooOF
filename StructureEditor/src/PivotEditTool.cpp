#include "PivotEditTool.h"
#include "ofApp.h"
void PivotEditTool::enter()
{
	parent->base->open();
}
void PivotEditTool::leave()
{
	parent->base->close();
}
void PivotEditTool::draw()
{
	// nothing to draw
}
void PivotEditTool::mouseDown(int x, int y)
{
	bool swallow = parent->base->pick(x, y);
	if (!swallow)
	{
		parent->image->pick(x, y);
	}
}
void PivotEditTool::mouseDrag(int x, int y)
{
	parent->base->drag(x, y);
	parent->image->drag(x, y);
}
void PivotEditTool::mouseMove(int x, int y)
{

}
void PivotEditTool::mouseUp(int x, int y)
{
	parent->base->release();
	parent->image->release();
}