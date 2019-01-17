#pragma once
#ifndef _BACKGROUND_H             // prevent multiple definitions if this 
#define _BACKGROUND_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#include "image.h"
#include "entity.h"
#include <math.h>

namespace backgroundNS
{
	const int LEVEL1_TILE_HEIGHT = 1024;
	const int LEVEL1_TILE_WIDTH = 1024;
	const float LEVEL1_TILE_SCALE = 2.0;


}
class Background : Entity {
private:
	//textureManager BULLET_TEXTURE;

	Image backgroundImage;

public:

	//constructor
	Background();

	//de-stuctor
	virtual ~Background();

	virtual bool initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM);


	//void setPositionVector(float x, float y) {
	//	//pls change this in the future
	//	this->setX(x + 10);
	//	spriteData.x = this->getX();
	//	this->setY(y);
	//	spriteData.y = this->getY();
	//}

	void Background::draw();

	void Background::update(float frameTime)
	{
		Entity::update(frameTime);
	}

};




#endif
