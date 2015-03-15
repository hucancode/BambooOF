#include "Structure.h"
#include "pugixml.hpp"
Structure::Structure()
{
	collision = new StructureCollision();
	image = new StructureImage();
	for (int i = 1; i <= 13; i++)
	{
		image->addChilren("data/2547-" + ofToString(i) + ".png");
	}
}
Structure::~Structure()
{

}
bool Structure::pick(int x, int y)
{
	bool swallow;
	swallow = image->pick(x, y);
	if (swallow)
	{
		return true;
	}
	swallow = collision->pick(x, y);
	if (swallow)
	{
		return true;
	}
	return false;
}
void Structure::drag(int x, int y)
{
	image->drag(x, y);
	collision->drag(x, y);
}
void Structure::release()
{
	image->release();
	collision->release();
}
void Structure::draw()
{
	if (image->isOpening())
	{
		collision->draw();
		image->draw();
	}
	else
	{
		image->draw();
		collision->draw();
	}
}
void Structure::import(string file_name)
{

}
void Structure::export(string file_name)
{
	// init xml...
	pugi::xml_document doc;
	pugi::xml_node structure_node = doc.append_child("structure");
	pugi::xml_node pieces_node = structure_node.append_child("pieces");
	pieces_node.append_attribute("count") = ofToString(collision->anchors.size() + 1).c_str();
	ILuint left;
	ILuint right;
	{
		right = ilGenImage();
		ilBindImage(right);
		ILboolean ret = ilCopyImage(image->id);
		if (!ret)
		{
			ofLogError() << "can not copy image";
			return;
		}
	}
	int x = 0;
	for (int i = 0; i < collision->anchors.size(); i++)
	{
		string piece_image = file_name + ofToString(i) + ".png";
		int ax = collision->anchors[i].x - image->position.x;
		ax = max(0, min(ax, image->width));
		int ay = collision->anchors[i].y - image->position.y;
		int lwidth;
		int lheight;
		{// build & save left image
			lwidth = ax - x;
			lheight = image->height;
			left = ilGenImage();
			ilBindImage(left);
			ILboolean ret = ilTexImage(lwidth, lheight, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, NULL);
			if (!ret)
			{
				ofLogError() << "can not create image";
				return;
			}
			ilClearColour(0, 0, 0, 0);
			ilClearImage();
			ilDisable(IL_BLIT_BLEND);
			ilBlit(
				right,										// which image
				0, 0,										// where to put this image
				0,											// don't care
				x, 0,										// where to start copying the image
				0,											// don't care
				lwidth, lheight,							// how many pixel to copy
				1);											// don't care
			ilEnable(IL_BLIT_BLEND);
			ilSaveImage(piece_image.c_str());
			x = ax;
			ilDeleteImage(left);
		}
		{// update right image
			left = ilGenImage();
			ilBindImage(left);
			lwidth = image->width - x;
			lheight = image->height;
			ILboolean ret = ilTexImage(lwidth, lheight, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, NULL);
			if (!ret)
			{
				ofLogError() << "can not create image";
				return;
			}
			ilClearColour(0, 0, 0, 0);
			ilClearImage();
			ilDisable(IL_BLIT_BLEND);
			ilBlit(
				right,										// which image
				0, 0,										// where to put this image
				0,											// don't care
				x, 0,										// where to start copying the image
				0,											// don't care
				lwidth, lheight,							// how many pixel to copy
				1);											// don't care
			ilEnable(IL_BLIT_BLEND);
			swap(left, right);
			ilDeleteImage(left);
		}
		{// update xml
			pugi::xml_node piece_node = pieces_node.append_child("piece");
			piece_node.append_attribute("image") = piece_image.c_str();
			piece_node.append_attribute("anchor") = ofToString(ay).c_str();
		}
	}
	{
		ilBindImage(right);
		ilSaveImage((file_name + ofToString(collision->anchors.size()) + ".png").c_str());
		ilDeleteImage(right);
	}
	{
		pugi::xml_node piece_node = pieces_node.append_child("piece");
		piece_node.append_attribute("image") = (file_name + ofToString(collision->anchors.size()) + ".png").c_str();
		piece_node.append_attribute("anchor") = ofToString(collision->pivotRight->y).c_str();
	}
	{// many properties come here

	}
	{// finish saving xml
		doc.save_file((file_name + ".xml").c_str());
	}
}