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
	m_Camera->begin();
	{
		ofxSpriteCommands::iterator it = m_SolidCommands.begin();
		for(;it != m_SolidCommands.end();it++)
		{
			ofxSpriteCommand* cmd = *it;
			cmd->Render();
		}
	}
	{
		ofxSpriteCommands::iterator it = m_TransparentCommands.begin();
		for(;it != m_TransparentCommands.end();it++)
		{
			ofxSpriteCommand* cmd = *it;
			cmd->Render();
		}
	}
	m_Camera->end();
}
void ofxSpriteRenderer::PushSprite(ofxSpriteQuad* sprite)
{
	if(sprite->IsTransparent())
	{
		m_TransparentQuads.push_back(sprite);
		// TODO: find command for sprite to get into
	}
	else
	{
		m_SolidQuads.push_back(sprite);
		// find command for sprite to get into
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

static bool SolidQuadCompare(ofxSpriteQuad* quadA, ofxSpriteQuad* quadB)
{
	if(quadA->GetVisibility() == QUAD_VISIBILITY_FAR_SCREEN) return true;
	if(quadB->GetVisibility() == QUAD_VISIBILITY_FAR_SCREEN) return true;
	ofVec3f camera_position = ofxSpriteRenderer::GetInstance()->GetCamera()->getGlobalPosition();
	return quadA->CalculateDistanceToCamera(camera_position) < quadB->CalculateDistanceToCamera(camera_position);
}
static bool TransparentQuadCompare(ofxSpriteQuad* quadA, ofxSpriteQuad* quadB)
{
	if(quadA->GetVisibility() == QUAD_VISIBILITY_FAR_SCREEN) return true;
	if(quadB->GetVisibility() == QUAD_VISIBILITY_FAR_SCREEN) return true;
	ofVec3f camera_position = ofxSpriteRenderer::GetInstance()->GetCamera()->getGlobalPosition();
	return quadA->CalculateDistanceToCamera(camera_position) > quadB->CalculateDistanceToCamera(camera_position);
}
void ofxSpriteRenderer::BuildSolidCommands(unsigned int i, unsigned int j)
{
	sort(m_SolidQuads.begin()+i, m_SolidQuads.begin()+j, SolidQuadCompare);
	{
		ofxSpriteCommands::iterator it = m_SolidCommands.begin();
		for(;it != m_SolidCommands.end();it++)
		{
			ofxSpriteCommand* command = *it;
			delete command;
		}
		m_SolidCommands.clear();
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
	sort(m_TransparentQuads.begin()+i, m_TransparentQuads.begin()+j, TransparentQuadCompare);
	{
		ofxSpriteCommands::iterator it = m_TransparentCommands.begin();
		for(;it != m_TransparentCommands.end();it++)
		{
			ofxSpriteCommand* command = *it;
			delete command;
		}
		m_TransparentCommands.clear();
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
	return cmdA->GetFirstSpriteIndex() >= cmdB->GetFirstSpriteIndex();
}
static bool TransparentCommandCompare(ofxSpriteCommand* cmdA, ofxSpriteCommand* cmdB)
{
	return cmdA->GetFirstSpriteIndex() <= cmdB->GetFirstSpriteIndex();
}
void ofxSpriteRenderer::Update()
{
	{
		ofxSpriteQuads::iterator it = m_TransparentQuads.begin();
		for(;it != m_TransparentQuads.end();it++)
		{
			ofxSpriteQuad* item = *it;
			item->Update(0.030f);
			UpdateVisibility(item);
			item->SubmitChanges();
		}
	}
	{
		ofxSpriteQuads::iterator it = m_SolidQuads.begin();
		for(;it != m_SolidQuads.end();it++)
		{
			ofxSpriteQuad* item = *it;
			item->Update(0.030f);
			UpdateVisibility(item);
			item->SubmitChanges();
		}
	}
	m_CameraUpdated = true;
	// TODO: it's a fancy and risky algorithm, need to test, test many times
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
		while(index < bound)
		{
			if(need_to_rebuild[k])
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
					int max_sprite_index = m_SolidCommands[left]->m_LastSpriteIndex;
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
	{
		int size = m_TransparentCommands.size();
		bool* need_to_rebuild = new bool[size];
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
				int right = i;
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
				int left = i;
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
		while(index < bound)
		{
			if(need_to_rebuild[k])
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
					int max_sprite_index = m_TransparentCommands[left]->m_LastSpriteIndex;
					for(int i=left;i<=right;i++)
					{
						ofxSpriteCommand* command = m_TransparentCommands[i];
						m_TransparentCommands.erase(m_TransparentCommands.begin() + i);
						delete command;
						bound--;
					}
					BuildTransparentCommands(min_sprite_index, max_sprite_index);
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
			//m_TransparentCommands[i]->m_FirstSpriteIndex;
			//m_TransparentCommands[i]->m_LastSpriteIndex;
			//m_TransparentCommands[i]->m_DistanceMin;
			//m_TransparentCommands[i]->m_DistanceMax;
		}
	}
}
const int g_ScreenWidth = 800;
const int g_ScreenHeight = 600;
void ofxSpriteRenderer::UpdateVisibility(ofxSpriteQuad* quad)
{
	unsigned int frame_number = 1;
	if(quad->m_Visibility == QUAD_VISIBILITY_FAR_SCREEN 
		&& frame_number % FAR_SCREEN_UPDATE_SEQUENCE != 0)
	{
		return;
	}
	if(!(quad->IsScreenPositionUpdated() && m_CameraUpdated))
	{
		if(quad->m_Status == QUAD_STATUS_NO_CHANGE)
		{
			quad->m_Status = QUAD_STATUS_SAFE_CHANGE;
		}
		quad->CalculateScreenPosition(m_Camera->getGlobalPosition());
	}
	if(quad->m_Status == QUAD_STATUS_NO_CHANGE || quad->m_Status == QUAD_STATUS_MATERIAL_CHANGE) return;
	float x_min = quad->GetScreenPosition().x - quad->GetWidth()*0.5;
	float x_max = x_min + quad->GetWidth();
	float y_min = quad->GetScreenPosition().y;
	float y_max = y_min + quad->GetHeight();

	if(y_max < -FAR_SCREEN_DISTANCE_THRESHOLD)
	{
		quad->m_Visibility = QUAD_VISIBILITY_FAR_SCREEN;
	}
	else if(x_max < -FAR_SCREEN_DISTANCE_THRESHOLD)
	{
		quad->m_Visibility = QUAD_VISIBILITY_FAR_SCREEN;
	}
	else if(y_min > FAR_SCREEN_DISTANCE_THRESHOLD)
	{
		quad->m_Visibility = QUAD_VISIBILITY_FAR_SCREEN;
	}
	else if(x_min > FAR_SCREEN_DISTANCE_THRESHOLD)
	{
		quad->m_Visibility = QUAD_VISIBILITY_FAR_SCREEN;
	}
	else if(y_max < -1.0f)
	{
		quad->m_Visibility = QUAD_VISIBILITY_OFF_SCREEN;
	}
	else if(x_max < -1.0f)
	{
		quad->m_Visibility = QUAD_VISIBILITY_OFF_SCREEN;
	}
	else if(y_min > 1.0f)
	{
		quad->m_Visibility = QUAD_VISIBILITY_OFF_SCREEN;
	}
	else if(x_min > 1.0f)
	{
		quad->m_Visibility = QUAD_VISIBILITY_OFF_SCREEN;
	}
	else
	{
		quad->m_Visibility = QUAD_VISIBILITY_IN_SCREEN;
	}
}