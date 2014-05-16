#pragma once
struct ofxSpriteMaterial
{
	unsigned int TextureCount;
	GLuint* TextureID;
	GLuint ShaderID;
	GLint ShaderXYZID;
	GLint* ShaderUVID;
};