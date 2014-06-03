#include "ofxSpriteRenderer.h"
ofxSpriteRenderer* ofxSpriteRenderer::s_Instance = 0;
ofxSpriteRenderer::ofxSpriteRenderer()
{
	ofxSpriteCommand::GenerateSharedIndices();
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
	// it's a fancy and risky algorithm, need to test, test many times
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
	}
}

bool ofxSpriteRenderer::CheckVisibility(ofxSpriteQuad* quad)
{
	return true;
}