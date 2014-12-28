#include "ofxBaseCommand.h"
ofxBaseCommand::ofxBaseCommand()
{
	m_Batched = true;
	m_Shader = 0;
	m_VerticesSize = 0;
}
ofxBaseCommand::~ofxBaseCommand()
{
}
void ofxBaseCommand::Render()
{
}
unsigned int ofxBaseCommand::GetRenderedVertices()
{
	return m_VerticesSize;
}
bool ofxBaseCommand::IsBatchedCommand()
{
	return m_Batched;
}