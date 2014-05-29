#include "ofxSpriteRenderer.h"
ofxSpriteRenderer* ofxSpriteRenderer::s_Instance = 0;
void ofxSpriteRenderer::Render()
{
	ofxSpriteCommands::iterator it = m_Commands.begin();
	for(;it != m_Commands.end();it++)
	{
		ofxSpriteCommand* cmd = *it;
		cmd->Render();
	}
}
void ofxSpriteRenderer::PushSprite(ofxSpriteQuad* sprite)
{
	if(sprite->IsTransparent())
	{
		m_TransparentQuads.push_back(sprite);
	}
	else
	{
		m_SolidQuads.push_back(sprite);
	}
}
void ofxSpriteRenderer::RemoveSprite(ofxSpriteQuad* sprite)
{
	if(sprite->IsTransparent())
	{
		ofxSpriteQuads::iterator it = m_TransparentQuads.begin() + sprite->m_IndexInCommand;
		m_TransparentQuads.erase(it);
	}
	else
	{
		ofxSpriteQuads::iterator it = m_SolidQuads.begin() + sprite->m_IndexInCommand;
		m_SolidQuads.erase(it);
	}
}
void ofxSpriteRenderer::RequestUpdate(ofxRequest request)
{

}
void ofxSpriteRenderer::SolveRequest()
{
	ofxRequests::iterator it = m_UpdateRequest.begin();
	for(;it != m_UpdateRequest.end();it++)
	{
		ofxRequest item = *it;
		ofxSpriteQuad* quad = item.is_transparent?m_TransparentQuads[item.index]:m_SolidQuads[item.index];
		QUAD_UPDATE_REASON reason = item.reason;
		ofVec3f camera_position = m_Camera->getGlobalPosition();
		/* cases:
		 1. leave command, merge left
		 2. leave command, merge right
		 3. leave command, split left
		 4. leave command, split right
		 5. split command
		 6. in command, move left
		 7. in command, move right
		 8. in command, stand still
		*/
		/* rules:
		 1. never merge a sprite into a splited command
		 2. never move a sprite along a unsorted command
		*/
	}
}
static bool SolidQuadCompare(ofxSpriteQuad* quadA, ofxSpriteQuad* quadB)
{
	if(quadA->IsFarFromScreen()) return true;
	if(quadB->IsFarFromScreen()) return true;
	ofVec3f camera_position = ofxSpriteRenderer::GetInstance()->GetCamera()->getGlobalPosition();
	return quadA->CalculateDistanceToCamera(camera_position) < quadB->CalculateDistanceToCamera(camera_position);
}
static bool TransparentQuadCompare(ofxSpriteQuad* quadA, ofxSpriteQuad* quadB)
{
	if(quadA->IsFarFromScreen()) return true;
	if(quadB->IsFarFromScreen()) return true;
	ofVec3f camera_position = ofxSpriteRenderer::GetInstance()->GetCamera()->getGlobalPosition();
	return quadA->CalculateDistanceToCamera(camera_position) > quadB->CalculateDistanceToCamera(camera_position);
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
			command->PushSprite(sprite);
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