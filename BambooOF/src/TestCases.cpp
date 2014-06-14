#include "TestCases.h"

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
	((ofxMonoMaterial*)material)->LoadTexturePNG("data/plops/sprint0001.png");
	material->BuildMaterial();
	ofxSpriteQuad* sprite = new ofxSpriteQuad();
	sprite->SetMaterial(material);
	// 1.0x1.0 is medium to this scene
	sprite->SetWidth(1.0f);
	sprite->SetHeight(1.0f);
	// 192x192 is size of sprint0001.png
	sprite->SetLogicWidth(192);
	sprite->SetLogicHeight(192);
	sprite->SetSpriteRect(0,0,0,192,192);
	sprite->SetTextureRect(0,0,0,192,192);
	sprite->MoveTo(ofVec3f(100.0f,100.0f,0.0f));
	ofxSpriteRenderer::GetInstance()->PushSprite(sprite);
	ofxSpriteRenderer::GetInstance()->GetCamera()->setAspectRatio(800.0/600.0);
	ofxSpriteRenderer::GetInstance()->GetCamera()->scale = 20;
}
void SpriteTest::Update()
{
	ofxSpriteRenderer::GetInstance()->Update();
}
void SpriteTest::Render()
{
	ofxSpriteRenderer::GetInstance()->Render();
}
void RendererTest::Setup()
{
	ofxSpriteMaterial* material = new ofxMonoMaterial();
	material->LoadShader("mono_shader.vertex","mono_shader.frag");
	((ofxMonoMaterial*)material)->LoadTexturePNG("data/plops/sprint0001.png");
	material->BuildMaterial();
	for(int i=0;i<10;i++)
	{
		for(int j=0;j<10;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->SetMaterial(material);
			// 1.0x1.0 is medium to this scene
			sprite->SetWidth(1.0f);
			sprite->SetHeight(1.0f);
			// 192x192 is size of sprint0001.png
			sprite->SetLogicWidth(192);
			sprite->SetLogicHeight(192);
			sprite->SetSpriteRect(0,0,0,192,192);
			sprite->SetTextureRect(0,0,0,192,192);
			sprite->MoveTo(ofVec3f(i*80.0f,j*60.0f,0.0f));
			ofxSpriteRenderer::GetInstance()->PushSprite(sprite);
		}
	}
	ofxSpriteRenderer::GetInstance()->GetCamera()->setAspectRatio(800.0/600.0);
	ofxSpriteRenderer::GetInstance()->GetCamera()->scale = 20;
}
void RendererTest::Update()
{
	ofxSpriteRenderer::GetInstance()->Update();
}
void RendererTest::Render()
{
	ofxSpriteRenderer::GetInstance()->Render();
}
void SpriteBenchmarkTest::Setup()
{
}
void SpriteBenchmarkTest::Update()
{
}
void SpriteBenchmarkTest::Render()
{
}