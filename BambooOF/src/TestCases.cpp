#include "TestCases.h"

//Test* current_test = new SpriteTest();
//Test* current_test = new RendererTest();
Test* current_test = new SortingTest();
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
	ofxSpriteRenderer::CreateInstance();
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
	ofxSpriteRenderer::CreateInstance();
	ofxSpriteMaterial* material = new ofxMonoMaterial();
	material->LoadShader("mono_shader.vertex","mono_shader.frag");
	((ofxMonoMaterial*)material)->LoadTexturePNG("data/plops/sprint0001.png");
	material->BuildMaterial();
	/*for(int i=-10;i<10;i++)
	{
		for(int j=-10;j<10;j++)
		{*/
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
void SortingTest::Setup()
{
	ofxSpriteRenderer::CreateInstance();
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
			// 1.0x1.0 is medium to this scene
			float screen_width = ofGetWindowWidth();
			float screen_height = ofGetWindowHeight();
			sprite->SetLogicWidth(192);
			sprite->SetLogicHeight(192);
			sprite->SetSpriteRect(0,0,0,192,192);
			sprite->SetTextureRect(0,0,0,192,192);
			sprite->MoveTo(ofVec3f(i*192.0f,j*192.0f,i*0.01f));
			ofxSpriteRenderer::GetInstance()->PushSprite(sprite);
			if(i==-20 && j==-20) spriteA = sprite;
			if(i==19 && j==19) spriteB = sprite;
		}
	}
	ofxSpriteRenderer::GetInstance()->GetCamera()->setAspectRatio(800.0/600.0);
	ofxSpriteRenderer::GetInstance()->GetCamera()->scale = 20;
	spriteA->MoveBy(ofVec3f(0.0f,0.0f,0.00001f));
}
void SortingTest::Update()
{
	ofxSpriteRenderer::GetInstance()->Update();
	if(spriteA->GetWorldPosition().z < 0.2f)
	{
		spriteA->MoveBy(ofVec3f(0.0f,0.0f,0.00005f));
	}
	else
	{
		spriteA->MoveBy(ofVec3f(0.0f,0.0f,-0.4f));
	}
	if(spriteB->GetWorldPosition().z > -0.2f)
	{
		spriteB->MoveBy(ofVec3f(0.0f,0.0f,-0.00005f));
	}
	else
	{
		spriteB->MoveBy(ofVec3f(0.0f,0.0f,0.4f));
	}
	printf("spriteA->m_IndexInRenderer = %d\n", spriteA->m_IndexInRenderer);
	printf("spriteB->m_IndexInRenderer = %d\n", spriteB->m_IndexInRenderer);
}
void SortingTest::Render()
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