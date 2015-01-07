#include "JxEngineTest.h"
#include "JxAnimationEngine/JxAnimationCache.h"
void JxAnimationTest::Setup()
{
	new JxAnimationCache;
	sprite = new JxAnimationCombo();
	sprite->MoveTo(-128,0,0);
	sprite->SetHelmAnimation("tianwang3");
	sprite->SetClothAnimation("tianwang3");
	sprite->SetArmLAnimation("tianwang3");
	sprite->SetArmRAnimation("tianwang3");
	sprite->SetWeaponDualLAnimation("blade2");
	sprite->SetWeaponDualRAnimation("blade2");
	sprite->SetWeapon(JX_WEAPON_STATE_DUAL);
	sprite->SetHorseTailAnimation("horse1");
	sprite->SetHorseBackAnimation("horse1");
	sprite->SetHorseHeadAnimation("horse1");
	sprite->SetHorse(JX_HORSE_STATE_ENABLE);
	sprite->SetAction(JX_ACTION_STATE_ATK);
	direction = 0;
}
void JxAnimationTest::Update()
{
	ofxRENDERER->Update();
}
void JxAnimationTest::Render()
{
	ofxRENDERER->Render();
}
void JxAnimationTest::KeyPressed(int key)
{
	if(key == 'w') 
	{
		direction++;
		if(direction == 8)
		{
			direction = 0;
		}
		sprite->SetDirection((JX_DIRECTION)direction);
	}
	if(key == 'q') 
	{
		direction--;
		if(direction == -1)
		{
			direction = 7;
		}
		sprite->SetDirection((JX_DIRECTION)direction);
	}
}
void JxSortingTest::Setup()
{
	new JxAnimationCache;
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			sprite[i][j] = new JxAnimationCombo();
			sprite[i][j]->MoveTo(i*256,0,j*256);
			sprite[i][j]->SetHelmAnimation("tianwang3");
			sprite[i][j]->SetClothAnimation("tianwang3");
			sprite[i][j]->SetArmLAnimation("tianwang3");
			sprite[i][j]->SetArmRAnimation("tianwang3");
			sprite[i][j]->SetWeaponDualLAnimation("blade2");
			sprite[i][j]->SetWeaponDualRAnimation("blade2");
			sprite[i][j]->SetWeapon(JX_WEAPON_STATE_DUAL);
			sprite[i][j]->SetHorseTailAnimation("horse1");
			sprite[i][j]->SetHorseBackAnimation("horse1");
			sprite[i][j]->SetHorseHeadAnimation("horse1");
			sprite[i][j]->SetHorse(JX_HORSE_STATE_ENABLE);
			sprite[i][j]->SetAction(JX_ACTION_STATE_ATK);
		}
	}
	
	direction = 0;
}
void JxSortingTest::Update()
{
	ofxRENDERER->Update();
}
void JxSortingTest::Render()
{
	ofxRENDERER->Render();
}
void JxSortingTest::KeyPressed(int key)
{
	if(key == 'w') 
	{
		direction++;
		if(direction == 8)
		{
			direction = 0;
		}
		for(int i=0;i<10;i++)
		{
			for(int j=0;j<10;j++)
			{
				sprite[i][j]->SetDirection((JX_DIRECTION)direction);
			}
		}
	}
	if(key == 'q') 
	{
		direction--;
		if(direction == -1)
		{
			direction = 7;
		}
		for(int i=0;i<10;i++)
		{
			for(int j=0;j<10;j++)
			{
				sprite[i][j]->SetDirection((JX_DIRECTION)direction);
			}
		}
	}
}