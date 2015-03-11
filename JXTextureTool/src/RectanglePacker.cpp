#include "RectanglePacker.h"
RectanglePacker::RectanglePacker()
{
	m_FreeRectangleSizeCap = 1;
	m_CurrentWidth = 32;
	m_CurrentHeight = 32;
	m_AutoGrow = true;
	Node* initial_node = new Node();
	initial_node->rotated = false;
	initial_node->open_w = true;
	initial_node->open_h = true;
	initial_node->x = 0;
	initial_node->y = 0;
	initial_node->w = m_CurrentWidth;
	initial_node->h = m_CurrentHeight;
	m_FreeRectangle.push_back(initial_node);
}
RectanglePacker::~RectanglePacker()
{
}
bool RectanglePacker::Covered(ofRectangle parent, ofRectangle child)
{
	bool min_inside = child.getMinX() >= parent.getMinX() && child.getMinY() >= parent.getMinY() && 
           child.getMinX() <= parent.getMaxX() && child.getMinY() <= parent.getMaxY();
	bool max_inside = child.getMaxX() >= parent.getMinX() && child.getMaxY() >= parent.getMinY() && 
           child.getMaxX() <= parent.getMaxX() && child.getMaxY() <= parent.getMaxY();
	return min_inside && max_inside;
}
void RectanglePacker::CleanOverlapData()
{
	for(int i=0;i<m_FreeRectangle.size();i++)
	{
		if((m_FreeRectangle[i]->w < m_FreeRectangleSizeCap && !m_FreeRectangle[i]->open_w) || 
			(m_FreeRectangle[i]->h < m_FreeRectangleSizeCap && !m_FreeRectangle[i]->open_h))
		{
			m_FreeRectangle.erase(m_FreeRectangle.begin() + i);
			i--;
		}
	}
	int n = m_FreeRectangle.size();
	for(int i=0;i<n-1;i++)
	{
		if(!m_FreeRectangle[i]->open_h && !m_FreeRectangle[i]->open_w) continue;
		ofRectangle rect_i(m_FreeRectangle[i]->x, m_FreeRectangle[i]->y, m_FreeRectangle[i]->w, m_FreeRectangle[i]->h);
		for(int j=i+1;j<n;j++)
		{
			if(!m_FreeRectangle[j]->open_h && !m_FreeRectangle[j]->open_w) continue;
			ofRectangle rect_j(m_FreeRectangle[j]->x, m_FreeRectangle[j]->y, m_FreeRectangle[j]->w, m_FreeRectangle[j]->h);
			if(Covered(rect_i,rect_j))
			{
				m_FreeRectangle.erase(m_FreeRectangle.begin()+j);
				j--;
				n--;
			}
			else if(Covered(rect_j,rect_i))
			{
				m_FreeRectangle.erase(m_FreeRectangle.begin()+i);
				i--;
				n--;
				break;
			}
		}
	}
}
void RectanglePacker::SetRectangleSizeCap(int value)
{
	m_FreeRectangleSizeCap = value;
	CleanOverlapData();
}
bool RectanglePacker::Grow()
{
	if(m_CurrentWidth >= TEXTURE_MAX_SIZE) return false;
	for(int i=0;i<m_FreeRectangle.size();i++)
	{
		if((m_FreeRectangle[i]->w < m_FreeRectangleSizeCap && !m_FreeRectangle[i]->open_w) || 
			(m_FreeRectangle[i]->h < m_FreeRectangleSizeCap && !m_FreeRectangle[i]->open_h))
		{
			m_FreeRectangle.erase(m_FreeRectangle.begin() + i);
			i--;
		}
	}
	for(int i=0;i<m_FreeRectangle.size();i++)
	{
		if(m_FreeRectangle[i]->open_w)
		{
			m_FreeRectangle[i]->w += m_CurrentWidth;
		}
		if(m_FreeRectangle[i]->open_h)
		{
			m_FreeRectangle[i]->h += m_CurrentHeight;
		}
	}
	Node* vertical_node = new Node();
	vertical_node->open_w = true;
	vertical_node->open_h = true;
	vertical_node->x = m_CurrentWidth;
	vertical_node->y= 0;
	vertical_node->w = m_CurrentWidth;
	vertical_node->h = m_CurrentHeight*2;
	Node* horizontal_node = new Node();
	horizontal_node->open_w = true;
	horizontal_node->open_h = true;
	m_FreeRectangle.push_back(vertical_node);
	horizontal_node->x = 0;
	horizontal_node->y= m_CurrentHeight;
	horizontal_node->w = m_CurrentWidth*2;
	horizontal_node->h = m_CurrentHeight;
	m_FreeRectangle.push_back(horizontal_node);
	m_CurrentWidth *= 2;
	m_CurrentHeight *= 2;
	return true;
}
bool RectanglePacker::Insert(Node* node)
{
	int best_fit = FindNearestNodeFit(node);
	while(m_AutoGrow && best_fit == -1)
	{
		if(Grow())
		{
			best_fit = FindNearestNodeFit(node);
		}
		else
		{
			break;
		}
	}
	if(best_fit != -1)
	{
		Place(node, best_fit);
		CleanOverlapData();
		CheckOverlap();
		ofLogNotice() << "rectangle packer, free rectangle to process = "<<m_FreeRectangle.size();
		return true;
	}
	else
	{
		return false;
	}
}
void RectanglePacker::SetAutoGrow(bool value)
{
	m_AutoGrow = value;
}
Node* RectanglePacker::QueryUsedRectangle(int i)
{
	return m_UsedRectangle[i];
}
int RectanglePacker::GetNumUsedRectangle()
{
	return m_UsedRectangle.size();
}
Node* RectanglePacker::QueryFreeRectangle(int i)
{
	return m_FreeRectangle[i];
}
int RectanglePacker::GetNumFreeRectangle()
{
	return m_FreeRectangle.size();
}
int RectanglePacker::GetCurrentHeight()
{
	return m_CurrentHeight;
}
int RectanglePacker::GetCurrentWidth()
{
	return m_CurrentWidth;
}
void RectanglePacker::Place(Node* node, int free_space_index)
{
	Node* free_space = m_FreeRectangle[free_space_index];
	if(free_space->rotated)
	{
		free_space->rotate();
		node->rotate();
	}

	int size = m_FreeRectangle.size();
	node->x = free_space->x;
	node->y = free_space->y;
	
	for(int i=0;i<size;i++)
	{
		Node* space = m_FreeRectangle[i];
		ofRectangle node_rect(node->x, node->y, node->w, node->h);
		ofRectangle space_rect(space->x, space->y, space->w, space->h);
		if(!space_rect.intersects(node_rect)) continue;
		
		// New node at the top side of the used node.
		if(node_rect.getMinY() > space_rect.getMinY())
		{
			Node* new_node = new Node();
			new_node->x = space_rect.getX();
			new_node->y = space_rect.getY();
			new_node->w = space_rect.getWidth();
			new_node->h = node_rect.getMinY() - space_rect.getMinY();
			new_node->open_w = new_node->x + new_node->w == m_CurrentWidth;
			new_node->open_h = new_node->y + new_node->h == m_CurrentHeight;
			if((new_node->w >= m_FreeRectangleSizeCap || new_node->open_w) && 
				(new_node->h >= m_FreeRectangleSizeCap || new_node->open_h))
			{
				m_FreeRectangle.push_back(new_node);
			}
			
		}
		// New node at the bottom side of the used node.
		if(node_rect.getMaxY() < space_rect.getMaxY())
		{
			Node* new_node = new Node();
			new_node->x = space_rect.getX();
			new_node->y = node_rect.getMaxY();
			new_node->w = space_rect.getWidth();
			new_node->h = space_rect.getMaxY() - node_rect.getMaxY();
			new_node->open_w = new_node->x + new_node->w == m_CurrentWidth;
			new_node->open_h = new_node->y + new_node->h == m_CurrentHeight;
			if((new_node->w >= m_FreeRectangleSizeCap || new_node->open_w) && 
				(new_node->h >= m_FreeRectangleSizeCap || new_node->open_h))
			{
				m_FreeRectangle.push_back(new_node);
			}
		}
		// New node at the left side of the used node.
		if(node_rect.getMinX() > space_rect.getMinX())
		{
			Node* new_node = new Node();
			new_node->x = space_rect.getX();
			new_node->y = space_rect.getY();
			new_node->w = node_rect.getMinX() - space_rect.getMinX();
			new_node->h = space_rect.getHeight();
			new_node->open_w = new_node->x + new_node->w == m_CurrentWidth;
			new_node->open_h = new_node->y + new_node->h == m_CurrentHeight;
			if((new_node->w >= m_FreeRectangleSizeCap || new_node->open_w) && 
				(new_node->h >= m_FreeRectangleSizeCap || new_node->open_h))
			{
				m_FreeRectangle.push_back(new_node);
			}
		}
		// New node at the right side of the used node.
		if(node_rect.getMaxX() < space_rect.getMaxX())
		{
			Node* new_node = new Node();
			new_node->x = node_rect.getMaxX();
			new_node->y = space_rect.getY();
			new_node->w = space_rect.getMaxX() - node_rect.getMaxX();
			new_node->h = space_rect.getHeight();
			new_node->open_w = new_node->x + new_node->w == m_CurrentWidth;
			new_node->open_h = new_node->y + new_node->h == m_CurrentHeight;
			if((new_node->w >= m_FreeRectangleSizeCap || new_node->open_w) && 
				(new_node->h >= m_FreeRectangleSizeCap || new_node->open_h))
			{
				m_FreeRectangle.push_back(new_node);
			}
		}
		m_FreeRectangle.erase(m_FreeRectangle.begin() + i);
		i--;
		size--;
		delete space;
		
	}
	m_UsedRectangle.push_back(node);
}
bool RectanglePacker::CheckOverlap()
{
	return false;
	for(int i=0;i<m_FreeRectangle.size();i++)
	{
		for(int j=0;j<m_UsedRectangle.size();j++)
		{
			ofRectangle free(m_FreeRectangle[i]->x, m_FreeRectangle[i]->y, m_FreeRectangle[i]->w, m_FreeRectangle[i]->h);
			ofRectangle used(m_UsedRectangle[j]->x, m_UsedRectangle[j]->y, m_UsedRectangle[j]->w, m_UsedRectangle[j]->h);
			if(free.intersects(used))
				return true;
		}
	}

	return false;
}
int RectanglePacker::FindNearestNodeFit(Node* node)
{
	int best_index = -1;
	int best_near_distance = m_CurrentWidth + 1;
	int best_far_distance = best_near_distance;
	for(int i=0;i<m_FreeRectangle.size();i++)
	{
		if(m_FreeRectangle[i]->w >= node->w && m_FreeRectangle[i]->h >= node->h)
		{
			int dx = m_FreeRectangle[i]->x + node->w;
			int dy = m_FreeRectangle[i]->y + node->h;
			int far_distance = max(dx, dy);
			int near_distance = min(dx, dy);
			if(far_distance < best_far_distance)
			{
				best_index = i;
				best_far_distance = far_distance;
				best_near_distance = near_distance;
			}
			else if(far_distance == best_far_distance && near_distance < best_near_distance)
			{
				best_index = i;
				best_far_distance = far_distance;
				best_near_distance = near_distance;
			}
		}
		if(m_FreeRectangle[i]->w >= node->h && m_FreeRectangle[i]->h >= node->w)
		{
			int dx = m_FreeRectangle[i]->x + node->h;
			int dy = m_FreeRectangle[i]->y + node->w;
			int far_distance = max(dx, dy);
			int near_distance = min(dx, dy);
			if(far_distance < best_far_distance)
			{
				best_index = i;
				best_far_distance = far_distance;
				best_near_distance = near_distance;
				node->rotate();
			}
			else if(far_distance == best_far_distance && near_distance < best_near_distance)
			{
				best_index = i;
				best_far_distance = far_distance;
				best_near_distance = near_distance;
				node->rotate();
			}
		}
	}
	return best_index;
}