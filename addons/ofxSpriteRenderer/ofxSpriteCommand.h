#pragma once
#include "ofMain.h"
#include "ofxSpriteMaterial.h"
#include "ofxSpriteQuad.h"
class ofxSpriteCommand
{
	friend class ofxSpriteQuad;
	friend class ofxSpriteRenderer;
private:
	ofxSpriteMaterial* m_Material;
	vector<ofxVertex> m_Vertices;
	vector<GLushort> m_Indices;
	GLuint m_VBOId;
	GLuint m_IBOId;
private:
	bool m_Spliting;
	bool m_Splited;
	bool m_Unsorting;
	bool m_Unsorted;
public:
	ofxSpriteCommand();
	~ofxSpriteCommand();
	ofxSpriteMaterial* GetMaterial() { return m_Material; }
	void SetMaterial(ofxSpriteMaterial* material) { m_Material = material; }
	void Bind();
	void Unbind();
	void Render();
	void PushSprite(ofxSpriteQuad* sprite);
	void UpdateSprite(ofxSpriteQuad* sprite);
public:
	bool IsSplited()
	{
		return m_Splited;
	}
	bool IsUnsorted()
	{
		return m_Unsorted;
	}
};
typedef vector<ofxSpriteCommand*> ofxSpriteCommands;