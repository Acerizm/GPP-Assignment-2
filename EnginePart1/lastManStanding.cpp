#include "lastManStanding.h"
#include "player.h"
#include <string>
#include <Mmsystem.h>
#include <mciapi.h>
#include <sstream>
#include <iomanip>
#include "background.h"
#pragma comment(lib, "Winmm.lib")


using namespace std;
//=============================================================================
// Constructor
//=============================================================================
LastManStanding::LastManStanding()
{
	/*srand(time(NULL));
	hpText = new TextDX();
*/
}

//=============================================================================
// Destructor
//=============================================================================
LastManStanding::~LastManStanding()
{
	releaseAll();           // call onLostDevice() for every graphics item
}
//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void LastManStanding::initialize(HWND hwnd)
{
	Game::initialize(hwnd); // throws GameError
	player1 = new Player();
	//create the camera
	//camera = new Camera(GAME_WIDTH,GAME_HEIGHT,0, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),&mainPlayer);

	if (!BackgroundTexture.initialize(graphics, Background))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing backgroundTexture"));
	if (!BackgroundImage.initialize(graphics, backgroundWidth, backgroundHeight, 0, &BackgroundTexture)) {
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing backgroundImage"));
	}

	BackgroundImage.setCurrentFrame(0);

	BackgroundImage.setX(0);
	BackgroundImage.setY(0);
	if (!Player1Texture.initialize(graphics, PLAYER))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing backgroundTexture"));
	if (!player1->initialize(this, playerNS::PLAYER_WIDTH, playerNS::PLAYER_HEIGHT, 4, &Player1Texture)) 
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player1"));
	}

	/*player1->setX(GAME_WIDTH / 2);
	player1->setY(GAME_HEIGHT / 2);*/
	player1->setPositionVector(GAME_WIDTH / 2, GAME_HEIGHT / 2);
	player1->setSpriteDataXnY(GAME_WIDTH / 2, GAME_HEIGHT / 2);
	player1->setFrames(playerNS::PLAYER_START_FRAME, playerNS::PLAYER_END_FRAME);
	player1->setFrameDelay(playerNS::PLAYER_ANIMATION_DELAY);
	//player1->setCurrentFrame(0);
	player1->setScale(1);



	//damn annoying when debug so many times Xddd
	/*mciSendString("open \"audio\\deathSong.wav\" type waveaudio alias sound", NULL, 0, NULL);
	mciSendString("open \"audio\\backGroundMusic.wav\" type waveaudio alias backGroundMusic", NULL, 0, NULL);*/

	return;
}

std::string to_format(const int number) {
	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << number;
	return ss.str();
}

//=============================================================================
// Update all game items
//=============================================================================
void LastManStanding::update(Timer *gameTimer)
{
	BackgroundImage.update(frameTime);
	player1->update(frameTime);

	if (input->wasKeyPressed('W')) 
	{
		player1->setX(player1->getX() + 10);
		player1->setY(player1->getY() - 10);

	}
	
	//Add gravity here with acceleration if the player does not press anything
	// can use projectile formula
	player1->setY(player1->getY() + 0.05);
}



//=============================================================================
// Handle collisions
//=============================================================================
void LastManStanding::collisions(Timer *gameTimer) {
	

	///////////////////////////////////////////////////////////////////////////////////////////////////
	//Scenario : zombie collided with one another
	// Darren
	/*for each (Zombie *zombie in zombieList) 
	{
		for each (Zombie *zombie2 in zombieList) 
		{
			if (zombie == zombie2)
				continue;
			else 
			{
				if (zombie->collidesWith(*zombie2, collisionVector)) 
				{
					VECTOR2 unitCollisionVector;
					(zombie)->bounce(collisionVector, *zombie2);
					Vector2Normalize(&unitCollisionVector, &collisionVector);

					(zombie)->setX(zombie->getX() - unitCollisionVector.x*frameTime * 300.0f);
					(zombie)->setSpriteDataX(zombie->getX());
					(zombie)->setY(zombie->getY() - unitCollisionVector.y*frameTime * 300.0f);
					(zombie)->setSpriteDataY(zombie->getY());
					(zombie2)->bounce(collisionVector, *zombie);
					Vector2Normalize(&unitCollisionVector, &collisionVector);

					(zombie2)->setX(zombie2->getX() + unitCollisionVector.x*frameTime * 300.0f);
					(zombie2)->setSpriteDataX(zombie2->getX());
					(zombie2)->setY(zombie2->getY() + unitCollisionVector.y*frameTime * 300.0f);
					(zombie2)->setSpriteDataY(zombie2->getY());

				}
			}
		}
	}*/
	//////////////////////////////////////////////////////////////////////////////////////////////
}

//=============================================================================
// Render game items
//=============================================================================
void LastManStanding::render()
{
	
	graphics->spriteBegin();                // begin drawing sprites
	BackgroundImage.draw();
	player1->draw();
	
	graphics->spriteEnd();                  // end drawing sprites

}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void LastManStanding::ai(Timer *gameTimer)
{

}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void LastManStanding::releaseAll()
{
	return;

}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void LastManStanding::resetAll()
{
	return;

}

