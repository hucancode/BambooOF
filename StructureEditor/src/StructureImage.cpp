#include "StructureImage.h"
int StructureImage::magnetPower = 10;
bool StructureImage::magnetEnabled = 10;
StructureImage::StructureImage()
{
	opening = true;
	draggedChild = -1;
	width = 0;
	height = 0;
	id = ilGenImage();
}
StructureImage::~StructureImage()
{
	ilDeleteImage(id);
	for (int i = 0; i < children.size(); i++)
	{
		ilDeleteImage(children[i].id);
	}
}
void StructureImage::addChilren(string image_file)
{
	StructurePiece child;
	{
		child.id = ilGenImage();
		ilBindImage(child.id);
		ILboolean loaded = ilLoadImage(image_file.c_str());
		if (loaded == IL_FALSE)
		{
			ILenum error = ilGetError();
			ofLogError() << "DevIL failed to load image " << image_file.c_str() << endl << "error code " << error;
		}
	}
	{
		ILinfo info;
		iluGetImageInfo(&info);
		ILubyte* devil_data;
		devil_data = ilGetData();

		ofPixels data;
		data.allocate(info.Width, info.Height, info.Bpp);
		data.setFromPixels(devil_data, info.Width, info.Height, info.Bpp);
		child.image.setFromPixels(data);
	}
	{
		child.position.x = (width - child.image.width) / 2;
		child.position.y = (height - child.image.height) / 2;
		child.position.x = max(0.0f, child.position.x);
		child.position.y = max(0.0f, child.position.y);
	}
	children.push_back(child);
	verticalMagnets.push_back(child.position.x);
	verticalMagnets.push_back(child.position.x + child.image.width);
	horizontalMagnets.push_back(child.position.y);
	horizontalMagnets.push_back(child.position.y + child.image.height);
	width = max(width, verticalMagnets.back());
	height = max(height, horizontalMagnets.back());
}
void StructureImage::open()
{
	opening = true;
}
bool StructureImage::pick(int x, int y)
{
	if (opening)
	{
		x -= position.x;
		y -= position.y;
		for (int i = 0; i < children.size(); i++)
		{
			if (x < children[i].position.x || x > children[i].position.x + children[i].image.width)
			{
				continue;
			}
			if (y < children[i].position.y || y > children[i].position.y + children[i].image.height)
			{
				continue;
			}
			draggedChild = i;
			dragOffsetX = children[i].position.x - x;
			dragOffsetY = children[i].position.y - y;
			return true;
		}
		return false; 
	}
	else
	{
		if (x < position.x || x > position.x + width)
		{
			return false;
		}
		if (y < position.y || y > position.y + height)
		{
			return false;
		}
		draggedChild = 0;
		dragOffsetX = position.x - x;
		dragOffsetY = position.y - y;
		return true;
	}
}
void StructureImage::drag(int x, int y)
{
	if (draggedChild == -1)
	{
		return;
	}
	if (opening)
	{
		x -= position.x;
		y -= position.y;
		children[draggedChild].position.x = x + dragOffsetX;
		children[draggedChild].position.y = y + dragOffsetY;
		int i = draggedChild * 2;
		int j = i + 1;

		if (magnetEnabled)
		{
			int best_distance;
			best_distance = width;
			for (int k = 0; k < verticalMagnets.size(); k++)
			{
				if (k == i || k == j)
				{
					continue;
				}
				int distance;
				int child_x;
				child_x = children[draggedChild].position.x;
				distance = child_x - verticalMagnets[k];
				if (abs(distance) < magnetPower && abs(distance) < abs(best_distance))
				{
					best_distance = distance;
				}
				child_x = children[draggedChild].position.x + children[draggedChild].image.width;
				distance = child_x - verticalMagnets[k];
				if (abs(distance) < magnetPower && abs(distance) < abs(best_distance))
				{
					best_distance = distance;
				}
			}
			if (abs(best_distance) <= magnetPower)
			{
				children[draggedChild].position.x -= best_distance;
			}
			best_distance = height;
			for (int k = 0; k < horizontalMagnets.size(); k++)
			{
				if (k == i || k == j)
				{
					continue;
				}
				int distance;
				int child_y;
				child_y = children[draggedChild].position.y;
				distance = child_y - horizontalMagnets[k];
				if (abs(distance) < magnetPower && abs(distance) < abs(best_distance))
				{
					best_distance = distance;
				}
				child_y = children[draggedChild].position.y + children[draggedChild].image.height;
				distance = child_y - horizontalMagnets[k];
				if (abs(distance) < magnetPower && abs(distance) < abs(best_distance))
				{
					best_distance = distance;
				}
			}
			if (abs(best_distance) <= magnetPower)
			{
				children[draggedChild].position.y -= best_distance;
			}
		}
		children[draggedChild].position.x = max(0.0f, children[draggedChild].position.x);
		children[draggedChild].position.y = max(0.0f, children[draggedChild].position.y);
		verticalMagnets[i] = children[draggedChild].position.x;
		verticalMagnets[j] = children[draggedChild].position.x + children[draggedChild].image.width;
		horizontalMagnets[i] = children[draggedChild].position.y;
		horizontalMagnets[j] = children[draggedChild].position.y + children[draggedChild].image.height;
		width = max(width, verticalMagnets[j]);
		height = max(height, horizontalMagnets[j]);
	}
	else
	{
		position.x = x + dragOffsetX;
		position.y = y + dragOffsetY;
	}
}
void StructureImage::release()
{
	draggedChild = -1;
	width = 0;
	height = 0;
	for (int i = 0; i < verticalMagnets.size(); i++)
	{
		width = max(width, verticalMagnets[i]);
	}
	for (int i = 0; i < horizontalMagnets.size(); i++)
	{
		height = max(height, horizontalMagnets[i]);
	}
}
void StructureImage::close()
{
	opening = false;
	ilDeleteImage(id);
	id = ilGenImage();
	ilBindImage(id);
	ILboolean ret = ilTexImage(width, height, 1, 4, IL_RGBA, IL_UNSIGNED_BYTE, NULL);
	if (!ret)
	{
		ofLogError() << "can not create image";
		return;
	}
	ilClearColour(0, 0, 0, 0);
	ilClearImage();
	ilDisable(IL_BLIT_BLEND);
	for (int i = 0; i < children.size(); i++)
	{
		ilBlit(
			children[i].id,										// which image
			children[i].position.x, children[i].position.y,		// where to put this image
			0,													// don't care
			0, 0,												// where to start copying the image
			0,													// don't care
			children[i].image.width, children[i].image.height,	// how many pixel to copy
			1);													// don't care
	}
	ilEnable(IL_BLIT_BLEND);
	{
		ILubyte* devil_data;
		devil_data = ilGetData();
		ofPixels data;
		data.allocate(width, height, 4);
		data.setFromPixels(devil_data, width, height, 4);
		image.setFromPixels(data);
		image.mirror(true, false);
	}
}
void StructureImage::draw()
{
	ofPushMatrix();
	ofTranslate(position);
	if (opening)
	{
		vector<StructurePiece>::reverse_iterator it = children.rbegin();
		for (; it != children.rend(); it++)
		{
			StructurePiece item = *it;
			item.image.draw(item.position);
		}
		ofSetHexColor(0x0077FF);
		for (int i = 0; i < verticalMagnets.size(); i++)
		{
			if (i / 2 == draggedChild)
			{
				continue;
			}
			if (i > 0)
			{
				if (verticalMagnets[i - 1] == verticalMagnets[i])
				{
					continue;
				}
			}
			ofLine(verticalMagnets[i], 0, verticalMagnets[i], height);
		}
		for (int i = 0; i < horizontalMagnets.size(); i++)
		{
			if (i / 2 == draggedChild)
			{
				continue;
			}
			if (i > 0)
			{
				if (horizontalMagnets[i - 1] == horizontalMagnets[i])
				{
					continue;
				}
			}
			ofLine(0, horizontalMagnets[i], width, horizontalMagnets[i]);
		}
		ofSetHexColor(0xFFFF77);
		if (draggedChild != -1)
		{
			ofVec2f a, b, c, d;
			a = children[draggedChild].position;
			b = a + ofVec2f(children[draggedChild].image.width, 0);
			c = b + ofVec2f(0, children[draggedChild].image.height);
			d = c - ofVec2f(children[draggedChild].image.width, 0);
			ofLine(a, b);
			ofLine(b, c);
			ofLine(c, d);
			ofLine(d, a);
		}
		ofSetHexColor(0xFFFFFF);
	}
	else
	{
		image.draw(0, 0);
		if (draggedChild != -1)
		{
			ofVec2f a, b, c, d;
			a = ofVec2f(0, 0);
			b = a + ofVec2f(image.width, 0);
			c = b + ofVec2f(0, image.height);
			d = c - ofVec2f(image.width, 0);
			ofLine(a, b);
			ofLine(b, c);
			ofLine(c, d);
			ofLine(d, a);
		}
	}
	ofPopMatrix();
}
bool StructureImage::isOpening()
{
	return opening;
}