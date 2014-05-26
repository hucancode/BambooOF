#pragma once
#include "ofMain.h"
#include "ofxSpriteMaterial.h"
class ofxSpriteCommand
{
private:
	ofxSpriteMaterial* m_Material;
	vector<ofxVertex> m_Vertices;
	vector<GLushort> m_Indices;
	GLuint m_VBOId;
	GLuint m_IBOId;
public:
	ofxSpriteCommand()
	{
		glGenBuffers(1, &m_VBOId);
		glGenBuffers(1, &m_IBOId);
	}
	~ofxSpriteCommand()
	{
		glDeleteBuffers(1, &m_VBOId);
		glDeleteBuffers(1, &m_IBOId);
	}
	ofxSpriteMaterial* Material() { return m_Material; }
	void Material(ofxSpriteMaterial* material) { m_Material = material; }
	void Bind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_VBOId);
		glBufferData(GL_ARRAY_BUFFER, sizeof(m_Vertices), &m_Vertices[0], GL_DYNAMIC_DRAW);

		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_VBOId);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(m_Indices), &m_Indices[0], GL_STATIC_DRAW);
		m_Material->Bind();
	}
	void Unbind()
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		m_Material->Unbind();
	}
	void Render()
	{
		Bind();
		glDrawElements(GL_TRIANGLES, m_Indices.size(), GL_UNSIGNED_SHORT, 0);
		Unbind();
	}
	void AddSprite(ofxSpriteQuad* sprite)
	{
		ofxVertex vertexA, vertexB, vertexC, vertexD;
		GLushort start_index = m_Vertices.size();
		sprite->m_IndexInCommand = start_index*0.25;
		sprite->m_ParentCommand = this;
		m_Vertices.push_back(vertexA);
		m_Vertices.push_back(vertexB);
		m_Vertices.push_back(vertexC);
		m_Vertices.push_back(vertexD);
		m_Indices.push_back(start_index+0);
		m_Indices.push_back(start_index+1);
		m_Indices.push_back(start_index+2);
		m_Indices.push_back(start_index+2);
		m_Indices.push_back(start_index+3);
		m_Indices.push_back(start_index+0);
		UpdateSprite(sprite);

	}
	void UpdateSprite(ofxSpriteQuad* sprite)
	{
		unsigned int index = sprite->m_IndexInCommand;
		ofxVertex *vertexA, *vertexB, *vertexC, *vertexD;
		unsigned int index4 = index*4;
		vertexA = &m_Vertices[index4];
		vertexB = &m_Vertices[index4+1];
		vertexC = &m_Vertices[index4+2];
		vertexD = &m_Vertices[index4+3];

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
		{
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
				float texture_w = sprite->GetMaterial()->GetTextureSizeRaw(i);
				float texture_h = sprite->GetMaterial()->GetTextureSizeRaw(j);
				float sprite_w = sprite->GetWidth();
				float sprite_h = sprite->GetHeight();
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
};
#define ofxSpriteCommands vector<ofxSpriteCommand*>