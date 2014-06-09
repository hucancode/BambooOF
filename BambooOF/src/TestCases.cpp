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
	material->LoadShader("default_shader.vertex","default_shader.frag");
	((ofxMonoMaterial*)material)->LoadTexturePNG("data/plops/sprint0001.png");
	material->BuildMaterial();
	ofxSpriteQuad* sprite = new ofxSpriteQuad();
	sprite->SetMaterial(material);
	ofxSpriteRenderer::GetInstance()->PushSprite(sprite);
	//ofxSpriteRenderer::GetInstance()->BuildSolidCommands(0,1);
	ofxSpriteRenderer::GetInstance()->BuildTransparentCommands(0,1);
}
void SpriteTest::Update()
{
	ofxSpriteRenderer::GetInstance()->Update();
}
void SpriteTest::Render()
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