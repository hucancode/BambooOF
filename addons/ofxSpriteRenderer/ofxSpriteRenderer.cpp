#include "ofxSpriteRenderer.h"
ofxSpriteRenderer* ofxSpriteRenderer::s_Instance = 0;
ofxSpriteRenderer::ofxSpriteRenderer()
{
	ofxSpriteCommand::GenerateSharedIndices();
	m_Camera = new ofxOrthoCamera();
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
	//m_Camera->begin();
	{
		ofxSpriteCommands::iterator it = m_SolidCommands.begin();
		for(;it != m_SolidCommands.end();it++)
		{
			ofxSpriteCommand* cmd = *it;
			cmd->Render();
		}
	}
	{
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
		glEnable(GL_BLEND);
		//glDisable(GL_DEPTH_TEST);// transparent isn't work well with depth test
		glDepthMask(GL_FALSE);
		ofxSpriteCommands::iterator it = m_TransparentCommands.begin();
		for(;it != m_TransparentCommands.end();it++)
		{
			ofxSpriteCommand* cmd = *it;
			cmd->Render();
		}
		//glEnable(GL_DEPTH_TEST);
		glDepthMask(GL_TRUE);
		glDisable(GL_BLEND);
	}
	//m_Camera->end();
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
			command->SetMaterial(sprite->GetMaterial());
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
			command->SetMaterial(sprite->GetMaterial());
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
}

static bool SolidQuadCompare(ofxSpriteQuad* quadA, ofxSpriteQuad* quadB)
{
	if(quadA->GetVisibility() == QUAD_VISIBILITY_FAR_SCREEN) return true;
	if(quadB->GetVisibility() == QUAD_VISIBILITY_FAR_SCREEN) return true;
	ofVec3f camera_position = ofxSpriteRenderer::GetInstance()->GetCamera()->getGlobalPosition();
	return quadA->CalculateDistanceToCamera(camera_position) < quadB->CalculateDistanceToCamera(camera_position);
}
static bool TransparentQuadCompare(ofxSpriteQuad* quadA, ofxSpriteQuad* quadB)
{
	if(quadA->GetVisibility() == QUAD_VISIBILITY_FAR_SCREEN) 
		return true;
	if(quadB->GetVisibility() == QUAD_VISIBILITY_FAR_SCREEN) 
		return true;
	ofVec3f camera_position = ofxSpriteRenderer::GetInstance()->GetCamera()->getGlobalPosition();
	return quadA->CalculateDistanceToCamera(camera_position) > quadB->CalculateDistanceToCamera(camera_position);
}
void ofxSpriteRenderer::BuildSolidCommands(unsigned int i, unsigned int j)
{
	sort(m_SolidQuads.begin()+i, m_SolidQuads.begin()+j, SolidQuadCompare);
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
				m_SolidCommands.push_back(command);
				last_material = sprite->GetMaterial();
				command->SetMaterial(last_material);
#ifdef DEBUG
				m_DrawnBatches++;
				m_DrawnVertices += 6;
#endif
			}
			else
			{
				command = m_SolidCommands.back();
			}
			command->PushSprite(sprite);
		}
	}
}
void ofxSpriteRenderer::BuildTransparentCommands(unsigned int i, unsigned int j)
{
	printf("------------BuildTransparentCommands(%u,%u)\n",i,j);
	sort(m_TransparentQuads.begin()+i, m_TransparentQuads.begin()+j, TransparentQuadCompare);
	for(int i=0;i<m_TransparentQuads.size();i++)
	{
		m_TransparentQuads[i]->m_IndexInRenderer = i;
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
				m_TransparentCommands.push_back(command);
				last_material = sprite->GetMaterial();
				command->SetMaterial(last_material);
#ifdef DEBUG
				m_DrawnBatches++;
				m_DrawnVertices += 6;
#endif
			}
			else
			{
				command = m_TransparentCommands.back();
			}
			command->PushSprite(sprite);
		}
	}
}
static bool SolidCommandCompare(ofxSpriteCommand* cmdA, ofxSpriteCommand* cmdB)
{
	return cmdA->GetFirstSpriteIndex() > cmdB->GetFirstSpriteIndex();
}
static bool TransparentCommandCompare(ofxSpriteCommand* cmdA, ofxSpriteCommand* cmdB)
{
	return cmdA->GetFirstSpriteIndex() < cmdB->GetFirstSpriteIndex();
}
void ofxSpriteRenderer::Update()
{
	{
		ofxSpriteQuads::iterator it = m_TransparentQuads.begin();
		for(;it != m_TransparentQuads.end();it++)
		{
			ofxSpriteQuad* item = *it;
			item->Update(0.030f);
			item->UpdateVisibility(m_CameraUpdated);
			item->SubmitChanges();
		}
	}
	{
		ofxSpriteQuads::iterator it = m_SolidQuads.begin();
		for(;it != m_SolidQuads.end();it++)
		{
			ofxSpriteQuad* item = *it;
			item->Update(0.030f);
			item->UpdateVisibility(m_CameraUpdated);
			item->SubmitChanges();
		}
	}
	bool transparent_commands_refreshed = CleanUnusedTransparentQuads();
	bool solid_commands_refreshed = CleanUnusedSolidQuads();
	m_CameraUpdated = true;
	// TODO: it's a fancy and risky algorithm, need to test, test many times
	// for now, i can't find a way to test it. maybe it's bug will apear in future action
	if(!solid_commands_refreshed)
	{
		int size = m_SolidCommands.size();
		bool* need_to_rebuild = new bool[size];
		for(int i=0;i<size;i++)
		{
			ofxSpriteCommand* command = m_SolidCommands[i];
			if(need_to_rebuild[i]) continue;
			if(command->m_Status == COMMAND_STATUS_UNITED)
			{
				need_to_rebuild[i] = false;
			}
			else if(command->m_Status == COMMAND_STATUS_DISMISSED)
			{
				need_to_rebuild[i] = true;
			}
			else if(command->m_Status == COMMAND_STATUS_EXPANDED)
			{
				int right = i;
				while(right < size)
				{
					ofxSpriteCommand* expanded = m_SolidCommands[right];
					if(command->m_DistanceMin < expanded->m_DistanceMax)
					{
						need_to_rebuild[right++] = true;
					}
					else
					{
						break;
					}
				}
				int left = i;
				while(left > 0)
				{
					ofxSpriteCommand* expanded = m_SolidCommands[left];
					if(command->m_DistanceMax > expanded->m_DistanceMin)
					{
						need_to_rebuild[left--] = true;
					}
					else
					{
						break;
					}
				}
			}
		}
		int index = 0;
		int bound = m_SolidCommands.size();
		int k = 0;
		int left = -1;
		int right = -1;
		while(k <= size)
		{
			bool trace = true;
			if(k == size) trace = false;
			else if(!need_to_rebuild[k]) trace = false;
			if(trace)
			{
				if(left == -1)
				{
					left = index;
					right = index;
				}
				else
				{
					right = index;
				}
				index++;
			}
			else
			{
				if(right != -1)
				{
					int min_sprite_index = m_SolidCommands[left]->m_FirstSpriteIndex;
					int max_sprite_index = m_SolidCommands[right]->m_LastSpriteIndex;
					for(int i=left;i<=right;i++)
					{
						ofxSpriteCommand* command = m_SolidCommands[i];
						m_SolidCommands.erase(m_SolidCommands.begin() + i);
						delete command;
						bound--;
					}
					BuildSolidCommands(min_sprite_index, max_sprite_index);
					index = left;
					left = -1;
					right = -1;
				}
			}
			k++;
		}
		sort(m_SolidCommands.begin(),m_SolidCommands.end(), SolidCommandCompare);
		for(int i=0;i<m_SolidCommands.size();i++)
		{
			m_SolidCommands[i]->m_IndexInRenderer = i;
		}
	}
	if(!transparent_commands_refreshed)
	{
		int size = m_TransparentCommands.size();
		bool* need_to_rebuild = new bool[size];
		memset(need_to_rebuild,false,sizeof(bool)*size);
		for(int i=0;i<size;i++)
		{
			ofxSpriteCommand* command = m_TransparentCommands[i];
			if(need_to_rebuild[i]) continue;
			if(command->m_Status == COMMAND_STATUS_UNITED)
			{
				need_to_rebuild[i] = false;
			}
			else if(command->m_Status == COMMAND_STATUS_DISMISSED)
			{
				need_to_rebuild[i] = true;
			}
			else if(command->m_Status == COMMAND_STATUS_EXPANDED)
			{
				need_to_rebuild[i] = true;
				int right = i+1;
				while(right < size)
				{
					ofxSpriteCommand* expanded = m_TransparentCommands[right];
					if(command->m_DistanceMax > expanded->m_DistanceMin)
					{
						need_to_rebuild[right++] = true;
					}
					else
					{
						break;
					}
				}
				int left = i-1;
				while(left > 0)
				{
					ofxSpriteCommand* expanded = m_TransparentCommands[left];
					if(command->m_DistanceMin < expanded->m_DistanceMax)
					{
						need_to_rebuild[left--] = true;
					}
					else
					{
						break;
					}
				}
			}
		}
		int index = 0;
		int bound = m_TransparentCommands.size();
		int k = 0;
		int left = -1;
		int right = -1;
		while(k <= size)
		{
			bool trace = true;
			if(k == size) trace = false;
			else if(!need_to_rebuild[k]) trace = false;
			if(trace)
			{
				if(left == -1)
				{
					left = index;
					right = index;
				}
				else
				{
					right = index;
				}
				index++;
			}
			else
			{
				if(right != -1)
				{
					int min_sprite_index = m_TransparentCommands[left]->m_FirstSpriteIndex;
					int max_sprite_index = m_TransparentCommands[right]->m_LastSpriteIndex;
					for(int i=left;i<=right;i++)
					{
						ofxSpriteCommand* command = m_TransparentCommands[i];
						m_TransparentCommands.erase(m_TransparentCommands.begin() + i);
						delete command;
						bound--;
					}
					BuildTransparentCommands(min_sprite_index, max_sprite_index+1);
					index = left;
					left = -1;
					right = -1;
				}
			}
			
			k++;
		}
		sort(m_TransparentCommands.begin(),m_TransparentCommands.end(), TransparentCommandCompare);
		for(int i=0;i<m_TransparentCommands.size();i++)
		{
			m_TransparentCommands[i]->m_IndexInRenderer = i;
		}
	}
}
#define UNUSED_SOLID_QUAD_LIMIT 20
bool ofxSpriteRenderer::CleanUnusedSolidQuads()
{
	if(m_UnusedSolidQuads.size() < UNUSED_SOLID_QUAD_LIMIT) return false;
	for(int i=0;i < m_UnusedSolidQuads.size(); i++)
	{
		unsigned int index = m_UnusedSolidQuads[i];
		ofxSpriteQuads::iterator it = m_SolidQuads.begin() + index;
		m_SolidQuads.erase(it);
		delete (*it);
	}
	m_UnusedSolidQuads.clear();
	BuildSolidCommands(0, m_SolidCommands.size());
	return true;
}
#define UNUSED_TRANSPARENT_QUAD_LIMIT 20
bool ofxSpriteRenderer::CleanUnusedTransparentQuads()
{
	if(m_UnusedTransparentQuads.size() < UNUSED_TRANSPARENT_QUAD_LIMIT) return false;
	for(int i=0;i < m_UnusedTransparentQuads.size(); i++)
	{
		unsigned int index = m_UnusedTransparentQuads[i];
		ofxSpriteQuads::iterator it = m_TransparentQuads.begin() + index;
		m_TransparentQuads.erase(it);
		delete (*it);
	}
	m_UnusedTransparentQuads.clear();
	BuildSolidCommands(0, m_TransparentCommands.size());
	return true;
}