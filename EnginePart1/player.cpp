#include "player.h"
#include "graphics.h"
#include "entity.h"

Player::Player() :Entity()
{
	collisionType = entityNS::CIRCLE;
	spriteData.width = playerNS::PLAYER_WIDTH;
	spriteData.height = playerNS::PLAYER_HEIGHT;
	spriteData.scale = playerNS::PLAYER_SCALE;
	spriteData.x = 0;
	spriteData.y = 0;
	//spriteData.
};

Player::~Player() {
	//destroy the damn player here
}

//void Player::shootBullet(Graphics *graphics, TextureManager &texture, Image &playerImage) {
//	//create the bullet here
//	newBullet = new Bullet();
//	
//	newBullet->initialize(graphics, texture, newBullet->BULLET_IMAGE,playerImage.getDegrees());
//	BULLET_LIST.push_back(newBullet);
//	//this is to set the original location of the bullet when the user presses a button
//	// which should be near the player's gun tip
//	newBullet->setPositionVector(newBullet->BULLET_IMAGE, playerImage.getCenterX(), playerImage.getCenterY());
//
//
//};


//void Player::moveBullet(Image &playerImage, float gameWidth, float frameTime) 
//{
//	for each(Bullet* bullet in BULLET_LIST)
//	{
//		bullet->move(bullet->BULLET_IMAGE, playerImage, GAME_WIDTH, frameTime);
//
//	}
//}


//void Player::drawBullets() {
//	if (BULLET_LIST.size() != 0) {
//		for (list<Bullet*>::iterator it = BULLET_LIST.begin(); it != BULLET_LIST.end(); ) {
//			if ((*it)->BULLET_IMAGE.getX() > GAME_WIDTH || (*it)->BULLET_IMAGE.getX() < 0 || (*it)->BULLET_IMAGE.getY() > GAME_HEIGHT || (*it)->BULLET_IMAGE.getY() < 0)
//			{
//				SAFE_DELETE(*it);
//				it = BULLET_LIST.erase(it);
//				//just to check here
//				int check = BULLET_LIST.size();
//				//bool test = false;
//			}
//			else {
//				it++;
//			}
//		}
//
//		//when all okay go and draw the bullet
//		for each(Bullet* bullet in BULLET_LIST)
//		{
//			(bullet->BULLET_IMAGE).draw();
//		}
//	}
//}




bool Player::initialize(Game *gamePtr, int width, int height, int ncols,
	TextureManager *textureM)
{

	return(Entity::initialize(gamePtr, width, height, ncols, textureM));
}

void Player::draw() {
	this->graphics->spriteBegin();
	Image::draw();
}

void Player::update(float frameTime) 
{
	Entity::update(frameTime);
}

void Player::setSpriteDataXnY(float x, float y) 
{
	spriteData.x = x;
	spriteData.y = y;
}