#include "TestCases.h"

Test* current_test = new AnimationTest();
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
	ofxSpriteMaterial* material = new ofxMonoMaterial();
	material->LoadShader("mono_shader.vertex","mono_shader.frag");
	((ofxMonoMaterial*)material)->LoadTexturePNG("data/animal_tiger1_attack.png");
	material->BuildMaterial();
	ofxSpriteQuad* sprite = new ofxSpriteQuad();
	sprite->SetMaterial(material);
	sprite->SetLogicSize(192,192);
	sprite->SetSpriteRect(0,0,0,192,192);
	sprite->SetTextureRect(0,0,0,192,192);
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
	ofxSpriteMaterial* material = new ofxMonoMaterial();
	material->LoadShader("mono_shader.vertex","mono_shader.frag");
	((ofxMonoMaterial*)material)->LoadTexturePNG("data/plops/sprint0001.png");
	material->BuildMaterial();
	for(int i=-20;i<20;i++)
	{
		for(int j=-20;j<20;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->SetMaterial(material);
			sprite->SetLogicSize(192,192);
			sprite->SetSpriteRect(0,0,0,192,192);
			sprite->SetTextureRect(0,0,0,192,192);
			sprite->MoveTo(i*80.0f,j*60.0f,0.0f);
			ofxRENDERER->PushSprite(sprite);
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
	ofxSpriteMaterial* material = new ofxMonoMaterial();
	material->LoadShader("mono_shader.vertex","mono_shader.frag");
	((ofxMonoMaterial*)material)->LoadTexturePNG("data/plops/sprint0001.png");
	
	material->BuildMaterial();
	float space_x = 192.0f;
	float space_y = 192.0f;
	for(int i=-40;i<40;i++)
	{
		for(int j=-40;j<40;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->SetMaterial(material);
			
			sprite->SetLogicSize(192,192);
			sprite->SetSpriteRect(0,0,0,192,192);
			sprite->SetTextureRect(0,0,0,192,192);
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
	ofxSpriteMaterial** materials = new ofxSpriteMaterial*[8];
	for (int i = 0; i < 4; i++)
	{
		materials[i] = new ofxMonoMaterial();
		materials[i]->LoadShader("mono_shader.vertex","mono_shader.frag");
		((ofxMonoMaterial*)materials[i])->LoadTexturePNG(("data/plops/sprint000"+ofToString(i+1)+".png").c_str());
		materials[i]->BuildMaterial();
	}
	for (int i = 4; i < 8; i++)
	{
		materials[i] = new ofxPolyMaterial();
		materials[i]->LoadShader("poly_shader.vertex","poly_shader.frag");
		((ofxPolyMaterial*)materials[i])->LoadTexturePNG(("data/plops/sprint000"+ofToString(i+1)+".png").c_str(),0);
		materials[i]->BuildMaterial();
	}
	
	float space_x = 192.0f;
	float space_y = 192.0f;
	for(int i=-20;i<20;i++)
	{
		for(int j=-20;j<20;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->SetMaterial(materials[(j+20)/5]);
			
			sprite->SetLogicSize(192,192);
			sprite->SetSpriteRect(0,0,0,192,192);
			sprite->SetTextureRect(0,0,0,192,192);
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
	new ofxSpriteRenderer;
	ofxSpriteMaterial* material = new ofxMonoMaterial();
	material->LoadShader("mono_shader.vertex","mono_shader.frag");
	((ofxMonoMaterial*)material)->LoadTexturePNG("data/animal_tiger1_attack.png");
	material->BuildMaterial();
	ofxSpriteAnimation* animation = new ofxSpriteAnimation();
	animation->SetMaterial(material);
	animation->SetLogicSize(256,256);
	animation->MoveTo(0.0f,0.0f,0.0f);
	animation->SetFrameCount(24);

	animation->SetSequenceCount(2);
	animation->SetFrameTime(0,0.1f);
	{
		int i=0;
		animation->SetFrameData(i++,0,1,1,27,72,147,134,27,72);
		animation->SetFrameData(i++,0,1,76,27,72,147,134,27,72);
		animation->SetFrameData(i++,0,31,1,28,73,146,133,28,73);
		animation->SetFrameData(i++,0,62,1,28,72,146,134,28,72);
		animation->SetFrameData(i++,0,31,77,28,75,147,136,28,75);
		animation->SetFrameData(i++,0,62,76,27,71,147,133,27,71);
		animation->SetFrameData(i++,0,93,1,27,67,147,126,27,67);
		animation->SetFrameData(i++,0,123,1,28,78,146,131,28,78);
		animation->SetFrameData(i++,0,93,71,27,78,147,135,27,78);
		animation->SetFrameData(i++,0,1,151,27,75,147,140,27,75);
		animation->SetFrameData(i++,0,31,155,28,68,146,146,28,68);
		animation->SetFrameData(i++,0,62,150,28,72,146,140,28,72);
		animation->SetSequenceData(0,0,11);
		animation->SetFrameData(i++,0,154,1,93,67,113,139,93,67);
		animation->SetFrameData(i++,0,123,82,94,62,114,140,94,62);
		animation->SetFrameData(i++,0,93,152,93,62,118,140,93,62);
		animation->SetFrameData(i++,0,250,1,94,62,120,140,94,62);
		animation->SetFrameData(i++,0,220,82,97,60,119,143,97,60);
		animation->SetFrameData(i++,0,189,152,118,55,99,144,118,55);
		animation->SetFrameData(i++,0,347,1,117,55,85,134,117,55);
		animation->SetFrameData(i++,0,320,82,115,56,84,138,115,56);
		animation->SetFrameData(i++,0,310,152,112,55,88,143,112,55);
		animation->SetFrameData(i++,0,467,1,109,52,94,149,109,52);
		animation->SetFrameData(i++,0,438,82,106,54,99,149,106,54);
		animation->SetFrameData(i++,0,425,152,101,62,105,144,101,62);
		animation->SetSequenceData(1,12,23);
	}
	animation->SetSequence(0);
	ofxRENDERER->PushSprite(animation);
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
	ofxSpriteMaterial* material = new ofxMonoMaterial();
	material->LoadShader("mono_shader.vertex","mono_shader.frag");
	((ofxMonoMaterial*)material)->LoadTexturePNG("data/plops/sprint0001.png");
	
	material->BuildMaterial();
	float space_x = 192.0f;
	float space_y = 192.0f;
	for(int i=-40;i<40;i++)
	{
		for(int j=-50;j<50;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->SetMaterial(material);
			
			sprite->SetLogicSize(192,192);
			sprite->SetSpriteRect(0,0,0,192,192);
			sprite->SetTextureRect(0,0,0,192,192);
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
	ofxMonoMaterial** materials = new ofxMonoMaterial*[8];
	for (int i = 0; i < 8; i++)
	{
		materials[i] = new ofxMonoMaterial();
		materials[i]->LoadShader("mono_shader.vertex","mono_shader.frag");
		((ofxMonoMaterial*)materials[i])->LoadTexturePNG(("data/plops/sprint000"+ofToString(i+1)+".png").c_str());
		materials[i]->BuildMaterial();
	}
	float space_x = 192.0f;
	float space_y = 192.0f;
	for(int i=-60;i<60;i++)
	{
		for(int j=-60;j<60;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->SetMaterial(materials[(j+60)/15]);
			
			sprite->SetLogicSize(192,192);
			sprite->SetSpriteRect(0,0,0,192,192);
			sprite->SetTextureRect(0,0,0,192,192);
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
	if(spriteA->GetWorldPosition().z < 400.0f)
	{
		spriteA->MoveBy(0.0f,0.0f,15.0f);
	}
	else
	{
		spriteA->MoveBy(0.0f,0.0f,-800.0f);
	}
	if(spriteB->GetWorldPosition().z > -400.0f)
	{
		spriteB->MoveBy(0.0f,0.0f,-5.0f);
	}
	else
	{
		spriteB->MoveBy(0.0f,0.0f,800.0f);
	}
}
void SortBenchmarkTest::Render()
{
	ofxRENDERER->Render();
	printf("update time = %u\nrender time = %u\n",
		ofxRENDERER->GetUpdateTimeMilisecond(),
		ofxRENDERER->GetRenderTimeMilisecond());
}