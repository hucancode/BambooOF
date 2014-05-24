#pragma once
struct ofxVertex
{
	float X,Y,Z;// position
	float UV[64];// texture uv
	float CUV[64];// crop uv, the value determine if we should use this pixel or not, cuv in (0,1) = true, else = false
};
class ofxSpriteMaterial
{
private:
	unsigned int m_TextureCount;
	GLuint* m_TextureId;
	GLuint* m_TextureOrder;
	GLuint* m_TextureSize;
	GLuint m_ShaderProgramId;
	GLint m_ShaderXYZId;
	GLint* m_ShaderUVId;
	GLint* m_ShaderRectId;
	GLint* m_ShaderTextureId;
public:
	ofxSpriteMaterial()
	{
		m_TextureCount = 0;
		m_TextureId = 0;
		m_TextureOrder = 0;
		m_TextureSize = 0;
		m_ShaderProgramId = 0;
		m_ShaderXYZId = 0;
		m_ShaderUVId = 0;
		m_ShaderTextureId = 0;
	}
	~ofxSpriteMaterial()
	{
	}
	void SetMaxTexture(const int size)
	{
		if(m_TextureId) delete[] m_TextureId;
		m_TextureCount = size;
		m_TextureId = new GLuint[size];
		if(m_TextureSize) delete[] m_TextureSize;
		m_TextureSize = new GLuint[size*2];
		if(m_TextureOrder) delete[] m_TextureOrder;
		m_TextureOrder = new GLuint[size];
	}
	unsigned int GetTextureCount()
	{
		return m_TextureCount;
	}
	GLuint GetTextureSizeRaw(const int index)
	{
		return m_TextureSize[index]; 
	}
	GLuint GetTextureSizeX(const int index)
	{
		return m_TextureSize[index*2]; 
	}
	GLuint GetTextureSizeY(const int index)
	{
		return m_TextureSize[index*2+1]; 
	}
	void LoadTexture(const char* texture_file, const int index)
	{
		char* image_data;// TODO: do something to load it from file
		{
			GLint param = GL_CLAMP_TO_EDGE;
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_S,param);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_WRAP_T,param);
		}
		{
			GLint param = GL_CLAMP_TO_EDGE;
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,param);
			if(param == GL_LINEAR_MIPMAP_LINEAR || GL_LINEAR_MIPMAP_NEAREST)
			{
				glGenerateMipmap(GL_TEXTURE_2D);
				param = GL_LINEAR;
			}
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,param);
		}
		delete image_data;
	}
	void SetOrder(const int texture_id, const int order)
	{
		m_TextureOrder[order] = texture_id;
	}
	void LoadShader(const char* vs_file, const char* fs_file)
	{
		// TODO: do something to load shader
		/*GLuint vs_id = esLoadShader(GL_VERTEX_SHADER, vs_file);
		if ( vs_id == 0 )
		{
			return;
		}
		GLuint fs_id = esLoadShader(GL_FRAGMENT_SHADER, fs_file);
		if ( fs_id == 0 )
		{
			glDeleteShader( m_VSId );
			return;
		}
		m_ShaderProgramId = esLoadProgram(vs_id, fs_id);*/
	}
	void BuildMaterial()
	{
		m_ShaderUVId = new GLint[m_TextureCount];
		m_ShaderTextureId = new GLint[m_TextureCount];

		// get shader attribute id, everything here
		m_ShaderXYZId = glGetAttribLocation(m_ShaderProgramId, "position");
		for(int i=0;i<m_TextureCount;i++)
		{
			string i_string(ofToString(i));
			m_ShaderUVId[i] = glGetAttribLocation(m_ShaderProgramId, string("uv_"+i_string).c_str());
			m_ShaderTextureId[i] = glGetUniformLocation(m_ShaderProgramId, string("texture_"+i_string).c_str());
		}
	}
	void FreeMaterial()
	{
		// TODO: do some glDeleteWhatever here
	}
	void Bind()
	{
		glUseProgram(m_ShaderProgramId);
		// vertices
		glEnableVertexAttribArray(m_ShaderXYZId);
		glVertexAttribPointer(m_ShaderXYZId, 3, GL_FLOAT, GL_FALSE, sizeof(ofxVertex), (GLvoid*) offsetof( ofxVertex, X));

		// tex coords
		for(int i=0;i<m_TextureCount;i++)
		{
			int id = m_TextureOrder[i];
			glEnableVertexAttribArray(m_ShaderUVId[id]);
			glVertexAttribPointer(m_ShaderUVId[id], 2, GL_FLOAT, GL_FALSE, sizeof(ofxVertex), (GLvoid*) offsetof( ofxVertex, UV[i*2]));
		}

		// textures
		for(int i=0;i<m_TextureCount;i++)
		{
			int id = m_TextureOrder[i];
			glActiveTexture(GL_TEXTURE0+i);
			glBindTexture(GL_TEXTURE_2D, m_TextureId[id]);
			glUniform1i(m_TextureId[id], i);
			glUniform4fv(m_TextureRectId[id], 1, ptr);
		}
	}
	void Unbind()
	{
		glUseProgram(0);
		for(int i=0;i<m_TextureCount;i++)
		{
			glActiveTexture(GL_TEXTURE0+i);
			glBindTexture(GL_TEXTURE_2D, 0);
		}
	}
	
};