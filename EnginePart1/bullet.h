#pragma once
#ifndef _BULLET_H             // prevent multiple definitions if this 
#define _BULLET_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN
#include "image.h"
#include "entity.h"
#include <math.h>

namespace bulletNS 
{
	const int BULLET_START_FRAME = 0;
	const int BULLET_END_FRAME = 0;
	const float BULLET_ANIMATION_DELAY = 0;
	const int BULLET_HEIGHT = 47;
	const int BULLET_WIDTH = 47;
	const float BULLET_SCALE = 0.4;
	const float BULLET_SPEED = 300.0f;
	//bool isCollided = false;
	//friend class lastManStandi


}
class Bullet : Entity {
	private:
		//textureManager BULLET_TEXTURE;

		Image BULLET_IMAGE;
		friend class LastManStanding;
		friend class Player;
		float currentAngle;
		bool isCollided = false;

	public:
		//Image BULLET_IMAGE;

		//constructor
		Bullet();

		//de-stuctor
		virtual ~Bullet();

		//what the heck does the bullet need to store?
		//1. need to store the texture here
		//2. need to store the image here
		//3. handle collision here also

		//initialize the damn bullet
		//pass by reference here btw
		/*void initialize(Graphics *graphics, TextureManager &texture, Image &image, float currentAngle);*/

		virtual bool initialize(Game *gamePtr, int width, int height, int ncols, TextureManager *textureM, float currentAngle);

		/*void setPositionVector(Image &image,float playerXCenter, float playerYCenter) {
			image.setX(playerXCenter + 10);
			image.setY(playerYCenter);
			image.setScale(BULLET_SCALE);
			image.setFrames(BULLET_START_FRAME, BULLET_END_FRAME);
		}*/

		void setPositionVector(float x, float y) {
			//pls change this in the future
			this->setX(x + 10);
			spriteData.x = this->getX();
			this->setY(y);
			spriteData.y = this->getY();
		}

		//void move(Image &bulletImage, Image &playerImage, float gameWidth,float frameTime) {
		//	//Move the bullet based on the direction of the player image.
		//	bulletImage.setX(bulletImage.getX() + ((frameTime * BULLET_SPEED) * (cos(currentAngle*PI/180))));
		//	bulletImage.setY(bulletImage.getY() + ((frameTime * BULLET_SPEED) * (sin(currentAngle*PI/180))));

		//}

		void move(float frameTime) {
			this->setX(this->getX() + ((frameTime * bulletNS::BULLET_SPEED) * (cos(currentAngle*PI / 180))));
			/*spriteData.x = this->getX() + ((frameTime * bulletNS::BULLET_SPEED) * (cos(currentAngle*PI / 180)));*/
			spriteData.x = this->getX();
			this->setY(this->getY() + ((frameTime * bulletNS::BULLET_SPEED) * (sin(currentAngle*PI / 180))));
			/*spriteData.y = this->getY() + ((frameTime * bulletNS::BULLET_SPEED) * (sin(currentAngle*PI / 180)));*/
			spriteData.y = this->getY();
		}

		bool getIsCollided() {
			return isCollided;
		}

		void setIsCollided(bool value) {
			isCollided = value;
		}
};




#endif