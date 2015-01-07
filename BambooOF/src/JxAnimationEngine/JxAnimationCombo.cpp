#include "JxAnimationCombo.h"
#include "ofxSpriteRenderer.h"
#include "ofxShaderProgramCache.h"
#include "JxAnimationCache.h"

JxAnimationCombo::JxAnimationCombo():
	ofxBaseSprite()
{
	glGenBuffers(1, &m_VBOId);
	m_Renderable = true;
	m_FrameTimer = 0;
	m_FrameTime = 0;
	m_CurrentFrame = 0;
	m_FrameMin = 0;
	m_FrameMax = 0;
	m_Direction = JX_DIRECTION_DOWN;
	m_CurrentState = JX_ANIMATION_STATE_ZEN;
	m_Helm = 0;
	SetHelmAnimation("basic1");
	m_Cloth = 0;
	SetClothAnimation("basic1");
	m_ArmL = 0;
	SetArmLAnimation("basic1");
	m_ArmR = 0;
	SetArmRAnimation("basic1");
	m_WeaponLight = 0;
	m_WeaponHeavy = 0;
	m_WeaponDualL = 0;
	m_WeaponDualR = 0;
	m_HorseHead = 0;
	m_HorseBack = 0;
	m_HorseTail = 0;
	LoadShader(DEFAULT_JXANIMATION_SHADER);
	ofxRENDERER->PushSprite(this);
}
JxAnimationCombo::~JxAnimationCombo()
{
}

void JxAnimationCombo::SetColor(const ofFloatColor color)
{
}
void JxAnimationCombo::SetColorIntensity(const float color_intensity)
{
}
void JxAnimationCombo::SetOpacity(const float opacity)
{
}
void JxAnimationCombo::SetGender(const JX_GENDER gender)
{
	m_Gender = gender;
	RefreshFrameInfo();
}
void JxAnimationCombo::SetDirection(const JX_DIRECTION direction)
{
	m_Direction = direction;
	RefreshFrameInfo();
}
void JxAnimationCombo::SetAction(const JX_ACTION_STATE action_state)
{
	m_ActionState = action_state;
	m_CurrentState = (JX_ANIMATION_STATE)(m_CurrentState & 
		JX_VOID_ACTION_FLAG | action_state<<JX_ACTION_BIT_OFFSET);
	RefreshFrameInfo();
}
void JxAnimationCombo::SetWeapon(const JX_WEAPON_STATE weapon_state)
{
	m_WeaponState = weapon_state;
	m_CurrentState = (JX_ANIMATION_STATE)(m_CurrentState & JX_VOID_WEAPON_FLAG | 
		weapon_state<<JX_WEAPON_BIT_OFFSET);

	// TODO: not delete these object so fast, but cache them to avoid load/reload
	if(weapon_state != JX_WEAPON_STATE_LIGHT && m_WeaponLight)
	{
		delete m_WeaponLight;
		m_WeaponLight = 0;
	}
	if(weapon_state != JX_WEAPON_STATE_HEAVY && m_WeaponHeavy)
	{
		delete m_WeaponHeavy;
		m_WeaponHeavy = 0;
	}
	if(weapon_state != JX_WEAPON_STATE_DUAL && m_WeaponDualL)
	{
		if(m_WeaponDualL)
		{
			delete m_WeaponDualL;
			m_WeaponDualL = 0;
		}
		if(m_WeaponDualR)
		{
			delete m_WeaponDualR;
			m_WeaponDualR = 0;
		}
	}
	if(weapon_state != JX_WEAPON_STATE_DUAL && m_WeaponDualR)
	{
		delete m_WeaponDualR;
		m_WeaponDualR = 0;
	}
	RefreshFrameInfo();
}
void JxAnimationCombo::SetHorse(const JX_HORSE_STATE horse_state)
{
	m_HorseState = horse_state;
	if(m_HorseState == JX_HORSE_STATE_ENABLE)
	{
		m_CurrentState = (JX_ANIMATION_STATE)(m_CurrentState & 
			JX_VOID_HORSE_FLAG & JX_VOID_WEAPON_FLAG | 
			horse_state<<JX_HORSE_BIT_OFFSET);
	}
	else
	{
		m_CurrentState = (JX_ANIMATION_STATE)(m_CurrentState & 
			JX_VOID_HORSE_FLAG & JX_VOID_WEAPON_FLAG | 
			m_WeaponState<<JX_WEAPON_BIT_OFFSET | horse_state<<JX_HORSE_BIT_OFFSET);
	}

	// TODO: not delete these object so fast, but cache them to avoid load/reload
	if(horse_state != JX_HORSE_STATE_ENABLE)
	{
		if(m_HorseHead)
		{
			delete m_HorseHead;
			m_HorseHead = 0;
		}
		if(m_HorseBack)
		{
			delete m_HorseBack;
			m_HorseBack = 0;
		}
		if(m_HorseTail)
		{
			delete m_HorseTail;
			m_HorseTail = 0;
		}
	}
	RefreshFrameInfo();
}
void JxAnimationCombo::RefreshFrameInfo()
{
	m_FrameTimer = 0.0;
	m_FrameTime = 1.0/10.0;// 30FPS
	int len;
	if(m_Gender == JX_GENDER_MALE)
	{
		len = JX_ANIMATION_MALE_FRAME[m_CurrentState];
	}
	else
	{
		len = JX_ANIMATION_FEMALE_FRAME[m_CurrentState];
	}
	m_FrameMin = len*(int)m_Direction;
	m_FrameMax = m_FrameMin + len;
	m_CurrentFrame = m_FrameMin;
}
void JxAnimationCombo::SetHelmAnimation(string name)
{
	if(m_Helm)
	{
		m_Helm->DecreaseReference();
	}
	m_Helm = JXANIMATIONCACHE->GetResource(m_Gender?JX_ANIMATION_PATH_CHARACTER_HELM_M"/"+name+"/"+name+".xml":
		JX_ANIMATION_PATH_CHARACTER_HELM_F"/"+name+"/"+name+".xml");
	m_Helm->IncreaseReference();
}
void JxAnimationCombo::SetClothAnimation(string name)
{
	if(m_Cloth)
	{
		m_Cloth->DecreaseReference();
	}
	m_Cloth = JXANIMATIONCACHE->GetResource(m_Gender?JX_ANIMATION_PATH_CHARACTER_CLOTH_M"/"+name+"/"+name+".xml":
		JX_ANIMATION_PATH_CHARACTER_CLOTH_F"/"+name+"/"+name+".xml");
	m_Cloth->IncreaseReference();
}
void JxAnimationCombo::SetArmLAnimation(string name)
{
	if(m_ArmL)
	{
		m_ArmL->DecreaseReference();
	}
	m_ArmL = JXANIMATIONCACHE->GetResource(m_Gender?JX_ANIMATION_PATH_CHARACTER_ARML_M"/"+name+"/"+name+".xml":
		JX_ANIMATION_PATH_CHARACTER_ARML_F"/"+name+"/"+name+".xml");
	m_ArmL->IncreaseReference();
}
void JxAnimationCombo::SetArmRAnimation(string name)
{
	if(m_ArmR)
	{
		m_ArmR->DecreaseReference();
	}
	m_ArmR = JXANIMATIONCACHE->GetResource(m_Gender?JX_ANIMATION_PATH_CHARACTER_ARMR_M"/"+name+"/"+name+".xml":
		JX_ANIMATION_PATH_CHARACTER_ARMR_F"/"+name+"/"+name+".xml");
	m_ArmR->IncreaseReference();
}
void JxAnimationCombo::SetWeaponLightAnimation(string name)
{
	if(m_WeaponLight)
	{
		m_WeaponLight->DecreaseReference();
	}
	m_WeaponLight = JXANIMATIONCACHE->GetResource(m_Gender?JX_ANIMATION_PATH_WEAPON_LIGHT_M"/"+name+"/"+name+".xml":
		JX_ANIMATION_PATH_WEAPON_LIGHT_F"/"+name+"/"+name+".xml");
	m_WeaponLight->IncreaseReference();
}
void JxAnimationCombo::SetWeaponHeavyAnimation(string name)
{
	if(m_WeaponHeavy)
	{
		m_WeaponHeavy->DecreaseReference();
	}
	m_WeaponHeavy = JXANIMATIONCACHE->GetResource(m_Gender?JX_ANIMATION_PATH_WEAPON_HEAVY_M"/"+name+"/"+name+".xml":
		JX_ANIMATION_PATH_WEAPON_HEAVY_F"/"+name+"/"+name+".xml");
	m_WeaponHeavy->IncreaseReference();
}
void JxAnimationCombo::SetWeaponDualLAnimation(string name)
{
	if(m_WeaponDualL)
	{
		m_WeaponDualL->DecreaseReference();
	}
	m_WeaponDualL = JXANIMATIONCACHE->GetResource(m_Gender?JX_ANIMATION_PATH_WEAPON_DUALL_M"/"+name+"/"+name+".xml":
		JX_ANIMATION_PATH_WEAPON_DUALL_F"/"+name+"/"+name+".xml");
	m_WeaponDualL->IncreaseReference();
}
void JxAnimationCombo::SetWeaponDualRAnimation(string name)
{
	if(m_WeaponDualR)
	{
		m_WeaponDualR->DecreaseReference();
	}
	m_WeaponDualR = JXANIMATIONCACHE->GetResource(m_Gender?JX_ANIMATION_PATH_WEAPON_DUALR_M"/"+name+"/"+name+".xml":
		JX_ANIMATION_PATH_WEAPON_DUALR_F"/"+name+"/"+name+".xml");
	m_WeaponDualR->IncreaseReference();
}
void JxAnimationCombo::SetHorseHeadAnimation(string name)
{
	if(m_HorseHead)
	{
		m_HorseHead->DecreaseReference();
	}
	m_HorseHead = JXANIMATIONCACHE->GetResource(JX_ANIMATION_PATH_RIDE_HEAD"/"+name+"/"+name+".xml");
	m_HorseHead->IncreaseReference();
}
void JxAnimationCombo::SetHorseBackAnimation(const string name)
{
	if(m_HorseBack)
	{
		m_HorseBack->DecreaseReference();
	}
	m_HorseBack = JXANIMATIONCACHE->GetResource(JX_ANIMATION_PATH_RIDE_BACK"/"+name+"/"+name+".xml");
	m_HorseBack->IncreaseReference();
}
void JxAnimationCombo::SetHorseTailAnimation(const string name)
{
	if(m_HorseTail)
	{
		m_HorseTail->DecreaseReference();
	}
	m_HorseTail = JXANIMATIONCACHE->GetResource(JX_ANIMATION_PATH_RIDE_TAIL"/"+name+"/"+name+".xml");
	m_HorseTail->IncreaseReference();
}
void JxAnimationCombo::LoadShader(string shader_path)
{
	if(m_Shader)
	{
		m_Shader->DecreaseReference();
	}
	if(ofxSHADERPROGRAMCACHE->ResourceLoaded(DEFAULT_JXANIMATION_SHADER))
	{
		m_Shader = ofxSHADERPROGRAMCACHE->GetResource(shader_path);
	}
	else
	{
		m_Shader = new JxShaderProgram();
		m_Shader->Load(shader_path);
		ofxSHADERPROGRAMCACHE->PushResource(m_Shader, shader_path);
	}
	m_Shader->IncreaseReference();
}
void JxAnimationCombo::Update(const float delta_time)
{
	m_FrameTimer += delta_time;
	if(m_FrameTimer >= m_FrameTime)
	{
		m_CurrentFrame++;
		if(m_CurrentFrame >= m_FrameMax)
		{
			m_CurrentFrame = m_FrameMin;
		}
		m_FrameTimer -= m_FrameTime;
	}
}
void JxAnimationCombo::Render()
{
	for (int i = 0; i < 9; i++)
	{
		if(!m_RenderList[i]) continue;
		ofxTexture* texture = m_RenderList[i]->QueryTexture(m_CurrentState, m_CurrentFrame);
		texture->Bind((GLuint)m_RenderList[i]->GetCategory());
	}

	glBindBuffer(GL_ARRAY_BUFFER, m_VBOId);
	glBufferData(GL_ARRAY_BUFFER, sizeof(ofxVertex)*ofxBaseCommand::m_VerticesSize, &m_JxVertices[0], GL_DYNAMIC_DRAW);
	JxShaderProgram* shader = (JxShaderProgram*)m_Shader;
	shader->Bind();
	int vertex_index = 0;
	for (int i = 0; i < 9; i++)
	{
		if(!m_RenderList[i]) continue;
		// "array overflow" proof
		/*if(vertex_index >= ofxBaseCommand::m_VerticesSize - 4)
		{
			break;
		}*/
		shader->SetTextureSlot((GLuint)m_RenderList[i]->GetCategory());
		glDrawArrays(GL_QUADS, vertex_index, 4);
		vertex_index += 4;
	}

	glBindBuffer(GL_ARRAY_BUFFER, 0);
	//shader->Unbind();
}
#define SKEW45
#if defined(SKEW30)
#define QUAD_GRADIENT_Z 0.4472135954999579// sqrt(0.2)
#define QUAD_GRADIENT_Y 0.8944271909999159// sqrt(0.8)
#elif defined(SKEW45)
#define QUAD_GRADIENT_Z 0.7071067811865475// sqrt(0.5)
#define QUAD_GRADIENT_Y 0.7071067811865475// sqrt(0.5)
#endif
void JxAnimationCombo::SubmitChanges()
{
	{
		m_RenderList[JX_ANIMATION_COMBO_RENDER_ORDER[(int)m_Direction][0]] = m_Helm;
		m_RenderList[JX_ANIMATION_COMBO_RENDER_ORDER[(int)m_Direction][1]] = m_Cloth;
		m_RenderList[JX_ANIMATION_COMBO_RENDER_ORDER[(int)m_Direction][2]] = m_ArmL;
		m_RenderList[JX_ANIMATION_COMBO_RENDER_ORDER[(int)m_Direction][3]] = m_ArmR;
		if(m_WeaponState == JX_WEAPON_STATE_DUAL)
		{
			m_RenderList[JX_ANIMATION_COMBO_RENDER_ORDER[(int)m_Direction][4]] = m_WeaponDualL;
			m_RenderList[JX_ANIMATION_COMBO_RENDER_ORDER[(int)m_Direction][5]] = m_WeaponDualR;
		}
		else if(m_WeaponState == JX_WEAPON_STATE_HEAVY)
		{
			m_RenderList[JX_ANIMATION_COMBO_RENDER_ORDER[(int)m_Direction][4]] = m_WeaponHeavy;
			m_RenderList[JX_ANIMATION_COMBO_RENDER_ORDER[(int)m_Direction][5]] = 0;
		}
		else
		{
			m_RenderList[JX_ANIMATION_COMBO_RENDER_ORDER[(int)m_Direction][4]] = m_WeaponLight;
			m_RenderList[JX_ANIMATION_COMBO_RENDER_ORDER[(int)m_Direction][5]] = 0;
		}
		m_RenderList[JX_ANIMATION_COMBO_RENDER_ORDER[(int)m_Direction][6]] = m_HorseHead;
		m_RenderList[JX_ANIMATION_COMBO_RENDER_ORDER[(int)m_Direction][7]] = m_HorseBack;
		m_RenderList[JX_ANIMATION_COMBO_RENDER_ORDER[(int)m_Direction][8]] = m_HorseTail;
	}
	m_JxVertices.clear();
	for(int i=0;i<9;i++)
	{
		if(!m_RenderList[i]) continue;
		JxFrameInfo info = m_RenderList[i]->QueryFrame(m_CurrentState, m_CurrentFrame);
		JxVertex a, b, c, d;
		
		a.x = m_Position.x + info.x_min;
		a.y = m_Position.y + QUAD_GRADIENT_Y*info.y_min;
		a.z = m_Position.z - QUAD_GRADIENT_Z*info.y_min;
		b.x = m_Position.x + info.x_max;
		b.y = a.y;
		b.z = a.z;
		c.x = b.x;
		c.y = m_Position.y + QUAD_GRADIENT_Y*info.y_max;
		c.z = m_Position.z - QUAD_GRADIENT_Z*info.y_max;
		d.x = a.x;
		d.y = c.y;
		d.z = c.z;

		if(info.rotated)
		{
			a.u = info.u_min;
			a.v = info.v_max;

			b.u = info.u_min;
			b.v = info.v_min;

			c.u = info.u_max;
			c.v = info.v_min;

			d.u = info.u_max;
			d.v = info.v_max;
		}
		else
		{
			a.u = info.u_min;
			a.v = info.v_min;

			b.u = info.u_max;
			b.v = info.v_min;

			c.u = info.u_max;
			c.v = info.v_max;

			d.u = info.u_min;
			d.v = info.v_max;
		}
		m_JxVertices.push_back(a);
		m_JxVertices.push_back(b);
		m_JxVertices.push_back(c);
		m_JxVertices.push_back(d);
	}
	ofxBaseCommand::m_VerticesSize = m_JxVertices.size();
}