#pragma once
#include "ofMain.h"
#include "ofxPolyMaterial.h"
#include "ofxSpriteQuad.h"

enum COMMAND_STATUS
{
	COMMAND_STATUS_UNITED,
	COMMAND_STATUS_DISMISSED,
	COMMAND_STATUS_EXPANDED
};
#define COMMAND_CAPACITY 800

class ofxSpriteCommand
{
	friend class ofxSpriteQuad;
	friend class ofxSpriteRenderer;
public:
	static void GenerateSharedIndices(unsigned short number_of_quad=5000);
	static void DeleteSharedIndices();
//private:
public:
	unsigned short m_IndexInRenderer;
	unsigned int m_FirstSpriteIndex;
	unsigned int m_LastSpriteIndex;
	unsigned int m_VisibleSpriteCount;
	float m_DistanceMin;
	float m_DistanceMax;
	ofxSpriteMaterial* m_Material;
	vector<ofxVertex> m_Vertices;
	vector<bool> m_VisibleSprite;
	GLuint m_VBOId;
	GLsizei m_IndicesSize;
	static vector<GLushort> m_Indices;
	static GLuint m_IBOId;
	COMMAND_STATUS m_Status;
public:
	ofxSpriteCommand();
	~ofxSpriteCommand();
	
	ofxSpriteMaterial* GetMaterial() { return m_Material; }
	void SetMaterial(ofxSpriteMaterial* material) { m_Material = material; }
	void Bind();
	void Unbind();
	void Render();
	void PushSprite(ofxSpriteQuad* sprite);
	void EraseSprite(ofxSpriteQuad* sprite);
	void UpdateSprite(ofxSpriteQuad* sprite);
	void Rebuild();
public:
	unsigned int GetFirstSpriteIndex()
	{
		return m_FirstSpriteIndex;
	}
	unsigned int GetLastSpriteIndex()
	{
		return m_LastSpriteIndex;
	}
};
typedef vector<ofxSpriteCommand*> ofxSpriteCommands;