#include "Agent.h"

Agent::Agent(NavMesh* mesh)
{
	m_AgentID = -1;
	m_NavMesh = mesh;
	m_Moving = false;
	m_MovingRequested = false;
	m_Angle = 0;
}
Agent::Agent(NavMesh* mesh, int id)
{
	m_AgentID = -1;
	m_NavMesh = mesh;
	m_Moving = false;
	SetID(id);
}
Agent::~Agent()
{
}
void Agent::SetID(int id)
{
	m_AgentID = id;
	m_CrowdAgent = m_NavMesh->Crowd()->getAgent(m_AgentID);
	SyncPosition();
	SyncVelocity();
}
void Agent::SyncVelocity()
{
	if (!m_CrowdAgent || !m_CrowdAgent->active) return;
	if(m_Velocity[0] == m_CrowdAgent->vel[0]) 
		if(m_Velocity[1] == m_CrowdAgent->vel[1])
			if(m_Velocity[2] == m_CrowdAgent->vel[2])
				return;
	m_Velocity[0] = m_CrowdAgent->vel[0];
	m_Velocity[1] = m_CrowdAgent->vel[1];
	m_Velocity[2] = m_CrowdAgent->vel[2];
	m_Angle = 0;// calculate
	m_Moving = m_Velocity[0] || m_Velocity[1] || m_Velocity[2];
}
void Agent::SyncPosition()
{
	if (!m_CrowdAgent || !m_CrowdAgent->active) return;
	m_Position[0] = m_CrowdAgent->npos[0];
	m_Position[1] = m_CrowdAgent->npos[1];
	m_Position[2] = m_CrowdAgent->npos[2];
}
void Agent::Move(const float* p)
{
	static float target_pos[3];
	static dtPolyRef target_ref;
	const dtQueryFilter* filter = m_NavMesh->Crowd()->getFilter();
	const float* ext = m_NavMesh->Crowd()->getQueryExtents();
	m_NavMesh->Query()->findNearestPoly(p, ext, filter, &target_ref, target_pos);
	if (m_CrowdAgent && m_CrowdAgent->active)
		m_NavMesh->Crowd()->requestMoveTarget(m_AgentID, target_ref, target_pos);
}
void Agent::Force(const float* p)
{
	float vel[3];
	if (m_CrowdAgent && m_CrowdAgent->active)
	{
		// calculate velocity
		dtVsub(vel, p, m_CrowdAgent->npos);
		vel[1] = 0.0;
		dtVnormalize(vel);
		dtVscale(vel, vel, m_CrowdAgent->params.maxSpeed);
		m_NavMesh->Crowd()->requestMoveVelocity(m_AgentID, vel);
	}
}
void Agent::Stop()
{
	m_MovingRequested = m_Moving = false;
}