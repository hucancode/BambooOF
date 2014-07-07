#pragma once
struct ofxVertex
{
	float X,Y,Z;// position
	float UV[60];// texture uv
};
enum MATERIAL_TYPE
{
	MATERIAL_TYPE_UNKNOWN,
	MATERIAL_TYPE_MONO,
	MATERIAL_TYPE_POLY
};
class ofxSpriteMaterial
{
public:
	virtual MATERIAL_TYPE GetType(){ return MATERIAL_TYPE_UNKNOWN; }
	virtual bool LoadShader(const char* vs_file, const char* fs_file){ return true;}
	virtual void BuildMaterial(){}
	virtual void Bind(){}
	virtual void Unbind(){}
	virtual void IncreaseReference(){}
	virtual void DecreaseReference(){}
	virtual unsigned int GetTextureCount(){ return 0;}
	virtual ofVec2f GetTextureSize(const int index){return ofVec2f::zero();}
	
};