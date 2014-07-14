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
		ofxSpriteCommands::iterator it = m_SolidCommands.begin();
		for(;it != m_SolidCommands.end();it++)
		{
			ofxSpriteCommand* cmd = *it;
			delete cmd;
		}
		m_SolidCommands.clear();
	}
	{
		ofxSpriteCommands::iterator it = m_TransparentCommands.begin();
		for(;it != m_TransparentCommands.end();it++)
		{
			ofxSpriteCommand* cmd = *it;
			delete cmd;
		}
		m_TransparentCommands.clear();
	}
	{
		ofxSpriteQuads::iterator it = m_SolidQuads.begin();
		for(;it != m_SolidQuads.end();it++)
		{
			ofxSpriteQuad* quad = *it;
			delete quad;
		}
		m_SolidQuads.clear();
	}
	{
		ofxSpriteQuads::iterator it = m_TransparentQuads.begin();
		for(;it != m_TransparentQuads.end();it++)
		{
			ofxSpriteQuad* quad = *it;
			delete quad;
		}
		m_TransparentQuads.clear();
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
		ofxSpriteCommands::iterator it = m_SolidCommands.begin();
		for(;it != m_SolidCommands.end();it++)
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
	}
	{
		//glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		//glDisable(GL_DEPTH_TEST);// transparent isn't work well with depth test
		glDepthMask(GL_FALSE);
		ofxSpriteCommands::iterator it = m_TransparentCommands.begin();
		for(;it != m_TransparentCommands.end();it++)
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
	if(sprite->IsTransparent())
	{
		sprite->m_IndexInRenderer = m_TransparentQuads.size();
		m_TransparentQuads.push_back(sprite);
		bool last_command_expanded = false;
		if(m_TransparentCommands.size() > 0)
		{
			ofxSpriteCommand* last_command = m_TransparentCommands.back();
			if(last_command->m_Status == COMMAND_STATUS_EXPANDED)
			{
				last_command_expanded = true;
				last_command->PushSprite(sprite);
			}
		}
		if(!last_command_expanded)
		{
			ofxSpriteCommand* command = new ofxSpriteCommand();
			m_TransparentCommands.push_back(command);
			command->m_Status = COMMAND_STATUS_EXPANDED;
			command->m_Textures = sprite->m_Textures;
			command->m_Shader = sprite->m_Shader;
			command->PushSprite(sprite);
		}
	}
	else
	{
		m_SolidQuads.push_back(sprite);
		sprite->m_IndexInRenderer = m_SolidQuads.size();
		m_SolidQuads.push_back(sprite);
		bool last_command_expanded = false;
		if(m_SolidCommands.size() > 0)
		{
			ofxSpriteCommand* last_command = m_SolidCommands.back();
			if(last_command->m_Status == COMMAND_STATUS_EXPANDED)
			{
				last_command_expanded = true;
				last_command->PushSprite(sprite);
			}
		}
		if(!last_command_expanded)
		{
			ofxSpriteCommand* command = new ofxSpriteCommand();
			m_SolidCommands.push_back(command);
			command->m_Status = COMMAND_STATUS_EXPANDED;
			command->m_Textures = sprite->m_Textures;
			command->m_Shader = sprite->m_Shader;
			command->PushSprite(sprite);
		}
	}
}
void ofxSpriteRenderer::EraseSprite(ofxSpriteQuad* sprite)
{
	// tell command not to draw it
	sprite->m_ParentCommand->EraseSprite(sprite);
	// move it to the very far, so it won't get updated
	sprite->MoveTo(ofVec3f(0.0f,FAR_SCREEN_DISTANCE_THRESHOLD,0.0f));
	// tell renderer mark it as unused
	if(sprite->IsTransparent())
	{
		m_UnusedTransparentQuads.push_back(sprite->m_IndexInRenderer);
	}
	else
	{
		m_UnusedSolidQuads.push_back(sprite->m_IndexInRenderer);
	}
	sprite->m_IndexInRenderer = -1;
}

static bool SolidQuadCompare(ofxSpriteQuad* quadA, ofxSpriteQuad* quadB)
{
	/*if(quadA->GetVisibility() == QUAD_VISIBILITY_FAR_SCREEN) return true;
	if(quadB->GetVisibility() == QUAD_VISIBILITY_FAR_SCREEN) return true;*/
	//ofVec3f camera_position = ofxSpriteRenderer::GetInstance()->GetCamera()->getGlobalPosition();
	return quadA->GetWorldPosition().z > quadB->GetWorldPosition().z;
}
static bool TransparentQuadCompare(ofxSpriteQuad* quadA, ofxSpriteQuad* quadB)
{
	/*if(quadA->GetVisibility() == QUAD_VISIBILITY_FAR_SCREEN) 
		return true;
	if(quadB->GetVisibility() == QUAD_VISIBILITY_FAR_SCREEN) 
		return true;*/
	//ofVec3f camera_position = ofxSpriteRenderer::GetInstance()->GetCamera()->getGlobalPosition();
	return quadA->GetWorldPosition().z < quadB->GetWorldPosition().z;
}

void ofxSpriteRenderer::BuildSolidCommands(unsigned int begin_index, unsigned int end_index)
{
	printf("------------BuildSolidCommands(%u,%u)\n",begin_index,end_index);
	sort(m_SolidQuads.begin()+begin_index, m_SolidQuads.begin()+end_index+1, SolidQuadCompare);
	for(int i=begin_index;i<=end_index;i++)
	{
		m_SolidQuads[i]->m_IndexInRenderer = i;
	}
	{
		ofxShaderProgram* last_shader = 0;
		ofxTextures last_texture;
		ofxSpriteQuads::iterator it = m_SolidQuads.begin()+begin_index;
		ofxSpriteQuads::iterator bound_it = m_SolidQuads.begin()+end_index+1;
		int sprite_count = 0;
		for(;it != bound_it;it++)
		{
			ofxSpriteQuad* sprite = *it;
			ofxSpriteCommand* command;
			if(last_shader != sprite->GetShader() || last_texture != sprite->GetTextures() ||  sprite_count > COMMAND_CAPACITY)
			{
				command = new ofxSpriteCommand();
				m_SolidCommands.push_back(command);
				last_shader = sprite->GetShader();
				last_texture = sprite->GetTextures();
				command->m_Shader = last_shader;
				command->m_Textures = last_texture;
				sprite_count = 0;
			}
			else
			{
				command = m_SolidCommands.back();
			}
			command->PushSprite(sprite);
			sprite_count++;
		}
	}
}

void ofxSpriteRenderer::BuildTransparentCommands(unsigned int begin_index, unsigned int end_index)
{
	printf("------------BuildTransparentCommands(%u,%u)\n",begin_index,end_index);
	unsigned long long time_start_build = ofGetSystemTime();
	sort(m_TransparentQuads.begin()+begin_index, m_TransparentQuads.begin()+end_index+1, TransparentQuadCompare);
	for(int i=begin_index;i<=end_index;i++)
	{
		m_TransparentQuads[i]->m_IndexInRenderer = i;
	}
	{
		ofxShaderProgram* last_shader = 0;
		ofxTextures last_texture;
		ofxSpriteQuads::iterator it = m_TransparentQuads.begin()+begin_index;
		ofxSpriteQuads::iterator bound_it = m_TransparentQuads.begin()+end_index+1;
		int sprite_count = 0;
		for(;it != bound_it;it++)
		{
			ofxSpriteQuad* sprite = *it;
			ofxSpriteCommand* command;
			if(last_shader != sprite->GetShader() || last_texture != sprite->GetTextures() || sprite_count > COMMAND_CAPACITY)
			{
				command = new ofxSpriteCommand();
				m_TransparentCommands.push_back(command);
				last_shader = sprite->GetShader();
				last_texture = sprite->GetTextures();
				command->m_Shader = last_shader;
				command->m_Textures = last_texture;
				sprite_count = 0;
			}
			else
			{
				command = m_TransparentCommands.back();
			}
			command->PushSprite(sprite);
			sprite_count++;
		}
	}
	unsigned long long time_finish_build = ofGetSystemTime();
	printf("build time =  %llu\n",time_finish_build - time_start_build);
}
static bool SolidCommandCompare(ofxSpriteCommand* cmdA, ofxSpriteCommand* cmdB)
{
	return cmdA->GetFirstSpriteIndex() < cmdB->GetFirstSpriteIndex();
}
static bool TransparentCommandCompare(ofxSpriteCommand* cmdA, ofxSpriteCommand* cmdB)
{
	return cmdA->GetFirstSpriteIndex() < cmdB->GetFirstSpriteIndex();
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
		ofxSpriteQuads::iterator it = m_TransparentQuads.begin();
		for(;it != m_TransparentQuads.end();it++)
		{
			ofxSpriteQuad* item = *it;
			item->Update(0.030f);
			item->SubmitChanges();
		}
	}
	{
		ofxSpriteQuads::iterator it = m_SolidQuads.begin();
		for(;it != m_SolidQuads.end();it++)
		{
			ofxSpriteQuad* item = *it;
			item->Update(0.030f);
			item->SubmitChanges();
		}
	}
	bool transparent_commands_refreshed = CleanUnusedTransparentQuads();
	bool solid_commands_refreshed = CleanUnusedSolidQuads();
	// TODO: it's a fancy and risky algorithm, need to test, test many times
	// for now, after some rough test & fix, it works, hope it works fine
	if(!solid_commands_refreshed)
	{
		// find overlap
		int size = m_SolidCommands.size();
		bool* need_to_rebuild = new bool[size];
		memset(need_to_rebuild,false,sizeof(bool)*size);
		for(int i=0;i<size;i++)
		{
			ofxSpriteCommand* command = m_SolidCommands[i];
			if(command->m_Status == COMMAND_STATUS_DISMISSED)
			{
				need_to_rebuild[i] = true;
			}
			else if(command->m_Status == COMMAND_STATUS_EXPANDED)
			{
				need_to_rebuild[i] = true;
				int right = i + 1, left = i - 1;
				while(right < size)
				{
					if(command->m_DistanceMin < m_SolidCommands[right]->m_DistanceMax)
					{
						need_to_rebuild[right++] = true;
						continue;
					}
					break;
				}
				while(left > 0)
				{
					if(command->m_DistanceMax > m_SolidCommands[left]->m_DistanceMin)
					{
						need_to_rebuild[left--] = true;
						continue;
					}
					break;
				}
			}
		}
		// trace
		int index = 0;
		int k = 0;
		int left = -1;
		int right = -1;
		while(k <= size)
		{
			// find segment
			if(k != size) if(need_to_rebuild[k])
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
				int min_sprite_index = m_SolidCommands[left]->m_FirstSpriteIndex;
				int max_sprite_index = m_SolidCommands[right]->m_LastSpriteIndex;
				BuildSolidCommands(min_sprite_index, max_sprite_index);
				for(int i=left;i<=right;right--)
				{
					ofxSpriteCommand* command = m_SolidCommands[i];
					m_SolidCommands.erase(m_SolidCommands.begin() + i);
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
		// sort
		sort(m_SolidCommands.begin(),m_SolidCommands.end(), SolidCommandCompare);
		for(int i=0;i<m_SolidCommands.size();i++)
		{
			m_SolidCommands[i]->m_IndexInRenderer = i;
		}
	}
	if(!transparent_commands_refreshed)
	{
		// find overlap
		int size = m_TransparentCommands.size();
		bool* need_to_rebuild = new bool[size];
		memset(need_to_rebuild,false,sizeof(bool)*size);
		for(int i=0;i<size;i++)
		{
			ofxSpriteCommand* command = m_TransparentCommands[i];
			if(command->m_Status == COMMAND_STATUS_DISMISSED)
			{
				need_to_rebuild[i] = true;
			}
			else if(command->m_Status == COMMAND_STATUS_EXPANDED)
			{
				need_to_rebuild[i] = true;
				int right = i+1, left = i-1;
				while(right < size)
				{
					if(command->m_DistanceMax > m_TransparentCommands[right]->m_DistanceMin)
					{
						need_to_rebuild[right++] = true;
						continue;
					}
					break;
				}
				while(left > 0)
				{
					if(command->m_DistanceMin < m_TransparentCommands[left]->m_DistanceMax)
					{
						need_to_rebuild[left--] = true;
						continue;
					}
					break;
				}
			}
		}
		// trace
		int index = 0;
		int k = 0;
		int left = -1;
		int right = -1;
		while(k <= size)
		{
			// find segment
			if(k != size) if(need_to_rebuild[k])
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
				int min_sprite_index = m_TransparentCommands[left]->m_FirstSpriteIndex;
				int max_sprite_index = m_TransparentCommands[right]->m_LastSpriteIndex;
				BuildTransparentCommands(min_sprite_index, max_sprite_index);
				for(int i=left;i<=right;right--)
				{
					ofxSpriteCommand* command = m_TransparentCommands[i];
					m_TransparentCommands.erase(m_TransparentCommands.begin() + i);
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
		// sort
		sort(m_TransparentCommands.begin(),m_TransparentCommands.end(), TransparentCommandCompare);
		for(int i=0;i<m_TransparentCommands.size();i++)
		{
			m_TransparentCommands[i]->m_IndexInRenderer = i;
		}
	}
	m_CameraMove = false;
	m_CameraForce = false;
#ifdef _DEBUG
	unsigned long long time_finish_update = ofGetSystemTime();
	m_UpdateTimeMilisecond = time_finish_update - time_start_update;
#endif
}
#define UNUSED_SOLID_QUAD_LIMIT 20
bool ofxSpriteRenderer::CleanUnusedSolidQuads()
{
	if(m_UnusedSolidQuads.size() < UNUSED_SOLID_QUAD_LIMIT) return false;
	sort(m_UnusedSolidQuads.begin(),m_UnusedSolidQuads.end());
	for(int i=0;i < m_UnusedSolidQuads.size(); i++)
	{
		unsigned int index = m_UnusedSolidQuads[i] - i;
		ofxSpriteQuads::iterator it = m_SolidQuads.begin() + index;
		m_SolidQuads.erase(it);
	}
	m_UnusedSolidQuads.clear();
	BuildSolidCommands(0, m_SolidCommands.size());
	return true;
}
#define UNUSED_TRANSPARENT_QUAD_LIMIT 20
bool ofxSpriteRenderer::CleanUnusedTransparentQuads()
{
	if(m_UnusedTransparentQuads.size() < UNUSED_TRANSPARENT_QUAD_LIMIT) return false;
	sort(m_UnusedTransparentQuads.begin(),m_UnusedTransparentQuads.end());
	for(int i=0;i < m_UnusedTransparentQuads.size(); i++)
	{
		unsigned int index = m_UnusedTransparentQuads[i] - i;
		ofxSpriteQuads::iterator it = m_TransparentQuads.begin() + index;
		m_TransparentQuads.erase(it);
	}
	m_UnusedTransparentQuads.clear();
	BuildSolidCommands(0, m_TransparentCommands.size());
	return true;
}
void ofxSpriteRenderer::SetRenderSize(unsigned int width, unsigned int height)
{
	m_CameraMove = true;
	m_CameraForce = true;
	for(int i=0;i<m_TransparentQuads.size();i++)
	{
		m_TransparentQuads[i]->SetLogicWidth(m_TransparentQuads[i]->GetLogicWidth());
		m_TransparentQuads[i]->SetLogicHeight(m_TransparentQuads[i]->GetLogicHeight());
	}
	for(int i=0;i<m_SolidQuads.size();i++)
	{
		m_SolidQuads[i]->SetLogicWidth(m_SolidQuads[i]->GetLogicWidth());
		m_SolidQuads[i]->SetLogicHeight(m_SolidQuads[i]->GetLogicHeight());
	}
	m_Camera->SetScale(min(width,height)*0.5);
	m_RenderRect.z = width*0.5;
	m_RenderRect.w = height*0.5;
	m_RenderRect.x = -m_RenderRect.z;
	m_RenderRect.y = -m_RenderRect.w;
}