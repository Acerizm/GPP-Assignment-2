#pragma once
#ifndef _ZOMBIE_H
#define _ZOMBIE_H
#include "image.h"
#include "entity.h"
#include "player.h"
#include <cmath>
#include "textDX.h"
using namespace std;

namespace zombieNS {
	//add the spriteData stuff here
	const int ZOMBIE_MOVING_START_FRAME = 0;
	const int ZOMBIE_MOVING_END_FRAME = 9;
	const float ZOMBIE_MOVING_ANIMATION_DELAY = 0.2f;
	const int ZOMBIE_MOVING_COLS = 3;
	const int ZOMBIE_MOVING_HEIGHT = 311;
	const int ZOMBIE_MOVING_WIDTH = 288;
	const float ZOMBIE_MOVING_SCALE = 0.3f;
	const float ZOMBIE_SPEED = 100.0f;
}


class Zombie : public Entity
{
private:
	Image ZOMBIE_MOVING_IMAGE;
	friend class LastManStanding;
	float unitVectorXCor;
	float unitVectorYCor;
	/*TextureManager healthBarRedTexture;*/
	float zombieMaxHp = 100;
	float zombieCurrentHP;
	//TextureManager enemyHealthBarBackGroundTexture;
	Image *healthBarRed;
	Image *enemyHealthBarBackGround;
	TextDX *zombieHpText;
	friend class LastManStanding;
	bool isCollided = false;
	bool isBoss = false;

public:
	Zombie();
	~Zombie();
	//void initialize(Graphics *graphics, TextureManager &texture, Image &image);
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols,TextureManager *textureM, TextureManager *textureHealthBar, TextureManager *textureEnemyHealthBar,Graphics *graphics);

	void setPositionVector(float x, float y);

	//what the fk do i need here
	//1. the player current position
	//2. move the zombie to the player
	// 3. if hit - > zombie move to (0,0) and restart as test
	//void attackPlayer(Graphics *graphics, Image &zombieImage, Image &playerImage,float frameTime);
	void attackPlayer(Player *mainPlayer, float frameTime, float scalar);

	void getUnitVectorCoordinates(Player *mainPlayer);

	TextDX *getZombieHPText() {
		return zombieHpText;
	}

	Image *getHealthBarRed() {
		return healthBarRed;
	}

	Image *getEnemyHealthBarBackGround() {
		return enemyHealthBarBackGround;
	}

	float getCurrentZombieHP() {
		return zombieCurrentHP;
	}

	float getZombieMaxHp() {
		return zombieMaxHp;
	}
	bool getIsCollided() {
		return isCollided;
	}

	void setIsCollided(bool value) {
		isCollided = value;
	}
	void setPercentage(float currentPercentage);

	float getSpriteDataX() {
		return spriteData.x;
	}

	float getSpriteDataY() {
		return spriteData.y;
	}

	void setSpriteDataX(float x) {
		spriteData.x = x;
	}

	void setSpriteDataY(float y) {
		spriteData.y = y;
	}

	bool getIsBoss() {
		return isBoss;
	}

	void setIsBoss() {
		isBoss = true;
	}

	void setZombieMaxHP(float hp) 
	{
		zombieMaxHp = hp;
	}


};

#endif