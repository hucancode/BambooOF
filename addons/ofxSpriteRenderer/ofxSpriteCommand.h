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
	static void Unbind()
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
		ofxVertex vertexA;
		vertexA.X = sprite->WorldPosition.x - sprite->Quad[0]*0.5;
		vertexA.Y = sprite->WorldPosition.y;
		vertexA.Z = sprite->WorldPosition.z;
		ofxVertex vertexB;
		vertexB.X = vertexA.X + sprite->Quad[0];
		vertexB.Y = vertexA.Y;
		vertexB.Z = vertexA.Z;
		ofxVertex vertexC;
		vertexC.X = vertexB.X;
		vertexC.Y = vertexB.Y + sprite->Quad[1];
		vertexC.Z = vertexA.Z;
		ofxVertex vertexD;
		vertexD.X = vertexA.X;
		vertexD.Y = vertexC.Y;
		vertexD.Z = vertexA.Z;
		{
			int i = 0;
			int x = 0;
			int y = 1;
			for(;i<sprite->Material->TextureCount();i++,x+=2,y+=2)
			{
				// set uv for 4 vertices
				float dw = sprite->Material->TextureSize()[x]/sprite->Quad[0];
				float dh = sprite->Material->TextureSize()[y]/sprite->Quad[1];
				float dx = -sprite->TextureCoord[x]*dw;
				float dy = -sprite->TextureCoord[y]*dh;

				vertexA.UV[x] = dx;
				vertexA.UV[y] = dy;
				vertexB.UV[x] = vertexA.UV[x] + dw;
				vertexB.UV[y] = vertexA.UV[y];
				vertexC.UV[x] = vertexB.UV[x];
				vertexC.UV[y] = vertexB.UV[y] + dh;
				vertexD.UV[x] = vertexA.UV[x];
				vertexD.UV[y] = vertexC.UV[y];
			}
		}
		GLushort start_index = m_Vertices.size();
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
	}
};
#define ofxSpriteCommands vector<ofxSpriteCommand*>