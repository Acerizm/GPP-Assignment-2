#pragma once
#ifndef _PLAYER_H             // prevent multiple definitions if this 
#define _PLAYER_H             // ..file is included in more than one place

//#include "game.h"
//#include "textureManager.h"
//#include "image.h"
//#include "lastManStanding.h"
#include "bullet.h"
#include <list>
#include "entity.h"
#include "constants.h"
#define VECTOR2 D3DXVECTOR2
//#define Vector2Normalize D3DXVec2Normalize()
#define Vector2Normalize D3DXVec2Normalize

using namespace std;

// I have refactored all codes as they need to be inherited from the entity class

namespace playerNS
{
	const int PLAYER_SHOOTING_START_FRAME = 0;
	const int PLAYER_SHOOTING_END_FRAME = 2;
	const float PLAYER_SHOOTING_ANIMATION_DELAY = 0.2f;
	const int PLAYER_SHOOTING_TEXTURE_COLS = 3;
	const int PLAYER_SHOOTING_HEIGHT = 206;
	const int PLAYER_SHOOTING_WIDTH = 312;
	const float PLAYER_SHOOTING_SCALE = 0.3f;
	const int X = GAME_WIDTH / 2 - PLAYER_SHOOTING_WIDTH / 2;   // location on screen
	const int Y = GAME_HEIGHT / 2 - PLAYER_SHOOTING_HEIGHT / 2;
	const int   TEXTURE_COLS = 8;           // texture has 8 columns
	const float PLAYER_MOVEMENTSPEED = 200.0f;

}


class Player : public Entity
{
private:
	//TextureManager PLAYER_SHOOTING_TEXTURE;
	Image PLAYER_SHOOTING_IMAGE;
	//vector<Bullet*> bulletList;
	Bullet *newBullet;
	list<Bullet*> BULLET_LIST;
	friend class LastManStanding;
	friend class Camera;
	//take in lastManStanding to use graphics xd
	//LastManStanding last_Man_Standing;
	int zombieKillCount;
	Image PLAYER_SHOOTING_TILE_IMAGE;
	float playerCurrentHp;

public:
	Player();
	~Player();
	//void shootBullet(Graphics*graphics,TextureManager &texture,Image &image);
	void shootBullet(TextureManager &bulletTexture, Player *mainPlayer, Game *gameptr, float angle);
	//void moveBullet(Image &playerImage, float gameWidth, float frameTime);
	void moveBullet(float frameTime);
	void drawBullets();
	//void initialize(Graphics *graphics, TextureManager &texture, Image &image);
	void setPositionVector(float x,float y) {
		this->setX(x);
		this->setY(y);
	}
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
		TextureManager *textureM);
	virtual void draw();
	void update(float frameTime);
	void setSpriteDataXnY(float x,float y);
	//void setSpriteDataY(float y);
};




#endif