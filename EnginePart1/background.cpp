#include "background.h"
#pragma once
#include "textureManager.h"
#include "graphics.h"
using namespace std;



//initialize the damn bullet immediately
// pass by pointer here using pointer as we want to use the original graphics addr
// pass by address here for the filepath as the file is an adrress xd

Background::Background()
{
	collisionType = entityNS::CIRCLE;
	spriteData.width = backgroundNS::LEVEL1_TILE_WIDTH;
	spriteData.height = backgroundNS::LEVEL1_TILE_HEIGHT;
	spriteData.scale = backgroundNS::LEVEL1_TILE_SCALE;
	/*spriteData.x = backgroundNS::LEVEL1_TILE_WIDTH;
	spriteData.y = backgroundNS::LEVEL1_TILE_HEIGHT;*/
	spriteData.x = 0;
	spriteData.y = 0;
};

Background::~Background(){


}

bool Background::initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM)
{
	this->setFrames(0,0);
	this->setCurrentFrame(0);
	this->setFrameDelay(0);
	//this->setLoop(true);    
	bool test = Entity::initialize(gamePtr, width, height, ncols, textureM);
		// do not loop animation
	return(test);

}

void Background::draw() {
	this->graphics->spriteBegin();
	Image::draw();
}


