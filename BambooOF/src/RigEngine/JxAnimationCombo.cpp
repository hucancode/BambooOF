#include "JxAnimationCombo.h"

JxAnimationCombo::JxAnimationCombo()
{
	m_Helm = new JxAnimation();
	m_Helm->SetTextureSlot(0);
	m_Cloth = new JxAnimation();
	m_Cloth->SetTextureSlot(1);
	m_HandL = new JxAnimation();
	m_HandL->SetTextureSlot(2);
	m_HandR = new JxAnimation();
	m_HandR->SetTextureSlot(3);
	m_WeaponLight = 0;//4
	m_WeaponHeavy = 0;//4
	m_WeaponDualL = 0;//4
	m_WeaponDualR = 0;//5
	m_HorseHead = 0;//6
	m_HorseBack = 0;//7
	m_HorseTail = 0;//8
}
JxAnimationCombo::~JxAnimationCombo()
{
}
void JxAnimationCombo::SetColor(ofFloatColor color)
{
}
void JxAnimationCombo::SetColorIntensity(float color_intensity)
{
}
void JxAnimationCombo::SetOpacity(float opacity)
{
}
void JxAnimationCombo::SetHelm(string name)
{
}
void JxAnimationCombo::SetCloth(string name)
{
}
void JxAnimationCombo::SetHandL(string name)
{
}
void JxAnimationCombo::SetHandR(string name)
{
}
void JxAnimationCombo::SetWeaponFree()
{
}
void JxAnimationCombo::SetWeaponLight(string name)
{
}
void JxAnimationCombo::SetWeaponHeavy(string name)
{
}
void JxAnimationCombo::SetWeaponDualL(string name)
{
}
void JxAnimationCombo::SetWeaponDualR(string name)
{
}
void JxAnimationCombo::SetHorseHead(string name)
{
}
void JxAnimationCombo::SetHorseBack(string name)
{
}
void JxAnimationCombo::SetHorseTail(string name)
{
}
void JxAnimationCombo::SetState(const JX_ANIMATION_STATE state)
{
	m_CurrentState = state;
	m_Helm->SetState(state);
	m_Cloth->SetState(state);
	m_HandL->SetState(state);
	m_HandR->SetState(state);
	if(m_WeaponLight)
	{
		m_WeaponLight->SetState(state);
	}
	if(m_WeaponHeavy)
	{
		m_WeaponHeavy->SetState(state);
	}
	if(m_WeaponDualL)
	{
		m_WeaponDualL->SetState(state);
	}
	if(m_WeaponDualL)
	{
		m_WeaponDualR->SetState(state);
	}
	if(m_HorseHead)
	{
		m_HorseHead->SetState(state);
	}
	if(m_HorseBack)
	{
		m_HorseBack->SetState(state);
	}
	if(m_HorseTail)
	{
		m_HorseTail->SetState(state);
	}
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
		texture->Bind(0);
	}
	{
		ofxTexture* texture = m_Cloth->QueryTexture();
		texture->Bind(1);
	}
	{
		ofxTexture* texture = m_HandL->QueryTexture();
		texture->Bind(2);
	}
	{
		ofxTexture* texture = m_HandR->QueryTexture();
		texture->Bind(3);
	}
	if(m_WeaponLight)
	{
		ofxTexture* texture = m_WeaponLight->QueryTexture();
		m_WeaponLight->SetTextureSlot(4);
		texture->Bind(4);
	}
	if(m_WeaponHeavy)
	{
		ofxTexture* texture = m_WeaponHeavy->QueryTexture();
		m_WeaponHeavy->SetTextureSlot(4);
		texture->Bind(4);
	}
	if(m_WeaponDualL)
	{
		ofxTexture* texture = m_WeaponDualL->QueryTexture();
		m_WeaponDualL->SetTextureSlot(4);
		texture->Bind(4);
	}
	if(m_WeaponDualR)
	{
		ofxTexture* texture = m_WeaponDualR->QueryTexture();
		m_WeaponDualR->SetTextureSlot(5);
		texture->Bind(5);
	}
	if(m_HorseHead)
	{
		ofxTexture* texture = m_HorseHead->QueryTexture();
		m_HorseHead->SetTextureSlot(6);
		texture->Bind(6);
	}
	if(m_HorseBack)
	{
		ofxTexture* texture = m_HorseBack->QueryTexture();
		m_HorseBack->SetTextureSlot(7);
		texture->Bind(7);
	}
	if(m_HorseTail)
	{
		ofxTexture* texture = m_HorseTail->QueryTexture();
		m_HorseTail->SetTextureSlot(8);
		texture->Bind(8);
	}
	// render using m_JxVertices
}
void JxAnimationCombo::SubmitChanges()
{
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
		m_JxVertices.push_back(d);
	}
}