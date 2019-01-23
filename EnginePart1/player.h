#pragma once
#ifndef _PLAYER_H             // prevent multiple definitions if this 
#define _PLAYER_H             // ..file is included in more than one place

//#include "game.h"
//#include "textureManager.h"
//#include "image.h"
//#include "lastManStanding.h"
#include <list>
#include "entity.h"
#include "constants.h"
#define VECTOR2 D3DXVECTOR2
//#define Vector2Normalize D3DXVec2Normalize()
#define Vector2Normalize D3DXVec2Normalize

using namespace std;

namespace playerNS
{
	const int PLAYER_START_FRAME = 0;
	const int PLAYER_END_FRAME = 3;
	const float PLAYER_ANIMATION_DELAY = 0.2f;
	const int PLAYER_TEXTURE_COLS = 0;
	const int PLAYER_HEIGHT = 32;
	const int PLAYER_WIDTH = 32;
	const float PLAYER_SCALE = 3;
	//const int X = GAME_WIDTH / 2 - PLAYER_SHOOTING_WIDTH / 2;   // location on screen
	//const int Y = GAME_HEIGHT / 2 - PLAYER_SHOOTING_HEIGHT / 2;
	//const int   TEXTURE_COLS =0;           // texture has 8 columns
	//const float PLAYER_MOVEMENTSPEED = 200.0f;

}


class Player : public Entity
{
private:
	//TextureManager PLAYER_SHOOTING_TEXTURE;
	/*Image PLAYER_SHOOTING_IMAGE;*/
	//vector<Bullet*> bulletList;

public:
	Player();
	~Player();
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