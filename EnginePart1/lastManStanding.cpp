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
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <ctime>    
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
	menuOn = true;
	startText = new TextDX();
	instructionsText = new TextDX();
	quitText = new TextDX();
	scoreText = new TextDX();
	menuOptionNo = 2;
	countDownOn = false;
	
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
	player1 = new Player();
	camera = new Camera(GAME_WIDTH, GAME_HEIGHT, 0, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	//create the camera
	//camera = new Camera(GAME_WIDTH,GAME_HEIGHT,0, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),&mainPlayer);

	if (!BackgroundTexture.initialize(graphics, Background))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing backgroundTexture"));
	if (!BackgroundImage.initialize(graphics, backgroundWidth, backgroundHeight, 0, &BackgroundTexture)) {
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing backgroundImage"));
	}
	if(!heartTexture.initialize(graphics, HEART_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing heartTexture"));


	fontBig.initialize(graphics, 256, false, false, "Arial Bold");
	fontBig.setFontColor(graphicsNS::RED);
	// init the texts for menu 
	if (startText->initialize(graphics, 30, false, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing pausedText font"));
	if (instructionsText->initialize(graphics, 30, false, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing pausedText font"));
	if (quitText->initialize(graphics, 30, false, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing pausedText font"));

	if (scoreText->initialize(graphics,30, false, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing scoreText font"));


	BackgroundImage.setCurrentFrame(0);
	

	BackgroundImage.setX(0);
	BackgroundImage.setY(0);
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
	//player1->setCollisionRadius(30.0f);

	for (int i = 0; i < player1->getNumberOfLifes(); i++)
	{
		Heart *heartTemp = new Heart();
		if (!heartTemp->initialize(this,heartNS::HEART_WIDTH, heartNS::HEART_HEIGHT, heartNS::HEART_TEXTURE_COLS, &heartTexture))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing heart"));
		heartTemp->setY(0);
		heartTemp->setHeartNo(i);
		heartTemp->setScale(heartNS::HEART_SCALE);
		heartTemp->setX(GAME_WIDTH / 20 * i);
		heartList.push_back(heartTemp);
	}

	obstaclesInitialize(true);
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
	long int timePassedint = static_cast<long int> (timePassed);
	if (countDownOn)
	{
		countDownTimer = COUNT_DOWN - timePassedint;
		if (countDownTimer < 0)
		{
			countDownOn = false;
			camera->setCameraHorizontalSpeed(0.3f);
		}
	}
	float cameraDifferenceX = 0;
	float cameraDifferenceY = 0;
	int tempScore = 0;
	int distanceTravelled = 0;
	


	if ((camera->getCameraX() + GAME_WIDTH / 2) > GAME_WIDTH)
	{
		cameraDifferenceX = (camera->getCameraX() + GAME_WIDTH / 2) - GAME_WIDTH;
	}
	if ((camera->getCameraY() + GAME_HEIGHT / 2) > GAME_HEIGHT)
	{
		cameraDifferenceY = (camera->getCameraY() + GAME_HEIGHT / 2) - GAME_HEIGHT;
	}
	for (int i = 0; i < cameraDifferenceX; i++)
	{
		tempScore = player1->getNumberOfLifes();
		player1->setScore(player1->getScore() + tempScore);
	}
	//scoring system boiis
	

	if (menuOn)
	{
		camera->setCameraHorizontalSpeed(0.0f);
		if (menuOptionNo == 2)
		{
			startText->setFontColor(graphicsNS::YELLOW);
			instructionsText->setFontColor(graphicsNS::WHITE);
			quitText->setFontColor(graphicsNS::WHITE);
		}
		if (menuOptionNo == 1)
		{
			startText->setFontColor(graphicsNS::WHITE);
			instructionsText->setFontColor(graphicsNS::YELLOW);
			quitText->setFontColor(graphicsNS::WHITE);
		}
		if (menuOptionNo == 0)
		{
			startText->setFontColor(graphicsNS::WHITE);
			instructionsText->setFontColor(graphicsNS::WHITE);
			quitText->setFontColor(graphicsNS::YELLOW);
		}

		if (input->wasKeyPressed(VK_DOWN))
		{
			if (menuOptionNo != 0)
			{
				menuOptionNo -= 1;
			}
		}
		if (input->wasKeyPressed(VK_UP))
		{
			if (menuOptionNo != 2)
			{
				menuOptionNo += 1;
			}
		}
		if (input->wasKeyPressed(VK_RETURN))
		{
			if (menuOptionNo == 2)
			{
				menuOn = !menuOn;
				t = std::time(0);
				countDownOn = true;
			}
			else if (menuOptionNo == 1)
			{
				//show instructions here
				//showInstruction = true;
				if (input->wasKeyPressed(VK_ESCAPE))
				{
					//showInstruction = false;
				}
			}
			else if (menuOptionNo == 0)
			{
				PostQuitMessage(0);
			}
		}
		/*if (showInstruction)
		{
			if (input->wasKeyPressed(VK_ESCAPE))
			{
				showInstruction = false;
			}
		}
		*/
	}
		timePassed = std::time(0) - t;
		BackgroundImage.update(frameTime);
		player1->update(frameTime);
		if (camera) {
			camera->Update();
		}
		if (!menuOn) // basically all code for unpaused game should be place inside here boiis
		{
			this->startGame(cameraDifferenceX, cameraDifferenceY);
				
			
			if (input->wasKeyPressed(VK_ESCAPE))
			{
				menuOn = !menuOn;
			}

			player1->jump(frameTime, cameraDifferenceX, cameraDifferenceY);
			if (input->wasKeyPressed(VK_SPACE) && !countDownOn)
			{
				float currentAngle = player1->getRadians();
				player1->startJump(currentAngle, frameTime);
				player1->setFrameDelay(playerNS::PLAYER_ANIMATION_DELAY);


			}

			if (player1->getCurrentFrame() == playerNS::PLAYER_END_FRAME)
			{
				player1->setFrameDelay(AnimationDelayStop);
				player1->setCurrentFrame(0);
			}

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
	BackgroundImage.draw();
	player1->draw();
	Obstacle1->draw();
	scoreText->print("Score: "+ to_string(player1->getScore()), camera->getCameraX() - (GAME_WIDTH / 2), camera->getCameraY() + GAME_HEIGHT / 2 - 30);
	if (camera)
	{
		camera->setTransform(graphics);
	}
	for each (Heart *heartTemp in heartList)
	{
		heartTemp->draw();
	}
	if (menuOn)
	{
		BackgroundImage.draw();
		int textWidthStart = startText->GetTextWidth("START", startText->getFont());
		int textHeightStart = startText->GetTextHeight("START", startText->getFont());
		startText->print("START", camera->getCameraX() - textWidthStart/2, camera->getCameraY() - textHeightStart/2);

		int textWidthInstructions = instructionsText->GetTextWidth("INSTRUCTIONS", instructionsText->getFont());
		int textHeightInstructions = instructionsText->GetTextHeight("INSTRUCTIONS", instructionsText->getFont());
		instructionsText->print("INSTRUCTIONS", camera->getCameraX() - textWidthInstructions / 2, camera->getCameraY() + 30 - textHeightInstructions / 2);

		int textWidthQuit = quitText->GetTextWidth("QUIT GAME", quitText->getFont());
		int textHeightQuit = quitText->GetTextHeight("QUIT GAME", quitText->getFont());
		quitText->print("QUIT GAME", camera->getCameraX() - (textWidthQuit / 2), camera->getCameraY() + 60 - textHeightQuit / 2);
	}
	if (countDownOn)
	{
		if (countDownTimer - timePassed > 0)
		{
			int textWidth = fontBig.GetTextWidth(to_string(countDownTimer - timePassed), fontBig.getFont());
			int textHeight = fontBig.GetTextHeight(to_string(countDownTimer - timePassed), fontBig.getFont());
			fontBig.print(to_string(countDownTimer - timePassed), camera->getCameraX() - textWidth/2, camera->getCameraY() - textHeight/2);
		}
		else if (countDownTimer - timePassed == 0)
		{
			int textWidth = fontBig.GetTextWidth("GO!", fontBig.getFont());
			int textHeight = fontBig.GetTextHeight("GO!", fontBig.getFont());
			fontBig.print("GO!", camera->getCameraX() - textWidth / 2, camera->getCameraY() - textHeight / 2);
		}
	}

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
void LastManStanding::startGame(float cameraDifferenceX, float cameraDifferenceY)
{
	
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
	countDownTimer = COUNT_DOWN;
	
}

