#include"ofxShaderProgram.h"

class JxShaderProgram
	:public ofxShaderProgram
{
public:
	JxShaderProgram();
	virtual ~JxShaderProgram();
	virtual void		BuildReferenceMap();
	virtual void		BindReferenceMap();
	void				SetTextureSlot(GLuint slot);
};