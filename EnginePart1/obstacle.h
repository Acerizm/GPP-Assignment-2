#pragma once
#ifndef _OBSTACLE_H
#define _OBSTACLE_H
#include "image.h"
#include "entity.h"
#include "player.h"
#include <cmath>
using namespace std;

namespace obstacleNS {
	//add the spriteData stuff here
	//this is for the barrel
	const int OBS1_START_FRAME = 0;
	const int OBS1_END_FRAME = 0;
	const float OBS1_ANIMATION_DELAY = 0.0;
	const int OBS1_COLS = 0;
	const int OBS1_HEIGHT = 32;
	const int OBS1_WIDTH = 32;
	const float OBS1_SCALE = 3.0f;

	
	
}


class Obstacle : public Entity
{
private:
	Image OBSTACLE_IMAGE;
	bool isCollided = false;
	friend class LastManStanding;

public:
	Obstacle();
	~Obstacle();
	//void initialize(Graphics *graphics, TextureManager &texture, Image &image);
	virtual bool initialize(Game *gamePtr,TextureManager *textureM, float positionX, float positionY, int ObstacleType);

	float getSpriteDataX() {
		return spriteData.x;
	}

	void setSpriteDataX(float x) {
		spriteData.x = x;
	}

	float getSpriteDataY() {
		return spriteData.y;
	}

	void setSpriteDataY(float y) {
		spriteData.y = y;
	}

	bool getIsCollided() {
		return isCollided;
	}

	void setIsCollided(bool value) {
		isCollided = value;
	}
};

#endif
