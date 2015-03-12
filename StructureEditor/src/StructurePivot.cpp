#include "StructurePivot.h"
ofTrueTypeFont StructurePivot::font;
StructurePivot::StructurePivot()
{
	dragged = false;
	disabled = false;
	x = 0;
	y = 0;
	ox = 0;
	oy = 0;
	if(!font.isLoaded())
	{
		font.loadFont(PIVOT_FONT_NAME, PIVOT_FONT_SIZE, true, true);
		font.setLineHeight(PIVOT_FONT_SIZE + 4.0f);
		font.setLetterSpacing(1.035);
	}
}
StructurePivot::~StructurePivot()
{
}
void StructurePivot::draw()
{
	ofFill();
	ofSetHexColor(0x000000);
	ofCircle(x, y, PIVOT_RADIUS+2);
	if(disabled)
	{
		ofSetHexColor(0x777777);
	}
	else
	{
		ofSetHexColor(0x00CCFF);
	}
	ofCircle(x, y, PIVOT_RADIUS);
	
	ofSetHexColor(0x000000);
	if(font.isLoaded())
	{
		font.drawString(name, x - name.size()*PIVOT_FONT_SIZE/2, y + PIVOT_FONT_SIZE/2);
	}
	else
	{
		ofDrawBitmapString(name, x - name.size()*4, y + 4);
	}
	ofSetHexColor(0xFFFFFF);
}
bool StructurePivot::isDragged()
{
	return dragged;
}
void StructurePivot::mouseDown(int mx, int my)
{
	if(ofDist(x, y, mx, my) < PIVOT_RADIUS)
	{
		dragged = true;
		ox = mx - x;
		oy = my - y;
	}
}
void StructurePivot::forceDrag(int mx, int my)
{
	dragged = true;
	ox = mx - x;
	oy = my - y;
}
void StructurePivot::mouseUp()
{
	dragged = false;
}
void StructurePivot::mouseDrag(int mx, int my)
{
	if(!dragged) return;
	x = mx - ox;
	y = my - oy;
}