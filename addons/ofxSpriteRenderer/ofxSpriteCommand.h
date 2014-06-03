#pragma once
#include "ofMain.h"
#include "ofxSpriteMaterial.h"
#include "ofxSpriteQuad.h"

enum COMMAND_STATUS
{
	COMMAND_STATUS_UNITED,
	COMMAND_STATUS_DISMISSED,
	COMMAND_STATUS_EXPANDED
	
};
class ofxSpriteCommand
{
	friend class ofxSpriteQuad;
	friend class ofxSpriteRenderer;
public:
	static void GenerateSharedIndices(unsigned int number_of_quad=5000);
	static void DeleteSharedIndices();
private:
	unsigned short m_IndexInRenderer;
	unsigned int m_FirstSpriteIndex;
	unsigned int m_LastSpriteIndex;
	float m_DistanceMin;
	float m_DistanceMax;
	ofxSpriteMaterial* m_Material;
	vector<ofxVertex> m_Vertices;
	GLuint m_VBOId;
	GLsizei m_IndicesSize;
	static vector<GLuint> m_Indices;
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
	void UpdateSprite(ofxSpriteQuad* sprite, bool update_status = true);
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