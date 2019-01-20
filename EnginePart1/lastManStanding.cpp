#include "lastManStanding.h"
#include "player.h"
#include "textDX.h"
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
	if (camera) {
		delete camera;
		camera = nullptr;
	}
	releaseAll();           // call onLostDevice() for every graphics item
}
//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void LastManStanding::initialize(HWND hwnd)
{
	Game::initialize(hwnd); // throws GameError

	//create the camera
	//camera = new Camera(GAME_WIDTH,GAME_HEIGHT,0, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),&mainPlayer);

	//// main player textures
	//if (!PLAYER_SHOOTING_TILE_TEXTURE.initialize(graphics, PLAYER_SHOOTING_TILE))
	//	throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));
	//if(!mainPlayer.initialize(this, playerNS::PLAYER_SHOOTING_WIDTH, playerNS::PLAYER_SHOOTING_HEIGHT, playerNS::PLAYER_SHOOTING_TEXTURE_COLS, &PLAYER_SHOOTING_TILE_TEXTURE))
	//	throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player"));


	///////////////////////////////////////////////////////////////////////////////////////////////


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

