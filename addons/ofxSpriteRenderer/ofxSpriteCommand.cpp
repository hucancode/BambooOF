#include "ofxSpriteCommand.h"
#include "ofxSpriteRenderer.h"
vector<GLushort> ofxSpriteCommand::m_Indices;
GLuint ofxSpriteCommand::m_IBOId;
void ofxSpriteCommand::GenerateSharedIndices(unsigned short number_of_quad)
{
	if(number_of_quad > 6000) return;
	number_of_quad *= 4;
	glGenBuffers(1, &m_IBOId);
	for(unsigned int i=0;i<number_of_quad;i+=4)
	{
		m_Indices.push_back(i+0);
		m_Indices.push_back(i+1);
		m_Indices.push_back(i+2);
		m_Indices.push_back(i+0);
		m_Indices.push_back(i+2);
		m_Indices.push_back(i+3);
	}
}
void ofxSpriteCommand::DeleteSharedIndices()
{
	glDeleteBuffers(1, &m_IBOId);
	m_Indices.clear();
}

ofxSpriteCommand::ofxSpriteCommand()
{
	glGenBuffers(1, &m_VBOId);
	//glGenBuffers(1, &m_IBOId);
	m_VisibleSpriteCount = 0;
	m_IndicesSize = 0;
	m_FirstSpriteIndex = -1;
	m_LastSpriteIndex = -1;
	m_DistanceMin = 0;
	m_DistanceMax = 0;
	m_Status = COMMAND_STATUS_UNITED;
}
ofxSpriteCommand::~ofxSpriteCommand()
{
	glDeleteBuffers(1, &m_VBOId);
	//glDeleteBuffers(1, &m_IBOId);
}
void ofxSpriteCommand::Bind()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_VBOId);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_IBOId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ofxVertex)*m_Vertices.size(), &m_Vertices[0], GL_DYNAMIC_DRAW);
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLushort)*m_IndicesSize, &m_Indices[0], GL_STATIC_DRAW);
	m_Material->Bind();
}
void ofxSpriteCommand::Unbind()
{
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	m_Material->Unbind();
}
void ofxSpriteCommand::Render()
{
	if(m_VisibleSpriteCount == 0) return;
	Bind();
	glDrawElements(GL_TRIANGLES, m_IndicesSize, GL_UNSIGNED_SHORT, 0);
	Unbind();
}
void ofxSpriteCommand::PushSprite(ofxSpriteQuad* sprite)
{
	ofVec3f camera_position = ofVec3f::zero();//ofxSpriteRenderer::GetInstance()->GetCamera()->getGlobalPosition();
	float distance = sprite->CalculateDistanceToCamera(camera_position);
	ofxVertex vertexA, vertexB, vertexC, vertexD;
	sprite->m_IndexInCommand = m_Vertices.size();
	sprite->m_ParentCommand = this;
	if(m_FirstSpriteIndex == -1)
	{
		m_FirstSpriteIndex = m_LastSpriteIndex = sprite->m_IndexInRenderer;
		m_DistanceMin = m_DistanceMax = distance;
	}
	if(sprite->m_IndexInRenderer < m_FirstSpriteIndex)
	{
		m_FirstSpriteIndex = sprite->m_IndexInRenderer;
	}
	else if(sprite->m_IndexInRenderer > m_LastSpriteIndex)
	{
		m_LastSpriteIndex = sprite->m_IndexInRenderer;
	}
	if(distance < m_DistanceMin)
	{
		m_DistanceMin = distance;
	}
	else if(distance > m_DistanceMax)
	{
		m_DistanceMax = distance;
	}
	m_Vertices.push_back(vertexA);
	m_Vertices.push_back(vertexB);
	m_Vertices.push_back(vertexC);
	m_Vertices.push_back(vertexD);
	m_IndicesSize += 6;
	m_VisibleSprite.push_back(true);
	m_VisibleSpriteCount++;
	UpdateSprite(sprite);
	
}
void ofxSpriteCommand::EraseSprite(ofxSpriteQuad* sprite)
{
	if(m_Status != COMMAND_STATUS_EXPANDED || m_Status == COMMAND_STATUS_DISMISSED) return;
	unsigned int index = sprite->m_IndexInCommand;
	bool visible = sprite->m_Visible && sprite->m_Visibility == QUAD_VISIBILITY_IN_SCREEN;
	unsigned int index4 = index*0.25;
	if(visible && !m_VisibleSprite[index4])
	{
		m_VisibleSpriteCount++;
		m_VisibleSprite[index4] = true;
	}
	else if(!visible && m_VisibleSprite[index4])
	{
		m_VisibleSpriteCount--;
		m_VisibleSprite[index4] = false;
	}
	m_Vertices.erase(m_Vertices.begin() + index);
	m_Vertices.erase(m_Vertices.begin() + index+1);
	m_Vertices.erase(m_Vertices.begin() + index+2);
	m_Vertices.erase(m_Vertices.begin() + index+3);
}
void ofxSpriteCommand::UpdateSprite(ofxSpriteQuad* sprite)
{
	// this is costly works, need to optimize it
	if(sprite->m_Status != QUAD_STATUS_NO_CHANGE && m_Status != COMMAND_STATUS_EXPANDED)
	{
		if(sprite->m_Status == QUAD_STATUS_POSITION_CHANGE)
		{
			ofVec3f camera_position = ofVec3f::zero();//ofxSpriteRenderer::GetInstance()->GetCamera()->getGlobalPosition();
			float distance = sprite->CalculateDistanceToCamera(camera_position);
			if(distance > m_DistanceMax || distance < m_DistanceMin)
			{
				ofxSpriteCommand* prev;
				ofxSpriteCommand* next;
				unsigned short index = this->m_IndexInRenderer;
				if(sprite->IsTransparent())
				{
					unsigned short max_index = ofxSpriteRenderer::GetInstance()->
						m_TransparentCommands.size() - 1;
					prev = index == 0?0:ofxSpriteRenderer::GetInstance()->
						m_TransparentCommands[index-1];
					next = index == max_index?0:ofxSpriteRenderer::GetInstance()->
						m_TransparentCommands[index+1];
				}
				else
				{
					unsigned short max_index = ofxSpriteRenderer::GetInstance()->
						m_SolidCommands.size() - 1;
					prev = index == 0?0:ofxSpriteRenderer::GetInstance()->
						m_SolidCommands[index-1];
					next = index == max_index?0:ofxSpriteRenderer::GetInstance()->
						m_SolidCommands[index+1];
				}
				bool expand = false;
				if(prev)
				{
					if((distance > prev->m_DistanceMin && sprite->IsTransparent()) ||
						(distance < prev->m_DistanceMax && !sprite->IsTransparent()))
					{
						expand = true;
					}
				}
				if(next && !expand)
				{
					if((distance < next->m_DistanceMax && sprite->IsTransparent()) ||
						(distance > next->m_DistanceMin && !sprite->IsTransparent()))
					{
						expand = true;
					}
				}
				if(expand)
				{
					m_Status = COMMAND_STATUS_EXPANDED;
				}
				if(m_DistanceMax < distance) m_DistanceMax = distance;
				if(m_DistanceMin > distance) m_DistanceMin = distance;
			}
			if(m_Status == COMMAND_STATUS_UNITED)
			{
				ofxSpriteQuad* prev;
				ofxSpriteQuad* next;
				unsigned int index = sprite->m_IndexInRenderer;

				if(sprite->IsTransparent())
				{
					unsigned int max_index = ofxSpriteRenderer::GetInstance()->
						m_TransparentQuads.size() - 1;
					prev = index == 0?0:ofxSpriteRenderer::GetInstance()->
						m_TransparentQuads[index-1];
					next = index == max_index?0:ofxSpriteRenderer::GetInstance()->
						m_TransparentQuads[index+1];
				}
				else
				{
					unsigned int max_index = ofxSpriteRenderer::GetInstance()->
						m_SolidQuads.size() - 1;
					prev = index == 0?0:ofxSpriteRenderer::GetInstance()->
						m_SolidQuads[index-1];
					next = index == max_index?0:ofxSpriteRenderer::GetInstance()->
						m_SolidQuads[index+1];
				}
				bool dismiss = false;
				if(prev)
				{
					float prev_distance = prev->CalculateDistanceToCamera(camera_position);
					if((distance > prev_distance && sprite->IsTransparent()) || 
						(distance < prev_distance && !sprite->IsTransparent()))
					{
						dismiss = true;
					}
				}
				if(next && !dismiss)
				{
					float next_distance = next->CalculateDistanceToCamera(camera_position);
					if((distance < next_distance && sprite->IsTransparent()) ||
						(distance > next_distance && !sprite->IsTransparent()))
					{
						dismiss = true;
					}
				}
				if(dismiss)
				{
					m_Status = COMMAND_STATUS_DISMISSED;
				}
			}
		}
		else if(sprite->m_Status == QUAD_STATUS_MATERIAL_CHANGE)
		{
			m_Status = COMMAND_STATUS_DISMISSED;
		}
	}
	if(m_Status == COMMAND_STATUS_EXPANDED || m_Status == COMMAND_STATUS_DISMISSED)
	{
		return;
	}
	unsigned int index = sprite->m_IndexInCommand;
	bool visible = sprite->m_Visible && sprite->m_Visibility == QUAD_VISIBILITY_IN_SCREEN;
	unsigned int index4 = index*0.25;
	if(visible && !m_VisibleSprite[index4])
	{
		m_VisibleSpriteCount++;
		m_VisibleSprite[index4] = true;
	}
	else if(!visible && m_VisibleSprite[index4])
	{
		m_VisibleSpriteCount--;
		m_VisibleSprite[index4] = false;
	}
	ofxVertex *vertexA, *vertexB, *vertexC, *vertexD;
	vertexA = &m_Vertices[index];
	vertexB = &m_Vertices[index+1];
	vertexC = &m_Vertices[index+2];
	vertexD = &m_Vertices[index+3];
	{
		vertexA->X = sprite->m_glPosition[0].x;
		vertexA->Y = sprite->m_glPosition[0].y;
		vertexA->Z = sprite->m_glPosition[0].z;
		vertexB->X = sprite->m_glPosition[1].x;
		vertexB->Y = sprite->m_glPosition[1].y;
		vertexB->Z = sprite->m_glPosition[1].z;
		vertexC->X = sprite->m_glPosition[2].x;
		vertexC->Y = sprite->m_glPosition[2].y;
		vertexC->Z = sprite->m_glPosition[2].z;
		vertexD->X = sprite->m_glPosition[3].x;
		vertexD->Y = sprite->m_glPosition[3].y;
		vertexD->Z = sprite->m_glPosition[3].z;
	}
	for(int i = 0;i < sprite->GetMaterial()->GetTextureCount();i++)
	{
		int x = i<<2;
		int y = x+1;
		int z = y+1;
		int w = z+1;
		vertexA->UV[x] = sprite->m_glUV[i].x;
		vertexA->UV[y] = sprite->m_glUV[i].y;
		vertexB->UV[x] = sprite->m_glUV[i].z;
		vertexB->UV[y] = sprite->m_glUV[i].y;
		vertexC->UV[x] = sprite->m_glUV[i].z;
		vertexC->UV[y] = sprite->m_glUV[i].w;
		vertexD->UV[x] = sprite->m_glUV[i].x;
		vertexD->UV[y] = sprite->m_glUV[i].w;
		vertexA->UV[z] = sprite->m_glCUV[i].x;
		vertexA->UV[w] = sprite->m_glCUV[i].y;
		vertexB->UV[z] = sprite->m_glCUV[i].z;
		vertexB->UV[w] = sprite->m_glCUV[i].y;
		vertexC->UV[z] = sprite->m_glCUV[i].z;
		vertexC->UV[w] = sprite->m_glCUV[i].w;
		vertexD->UV[z] = sprite->m_glCUV[i].x;
		vertexD->UV[w] = sprite->m_glCUV[i].w;
	}
}