#include "ofxTexture.h"
ofxTexture::ofxTexture()
	:ofxResource()
{
	glGenTextures(1, &m_TextureId);
}
ofxTexture::~ofxTexture()
{
	FreeImage_Unload(m_ImageData);
	glDeleteTextures(1, &m_TextureId);
}
bool ofxTexture::Load(string texture_file)
{
	m_ImageData = FreeImage_Load(FIF_PNG, texture_file.c_str(), PNG_DEFAULT);
	FreeImage_FlipVertical(m_ImageData);
	unsigned int bpp = FreeImage_GetBPP(m_ImageData);
	unsigned int width = FreeImage_GetWidth(m_ImageData);
	unsigned int height = FreeImage_GetHeight(m_ImageData);
	BYTE* pixel_data = FreeImage_GetBits(m_ImageData);
	{
		GLenum format = bpp==24?GL_RGB:GL_RGBA;
		//glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, m_TextureId);
		glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, pixel_data);
		m_TextureSize.x = width;
		m_TextureSize.y = height;
	}
	{
		GLint param = GL_CLAMP_TO_EDGE;
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,param);
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,param);
	}
	{
		GLint param = GL_LINEAR;
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, param);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, param);
		if(param == GL_LINEAR_MIPMAP_LINEAR || GL_LINEAR_MIPMAP_NEAREST)
		{
			glGenerateMipmap(GL_TEXTURE_2D);
		}
	}
	/*delete pixel_data;
	delete image_data;*/
	return true;
}
void ofxTexture::IncreaseReference()
{
	ofxResource::IncreaseReference();
}
void ofxTexture::DecreaseReference()
{
	ofxResource::DecreaseReference();
}
bool ofxTexture::IsUnused()
{
	return ofxResource::IsUnused();
}
void ofxTexture::Bind(GLuint slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, m_TextureId);
}
void ofxTexture::Unbind(GLuint slot)
{
	glActiveTexture(GL_TEXTURE0 + slot);
	glBindTexture(GL_TEXTURE_2D, 0);
}
ofVec2f ofxTexture::GetTextureSize()
{
	return m_TextureSize; 
}
/* ----------------------------------
texture operation
---------------------------------- */
void ofxTexture::Allocate(unsigned int width, unsigned int height)
{
}
ofColor ofxTexture::GetPixel(unsigned int x, unsigned int y)
{
	return ofColor::aliceBlue;
}
void ofxTexture::SetPixel(unsigned int x, unsigned int y, ofColor color)
{
}
void ofxTexture::BlockTransfer(ofxTexture* source, ofRectangle source_rect, ofVec2f dest_pos)
{
}
void ofxTexture::StretchTransfer(ofxTexture* source, ofRectangle source_rect, ofRectangle dest_rect)
{
}
void ofxTexture::Fill(ofColor color, ofRectangle dest_rect)
{
}
void ofxTexture::Clear(ofRectangle dest_rect)
{
}
void ofxTexture::DrawString(ofxBitmapFont* font, string text, ofVec2f dest_pos, float font_size, ofVec2f bound)
{
}