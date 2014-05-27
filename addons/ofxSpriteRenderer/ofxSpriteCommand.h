#pragma once
#include "ofMain.h"
#include "ofxSpriteMaterial.h"
#include "ofxSpriteQuad.h"
class ofxSpriteCommand
{
private:
	ofxSpriteMaterial* m_Material;
	vector<ofxVertex> m_Vertices;
	vector<GLushort> m_Indices;
	GLuint m_VBOId;
	GLuint m_IBOId;
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
};
#define ofxSpriteCommands vector<ofxSpriteCommand*>