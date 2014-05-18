#pragma once
struct ofxSpriteMaterial
{
	unsigned int TextureCount;
	GLuint* TextureID;
	GLuint* TextureSize;
	GLuint ShaderID;
	GLint ShaderXYZID;
	GLint* ShaderUVID;
};