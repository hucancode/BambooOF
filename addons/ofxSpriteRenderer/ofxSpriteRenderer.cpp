#include "ofxSpriteRenderer.h"
ofxSpriteRenderer* ofxSpriteRenderer::s_Instance = 0;
ofxSpriteRenderer* ofxSpriteRenderer::GetInstance()
{
	return s_Instance;
}
void ofxSpriteRenderer::DestroyInstance()
{
	if(!s_Instance) return;
	delete s_Instance;
	s_Instance = 0;
}
ofxSpriteRenderer::ofxSpriteRenderer()
{
	if(s_Instance) return;
	ofxSpriteCommand::GenerateSharedIndices();
	m_Camera = new ofxOrthoCamera();
	s_Instance = this;
	m_CameraMove = true;
	m_CameraForce = true;
}
ofxSpriteRenderer::~ofxSpriteRenderer()
{
	ofxSpriteCommand::DeleteSharedIndices();
	{
		ofxSpriteCommands::iterator it = m_Commands.begin();
		for(;it != m_Commands.end();it++)
		{
			ofxSpriteCommand* cmd = *it;
			delete cmd;
		}
		m_Commands.clear();
	}
	{
		ofxSpriteQuads::iterator it = m_Quads.begin();
		for(;it != m_Quads.end();it++)
		{
			ofxSpriteQuad* quad = *it;
			delete quad;
		}
		m_Quads.clear();
	}
}
static bool QuadCompare(ofxSpriteQuad* quadA, ofxSpriteQuad* quadB)
{
	return quadA->IsBehind(quadB);
}
void ofxSpriteRenderer::Render()
{
	for(ofxSpriteCommands::iterator it = m_Commands.begin();it != m_Commands.end();it++)
	{
		ofxSpriteCommand* item = *it;
		delete item;
	}
	m_Commands.clear();
	printf("------------BuildCommands()\n");
	unsigned long long time_start_build = ofGetSystemTime();
	sort(m_Quads.begin(), m_Quads.end(), QuadCompare);
	for(int i=0;i<m_Quads.size();i++)
	{
		m_Quads[i]->SetID(i);
	}
	unsigned long long time_finish_sort = ofGetSystemTime();
	printf("sort time =  %llu\n", time_finish_sort - time_start_build);
	int sprite_count = 0;
	for(ofxSpriteQuads::iterator it = m_Quads.begin();it != m_Quads.end();it++)
	{
		ofxSpriteQuad* sprite = *it;
		if(sprite->GetVisibility() != QUAD_VISIBILITY_IN_SCREEN && sprite->GetVisibility() != QUAD_VISIBILITY_IN_SCREEN ||
			!sprite->GetVisible())
		{
			continue;
		}
		ofxSpriteCommand* command;
		if(m_Commands.size() == 0)
		{
			command = new ofxSpriteCommand();
			command->SetShader(sprite->GetShader());
			command->SetTexture(sprite->GetTexture());
			m_Commands.push_back(command);
			sprite_count = 0;
		}
		else
		{
			command = m_Commands.back();
			if(command->GetShader() != sprite->GetShader() 
				|| command->GetTexture() != sprite->GetTexture() 
				|| sprite_count > COMMAND_CAPACITY)
			{
				command = new ofxSpriteCommand();
				command->SetShader(sprite->GetShader());
				command->SetTexture(sprite->GetTexture());
				m_Commands.push_back(command);
				sprite_count = 0;
			}
		}
		command->PushSprite(sprite);
		sprite_count++;
	}
	unsigned long long time_finish_build = ofGetSystemTime();
	printf("build time =  %llu\n", time_finish_build - time_start_build);
#ifdef _DEBUG
	m_DrawnBatches = 0;
	m_DrawnVertices = 0;
	unsigned long long time_start_render = ofGetSystemTime();
#endif
	m_TransformMatrix = ofGetCurrentMatrix(OF_MATRIX_MODELVIEW)*ofGetCurrentMatrix(OF_MATRIX_PROJECTION);
	//m_Camera->begin();
	{
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		//glDisable(GL_DEPTH_TEST);// transparent isn't work well with depth test
		glDepthMask(GL_FALSE);
		for(ofxSpriteCommands::iterator it = m_Commands.begin();it != m_Commands.end();it++)
		{
			ofxSpriteCommand* cmd = *it;
			cmd->Render();
#ifdef _DEBUG
			m_DrawnBatches++;
			m_DrawnVertices += cmd->m_VerticesSize;
#endif
		}
		//glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
	//m_Camera->end();
#ifdef _DEBUG
	unsigned long long time_finish_render = ofGetSystemTime();
	m_RenderTimeMilisecond = time_finish_render - time_start_render;
#endif
}
void ofxSpriteRenderer::PushSprite(ofxSpriteQuad* sprite)
{
	sprite->SetID(m_Quads.size());
	m_Quads.push_back(sprite);
}
void ofxSpriteRenderer::EraseSprite(ofxSpriteQuad* sprite)
{
	m_Quads.erase(m_Quads.begin() + sprite->GetID());
	sprite->SetID(-1);
}

void ofxSpriteRenderer::Update()
{
#ifdef _DEBUG
	unsigned long long time_start_update = ofGetSystemTime();
#endif
	m_WorldRect.x = m_Camera->getPosition().x;
	m_WorldRect.y = m_Camera->getPosition().z;
	float width = m_RenderRect.z;
	float height = m_RenderRect.w*2.0;
	m_WorldRect.x -= width;
	m_WorldRect.y -= height;
	m_WorldRect.z = m_WorldRect.x + width*2;
	m_WorldRect.w = m_WorldRect.y + height*2;
	{
		ofxSpriteQuads::iterator it = m_Quads.begin();
		for(;it != m_Quads.end();it++)
		{
			ofxSpriteQuad* item = *it;
			item->Update(0.030f);
			item->SubmitChanges();
		}
	}
	m_CameraMove = false;
	m_CameraForce = false;
#ifdef _DEBUG
	unsigned long long time_finish_update = ofGetSystemTime();
	m_UpdateTimeMilisecond = time_finish_update - time_start_update;
#endif
}
void ofxSpriteRenderer::SetRenderSize(unsigned int width, unsigned int height)
{
	m_CameraMove = true;
	m_CameraForce = true;
	for(int i=0;i<m_Quads.size();i++)
	{
		m_Quads[i]->m_DimensionChange = true;
	}
	m_Camera->SetScale(min(width,height)*0.5);
	m_RenderRect.z = width*0.5;
	m_RenderRect.w = height*0.5;
	m_RenderRect.x = -m_RenderRect.z;
	m_RenderRect.y = -m_RenderRect.w;
}
void ofxSpriteRenderer::MoveCamera(float x, float y, float z)
{
	m_Camera->setPosition(
		m_Camera->getPosition().x + x,
		m_Camera->getPosition().y + y,
		m_Camera->getPosition().z + z);
	m_CameraMove = true;
	if(abs(x) > FAR_SCREEN_SPEED_THRESHOLD || abs(y) > FAR_SCREEN_SPEED_THRESHOLD)
		m_CameraForce = true;
}