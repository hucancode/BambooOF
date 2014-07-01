#include "TestCases.h"

Test* current_test = new SortBenchmarkTest();
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
	new ofxSpriteRenderer;
	ofxSpriteMaterial* material = new ofxMonoMaterial();
	material->LoadShader("mono_shader.vertex","mono_shader.frag");
	((ofxMonoMaterial*)material)->LoadTexturePNG("data/plops/sprint0001.png");
	material->BuildMaterial();
	ofxSpriteQuad* sprite = new ofxSpriteQuad();
	sprite->SetMaterial(material);
	sprite->SetLogicWidth(192);
	sprite->SetLogicHeight(192);
	sprite->SetSpriteRect(0,0,0,192,192);
	sprite->SetTextureRect(0,0,0,192,192);
	sprite->MoveTo(ofVec3f(0.0f,0.0f,0.1f));
	ofxRENDERER->PushSprite(sprite);
	ofxRENDERER->GetCamera()->setAspectRatio(ofGetWindowWidth()/ofGetWindowHeight());
	ofxRENDERER->GetCamera()->SetScale(40);
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
	new ofxSpriteRenderer;
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
			sprite->SetLogicWidth(192);
			sprite->SetLogicHeight(192);
			sprite->SetSpriteRect(0,0,0,192,192);
			sprite->SetTextureRect(0,0,0,192,192);
			sprite->MoveTo(ofVec3f(i*80.0f,j*60.0f,0.0f));
			ofxRENDERER->PushSprite(sprite);
		}
	}
	ofxRENDERER->GetCamera()->setAspectRatio(ofGetWindowWidth()/ofGetWindowHeight());
	ofxRENDERER->GetCamera()->SetScale(40);
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
	float screen_width = ofGetWindowWidth();
	float screen_height = ofGetWindowHeight();
	new ofxSpriteRenderer;
	ofxRENDERER->GetCamera()->setAspectRatio(screen_width/screen_height);
	ofxRENDERER->GetCamera()->SetScale(40);
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
			
			sprite->SetLogicWidth(192);
			sprite->SetLogicHeight(192);
			sprite->SetSpriteRect(0,0,0,192,192);
			sprite->SetTextureRect(0,0,0,192,192);
			sprite->MoveTo(ofVec3f(i*space_x,i*0.01f,j*space_y));
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
		spriteA->MoveBy(ofVec3f(0.0f,0.0f,0.0001f));
	}
	else
	{
		spriteA->MoveBy(ofVec3f(0.0f,0.0f,-0.4f));
	}
	if(spriteB->GetWorldPosition().z > -0.2f)
	{
		spriteB->MoveBy(ofVec3f(0.0f,0.0f,-0.0001f));
	}
	else
	{
		spriteB->MoveBy(ofVec3f(0.0f,0.0f,0.4f));
	}
}
void SortingTest::Render()
{
	ofxRENDERER->Render();
}
void TextureTest::Setup()
{
	float screen_width = ofGetWindowWidth();
	float screen_height = ofGetWindowHeight();
	new ofxSpriteRenderer;
	ofxRENDERER->GetCamera()->setAspectRatio(screen_width/screen_height);
	ofxRENDERER->GetCamera()->SetScale(40);
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
	for(int i=-20;i<20;i++)
	{
		for(int j=-20;j<20;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->SetMaterial(materials[(j+20)/5]);
			
			sprite->SetLogicWidth(192);
			sprite->SetLogicHeight(192);
			sprite->SetSpriteRect(0,0,0,192,192);
			sprite->SetTextureRect(0,0,0,192,192);
			sprite->MoveTo(ofVec3f(i*space_x,0.0f,j*space_y));
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
	float screen_width = ofGetWindowWidth();
	float screen_height = ofGetWindowHeight();
	new ofxSpriteRenderer;
	
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
	float screen_width = ofGetWindowWidth();
	float screen_height = ofGetWindowHeight();
	new ofxSpriteRenderer;
	ofxRENDERER->GetCamera()->setAspectRatio(screen_width/screen_height);
	ofxRENDERER->GetCamera()->SetScale(40);
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
			
			sprite->SetLogicWidth(192);
			sprite->SetLogicHeight(192);
			sprite->SetSpriteRect(0,0,0,192,192);
			sprite->SetTextureRect(0,0,0,192,192);
			sprite->MoveTo(ofVec3f(i*space_x,0.0f,j*space_y));
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
	float screen_width = ofGetWindowWidth();
	float screen_height = ofGetWindowHeight();
	new ofxSpriteRenderer;
	ofxRENDERER->GetCamera()->setAspectRatio(screen_width/screen_height);
	ofxRENDERER->GetCamera()->SetScale(min(screen_width,screen_height)*0.5);
	ofxSpriteMaterial* material = new ofxMonoMaterial();
	material->LoadShader("mono_shader.vertex","mono_shader.frag");
	((ofxMonoMaterial*)material)->LoadTexturePNG("data/plops/sprint0001.png");
	
	material->BuildMaterial();
	float space_x = 192.0f;
	float space_y = 192.0f;
	for(int i=-60;i<60;i++)
	{
		for(int j=-60;j<60;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->SetMaterial(material);
			
			sprite->SetLogicWidth(192);
			sprite->SetLogicHeight(192);
			sprite->SetSpriteRect(0,0,0,192,192);
			sprite->SetTextureRect(0,0,0,192,192);
			sprite->MoveTo(ofVec3f(i*space_x,0.0f,j*space_y));
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
	if(spriteA->GetWorldPosition().z < 0.2f)
	{
		//spriteA->MoveBy(ofVec3f(0.0f,0.0f,0.0001f));
	}
	else
	{
		spriteA->MoveBy(ofVec3f(0.0f,0.0f,-0.4f));
	}
	if(spriteB->GetWorldPosition().z > -400.0f)
	{
		spriteB->MoveBy(ofVec3f(0.0f,0.0f,-5.0f));
		printf("%f\n",spriteB->GetWorldPosition().z);
	}
	else
	{
		spriteB->MoveBy(ofVec3f(0.0f,0.0f,800.0f));
	}
}
void SortBenchmarkTest::Render()
{
	ofxRENDERER->Render();
	printf("update time = %u\nrender time = %u\n",ofxRENDERER->GetUpdateTimeMilisecond(),ofxRENDERER->GetRenderTimeMilisecond());
}