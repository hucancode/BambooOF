#include "JxEngineTest.h"
#include "JxAnimationEngine/JxAnimationCombo.h"
void JxAnimationTest::Setup()
{
	JxAnimationCombo* sprite = new JxAnimationCombo();
	sprite->SetWeaponDualLAnimation("blade2");
	sprite->SetWeaponDualRAnimation("blade2");
	sprite->SetWeapon(JX_WEAPON_STATE_DUAL);
	sprite->SetHorseTailAnimation("horse1");
	sprite->SetHorseBackAnimation("horse1");
	sprite->SetHorseHeadAnimation("horse1");
	sprite->SetHorse(JX_HORSE_STATE_ENABLE);
	sprite->SetAction(JX_ACTION_STATE_ATK);
}
void JxAnimationTest::Update()
{
	ofxRENDERER->Update();
}
void JxAnimationTest::Render()
{
	ofxRENDERER->Render();
}