#pragma once
#include "ofxSpriteRenderer.h"
class Test
{
public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
};
class SpriteTest: public Test
{
private:

public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
};
class RendererTest: public Test
{
private:

public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
};
class SortingTest: public Test
{
private:
	ofxSpriteQuad* spriteA;
	ofxSpriteQuad* spriteB;
public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
};
class SpriteBenchmarkTest: public Test
{
public:
	virtual void Setup();
	virtual void Update();
	virtual void Render();
};