#pragma once
#include "ofMain.h"
#define TEXTURE_MAX_SIZE 4096
struct Node
{
	int x, y, w, h;
	bool rotated;
	bool open_w;
	bool open_h;
	Node()
	{
		rotated = false;
		open_w = false;
		open_h = false;
	}
	void rotate()
	{
		swap(w,h);
		swap(open_w, open_h);
		rotated = !rotated;
	}
};
class RectanglePacker
{
private:
	bool m_AutoGrow;
	int m_CurrentWidth;
	int m_CurrentHeight;
	vector<Node*> m_FreeRectangle;
	vector<Node*> m_UsedRectangle;
	int m_FreeRectangleSizeCap;
public:
	RectanglePacker();
	~RectanglePacker();
	void SetAutoGrow(bool value);
	bool Grow();
	bool Insert(Node* node);
public:
	void SetRectangleSizeCap(int value);
	void CleanOverlapData();
	Node* QueryUsedRectangle(int i);
	int GetNumUsedRectangle();
	Node* QueryFreeRectangle(int i);
	int GetNumFreeRectangle();
	int GetCurrentHeight();
	int GetCurrentWidth();
	bool CheckOverlap();
private:
	void Place(Node* node, int free_space_index);
	int FindNearestNodeFit(Node* node);
	static bool Covered(ofRectangle parent, ofRectangle child);
};