#include "JxStructure.h"
JxStructure::JxStructure()
{
}
JxStructure::~JxStructure()
{
}
bool JxStructure::Load(string xml_file)
{
}
void JxStructure::BuildModel()
{
	{// projecting
		vector<ofVec2f>::iterator it = m_BasePolygon.begin();
		for (; it != m_BasePolygon.end(); it++)
		{
			ofVec2f point = *it;
			ofVec3f projection;
			projection.x = point.x;
			projection.y = 0;
			projection.z = point.y / cos(PI*0.125);
			m_Model.push_back(projection);
		}
	}
	{// extending
		vector<ofVec3f>::iterator it = m_Model.begin();
		for (; it != m_Model.end(); it++)
		{
			ofVec3f point = *it;
			ofVec3f extend;
			extend.x = point.x;
			extend.y = m_Height;
			extend.z = point.z;
			m_Model.push_back(extend);
		}
	}
}