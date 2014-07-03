#include "ofxOrthoCamera.h"

/* =================================================================
ofxOrthoCamera
- this class inherrit from ofEasyCamera, and used to do orthographic projection
- code implementation isn't optimized
================================================================= */
ofxOrthoCamera::ofxOrthoCamera()
{
	isOrtho = true;
	m_ProjectionUpdated = false;
	m_ModelViewProjectionnUpdated = false;
	m_InverseCameraUpdated = false;
	viewport = ofRectangle(0,0,0,0);
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
	ofLoadMatrix(GetProjectionMatrix());

	ofSetMatrixMode(OF_MATRIX_MODELVIEW);
	ofLoadMatrix(GetModelViewMatrix());
}
ofMatrix4x4 ofxOrthoCamera::GetProjectionMatrix()
{
	if(!m_ProjectionUpdated)
	{
		m_ProjectionMatrix.makeOrthoMatrix(-m_ScaleX, m_ScaleX, -m_ScaleY, m_ScaleY, -20000, 20000);
		m_ProjectionUpdated = true;
	}
	return m_ProjectionMatrix;
}
bool ofxOrthoCamera::ModelViewUpdated()
{
	bool ret = m_LocalTransformMatrix._mat[0] == getLocalTransformMatrix()._mat[0] && 
		m_LocalTransformMatrix._mat[1] == getLocalTransformMatrix()._mat[1] &&
		m_LocalTransformMatrix._mat[2] == getLocalTransformMatrix()._mat[2] &&
		m_LocalTransformMatrix._mat[3] == getLocalTransformMatrix()._mat[3];
	if(!ret)
	{
		m_ModelViewProjectionnUpdated = false;
		m_InverseCameraUpdated = false;
	}
	return ret;
}
ofMatrix4x4 ofxOrthoCamera::GetModelViewMatrix()
{
	if(!ModelViewUpdated())
	{
		m_LocalTransformMatrix = getLocalTransformMatrix();
		m_ModelViewMatrix = ofMatrix4x4::getInverseOf(m_LocalTransformMatrix);
		m_ModelViewProjectionnUpdated = false;
		m_InverseCameraUpdated = false;
	}
	return m_ModelViewMatrix;
}
ofMatrix4x4 ofxOrthoCamera::GetModelViewProjectionMatrix()
{
	if(!(ModelViewUpdated() && m_ProjectionUpdated && m_ModelViewProjectionnUpdated))
	{
		m_ModelViewProjectionMatrix = GetModelViewMatrix() * GetProjectionMatrix();
		m_InverseCameraUpdated = false;
	}
	m_ModelViewProjectionnUpdated = true;
	return m_ModelViewProjectionMatrix;
}
ofMatrix4x4 ofxOrthoCamera::GetInverseCameraMatrix()
{
	if(!(ModelViewUpdated() && m_ProjectionUpdated && m_ModelViewProjectionnUpdated && m_InverseCameraUpdated))
	{
		m_InverseCameraMatrix = ofMatrix4x4::getInverseOf(GetModelViewProjectionMatrix());
	}
	m_InverseCameraUpdated = true;
	return m_InverseCameraMatrix;
}
ofVec3f ofxOrthoCamera::OrthoScreenToWorld(ofVec3f ScreenXYZ, ofRectangle viewport)
{
	//convert from screen to camera
	ofVec3f CameraXYZ;
	CameraXYZ.x = 2.0f * (ScreenXYZ.x - viewport.x) / viewport.width - 1.0f;
	CameraXYZ.y = 1.0f - 2.0f *(ScreenXYZ.y - viewport.y) / viewport.height;
	CameraXYZ.z = ScreenXYZ.z;

	//convert camera to world
	return CameraXYZ * GetInverseCameraMatrix();
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
	m_ProjectionUpdated = false;
}
float ofxOrthoCamera::GetScale()
{
	return m_Scale;
}