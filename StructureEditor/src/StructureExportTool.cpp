#include "StructureExportTool.h"
#include "ofApp.h"
void StructureExportTool::enter()
{
	parent->structure->export("hehe");
	parent->alertMessage("export complete!");
}
void StructureExportTool::leave()
{
	
}
void StructureExportTool::draw()
{
	// nothing to draw
}
void StructureExportTool::mouseDown(int x, int y)
{
	
}
void StructureExportTool::mouseDrag(int x, int y)
{
	
}
void StructureExportTool::mouseMove(int x, int y)
{

}
void StructureExportTool::mouseUp(int x, int y)
{
	
}