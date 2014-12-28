#include "JxEngineTest.h"
#include "JxAnimationEngine/JxAnimationCombo.h"
void JxAnimationTest::Setup()
{
	JxAnimationCombo* sprite = new JxAnimationCombo();
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