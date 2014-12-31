#pragma once
#include "TestCases.h"
#include "JxAnimationEngine/JxAnimationCombo.h"
class JxAnimationTest: public Test
{
private:
	int direction;
	JxAnimationCombo* sprite;
public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
	virtual void KeyPressed(int key);
};
class JxSortingTest: public Test
{
private:
	int direction;
	JxAnimationCombo* sprite[10][10];
public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
	virtual void KeyPressed(int key);
};