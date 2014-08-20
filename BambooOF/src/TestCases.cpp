#include "TestCases.h"
#include "ofxTextureCache.h"
#include "ofxShaderCache.h"
#include "ofxShaderProgramCache.h"
Test* current_test = new RendererTest();
ofxSpriteQuad* spriteObstacle;
void Test::Setup()
{
}
void Test::Update()
{
}
void Test::Render()
{
}
void SpriteTest::Setup()
{
	ofxSpriteQuad* sprite = new ofxSpriteQuad();
	sprite->LoadShader("sprite2d");
	sprite->SetTexture("data/animal_tiger1_attack.png");
	
	sprite->SetSpriteRect(0,0,192,192);
	sprite->SetTextureRect(0,0,192,192);
	sprite->MoveTo(0.0f,0.0f,0.1f);
	ofxRENDERER->PushSprite(sprite);
	spriteObstacle = sprite;
}
void SpriteTest::Update()
{
	ofxRENDERER->Update();
}
void SpriteTest::Render()
{
	ofxRENDERER->Render();
}
void RendererTest::Setup()
{
	for(int i=-20;i<20;i++)
	{
		for(int j=-20;j<20;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->LoadShader("sprite2d");
			sprite->SetTexture("data/sprint0001.png");
			
			sprite->SetSpriteRect(0,0,192,192);
			sprite->SetTextureRect(0,0,192,192);
			sprite->MoveTo(i*80.0f,0.0f,j*60.0f);
			ofxRENDERER->PushSprite(sprite);
			if(i==0 && j==0) spriteObstacle = sprite;
		}
	}
}
void RendererTest::Update()
{
	ofxRENDERER->Update();
}
void RendererTest::Render()
{
	ofxRENDERER->Render();
}

void SortingTest::Setup()
{
	float space_x = 192.0f;
	float space_y = 192.0f;
	for(int i=-40;i<40;i++)
	{
		for(int j=-40;j<40;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->LoadShader("sprite2d");
			sprite->SetTexture("data/sprint0001.png");
			
			sprite->SetSpriteRect(0,0,192,192);
			sprite->SetTextureRect(0,0,192,192);
			sprite->MoveTo(i*space_x,i*0.01f,j*space_y);
			ofxRENDERER->PushSprite(sprite);
			if(i==0 && j==0) spriteA = sprite;
			if(i==1 && j==1) spriteB = sprite;
		}
	}
	spriteObstacle = spriteA;
	
}
void SortingTest::Update()
{
	ofxRENDERER->Update();
	if(spriteA->GetWorldPosition().z < 0.2f)
	{
		spriteA->MoveBy(0.0f,0.0f,0.0001f);
	}
	else
	{
		spriteA->MoveBy(0.0f,0.0f,-0.4f);
	}
	if(spriteB->GetWorldPosition().z > -0.2f)
	{
		spriteB->MoveBy(0.0f,0.0f,-0.0001f);
	}
	else
	{
		spriteB->MoveBy(0.0f,0.0f,0.4f);
	}
}
void SortingTest::Render()
{
	ofxRENDERER->Render();
}
void TextureTest::Setup()
{
	float space_x = 192.0f;
	float space_y = 192.0f;
	for(int i=-20;i<20;i++)
	{
		for(int j=-20;j<20;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->LoadShader("sprite2d");
			sprite->SetTexture(("data/plops/sprint000"+ofToString((j+20)/5+1)+".png").c_str());
			
			sprite->SetSpriteRect(0,0,192,192);
			sprite->SetTextureRect(0,0,192,192);
			sprite->MoveTo(i*space_x,0.0f,j*space_y);
			ofxRENDERER->PushSprite(sprite);
			if(i==0 && j==0) spriteObstacle = sprite;
		}
	}
}
void TextureTest::Update()
{
	ofxRENDERER->Update();
}
void TextureTest::Render()
{
	ofxRENDERER->Render();
}

void AnimationTest::Setup()
{
	ofxSpriteAnimation* animation = new ofxSpriteAnimation();
	animation->LoadShader("sprite2d");
	animation->SetTexture("data/animal_tiger1_attack.png");
	animation->MoveTo(0.0f,0.0f,0.0f);
	animation->SetFrameCount(24);

	animation->SetSequenceCount(2);
	animation->SetFrameTime(0,0.1f);
	animation->SetFrameTime(1,0.1f);
	{
		int i=0;
		animation->SetFrameData(i++,1,1,27,72,147,134,27,72);
		animation->SetFrameData(i++,1,76,27,72,147,134,27,72);
		animation->SetFrameData(i++,31,1,28,73,146,133,28,73);
		animation->SetFrameData(i++,62,1,28,72,146,134,28,72);
		animation->SetFrameData(i++,31,77,28,75,147,136,28,75);
		animation->SetFrameData(i++,62,76,27,71,147,133,27,71);
		animation->SetFrameData(i++,93,1,27,67,147,126,27,67);
		animation->SetFrameData(i++,123,1,28,78,146,131,28,78);
		animation->SetFrameData(i++,93,71,27,78,147,135,27,78);
		animation->SetFrameData(i++,1,151,27,75,147,140,27,75);
		animation->SetFrameData(i++,31,155,28,68,146,146,28,68);
		animation->SetFrameData(i++,62,150,28,72,146,140,28,72);
		animation->SetSequenceData(0,0,11);
		animation->SetFrameData(i++,154,1,93,67,113,139,93,67);
		animation->SetFrameData(i++,123,82,94,62,114,140,94,62);
		animation->SetFrameData(i++,93,152,93,62,118,140,93,62);
		animation->SetFrameData(i++,250,1,94,62,120,140,94,62);
		animation->SetFrameData(i++,220,82,97,60,119,143,97,60);
		animation->SetFrameData(i++,189,152,118,55,99,144,118,55);
		animation->SetFrameData(i++,347,1,117,55,85,134,117,55);
		animation->SetFrameData(i++,320,82,115,56,84,138,115,56);
		animation->SetFrameData(i++,310,152,112,55,88,143,112,55);
		animation->SetFrameData(i++,467,1,109,52,94,149,109,52);
		animation->SetFrameData(i++,438,82,106,54,99,149,106,54);
		animation->SetFrameData(i++,425,152,101,62,105,144,101,62);
		animation->SetSequenceData(1,12,23);
	}
	animation->SetSequence(0);
	ofxRENDERER->PushSprite(animation);
	spriteObstacle = animation;
}
void AnimationTest::Update()
{
	ofxRENDERER->Update();
}
void AnimationTest::Render()
{
	ofxRENDERER->Render();
}

void SpriteBenchmarkTest::Setup()
{
	float space_x = 192.0f;
	float space_y = 192.0f;
	for(int i=-40;i<40;i++)
	{
		for(int j=-50;j<50;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->LoadShader("sprite2d");
			sprite->SetTexture("data/plops/sprint0001.png");
			
			sprite->SetSpriteRect(0,0,192,192);
			sprite->SetTextureRect(0,0,192,192);
			sprite->MoveTo(i*space_x,0.0f,j*space_y);
			ofxRENDERER->PushSprite(sprite);
			if(i==0 && j==0) spriteObstacle = sprite;
		}
	}
}
void SpriteBenchmarkTest::Update()
{
	ofxRENDERER->Update();
}
void SpriteBenchmarkTest::Render()
{
	ofxRENDERER->Render();
}
void SortBenchmarkTest::Setup()
{
	float space_x = 192.0f;
	float space_y = 192.0f;
	for(int i=-60;i<60;i++)
	{
		for(int j=-60;j<60;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->LoadShader("sprite2d");
			sprite->SetTexture(("data/plops/sprint000"+ofToString((j+20)/5+1)+".png").c_str());
			
			sprite->SetSpriteRect(0,0,192,192);
			sprite->SetTextureRect(0,0,192,192);
			sprite->MoveTo(i*space_x,0.0f,j*space_y);
			ofxRENDERER->PushSprite(sprite);
			if(i==0 && j==0) spriteObstacle = sprite;
			if(i==0 && j==0) spriteA = sprite;
			if(i==1 && j==1) spriteB = sprite;
		}
	}
}
void SortBenchmarkTest::Update()
{
	ofxRENDERER->Update();
	if(spriteA->GetWorldPosition().z < 1000.0f)
	{
		spriteA->MoveBy(0.0f,0.0f,15.0f);
	}
	else
	{
		spriteA->MoveBy(0.0f,0.0f,-2000.0f);
	}
	spriteB->MoveTo(ofxRENDERER->GetCamera()->getPosition());
	int a = 10;
}
void SortBenchmarkTest::Render()
{
	ofxRENDERER->Render();
	printf("update time = %u\nrender time = %u\n",
		ofxRENDERER->GetUpdateTimeMilisecond(),
		ofxRENDERER->GetRenderTimeMilisecond());
}
void AnimationBenchmarkTest::Setup()
{
	float space_x = 192.0f;
	float space_y = 192.0f;
	for(int i=-60;i<60;i++)
	{
		for(int j=-60;j<60;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->LoadShader("sprite2d");
			sprite->SetTexture(("data/plops/sprint000"+ofToString((j+20)/5+1)+".png").c_str());
			
			sprite->SetSpriteRect(0,0,192,192);
			sprite->SetTextureRect(0,0,192,192);
			sprite->MoveTo(i*space_x,0.0f,j*space_y);
			ofxRENDERER->PushSprite(sprite);
			if(i==0 && j==0) spriteObstacle = sprite;
		}
	}
}
void AnimationBenchmarkTest::Update()
{
	ofxRENDERER->Update();
}
void AnimationBenchmarkTest::Render()
{
	ofxRENDERER->Render();
	printf("update time = %u\nrender time = %u\n",
		ofxRENDERER->GetUpdateTimeMilisecond(),
		ofxRENDERER->GetRenderTimeMilisecond());
}