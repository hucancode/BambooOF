#include "TestCases.h"

Test* current_test = new MultiTextureAnimationTest();
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
void MultiTextureTest::Setup()
{
	ofxSpriteMaterial* material = new ofxPolyMaterial();
	material->LoadShader("poly_shader.vertex","poly_shader.frag");
	((ofxPolyMaterial*)material)->SetMaxTexture(2);
	((ofxPolyMaterial*)material)->LoadTexturePNG(0,"data/animal_tiger1_attack.png");
	((ofxPolyMaterial*)material)->LoadTexturePNG(1,"data/animal_tiger1_attack_shadow.png");
	((ofxPolyMaterial*)material)->SetOrder(0,0);
	((ofxPolyMaterial*)material)->SetOrder(1,1);
	material->BuildMaterial();

	ofxSpriteQuad* sprite = new ofxSpriteQuad();
	sprite->SetMaterial(material);
	sprite->SetLogicSize(256,256);
	sprite->SetSpriteRect(0,147,134,27,72);
	sprite->SetTextureRect(0,1,1,27,72);
	sprite->SetSpriteRect(1,147,162,45,56);
	sprite->SetTextureRect(1,1,1,45,56);

	sprite->MoveTo(0.0f,0.0f,0.0f);
	ofxRENDERER->PushSprite(sprite);
	spriteObstacle = sprite;
}
void MultiTextureTest::Update()
{
	ofxRENDERER->Update();
}
void MultiTextureTest::Render()
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
void MultiTextureAnimationTest::Setup()
{
	new ofxSpriteRenderer;
	ofxSpriteMaterial* material = new ofxPolyMaterial();
	material->LoadShader("poly_shader.vertex","poly_shader.frag");
	((ofxPolyMaterial*)material)->SetMaxTexture(2);
	((ofxPolyMaterial*)material)->LoadTexturePNG(0,"data/animal_tiger1_attack.png");
	((ofxPolyMaterial*)material)->LoadTexturePNG(1,"data/animal_tiger1_attack_shadow.png");
	((ofxPolyMaterial*)material)->SetOrder(0,0);
	((ofxPolyMaterial*)material)->SetOrder(1,1);
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
		// texture 1
		// sequence 1
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
		// sequence 2
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
		i = 0;
		// texture 2
		// sequence 1
		animation->SetFrameData(i++,1,1,1,45,56,147,162,45,56);
        animation->SetFrameData(i++,1,1,60,41,75,147,141,41,75);
        animation->SetFrameData(i++,1,49,1,35,80,150,134,35,80);
        animation->SetFrameData(i++,1,1,138,36,80,148,134,36,80);
        animation->SetFrameData(i++,1,49,84,35,56,148,159,35,56);
        animation->SetFrameData(i++,1,87,1,46,76,147,148,46,76);
        animation->SetFrameData(i++,1,136,1,59,83,147,151,59,83);
        animation->SetFrameData(i++,1,136,87,57,84,149,153,57,84);
        animation->SetFrameData(i++,1,1,221,52,83,152,155,52,83);
        animation->SetFrameData(i++,1,136,174,48,94,152,140,48,94);
        animation->SetFrameData(i++,1,49,143,47,57,150,168,47,57);
        animation->SetFrameData(i++,1,56,221,48,56,146,166,48,56);
		// sequence 2
        animation->SetFrameData(i++,1,198,1,95,57,116,155,95,57);
        animation->SetFrameData(i++,1,196,87,94,45,119,167,94,45);
        animation->SetFrameData(i++,1,187,174,92,44,123,166,92,44);
        animation->SetFrameData(i++,1,187,221,90,52,127,157,90,52);
        animation->SetFrameData(i++,1,296,1,99,50,121,161,99,50);
        animation->SetFrameData(i++,1,398,1,129,66,100,149,129,66);
        animation->SetFrameData(i++,1,293,87,137,79,87,141,137,79);
        animation->SetFrameData(i++,1,282,174,131,75,86,149,131,75);
        animation->SetFrameData(i++,1,530,1,124,80,93,147,124,80);
        animation->SetFrameData(i++,1,433,87,116,73,99,152,116,73);
        animation->SetFrameData(i++,1,416,174,106,60,108,157,106,60);
        animation->SetFrameData(i++,1,525,174,105,67,106,147,105,67);
		
	}
	animation->SetSequence(0);
	ofxRENDERER->PushSprite(animation);
	spriteObstacle = animation;
}
void MultiTextureAnimationTest::Update()
{
	ofxRENDERER->Update();
}
void MultiTextureAnimationTest::Render()
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