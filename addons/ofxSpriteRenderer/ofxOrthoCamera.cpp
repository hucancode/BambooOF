#include "ofxOrthoCamera.h"

/* =================================================================
ofxOrthoCamera
- this class inherrit from ofEasyCamera, and used to do orthographic projection
- code implementation isn't optimized
================================================================= */
ofxOrthoCamera::ofxOrthoCamera()
{
	isOrtho = true;
	SetScale(1);
}
ofxOrthoCamera::~ofxOrthoCamera()
{
}
void ofxOrthoCamera::begin(ofRectangle viewport)
{
	if(this->viewport != viewport)
	{
		this->viewport = viewport;
		SetScale(m_Scale);
	}
	if(!isActive)
	{
		ofPushView();
	}
	isActive = true;
	ofSetOrientation(ofGetOrientation(),vFlip);

	ofSetMatrixMode(OF_MATRIX_PROJECTION);
	ofLoadMatrix(getProjectionMatrix(viewport));

	ofSetMatrixMode(OF_MATRIX_MODELVIEW);
	ofLoadMatrix(getModelViewMatrix());
}
ofMatrix4x4 ofxOrthoCamera::getProjectionMatrix(ofRectangle viewport) const
{
	ofMatrix4x4 ortho;
	ortho.makeOrthoMatrix(-m_ScaleX, m_ScaleX, -m_ScaleY, m_ScaleY, -2000 * m_Scale, 2800 * m_Scale );
	return ortho;
}
ofVec3f ofxOrthoCamera::OrthoScreenToWorld(ofVec3f ScreenXYZ, ofRectangle viewport)
{
	//convert from screen to camera
	ofVec3f CameraXYZ;
	CameraXYZ.x = 2.0f * (ScreenXYZ.x - viewport.x) / viewport.width - 1.0f;
	CameraXYZ.y = 1.0f - 2.0f *(ScreenXYZ.y - viewport.y) / viewport.height;
	CameraXYZ.z = ScreenXYZ.z;

	//get inverse camera matrix
	ofMatrix4x4 inverseCamera;
	inverseCamera.makeInvertOf(getModelViewMatrix() * getProjectionMatrix(viewport));

	//convert camera to world
	return CameraXYZ * inverseCamera;
}
void ofxOrthoCamera::SetScale(float scale)
{
	m_Scale = scale;
	float ar = this->viewport.width / this->viewport.height;
	if(ar > 1.0f){
		//wide
		m_ScaleX = m_Scale * ar;
		m_ScaleY = m_Scale;
	}
	else{
		//tall
		m_ScaleX = m_Scale;
		m_ScaleY = ar==0?0:(m_Scale / ar);
	}
}
float ofxOrthoCamera::GetScale()
{
	return m_Scale;
}