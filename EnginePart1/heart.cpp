#include "heart.h"
#include "graphics.h"
#include "entity.h"

Heart::Heart() :Entity()
{
	collisionType = entityNS::CIRCLE;
	spriteData.width = heartNS::HEART_WIDTH;
	spriteData.height = heartNS::HEART_HEIGHT;
	spriteData.scale = heartNS::HEART_SCALE;
	spriteData.x = 0;
	spriteData.y = 0;
	//spriteData.
};

Heart::~Heart() {
	//destroy the damn player here
}





bool Heart::initialize(Game *gamePtr, int width, int height, int ncols,
	TextureManager *textureM)
{

	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

void Heart::draw() {
	this->graphics->spriteBegin();
	Image::draw();
}

void Heart::update(float frameTime)
{
	Entity::update(frameTime);
}

void Heart::setSpriteDataXnY(float x, float y)
{
	spriteData.x = x;
	spriteData.y = y;
}

void Heart::setHeartNo(int i)
{
	this->heartNo = i;
}

int Heart::getHeartNo()
{
	return this->heartNo;
}

