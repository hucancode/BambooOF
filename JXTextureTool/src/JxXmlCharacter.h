#pragma once
#include "ofMain.h"
#include "FreeImage.h"
#include "RectanglePacker.h"
struct JxFrame : Node
{
	unsigned int ox, oy;
	FIBITMAP* image_data;
	int texture_slot;
};
struct JxSequence
{
	vector<JxFrame*> frames;
	char sequence_id;
	string sequence_name;
};
class JxXmlCharacter
{
private:
	string m_Category;
	string m_ItemName;
	vector<JxSequence*> m_Sequences;
	vector<JxFrame*> m_FramesToPack;
	vector<bool> m_Packed;
	vector<RectanglePacker*> m_Packers;
	RectanglePacker* m_CurrentPacker;
	int m_CurrentIndex;
	int m_CurrentArea;
	int m_TotalArea;
	int m_TotalAreaLeft;
	bool m_Finished;
public:
	JxXmlCharacter();
	~JxXmlCharacter();
	void LoadOldXml(string directory, string item);
	void LoadXml(string directory, string item);
	bool Update();
	void Begin();
	void End(string directory);
public:
	RectanglePacker* GetCurrentPacker();
	int GetCurrentPackerUsed();
	float GetPercentComplete();
private:
	void CorrectSequenceName();
	void GenerateSequenceCode();
};