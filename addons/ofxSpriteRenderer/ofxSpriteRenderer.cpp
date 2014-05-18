#include "ofxSpriteRenderer.h"
void ofxSpriteRenderer::Render()
{
	ofxSpriteCommands::iterator it = m_Commands.begin();
	for(;it != m_Commands.end();it++)
    {
		ofxSpriteCommand* cmd = *it;
		// bind material
		glBindBuffer(GL_ARRAY_BUFFER, cmd->VBOID);
		glBufferData(GL_ARRAY_BUFFER, sizeof(cmd->Vertices), &cmd->Vertices[0], GL_DYNAMIC_DRAW);
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cmd->IBOID);
		glDrawElements(GL_TRIANGLES, cmd->Indices.size(), GL_UNSIGNED_SHORT, &cmd->Indices[0]);
	}
	// unbind material
	glBindBuffer(GL_ARRAY_BUFFER, 0);
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
}
static bool SpriteCompare(ofxSpriteQuad* quadA, ofxSpriteQuad* quadB)
{
	return true;
}
void ofxSpriteRenderer::BuildCommands()
{
	sort(m_Quads.begin(), m_Quads.end(), SpriteCompare);
	{
		// clear commands here
	}
	{
		unsigned short quads_to_draw = 0;
		ofxSpriteMaterial* last_material = 0;
		ofxSpriteQuads::iterator it = m_Quads.begin();
		for(;it != m_Quads.end();it++)
		{
			ofxSpriteQuad* sprite = *it;
			ofxSpriteCommand* command;
			if(last_material != sprite->Material)
			{
				command = new ofxSpriteCommand();
				m_Commands.push_back(command);
				last_material = sprite->Material;
				command->Material = last_material;
#ifdef DEBUG
				m_DrawnBatches++;
				m_DrawnVertices += 6;
#endif
				quads_to_draw = 1;
			}
			else
			{
				ofxSpriteCommand* command = m_Commands.back();
				quads_to_draw += 1;
			}
			ofxVertex vertexA;
			vertexA.x = sprite->WorldPosition.x - sprite->Quad[0]*0.5;
			vertexA.y = sprite->WorldPosition.y;
			ofxVertex vertexB;
			vertexB.x = vertexA.x + sprite->Quad[0];
			vertexB.y = vertexA.y;
			ofxVertex vertexC;
			vertexC.x = vertexB.x;
			vertexC.y = vertexB.y + sprite->Quad[1];
			ofxVertex vertexD;
			vertexD.x = vertexA.x;
			vertexD.y = vertexC.y;
			{
				int i = 0;
				int x = 0;
				int y = 1;
				for(;i<sprite->Material->TextureCount;i++,x+=2,y+=2)
				{
					// set uv for 4 vertices
					float dw = sprite->Material->TextureSize[x]/sprite->Quad[0];
					float dh = sprite->Material->TextureSize[y]/sprite->Quad[1];
					float dx = -sprite->TextureCoord[x]*dw;
					float dy = -sprite->TextureCoord[y]*dh;
					
					vertexA.uv[x] = dx;
					vertexA.uv[y] = dy;
					vertexB.uv[x] = vertexA.uv[x] + dw;
					vertexB.uv[y] = vertexA.uv[y];
					vertexC.uv[x] = vertexB.uv[x];
					vertexC.uv[y] = vertexB.uv[y] + dh;
					vertexD.uv[x] = vertexA.uv[x];
					vertexD.uv[y] = vertexC.uv[y];
				}
			}
			command->Vertices.push_back(vertexA);
			command->Vertices.push_back(vertexB);
			command->Vertices.push_back(vertexC);
			command->Vertices.push_back(vertexD);
			GLushort start_index = (quads_to_draw-1)*6;
			command->Indices.push_back(start_index+0);
			command->Indices.push_back(start_index+1);
			command->Indices.push_back(start_index+2);
			command->Indices.push_back(start_index+2);
			command->Indices.push_back(start_index+3);
			command->Indices.push_back(start_index+0);
			
		}
	}
	{
		ofxSpriteCommands::iterator it = m_Commands.begin();
		for(;it != m_Commands.end();it++)
		{
			ofxSpriteCommand* cmd = *it;
			glGenBuffers(1, &cmd->VBOID);
			glBindBuffer(GL_ARRAY_BUFFER, cmd->VBOID);
			glBufferData(GL_ARRAY_BUFFER, sizeof(cmd->Vertices), &cmd->Vertices[0], GL_DYNAMIC_DRAW);

			// vertices
			glEnableVertexAttribArray(cmd->Material->ShaderXYZID);
			glVertexAttribPointer(cmd->Material->ShaderXYZID, 3, GL_FLOAT, GL_FALSE, sizeof(ofxVertex), (GLvoid*) offsetof( ofxVertex, x));

			// tex coords
			for(int i=0;i<cmd->Material->TextureCount;i++)
			{
				glEnableVertexAttribArray(cmd->Material->ShaderUVID[i]);
				glVertexAttribPointer(cmd->Material->ShaderUVID[i], 2, GL_FLOAT, GL_FALSE, sizeof(ofxVertex), (GLvoid*) offsetof( ofxVertex, uv[i*2]));
			}
			glBindBuffer(GL_ARRAY_BUFFER, 0);
	
			glGenBuffers(1, &cmd->IBOID);
			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cmd->IBOID);
			glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cmd->Indices), &cmd->Indices[0], GL_STATIC_DRAW);

			glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
		}
	}
}
bool ofxSpriteRenderer::CheckVisibility(ofxSpriteQuad quad)
{
	return true;
}