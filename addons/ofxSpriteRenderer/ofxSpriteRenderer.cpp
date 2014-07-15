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
void ofxSpriteRenderer::Render()
{
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
		ofxSpriteCommands::iterator it = m_Commands.begin();
		for(;it != m_Commands.end();it++)
		{
			ofxSpriteCommand* cmd = *it;
			cmd->Render();
#ifdef _DEBUG
			if(cmd->m_VisibleSpriteCount != 0)
			{
				m_DrawnBatches++;
				m_DrawnVertices += cmd->m_Vertices.size();
			}
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
	sprite->m_IndexInRenderer = m_Quads.size();
	m_Quads.push_back(sprite);
	bool last_command_expanded = false;
	if(m_Commands.size() > 0)
	{
		ofxSpriteCommand* last_command = m_Commands.back();
		if(last_command->m_Status == COMMAND_STATUS_EXPANDED)
		{
			last_command_expanded = true;
			last_command->PushSprite(sprite);
		}
	}
	if(!last_command_expanded)
	{
		ofxSpriteCommand* command = new ofxSpriteCommand();
		m_Commands.push_back(command);
		command->m_Status = COMMAND_STATUS_EXPANDED;
		command->m_Textures = sprite->m_Textures;
		command->m_Shader = sprite->m_Shader;
		command->PushSprite(sprite);
	}
}
void ofxSpriteRenderer::EraseSprite(ofxSpriteQuad* sprite)
{
	// tell command not to draw it
	sprite->m_ParentCommand->EraseSprite(sprite);
	// move it to the very far, so it won't get updated
	sprite->MoveTo(ofVec3f(0.0f,FAR_SCREEN_DISTANCE_THRESHOLD,0.0f));
	// tell renderer mark it as unused
	m_UnusedQuads.push_back(sprite->m_IndexInRenderer);
	sprite->m_IndexInRenderer = -1;
}
static bool QuadCompare(ofxSpriteQuad* quadA, ofxSpriteQuad* quadB)
{
	/*if(quadA->GetVisibility() == QUAD_VISIBILITY_FAR_SCREEN) 
		return true;
	if(quadB->GetVisibility() == QUAD_VISIBILITY_FAR_SCREEN) 
		return true;*/
	//ofVec3f camera_position = ofxSpriteRenderer::GetInstance()->GetCamera()->getGlobalPosition();
	return quadA->GetWorldPosition().z < quadB->GetWorldPosition().z;
}
void ofxSpriteRenderer::BuildCommands(unsigned int begin_index, unsigned int end_index)
{
	printf("------------BuildCommands(%u,%u)\n",begin_index,end_index);
	unsigned long long time_start_build = ofGetSystemTime();
	sort(m_Quads.begin()+begin_index, m_Quads.begin()+end_index+1, QuadCompare);
	int max_index = m_Quads.size()-1;
	for(int i=begin_index;i<=end_index;i++)
	{
		m_Quads[i]->m_IndexInRenderer = i;
		if(i!=0) m_Quads[i]->m_PrevSibling = m_Quads[i-1];
		if(i!=max_index) m_Quads[i]->m_NextSibling = m_Quads[i+1];
	}
	{
		ofxShaderProgram* last_shader = 0;
		ofxTextures last_texture;
		ofxSpriteQuads::iterator it = m_Quads.begin()+begin_index;
		ofxSpriteQuads::iterator bound_it = m_Quads.begin()+end_index+1;
		int sprite_count = 0;
		for(;it != bound_it;it++)
		{
			ofxSpriteQuad* sprite = *it;
			ofxSpriteCommand* command;
			if(last_shader != sprite->GetShader() || last_texture != sprite->GetTextures() || sprite_count > COMMAND_CAPACITY)
			{
				command = new ofxSpriteCommand();
				m_Commands.push_back(command);
				last_shader = sprite->GetShader();
				last_texture = sprite->GetTextures();
				command->m_Shader = last_shader;
				command->m_Textures = last_texture;
				sprite_count = 0;
			}
			else
			{
				command = m_Commands.back();
			}
			command->PushSprite(sprite);
			sprite_count++;
		}
	}
	unsigned long long time_finish_build = ofGetSystemTime();
	printf("build time =  %llu\n",time_finish_build - time_start_build);
}
static bool CommandCompare(ofxSpriteCommand* cmdA, ofxSpriteCommand* cmdB)
{
	return cmdA->GetFirstSpriteIndex() < cmdB->GetFirstSpriteIndex();
}
void ofxSpriteRenderer::BuildOverlapStatus()
{
	for(int i=0;i<m_OverlapStatus.size();i++)
	{
		ofxSpriteCommand* command = m_Commands[i];
		if(command->m_Status == COMMAND_STATUS_DISMISSED)
		{
			m_OverlapStatus[i] = true;
		}
		else if(command->m_Status == COMMAND_STATUS_EXPANDED)
		{
			m_OverlapStatus[i] = true;
			int right = i + 1, left = i - 1;
			while(right < m_OverlapStatus.size())
			{
				if(command->m_DistanceMin < m_Commands[right]->m_DistanceMax)
				{
					m_OverlapStatus[right++] = true;
					continue;
				}
				break;
			}
			while(left > 0)
			{
				if(command->m_DistanceMax > m_Commands[left]->m_DistanceMin)
				{
					m_OverlapStatus[left--] = true;
					continue;
				}
				break;
			}
		}
	}
}
void ofxSpriteRenderer::SolveOverlap()
{
	int index = 0;
	int k = 0;
	int left = -1;
	int right = -1;
	while(k <= m_OverlapStatus.size())
	{
		// find segment
		if(k != m_OverlapStatus.size()) if(m_OverlapStatus[k])
		{
			if(left == -1)
			{
				right = left = index;
			}
			else
			{
				right = index;
			}
			index++;
			k++;
			continue;
		}
		// build segment
		if(right != -1)
		{
			int min_sprite_index = m_Commands[left]->m_FirstSpriteIndex;
			int max_sprite_index = m_Commands[right]->m_LastSpriteIndex;
			BuildCommands(min_sprite_index, max_sprite_index);
			for(int i=left;i<=right;right--)
			{
				ofxSpriteCommand* command = m_Commands[i];
				m_Commands.erase(m_Commands.begin() + i);
				delete command;
			}
			index = left;
			left = -1;
			right = -1;
		}
		else
		{
			index++;
		}
		k++;
	}
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
	bool transparent_commands_refreshed = CleanUnusedQuads();
	if(!transparent_commands_refreshed)
	{
		BuildOverlapStatus();
		SolveOverlap();
		sort(m_Commands.begin(),m_Commands.end(), CommandCompare);
		int max_index = m_Commands.size() - 1;
		for(int i=0;i<m_Commands.size();i++)
		{
			m_Commands[i]->m_IndexInRenderer = i;
			if(i!=0) m_Commands[i]->m_PrevSibling = m_Commands[i-1];
			if(i!=max_index) m_Commands[i]->m_NextSibling = m_Commands[i+1];
		}
	}
	m_CameraMove = false;
	m_CameraForce = false;
#ifdef _DEBUG
	unsigned long long time_finish_update = ofGetSystemTime();
	m_UpdateTimeMilisecond = time_finish_update - time_start_update;
#endif
}
#define UNUSED_TRANSPARENT_QUAD_LIMIT 20
bool ofxSpriteRenderer::CleanUnusedQuads()
{
	if(m_UnusedQuads.size() < UNUSED_TRANSPARENT_QUAD_LIMIT) return false;
	sort(m_UnusedQuads.begin(),m_UnusedQuads.end());
	for(int i=0;i < m_UnusedQuads.size(); i++)
	{
		unsigned int index = m_UnusedQuads[i] - i;
		ofxSpriteQuads::iterator it = m_Quads.begin() + index;
		m_Quads.erase(it);
	}
	m_UnusedQuads.clear();
	BuildCommands(0, m_Commands.size());
	return true;
}
void ofxSpriteRenderer::SetRenderSize(unsigned int width, unsigned int height)
{
	m_CameraMove = true;
	m_CameraForce = true;
	for(int i=0;i<m_Quads.size();i++)
	{
		m_Quads[i]->SetLogicWidth(m_Quads[i]->GetLogicWidth());
		m_Quads[i]->SetLogicHeight(m_Quads[i]->GetLogicHeight());
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