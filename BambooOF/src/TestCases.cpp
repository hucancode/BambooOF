#include "TestCases.h"
#include "ofxTextureCache.h"
#include "ofxShaderCache.h"
#include "ofxShaderProgramCache.h"
#include "ofxBitmapFontCache.h"
#include "ofxParticleEffect2D.h"
Test* current_test = new Particle2DTest();
ofxSpriteBase* spriteObstacle;
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
	sprite->SetTexture("data/psk_actor_idle.png");
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
			sprite->SetTexture("data/psk_actor_idle.png");
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
			sprite->SetTexture("data/psk_actor_idle.png");
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
	if(spriteA->GetPosition().z < 0.2f)
	{
		spriteA->MoveBy(0.0f,0.0f,0.0001f);
	}
	else
	{
		spriteA->MoveBy(0.0f,0.0f,-0.4f);
	}
	if(spriteB->GetPosition().z > -0.2f)
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
			sprite->SetTexture(("data/sprite"+ofToString(abs(j)%5+1)+".png").c_str());
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
	animation->SetTexture("data/psk_actor.png");
	animation->MoveTo(0.0f,0.0f,0.0f);
	animation->SetFrameCount(24);
	animation->SetSequenceCount(2);
	animation->SetFrameTime(0,0.1f);
	animation->SetFrameTime(1,0.1f);
	{
		int i=0;
		// run
		for(int x=0;x<3;x++)
		{
			for(int y=0;y<4;y++)
			{
				animation->SetFrameData(i++,x*170,y*170,170,170,0,0,170,170);
			}
		}
		animation->SetSequenceData(0,0,11);
		// respawn
		for(int x=3;x<6;x++)
		{
			for(int y=0;y<4;y++)
			{
				animation->SetFrameData(i++,x*170,y*170,170,170,0,0,170,170);
			}
		}
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
			sprite->SetTexture("data/psk_actor_idle.png");
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
void TextSpriteTest::Setup()
{
	string header = "this is a header, so i draw it big";
	string text = "hello world, this is a long long long text.";
	ofxBitmapFont* font = ofxBITMAPFONTCACHE->GetResource("data/verdana.xml");
	ofxTexture* texture = new ofxTexture();
	ofVec2f dimension_header = font->GetTextDimension(header, 44);
	ofVec2f dimension_text = font->GetTextDimension(text);
	texture->Allocate(max(dimension_header.x, dimension_text.x), dimension_header.y+dimension_text.y);
	texture->DrawString(header, font, ofVec2f(0,0), 44);
	texture->DrawString(text, font, ofVec2f(0,dimension_header.y));
	texture->FlipY();
	texture->SubmitChanges();
	ofxTEXTURECACHE->PushResource(texture, "txt_verdana"+text);
	ofxSpriteQuad* sprite = new ofxSpriteQuad();
	sprite->SetTexture("txt_verdana"+text);
	//sprite->SetOpacity(100);
	spriteObstacle = sprite;
}
void TextSpriteTest::Update()
{
	ofxRENDERER->Update();
}
void TextSpriteTest::Render()
{
	ofxRENDERER->Render();
}
void Particle2DTest::Setup()
{
	ofxParticleEffect2D::LoadSharedParticleTexture();
	ofxParticleEffect2D::BuildSinCosTable();
	ofxParticleEffect2D* sprite = new ofxParticleEffect2D();
	ofxEmitter2D* emitter = new ofxEmitter2D();
	emitter->position = ofVec2f(0,0);
	emitter->angle = 0;
	emitter->angle_var = TWO_PI;
	emitter->emission_rate = 10;
	emitter->emission_rate_var = 0;
	emitter->emission_time = 0.1f;
	emitter->emission_time_var = 0.0f;
	emitter->life = 2.0f;
	emitter->life_var = 0.0f;
	emitter->radial_accel = 1.0f;
	emitter->radial_accel_var = 1.0f;
	emitter->tangental_accel = -2.0f;
	emitter->tangental_accel_var = 1.0f;
	emitter->speed = 15.0f;
	emitter->speed_var = 20.0f;
	emitter->radius = 0.0f;
	emitter->radius_var = 20.0f;
	// TODO: color
	emitter->begin_size = 15.0f;
	emitter->begin_opacity;
	emitter->begin_opacity_var;
	emitter->begin_color_intensity;
	emitter->begin_color_intensity_var;
	emitter->begin_color;
	emitter->begin_color_var;
	emitter->end_size = 5.0f;
	emitter->end_opacity;
	emitter->end_opacity_var;
	emitter->end_color_intensity;
	emitter->end_color_intensity_var;
	emitter->end_color;
	emitter->end_color_var;

	sprite->AddEmitter(emitter);
	spriteObstacle = sprite;
}
void Particle2DTest::Update()
{
	ofxRENDERER->Update();
}
void Particle2DTest::Render()
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
			sprite->SetTexture("data/psk_actor_idle.png");
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
			sprite->SetTexture(("data/sprite"+ofToString(abs(j)%5+1)+".png").c_str());
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
	if(spriteA->GetPosition().z < 1000.0f)
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
			animation->SetTexture("data/psk_actor.png");
			animation->MoveTo(i*space_x,0.0f,j*space_y);
			animation->SetFrameCount(24);
			animation->SetSequenceCount(2);
			animation->SetFrameTime(0,0.1f);
			animation->SetFrameTime(1,0.1f);
			{
				int i=0;
				// run
				for(int x=0;x<3;x++)
				{
					for(int y=0;y<4;y++)
					{
						animation->SetFrameData(i++,x*170,y*170,170,170,0,0,170,170);
					}
				}
				animation->SetSequenceData(0,0,11);
				// respawn
				for(int x=3;x<6;x++)
				{
					for(int y=0;y<4;y++)
					{
						animation->SetFrameData(i++,x*170,y*170,170,170,0,0,170,170);
					}
				}
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
	if(spriteA->GetPosition().z < 1000.0f)
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
void TerrainBenchmarkTest::Setup()
{
	const int TERRAIN_WIDTH = 1000;
	const int TERRAIN_HEIGHT = 1000;
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
	for(int i=-40;i<40;i++)
	{
		for(int j=-40;j<40;j++)
		{
			ofxSpriteQuad* sprite = new ofxSpriteQuad();
			sprite->SetTexture("data/psk_actor_idle.png");
			sprite->MoveTo(i*192.0f,0.0f,j*192.0f);
			if(i==0 && j==0) spriteObstacle = sprite;
		}
	}
}
void TerrainBenchmarkTest::Update()
{
	ofxRENDERER->Update();
}
void TerrainBenchmarkTest::Render()
{
	terrain->RenderTiles();
	ofxRENDERER->Render();
}