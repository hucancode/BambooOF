#pragma once
#include <string.h>
#include "RecastHelper.h"
#include "NavMesh.h"
class Agent
{
protected:
	const dtCrowdAgent* m_CrowdAgent;
	NavMesh* m_NavMesh;
	bool m_Moving;// not sure agent want to move or being pushed
	bool m_MovingRequested;// agent want to move
	float m_Angle;
	int m_AgentID;
	float m_Velocity[3];
	float m_Position[3];
public:
	Agent(NavMesh* mesh);
	Agent(NavMesh* mesh, int id);
	~Agent();
	void SetID(int id);
	void SyncVelocity();
	void SyncPosition();
	void Move(const float* p);
	void Force(const float* p);
	void Stop();
};