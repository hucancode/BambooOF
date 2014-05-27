#include "ofxSpriteRenderer.h"
void ofxSpriteRenderer::Render()
{
	ofxSpriteCommands::iterator it = m_Commands.begin();
	for(;it != m_Commands.end();it++)
    {
		ofxSpriteCommand* cmd = *it;
		cmd->Render();
	}
}
void ofxSpriteRenderer::RequestUpdate(ofxSpriteQuad* sprite)
{
}
void ofxSpriteRenderer::SolveRequest()
{
}
static bool SolidQuadCompare(ofxSpriteQuad* quadA, ofxSpriteQuad* quadB)
{
	return true;
}
static bool TransparentQuadCompare(ofxSpriteQuad* quadA, ofxSpriteQuad* quadB)
{
	return true;
}
void ofxSpriteRenderer::BuildCommands()
{
	sort(m_SolidQuads.begin(), m_SolidQuads.end(), SolidQuadCompare);
	sort(m_TransparentQuads.begin(), m_TransparentQuads.end(), TransparentQuadCompare);
	{
		// clear commands here
	}
	{
		ofxSpriteMaterial* last_material = 0;
		ofxSpriteQuads::iterator it = m_SolidQuads.begin();
		for(;it != m_SolidQuads.end();it++)
		{
			ofxSpriteQuad* sprite = *it;
			ofxSpriteCommand* command;
			if(last_material != sprite->GetMaterial())
			{
				command = new ofxSpriteCommand();
				m_Commands.push_back(command);
				last_material = sprite->GetMaterial();
				command->SetMaterial(last_material);
#ifdef DEBUG
				m_DrawnBatches++;
				m_DrawnVertices += 6;
#endif
			}
			else
			{
				command = m_Commands.back();
			}
			command->AddSprite(sprite);
		}
	}
	{
		ofxSpriteMaterial* last_material = 0;
		ofxSpriteQuads::iterator it = m_TransparentQuads.begin();
		for(;it != m_TransparentQuads.end();it++)
		{
			ofxSpriteQuad* sprite = *it;
			ofxSpriteCommand* command;
			if(last_material != sprite->GetMaterial())
			{
				command = new ofxSpriteCommand();
				m_Commands.push_back(command);
				last_material = sprite->GetMaterial();
				command->SetMaterial(last_material);
#ifdef DEBUG
				m_DrawnBatches++;
				m_DrawnVertices += 6;
#endif
			}
			else
			{
				command = m_Commands.back();
			}
			command->PushSprite(sprite);
		}
	}
}
bool ofxSpriteRenderer::CheckVisibility(ofxSpriteQuad* quad)
{
	return true;
}