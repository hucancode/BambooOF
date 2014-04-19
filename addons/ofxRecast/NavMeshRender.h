#pragma once
class NavMeshRender
{
private:
	NavMesh* m_NavMesh;
	float m_CamX, m_CamY, m_CamZ;
	float m_RotateX, m_RotateY;
public:
	NavMeshRender();
	~NavMeshRender();
	bool RayCast(float* screen_pos, float* hit_pos);
	void Render();
};