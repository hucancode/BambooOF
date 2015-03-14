#include "ImageEditTool.h"
#include "ofApp.h"
void ImageEditTool::enter()
{
	parent->image->open();
}
void ImageEditTool::leave()
{
	parent->image->close();
}
void ImageEditTool::draw()
{
	// nothing to draw
}
void ImageEditTool::mouseDown(int x, int y)
{
	parent->image->pick(x, y);
}
void ImageEditTool::mouseDrag(int x, int y)
{
	parent->image->drag(x, y);
}
void ImageEditTool::mouseMove(int x, int y)
{
	
}
void ImageEditTool::mouseUp(int x, int y)
{
	parent->image->release();
}