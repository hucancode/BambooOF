#include "ImageEditTool.h"
#include "ofApp.h"
void ImageEditTool::enter()
{
	parent->structure->image->open();
}
void ImageEditTool::leave()
{
	parent->structure->image->close();
}
void ImageEditTool::draw()
{
	// nothing to draw
}
void ImageEditTool::mouseDown(int x, int y)
{
	parent->structure->image->pick(x, y);
}
void ImageEditTool::mouseDrag(int x, int y)
{
	parent->structure->image->drag(x, y);
}
void ImageEditTool::mouseMove(int x, int y)
{
	
}
void ImageEditTool::mouseUp(int x, int y)
{
	parent->structure->image->release();
}