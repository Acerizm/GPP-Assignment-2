
#include "Obstacle.h"
#include <string>

Obstacle::Obstacle() {
	collisionType = entityNS::ROTATED_BOX;
	spriteData.width = 0;
	spriteData.height = 0;
	spriteData.scale = 0;
	spriteData.x = 0;
	spriteData.y = 0;
	setCollisionRadius(100.0f);
}

Obstacle::~Obstacle() {

}

bool Obstacle::initialize(Game *gamePtr, TextureManager *textureM,float positionX, float positionY, int ObstacleType)
{
	//later will overide the width,height and the number of columns the texture/spritesheet has
	int width = 0;
	int height = 0;
	int textureColumns = 0;
	//compare throught the numbers allocated for each Obstacle
	// Example: 
	// 1 is for Obstacle1
	// 2 is for Obstacle2
	// 3 is tbc
	switch (ObstacleType) 
	{
	case (1):
		//this is for barrel
		width = obstacleNS::OBS1_WIDTH;
		height = obstacleNS::OBS1_HEIGHT;
		textureColumns = obstacleNS::OBS1_COLS;
		spriteData.width = obstacleNS::OBS1_WIDTH;
		spriteData.height = obstacleNS::OBS1_HEIGHT;
		spriteData.scale = obstacleNS::OBS1_SCALE;
		spriteData.x = positionX;
		spriteData.y = positionY;
		this->setFrames(obstacleNS::OBS1_START_FRAME, obstacleNS::OBS1_END_FRAME);
		this->setCurrentFrame(obstacleNS::OBS1_START_FRAME);
		this->setFrameDelay(obstacleNS::OBS1_ANIMATION_DELAY);
		return(Entity::initialize(gamePtr, width, height, textureColumns, textureM));

	default:
		//for testing purposes
		//use the barrel here as default
		width = obstacleNS::OBS1_WIDTH;
		height = obstacleNS::OBS1_HEIGHT;
		textureColumns = obstacleNS::OBS1_COLS;
		spriteData.x = positionX;
		spriteData.y = positionY;
		this->setFrames(obstacleNS::OBS1_START_FRAME, obstacleNS::OBS1_END_FRAME);
		this->setCurrentFrame(obstacleNS::OBS1_START_FRAME);
		this->setFrameDelay(obstacleNS::OBS1_ANIMATION_DELAY);
		return(Entity::initialize(gamePtr, width, height, textureColumns, textureM));
	}



}