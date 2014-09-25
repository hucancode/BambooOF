#include "ofxBaseCommand.h"
ofxBaseCommand::ofxBaseCommand()
{
	m_RenderedVertices = 0;
}
ofxBaseCommand::~ofxBaseCommand()
{
}
void ofxBaseCommand::Render()
{
}
unsigned int ofxBaseCommand::GetRenderedVertices()
{
	return m_RenderedVertices;
}