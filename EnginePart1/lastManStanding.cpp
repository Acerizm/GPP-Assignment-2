#include "lastManStanding.h"
#include "player.h"
#include <string>
#include <Mmsystem.h>
#include <mciapi.h>
#include <sstream>
#include <iomanip>
#include "background.h"
#include <math.h>
#include "heart.h"
#include <winsock2.h>
#include <ws2tcpip.h>
#include <stdlib.h>
#include "GameClient.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
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
	currentGameState = "IN-LOBBY";
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

	obstaclesInitialize(true);

	// Connect to the server //////////////////////////////////////////////////////////////
	gameClient = new GameClient();
	gameClient->createClient();
	gameClient->ConnectToServer();
	/////////////////////////////////////////////////////////////////////////////////////////

	//Initialize the background for the main menu
	if (!LobbyBackgroundTexture.initialize(graphics, LobbyBackground))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing lobbyBackgroundTexture"));
	if (!LobbyBackgroundImage.initialize(graphics, LobbyBackgroundWidth, LobbyBackgroundHeight, 0, &LobbyBackgroundTexture)) {
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing lobbybackgroundImage"));
	}
	LobbyBackgroundImage.setCurrentFrame(0);
	LobbyBackgroundImage.setX(0);
	LobbyBackgroundImage.setY(0);

	camera = new Camera(GAME_WIDTH, GAME_HEIGHT, 0, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	return;
}

void LastManStanding::obstaclesInitialize(bool value) 
{
	if (value == true) 
	{
		//Attributes
		Obstacle1 = new Obstacle();

		//methods || functions

		if (!ObsTexture.initialize(graphics, OBS1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Texture"));
		if (!Obstacle1->initialize(this, &ObsTexture, GAME_WIDTH / 4 * 3, GAME_HEIGHT/10 * 7.4, 1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Texture"));

		//set the states here
		Obstacle1->setMovementState("UP");
	}

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

	if (currentGameState == "IN-LOBBY") //this has to only run once because the player is still in the main lobby
	{
		LobbyBackgroundImage.update(frameTime);
		player1 = new Player();
		//create the camera
		if (!Player1Texture.initialize(graphics, PLAYER))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing backgroundTexture"));
		if (!player1->initialize(this, playerNS::PLAYER_WIDTH, playerNS::PLAYER_HEIGHT, 4, &Player1Texture))
		{
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player1"));
		}

		player1->setPositionVector(GAME_WIDTH / 2, GAME_HEIGHT / 2);
		player1->setSpriteDataXnY(GAME_WIDTH / 2, GAME_HEIGHT / 2);
		player1->setFrames(playerNS::PLAYER_START_FRAME, playerNS::PLAYER_END_FRAME);
		player1->setFrameDelay(AnimationDelayStop);
		player1->setCurrentFrame(0);
		player1->setScale(1);
		player1->setY(620 - player1->getHeight());
		player1->setDegrees(315);

		if (!heartTexture.initialize(graphics, HEART_IMAGE))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing heartTexture"));

		for (int i = 0; i < player1->getNumberOfLifes(); i++)
		{
			Heart *heartTemp = new Heart();
			if (!heartTemp->initialize(this, heartNS::HEART_WIDTH, heartNS::HEART_HEIGHT, heartNS::HEART_TEXTURE_COLS, &heartTexture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing heart"));
			heartTemp->setY(0);
			heartTemp->setHeartNo(i);
			heartTemp->setScale(heartNS::HEART_SCALE);
			heartTemp->setX(GAME_WIDTH / 20 * i);
			heartList.push_back(heartTemp);
		}

		if (camera) {
			camera->Update();
		}

	} // end of if statement
	if (currentGameState == "IN-GAME") 
	{
		gameClient->sendData("Haiqel Test");
		if (gameClient->getCurrentClient()->getData() != "")
			string test = gameClient->getCurrentClient()->getData();

		player1->update(frameTime);
		float cameraDifferenceX = 0;
		float cameraDifferenceY = 0;
		if ((camera->getCameraX() + GAME_WIDTH / 2) > GAME_WIDTH)
		{
			cameraDifferenceX = (camera->getCameraX() + GAME_WIDTH / 2) - GAME_WIDTH;
		}
		if ((camera->getCameraY() + GAME_HEIGHT / 2) > GAME_HEIGHT)
		{
			cameraDifferenceY = (camera->getCameraY() + GAME_HEIGHT / 2) - GAME_HEIGHT;
		}
		for each (Heart *heartTemp in heartList)
		{
			heartTemp->setX(cameraDifferenceX + GAME_WIDTH / 20 * heartTemp->getHeartNo());
			heartTemp->update(frameTime);

		}
		for (int i = player1->getNumberOfLifes(); i < heartList.size(); i)
		{
			if (heartList.size() > 0)
			{
				heartList.pop_back();
			}
		}


		if (input->wasKeyPressed(VK_SPACE))
		{
			float currentAngle = player1->getRadians();
			player1->startJump(currentAngle, frameTime);
			player1->setFrameDelay(playerNS::PLAYER_ANIMATION_DELAY);


		}
		if (camera) {
			camera->Update();
		}
		if (player1->getCurrentFrame() == playerNS::PLAYER_END_FRAME)
		{
			player1->setFrameDelay(AnimationDelayStop);
			player1->setCurrentFrame(0);
		}

		player1->jump(frameTime, cameraDifferenceX, cameraDifferenceY);

		obstaclesMovement();
	}
}


void LastManStanding::obstaclesMovement() 
{
	Obstacle1->update(frameTime);


	// 1) Obstacle1 moves up and down ez
	//  change the states here
	if (Obstacle1->getY() >= (GAME_HEIGHT / 10 * 7))
		Obstacle1->setMovementState("UP");
	else if (Obstacle1->getY() <= (GAME_HEIGHT / 10 * 3))
		Obstacle1->setMovementState("DOWN");

	if (Obstacle1->getMovementState() == "UP")
		Obstacle1->setY(Obstacle1->getY() - obstacleNS::OBS1_MOVEMENT_SPEED*frameTime);
	else if (Obstacle1->getMovementState() == "DOWN")
		Obstacle1->setY(Obstacle1->getY() + obstacleNS::OBS1_MOVEMENT_SPEED*frameTime);

}
//=============================================================================
// Handle collisions
//=============================================================================
void LastManStanding::collisions(Timer *gameTimer) {
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	VECTOR2 collisionVector;
	//Event/Scenario:
	// 1) The player collided with Osbtacle1
	if (player1->collidesWith(*Obstacle1, collisionVector)) 
	{
		//what happens after collision
		player1->setX(player1->getX() -collisionVector.x*frameTime*2);

	}
}

//=============================================================================
// Render game items
//=============================================================================
void LastManStanding::render()
{
	
	graphics->spriteBegin();                // begin drawing sprites
	//player1->draw();
	//Obstacle1->draw();
	LobbyBackgroundImage.draw();
	if (camera)
	{
		camera->setTransform(graphics);
	}
	/*for each (Heart *heartTemp in heartList)
	{
		heartTemp->draw();
	}*/
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

