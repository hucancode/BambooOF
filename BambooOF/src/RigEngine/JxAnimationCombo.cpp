#include "JxAnimationCombo.h"

JxAnimationCombo::JxAnimationCombo()
{
	m_CurrentState = JX_ANIMATION_STATE_ZEN;
	m_Helm = new JxAnimation();
	m_Helm->SetTextureSlot(JX_ANIMATION_HELM_TEXTURE_SLOT);
	m_Cloth = new JxAnimation();
	m_Cloth->SetTextureSlot(JX_ANIMATION_CLOTH_TEXTURE_SLOT);
	m_HandL = new JxAnimation();
	m_HandL->SetTextureSlot(JX_ANIMATION_HANDL_TEXTURE_SLOT);
	m_HandR = new JxAnimation();
	m_HandR->SetTextureSlot(JX_ANIMATION_HANDR_TEXTURE_SLOT);
	m_WeaponLight = 0;
	m_WeaponHeavy = 0;
	m_WeaponDualL = 0;
	m_WeaponDualR = 0;
	m_HorseHead = 0;
	m_HorseBack = 0;
	m_HorseTail = 0;
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
}
void JxAnimationCombo::SetAction(const JX_ACTION_STATE action_state)
{
	m_CurrentState = (JX_ANIMATION_STATE)(m_CurrentState & JX_VOID_ACTION_FLAG | action_state<<JX_ACTION_BIT_OFFSET);
}
void JxAnimationCombo::SetWeapon(const JX_WEAPON_STATE weapon_state)
{
	m_CurrentState = (JX_ANIMATION_STATE)(m_CurrentState & JX_VOID_WEAPON_FLAG | weapon_state<<JX_WEAPON_BIT_OFFSET);

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
}
void JxAnimationCombo::SetHorse(const JX_HORSE_STATE horse_state)
{
	m_CurrentState = (JX_ANIMATION_STATE)(m_CurrentState & JX_VOID_HORSE_FLAG | horse_state<<JX_HORSE_BIT_OFFSET);

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
}
void JxAnimationCombo::SetHelmAnimation(string name)
{
	LoadAnimation(m_Helm, m_Gender?JX_ANIMATION_PATH_CHARACTER_HEAD_M:JX_ANIMATION_PATH_CHARACTER_HEAD_F, name);
}
void JxAnimationCombo::SetClothAnimation(string name)
{
	LoadAnimation(m_Cloth, m_Gender?JX_ANIMATION_PATH_CHARACTER_CLOTH_M:JX_ANIMATION_PATH_CHARACTER_CLOTH_F, name);
}
void JxAnimationCombo::SetHandLAnimation(string name)
{
	LoadAnimation(m_HandL, m_Gender?JX_ANIMATION_PATH_CHARACTER_HANDL_M:JX_ANIMATION_PATH_CHARACTER_HANDL_F, name);
}
void JxAnimationCombo::SetHandRAnimation(string name)
{
	LoadAnimation(m_HandR, m_Gender?JX_ANIMATION_PATH_CHARACTER_HANDR_M:JX_ANIMATION_PATH_CHARACTER_HANDR_F, name);
}
void JxAnimationCombo::SetWeaponLightAnimation(string name)
{
	if(!m_WeaponLight)
	{
		m_WeaponLight = new JxAnimation();
		m_WeaponLight->SetTextureSlot(JX_ANIMATION_WEAPON_PRIMARY_TEXTURE_SLOT);
	}
	LoadAnimation(m_WeaponLight, m_Gender?JX_ANIMATION_PATH_WEAPON_LIGHT_M:JX_ANIMATION_PATH_WEAPON_LIGHT_F, name);
}
void JxAnimationCombo::SetWeaponHeavyAnimation(string name)
{
	if(!m_WeaponHeavy)
	{
		m_WeaponHeavy = new JxAnimation();
		m_WeaponHeavy->SetTextureSlot(JX_ANIMATION_WEAPON_PRIMARY_TEXTURE_SLOT);
	}
	LoadAnimation(m_WeaponHeavy, m_Gender?JX_ANIMATION_PATH_WEAPON_HEAVY_M:JX_ANIMATION_PATH_WEAPON_HEAVY_F, name);
}
void JxAnimationCombo::SetWeaponDualLAnimation(string name)
{
	if(!m_WeaponDualL)
	{
		m_WeaponDualL = new JxAnimation();
		m_WeaponDualL->SetTextureSlot(JX_ANIMATION_WEAPON_PRIMARY_TEXTURE_SLOT);
	}
	LoadAnimation(m_WeaponDualL, m_Gender?JX_ANIMATION_PATH_WEAPON_DUALL_M:JX_ANIMATION_PATH_WEAPON_DUALL_F, name);
}
void JxAnimationCombo::SetWeaponDualRAnimation(string name)
{
	if(!m_WeaponDualR)
	{
		m_WeaponDualR = new JxAnimation();
		m_WeaponDualR->SetTextureSlot(JX_ANIMATION_WEAPON_SECONDARY_TEXTURE_SLOT);
	}
	LoadAnimation(m_WeaponDualR, m_Gender?JX_ANIMATION_PATH_WEAPON_DUALR_M:JX_ANIMATION_PATH_WEAPON_DUALR_F, name);
}
void JxAnimationCombo::SetHorseHeadAnimation(string name)
{
	if(!m_HorseHead)
	{
		m_HorseHead = new JxAnimation();
		m_HorseHead->SetTextureSlot(JX_ANIMATION_HORSE_HEAD_TEXTURE_SLOT);
	}
	LoadAnimation(m_HorseHead, JX_ANIMATION_PATH_RIDE_HEAD, name);
}
void JxAnimationCombo::SetHorseBackAnimation(const string name)
{
	if(!m_HorseBack)
	{
		m_HorseBack = new JxAnimation();
		m_HorseBack->SetTextureSlot(JX_ANIMATION_HORSE_BACK_TEXTURE_SLOT);
	}
	LoadAnimation(m_HorseBack, JX_ANIMATION_PATH_RIDE_BACK, name);
}
void JxAnimationCombo::SetHorseTailAnimation(const string name)
{
	if(!m_HorseTail)
	{
		m_HorseTail = new JxAnimation();
		m_HorseTail->SetTextureSlot(JX_ANIMATION_HORSE_TAIL_TEXTURE_SLOT);
	}
	LoadAnimation(m_HorseTail, JX_ANIMATION_PATH_RIDE_TAIL, name);
}
void JxAnimationCombo::LoadAnimation(JxAnimation* animation, string path, const string name)
{
	path += "/";
	path += name;
	path += "/";
	path += name;
	path += ".xml";
	animation->Load(path);
}
void JxAnimationCombo::Update(const float delta_time)
{
	m_FrameTimer += delta_time;
	if(m_FrameTimer >= m_FrameTime)
	{
		m_CurrentFrame++;
		if(m_CurrentFrame > m_FrameMax)
		{
			m_CurrentFrame = m_FrameMin;
		}
		m_FrameTimer -= m_FrameTime;
		m_Helm->SetFrame(m_CurrentFrame);
		m_Cloth->SetFrame(m_CurrentFrame);
		m_HandL->SetFrame(m_CurrentFrame);
		m_HandR->SetFrame(m_CurrentFrame);
		if(m_WeaponLight)
		{
			m_WeaponLight->SetFrame(m_CurrentFrame);
		}
		if(m_WeaponHeavy)
		{
			m_WeaponHeavy->SetFrame(m_CurrentFrame);
		}
		if(m_WeaponDualL)
		{
			m_WeaponDualL->SetFrame(m_CurrentFrame);
		}
		if(m_WeaponDualR)
		{
			m_WeaponDualR->SetFrame(m_CurrentFrame);
		}
		if(m_HorseHead)
		{
			m_HorseHead->SetFrame(m_CurrentFrame);
		}
		if(m_HorseBack)
		{
			m_HorseBack->SetFrame(m_CurrentFrame);
		}
		if(m_HorseTail)
		{
			m_HorseTail->SetFrame(m_CurrentFrame);
		}
		
	}
}
void JxAnimationCombo::Render()
{
	SubmitChanges();
	{
		ofxTexture* texture = m_Helm->QueryTexture();
		texture->Bind(JX_ANIMATION_HELM_TEXTURE_SLOT);
	}
	{
		ofxTexture* texture = m_Cloth->QueryTexture();
		texture->Bind(JX_ANIMATION_CLOTH_TEXTURE_SLOT);
	}
	{
		ofxTexture* texture = m_HandL->QueryTexture();
		texture->Bind(JX_ANIMATION_HANDL_TEXTURE_SLOT);
	}
	{
		ofxTexture* texture = m_HandR->QueryTexture();
		texture->Bind(JX_ANIMATION_HANDR_TEXTURE_SLOT);
	}
	if(m_WeaponLight)
	{
		ofxTexture* texture = m_WeaponLight->QueryTexture();
		texture->Bind(JX_ANIMATION_WEAPON_PRIMARY_TEXTURE_SLOT);
	}
	if(m_WeaponHeavy)
	{
		ofxTexture* texture = m_WeaponHeavy->QueryTexture();
		texture->Bind(JX_ANIMATION_WEAPON_PRIMARY_TEXTURE_SLOT);
	}
	if(m_WeaponDualL)
	{
		ofxTexture* texture = m_WeaponDualL->QueryTexture();
		texture->Bind(JX_ANIMATION_WEAPON_PRIMARY_TEXTURE_SLOT);
	}
	if(m_WeaponDualR)
	{
		ofxTexture* texture = m_WeaponDualR->QueryTexture();
		texture->Bind(JX_ANIMATION_WEAPON_SECONDARY_TEXTURE_SLOT);
	}
	if(m_HorseHead)
	{
		ofxTexture* texture = m_HorseHead->QueryTexture();
		texture->Bind(JX_ANIMATION_HORSE_HEAD_TEXTURE_SLOT);
	}
	if(m_HorseBack)
	{
		ofxTexture* texture = m_HorseBack->QueryTexture();
		texture->Bind(JX_ANIMATION_HORSE_BACK_TEXTURE_SLOT);
	}
	if(m_HorseTail)
	{
		ofxTexture* texture = m_HorseTail->QueryTexture();
		texture->Bind(JX_ANIMATION_HORSE_TAIL_TEXTURE_SLOT);
	}
	// render using m_JxVertices
}
void JxAnimationCombo::SubmitChanges()
{
	m_Helm->SetState(m_CurrentState);
	m_Cloth->SetState(m_CurrentState);
	m_HandL->SetState(m_CurrentState);
	m_HandR->SetState(m_CurrentState);
	if(m_WeaponLight)
	{
		m_WeaponLight->SetState(m_CurrentState & JX_VOID_WEAPON_FLAG);
	}
	if(m_WeaponHeavy)
	{
		m_WeaponHeavy->SetState(m_CurrentState & JX_VOID_WEAPON_FLAG);
	}
	if(m_WeaponDualL)
	{
		m_WeaponDualL->SetState(m_CurrentState & JX_VOID_WEAPON_FLAG);
	}
	if(m_WeaponDualL)
	{
		m_WeaponDualR->SetState(m_CurrentState & JX_VOID_WEAPON_FLAG);
	}
	if(m_HorseHead)
	{
		m_HorseHead->SetState(m_CurrentState & JX_VOID_WEAPON_FLAG & JX_VOID_HORSE_FLAG);
	}
	if(m_HorseBack)
	{
		m_HorseBack->SetState(m_CurrentState & JX_VOID_WEAPON_FLAG & JX_VOID_HORSE_FLAG);
	}
	if(m_HorseTail)
	{
		m_HorseTail->SetState(m_CurrentState & JX_VOID_WEAPON_FLAG & JX_VOID_HORSE_FLAG);
	}
	static JxAnimation* render_array[11];
	// build render order here
	// and here
	// here
	m_JxVertices.clear();
	for(int i=0;i<11;i++)
	{
		if(!render_array[i]) continue;
		JxFrameInfo info = render_array[i]->QueryFrame();
		unsigned char texture_slot = render_array[i]->GetTextureSlot();
		JxVertex a, b, c, d;
		a.x = info.x_min;
		a.y = info.y_min;
		a.z = m_Position.z;
		a.u = info.u_min;
		a.v = info.v_min;
		a.texture = texture_slot;
		b.x = info.x_max;
		b.y = info.y_min;
		b.z = m_Position.z;
		b.u = info.u_max;
		b.v = info.v_min;
		b.texture = texture_slot;
		c.x = info.x_max;
		c.y = info.y_max;
		c.z = m_Position.z;
		c.u = info.u_max;
		c.v = info.v_max;
		c.texture = texture_slot;
		d.x = info.x_min;
		d.y = info.y_max;
		d.z = m_Position.z;
		d.u = info.u_min;
		d.v = info.v_max;
		d.texture = texture_slot;
		m_JxVertices.push_back(a);
		m_JxVertices.push_back(b);
		m_JxVertices.push_back(c);
		m_JxVertices.push_back(d);
	}
}