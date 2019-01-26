#pragma once
#ifndef _HEART_H             // prevent multiple definitions if this 
#define _HEART_H             // ..file is included in more than one place


#include <list>
#include "entity.h"
#include "constants.h"
#define VECTOR2 D3DXVECTOR2
#define Vector2Normalize D3DXVec2Normalize

using namespace std;

namespace heartNS
{
	const int HEART_START_FRAME = 0;
	const int HEART_END_FRAME = 0;
	const float HEART_ANIMATION_DELAY = 0.4f;
	const int HEART_TEXTURE_COLS = 0;
	const int HEART_HEIGHT = 136;
	const int HEART_WIDTH = 120;
	const float HEART_SCALE = 0.3f;
}


class Heart : public Entity
{
private:
	int heartNo = 0;
public:
	Heart();
	~Heart();
	void setPositionVector(float x, float y) {
		this->setX(x);
		this->setY(y);
	}
	virtual bool initialize(Game *gamePtr, int width, int height, int ncols,
		TextureManager *textureM);
	virtual void draw();
	void update(float frameTime);
	void setSpriteDataXnY(float x, float y);
	void setHeartNo(int i);
	int getHeartNo();
};
#endif