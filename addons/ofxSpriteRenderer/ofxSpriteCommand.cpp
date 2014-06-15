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
	ofVec3f camera_position = ofxSpriteRenderer::GetInstance()->GetCamera()->getGlobalPosition();
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
	if(sprite->m_Status != QUAD_STATUS_NO_CHANGE && m_Status != COMMAND_STATUS_EXPANDED)
	{
		if(sprite->m_Status == QUAD_STATUS_POSITION_CHANGE)
		{
			ofVec3f camera_position = ofxSpriteRenderer::GetInstance()->GetCamera()->getGlobalPosition();
			float distance = sprite->CalculateDistanceToCamera(camera_position);
			if(distance > m_DistanceMax || distance < m_DistanceMin)
			{
				ofxSpriteCommand* prev;
				ofxSpriteCommand* next;
				unsigned short index = this->m_IndexInRenderer;
				if(sprite->IsTransparent())
				{
					unsigned int max_index = ofxSpriteRenderer::GetInstance()->
						m_TransparentCommands.size() - 1;
					prev = index == 0?0:ofxSpriteRenderer::GetInstance()->
						m_TransparentCommands[index-1];
					next = index == max_index?0:ofxSpriteRenderer::GetInstance()->
						m_TransparentCommands[index+1];
				}
				else
				{
					unsigned int max_index = ofxSpriteRenderer::GetInstance()->
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
				if(next)
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
				else
				{
					if(m_DistanceMax < distance) m_DistanceMax = distance;
					if(m_DistanceMin > distance) m_DistanceMin = distance;
				}
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
				if(next)
				{
					float next_distance = next->CalculateDistanceToCamera(camera_position);
					if((distance < next_distance && sprite->IsTransparent()) ||
						(distance > next_distance && !sprite->IsTransparent()))
					{
						dismiss = true;
					}
				}
				if(dismiss)
					m_Status = COMMAND_STATUS_DISMISSED;
			}
		}
		else if(sprite->m_Status == QUAD_STATUS_MATERIAL_CHANGE)
		{
			m_Status = COMMAND_STATUS_DISMISSED;
		}
		return;
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
	{//position
		vertexA = &m_Vertices[index];
		vertexB = &m_Vertices[index+1];
		vertexC = &m_Vertices[index+2];
		vertexD = &m_Vertices[index+3];

		vertexA->X = sprite->GetPosition().x - sprite->GetWidth()*0.5;
		vertexA->Y = sprite->GetPosition().y;
		vertexA->Z = sprite->GetPosition().z;
		vertexB->X = vertexA->X + sprite->GetWidth();
		vertexB->Y = vertexA->Y;
		vertexB->Z = vertexA->Z;
		vertexC->X = vertexB->X;
		vertexC->Y = vertexB->Y + sprite->GetHeight();
		vertexC->Z = vertexA->Z;
		vertexD->X = vertexA->X;
		vertexD->Y = vertexC->Y;
		vertexD->Z = vertexA->Z;
	}
	{//uv & cuv
		int tex_index = 0;
		int i = 0;
		int j = 1;
		int x = 0;
		int y = 1;
		int w = 2;
		int h = 3;
		for(;tex_index < sprite->GetMaterial()->GetTextureCount();
			tex_index++,
			i += 2, j += 2, 
			x += 4, y += 4, w += 4, h += 4)
		{
			float texture_w;
			float texture_h;
			if(sprite->GetMaterial()->GetType() == MATERIAL_TYPE_MONO)
			{
				texture_w = ((ofxMonoMaterial*)sprite->GetMaterial())->GetTextureSizeX();
				texture_h = ((ofxMonoMaterial*)sprite->GetMaterial())->GetTextureSizeY();
			}
			else if(sprite->GetMaterial()->GetType() == MATERIAL_TYPE_POLY)
			{
				texture_w = ((ofxPolyMaterial*)sprite->GetMaterial())->GetTextureSizeRaw(i);
				texture_h = ((ofxPolyMaterial*)sprite->GetMaterial())->GetTextureSizeRaw(j);
			}
			else
			{
				texture_w = 1;
				texture_h = 1;
			}
			float sprite_w = sprite->GetLogicWidth();
			float sprite_h = sprite->GetLogicHeight();
			float texture_rect_x = sprite->GetTextureRectRaw(x);
			float texture_rect_y = sprite->GetTextureRectRaw(y);
			float texture_rect_w = sprite->GetTextureRectRaw(w);
			float texture_rect_h = sprite->GetTextureRectRaw(h);
			float sprite_rect_x = sprite->GetSpriteRectRaw(x);
			float sprite_rect_y = sprite->GetSpriteRectRaw(y);
			float sprite_rect_w = sprite->GetSpriteRectRaw(w);
			float sprite_rect_h = sprite->GetSpriteRectRaw(h);
			{
				float uv_w = texture_w/sprite_w;
				float uv_h = texture_h/sprite_h;
				float uv_min_x = (texture_rect_x - sprite_rect_x)/uv_w;
				float uv_min_y = (texture_rect_y - sprite_rect_y)/uv_h;
				float uv_max_x = uv_min_x + uv_w;
				float uv_max_y = uv_min_y + uv_h;
				vertexA->UV[i] = uv_min_x;
				vertexA->UV[j] = uv_min_y;
				vertexB->UV[i] = uv_max_x;
				vertexB->UV[j] = uv_min_y;
				vertexC->UV[i] = uv_max_x;
				vertexC->UV[j] = uv_max_y;
				vertexD->UV[i] = uv_min_x;
				vertexD->UV[j] = uv_max_y;
			}
			{
				float crop_w = sprite_w/sprite_rect_w;
				float crop_h = sprite_h/sprite_rect_h;
				float crop_min_x = -sprite_rect_x/sprite_rect_w;
				float crop_min_y = -sprite_rect_y/sprite_rect_h;
				float crop_max_x = crop_min_x + crop_w;
				float crop_max_y = crop_min_y + crop_h;
				vertexA->CUV[i] = crop_min_x;
				vertexA->CUV[j] = crop_min_y;
				vertexB->CUV[i] = crop_max_x;
				vertexB->CUV[j] = crop_min_y;
				vertexC->CUV[i] = crop_max_x;
				vertexC->CUV[j] = crop_max_y;
				vertexD->CUV[i] = crop_min_x;
				vertexD->CUV[j] = crop_max_y;
			}
		}
	}
}