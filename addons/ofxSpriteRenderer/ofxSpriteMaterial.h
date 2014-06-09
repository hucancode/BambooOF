#pragma once
struct ofxVertex
{
	float X,Y,Z;// position
	float UV[64];// texture uv
	float CUV[64];// crop uv, the value determine if we should use this pixel or not, cuv in (0,1) = true, else = false
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
	
};