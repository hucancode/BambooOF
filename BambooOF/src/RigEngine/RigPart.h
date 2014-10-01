#pragma once
#include "ofMain.h"
#include "StateCollectionJX.h"

struct RigPart
{
	string				name;
	RIG_ENGINE_PART		part;
	bool				gender;
	unsigned char		state;
};

string getTexturePath(RigPart part)
{
	return "";
}