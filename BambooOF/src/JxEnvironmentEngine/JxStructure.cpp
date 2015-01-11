#include "JxStructure.h"
#include <assert.h>
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
	//assert((m_BasePolygon.size() >= 3);
	m_ModelVertices.reserve((m_BasePolygon.size()*2 - 3)*2);
	int begin = 0;
	int mid;
	int end;
	{// projecting
		vector<ofxBasicVertex>::iterator it = m_BasePolygon.begin();
		for (; it != m_BasePolygon.end(); it++)
		{
			ofxBasicVertex point = *it;
			ofxBasicVertex projection;
			projection.x = point.x;
			projection.y = 0;
			projection.z = point.y / cos(PI*0.125);
			m_ModelVertices.push_back(projection);
		}
		mid = m_ModelVertices.size();
		for (; it != m_BasePolygon.end(); it++)
		{
			ofxBasicVertex point = *it;
			ofxBasicVertex projection;
			projection.x = point.x;
			projection.y = m_Height;
			projection.z = point.y / cos(PI*0.125);
			m_ModelVertices.push_back(projection);
		}
		end = m_ModelVertices.size();
	}
	{// bottom
		int size = mid;
		int a = begin;
		int b = a+1;
		int c = b+1;
		for (; c < size;c++)
		{
			m_ModelIndices.push_back(a);
			m_ModelIndices.push_back(b);
			m_ModelIndices.push_back(c);
			b = c;
		}
	}
	{// top
		int size = end;
		int a = mid;
		int b = a+1;
		int c = b+1;
		for (; c < size;c++)
		{
			m_ModelIndices.push_back(a);
			m_ModelIndices.push_back(b);
			m_ModelIndices.push_back(c);
			b = c;
		}
	}
	{// sides
		int size = mid;
		int a = begin;
		int b = a+1;
		for (; b < size;b++)
		{
			int c = b + mid;
			int d = a + mid;
			m_ModelIndices.push_back(a);
			m_ModelIndices.push_back(b);
			m_ModelIndices.push_back(c);
			m_ModelIndices.push_back(a);
			m_ModelIndices.push_back(c);
			m_ModelIndices.push_back(d);
			a = b;
		}
	}
}