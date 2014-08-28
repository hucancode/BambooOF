#include "TestCases.h"
#include "ofxTextureCache.h"
#include "ofxShaderCache.h"
#include "ofxShaderProgramCache.h"
Test* current_test = new TerrainSpriteTest();
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
	sprite->SetTexture("data/sprint0001.png");
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
			sprite->SetTexture("data/sprint0001.png");
			sprite->MoveTo(i*192.0f,0.0f,j*192.0f);
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
			sprite->SetTexture("data/sprint0001.png");
			sprite->MoveTo(i*space_x,0.0f,j*space_y);
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
			sprite->SetTexture(("data/sprint000"+ofToString(abs(j)%9+1)+".png").c_str());
			sprite->MoveTo(i*space_x,0.0f,j*space_y);
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

void TerrainTest::Setup()
{
	terrain = new ofxTerrain();
	terrain->Initialize(100, 100);
	terrain->LoadBaseTexture("data/base24.png");
	terrain->LoadGroundTexture("data/tile_with_code.png",0);
	terrain->PaintTile(10, 10);
	terrain->BuildTileMap();
}
void TerrainTest::Update()
{
}
void TerrainTest::Render()
{
	terrain->RenderTiles();
}
void TerrainSpriteTest::Setup()
{
	const int TERRAIN_WIDTH = 100;
	const int TERRAIN_HEIGHT = 100;
	const int SEED_MAX_WIDTH = 10;
	const int SEED_MAX_HEIGHT = 10;
	const int SEED_MAX_TRACE_X = 10;
	const int SEED_MAX_TRACE_Y = 10;
	terrain = new ofxTerrain();
	terrain->Initialize(TERRAIN_WIDTH, TERRAIN_HEIGHT);
	terrain->LoadBaseTexture("data/base24.png");
	terrain->LoadGroundTexture("data/tile.png", 0);

	// --------- this is a stupid terrain generating algorithm, you don't need to check
	for(int i=0;i<50;i++)
	{
		int x = ofRandom(0, TERRAIN_WIDTH - max(SEED_MAX_WIDTH,SEED_MAX_TRACE_X) - 1);
		int y = ofRandom(0, TERRAIN_HEIGHT - max(SEED_MAX_HEIGHT,SEED_MAX_TRACE_Y) - 1);
		int spread_x = ofRandom(0, SEED_MAX_WIDTH);
		int spread_y = ofRandom(0, SEED_MAX_HEIGHT);
		for(int j=0;j<spread_x;j++)
		{
			for(int k=0;k<spread_y;k++)
			{
				terrain->PaintTile(x+j, y+k);
			}
		}
		int trace_x = ofRandom(0, SEED_MAX_TRACE_X);
		int trace_y = ofRandom(0, SEED_MAX_TRACE_Y);
		for(int j=0;j<trace_x;j++)
		{
			terrain->PaintTile(x+j, y);
			terrain->PaintTile(x+j+1, y);
			terrain->PaintTile(x+j+1, y+1);
			terrain->PaintTile(x+j, y+1);
		}
		for(int j=0;j<trace_y;j++)
		{
			terrain->PaintTile(x, y+j);
			terrain->PaintTile(x+1, y+j);
			terrain->PaintTile(x+1, y+j+1);
			terrain->PaintTile(x, y+j+1);
		}
	}
	//--------------
	terrain->BuildTileMap();
	for(int i=-20;i<20;i++)
	{
		for(int j=-20;j<20;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->SetTexture("data/sprint0001.png");
			sprite->MoveTo(i*192.0f,0.0f,j*192.0f);
			if(i==0 && j==0) spriteObstacle = sprite;
		}
	}
}
void TerrainSpriteTest::Update()
{
	ofxRENDERER->Update();
}
void TerrainSpriteTest::Render()
{
	terrain->RenderTiles();
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
			sprite->SetTexture("data/sprint0001.png");
			sprite->MoveTo(i*space_x,0.0f,j*space_y);
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
	for(int i=-50;i<50;i++)
	{
		for(int j=-50;j<50;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->SetTexture(("data/sprint000"+ofToString(abs(j)%9+1)+".png").c_str());
			sprite->MoveTo(i*space_x,0.0f,j*space_y);
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
	for(int i=-50;i<50;i++)
	{
		for(int j=-50;j<50;j++)
		{
			ofxSpriteAnimation* animation = new ofxSpriteAnimation();
			animation->SetTexture("data/animal_tiger1_attack.png");
			animation->MoveTo(i*space_x,0.0f,j*space_y);
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
			if(i==0 && j==0) spriteObstacle = animation;
			if(i==0 && j==0) spriteA = animation;
			if(i==1 && j==1) spriteB = animation;
		}
	}
}
void AnimationBenchmarkTest::Update()
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
}
void AnimationBenchmarkTest::Render()
{
	ofxRENDERER->Render();
	printf("update time = %u\nrender time = %u\n",
		ofxRENDERER->GetUpdateTimeMilisecond(),
		ofxRENDERER->GetRenderTimeMilisecond());
}