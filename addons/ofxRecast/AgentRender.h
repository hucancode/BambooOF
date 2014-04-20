#pragma once
#include "Agent.h"
class AgentRender
{
private:
	Agent* m_Agent;
public:
	AgentRender();
	~AgentRender();
	void Render();
};