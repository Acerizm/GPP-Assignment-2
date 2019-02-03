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
#include "include/rapidjson/document.h"
#include "include/rapidjson/writer.h"
#include "include/rapidjson/stringbuffer.h"

// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment(lib, "Winmm.lib")


using namespace std;
using namespace rapidjson;
//=============================================================================
// Constructor
//=============================================================================
LastManStanding::LastManStanding()
{
	/*srand(time(NULL));
	hpText = new TextDX();
*/
	currentGameState = "IN-LOBBY";
	socketData = new SocketData();
	numOfPlayers = 1;
	timer = new Timer();
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
	//this only happens when the player has joined the lobby
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
	
	//Initialize the 1st player's selectiion
	if (!ID1Texture.initialize(graphics,ID1))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing lobbyBackgroundTexture"));
	//ID1Image = new Image();
	if(!ID1Image.initialize(graphics,426,720,0,&ID1Texture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing lobbyBackgroundTexture"));

	ID1Image.setX(0);
	ID1Image.setY(0);
	ID1Image.setCurrentFrame(0);

	//numOfPlayers++;
	camera = new Camera(GAME_WIDTH, GAME_HEIGHT, 0, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));
	return;
}

void LastManStanding::obstaclesInitialize(bool value) 
{
	if (value == true) 
	{
		//Attributes
		Obstacle1 = new Obstacle();
		Obstacle2 = new Obstacle();

		//methods || functions

		if (!ObsTexture.initialize(graphics, OBS1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Texture"));
		if (!Obstacle1->initialize(this, &ObsTexture, GAME_WIDTH / 4 * 3, GAME_HEIGHT/10 * 7.4, 1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Texture"));

		if (!Obs2Texture.initialize(graphics, OBS1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Texture"));
		if (!Obstacle2->initialize(this, &Obs2Texture, BackgroundWidth / 20 * 2, GAME_HEIGHT / 10 * 5, 1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Texture"));

		//set the states here
		Obstacle1->setMovementState("UP");
		Obstacle2->setMovementState("RIGHT");
	}

}

std::string to_format(const int number) {
	std::stringstream ss;
	ss << std::setw(2) << std::setfill('0') << number;
	return ss.str();
}

void LastManStanding::player1Initialize() 
{
	player1 = new Player();

	if (!Player1Texture.initialize(graphics, PLAYER))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing backgroundTexture"));
	if (!player1->initialize(this, playerNS::PLAYER_WIDTH, playerNS::PLAYER_HEIGHT, 4, &Player1Texture))
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player1"));
	}

	//player1->setPositionVector(GAME_WIDTH / 2, GAME_HEIGHT / 2);
	//player1->setSpriteDataXnY(GAME_WIDTH / 2, GAME_HEIGHT / 2);
	player1->setFrames(playerNS::PLAYER_START_FRAME, playerNS::PLAYER_END_FRAME);
	player1->setFrameDelay(AnimationDelayStop);
	player1->setCurrentFrame(0);
	player1->setScale(1);
	player1->setY(GAME_HEIGHT / 2);
	player1->setY(620 - player1->getHeight());
	player1->setDegrees(315);

}

void LastManStanding::player2Initalize() {
	player2 = new Player();

	if (!Player2Texture.initialize(graphics, PLAYER))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing backgroundTexture"));
	if (!player2->initialize(this, playerNS::PLAYER_WIDTH, playerNS::PLAYER_HEIGHT, 4, &Player2Texture))
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player2"));
	}

	//player1->setPositionVector(GAME_WIDTH / 2, GAME_HEIGHT / 2);
	//player1->setSpriteDataXnY(GAME_WIDTH / 2, GAME_HEIGHT / 2);
	player2->setFrames(playerNS::PLAYER_START_FRAME, playerNS::PLAYER_END_FRAME);
	player2->setFrameDelay(AnimationDelayStop);
	player2->setCurrentFrame(0);
	player2->setScale(1);
	player2->setY(GAME_HEIGHT / 3);
	player2->setX(GAME_WIDTH / 3);
	//player2->setY(620 - player1->getHeight());
	player2->setDegrees(315);
}

void LastManStanding::player3Initalize() {
	player3 = new Player();

	if (!Player3Texture.initialize(graphics, PLAYER))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing backgroundTexture"));
	if (!player3->initialize(this, playerNS::PLAYER_WIDTH, playerNS::PLAYER_HEIGHT, 4, &Player3Texture))
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player1"));
	}

	//player1->setPositionVector(GAME_WIDTH / 2, GAME_HEIGHT / 2);
	//player1->setSpriteDataXnY(GAME_WIDTH / 2, GAME_HEIGHT / 2);
	player3->setFrames(playerNS::PLAYER_START_FRAME, playerNS::PLAYER_END_FRAME);
	player3->setFrameDelay(AnimationDelayStop);
	player3->setCurrentFrame(0);
	player3->setScale(1);
	player3->setY(GAME_HEIGHT / 4);
	player3->setX(GAME_WIDTH / 4);
	//player2->setY(620 - player1->getHeight());
	player3->setDegrees(315);
}

//=============================================================================
// Update all game items
//=============================================================================
void LastManStanding::update(Timer *gameTimer)
{
	//tempSocketData is to store data received from other clients
	//socketData is to store data from the local machine
	if (currentGameState == "IN-LOBBY") //this has to only run once because the player is still in the main lobby
	{
		//1. Check if there is data coming from other clients
		string receivedJson = gameClient->getCurrentClient()->getData();
		//Create a new SocketData object for the received data
		tempSocketData = new SocketData();
		if (receivedJson != "") {
			Document document = tempSocketData->getDocument(receivedJson);
			//int s1 = document["id"].GetInt();
			tempSocketData->setID(document["id"].GetInt());
			tempSocketData->setNumOfPlayersVoted(document["numOfPlayersVoted"].GetInt());

			//tempID is the other player's connection
			int tempID = tempSocketData->getID();
			//tempNumOfPlayersVoted is when there is from the other client
			int tempNumOfPlayersVoted = tempSocketData->getNumOfPlayersVoted();

			//check if the ID has been taken up
			// since this is in a loop, no need to for-loop to check
			// this code is broken must fix in next patch
			//need to check for the first plyer

			if (currentPlayerID == tempID) {
				numOfPlayers++;
				currentPlayerID ++;
				drawPlayerSelectionBox++;
				socketData->setID(currentPlayerID);
			}

			if (currentPlayerID < tempID) {
				numOfPlayers = tempID;
				drawPlayerSelectionBox = tempID;
			}

			//if the player presses the enter key
			if (input->wasKeyPressed(0x0D)) {
				//if (numOfPlayersVoted == 0)
					numOfPlayersVoted++;
			}

			// check if the voting number is more than total players
			if (numOfPlayers >= 1) {
				//this happens when there is the other client has voted alr or has more votes
				if (numOfPlayersVoted < tempNumOfPlayersVoted)
					numOfPlayersVoted = tempNumOfPlayersVoted;
				if (numOfPlayersVoted == 0 && tempNumOfPlayersVoted == 0)
				{
					socketData->setNumOfPlayersVoted(numOfPlayersVoted);
				}
				if (numOfPlayersVoted == 0 && tempNumOfPlayersVoted > 0)
				{
					numOfPlayersVoted = tempNumOfPlayersVoted;
					socketData->setNumOfPlayersVoted(numOfPlayersVoted);
				}

				socketData->setNumOfPlayersVoted(numOfPlayersVoted);

			}
		}

		//Send the data to the server with the JsonFormatted
		//use the socketData not the tempSocketData(local)
		int numOfSecondsPassed = int(gameTimer->getCurrentElapsedTime(false));
		int test = timer->getCurrentElapsedTime(false);
		if (numOfSecondsPassed % 1 == 0 && numOfSecondsPassed != currentTime && numOfSecondsPassed != 0) {
			currentTime = numOfSecondsPassed;
			//if (currentGameState == "IN-LOBBY")
				//currentTime = static_cast<int>(timer->getCurrentElapsedTime(false));
			gameClient->sendData(socketData->getJsonData());
		}
		//Sleep(1000);
		LobbyBackgroundImage.update(frameTime);
		if (camera) {
			camera->Update();
		}
		ID1Image.update(frameTime);

		if (drawPlayerSelectionBox == 2 && ID2Image.getInitalized() != true ) 
		{
			//ID2Image = new Image();
			if (!ID2Image.initialize(graphics, 426, 720, 0, &ID1Texture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing lobbyBackgroundTexture"));

			ID2Image.setX(GAME_WIDTH / 3);
			ID2Image.setY(0);
			ID2Image.setCurrentFrame(0);
		}

		if (drawPlayerSelectionBox == 3 && ID3Image.getInitalized() != true)
		{
			//ID3Image = new Image();
			if (!ID3Image.initialize(graphics, 426, 720, 0, &ID1Texture))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing lobbyBackgroundTexture"));

			ID3Image.setX(GAME_WIDTH / 3 * 2);
			ID3Image.setY(0);
			ID3Image.setCurrentFrame(0);
		}

		ID2Image.update(frameTime);
		ID3Image.update(frameTime);

		//When this is a local game / not connected to the server or other clients
		if (receivedJson == "") {
			//if the player presses the enter key
			if (input->wasKeyPressed(0x0D)) {
				if (numOfPlayersVoted == 0)
					numOfPlayersVoted++;
			}
		}

		//check the voting system here
		//this voting system happens when there is only 1 player
		if (float(numOfPlayersVoted) / float(numOfPlayers) * 100 >= 50) {
			currentGameState = "LOADING-GAME";
		}
		

	} // end of if statement for currentGameState = "IN-LOBBY"

	if (currentGameState == "LOADING-GAME") 
	{
		//1. Check if there is data coming from other clients
		string receivedJson = gameClient->getCurrentClient()->getData();
		//Create a new SocketData object for the received data
		tempSocketData = new SocketData();

		if (!BackgroundTexture.initialize(graphics, Background))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing BackgroundTexture"));
		if (!BackgroundImage.initialize(graphics, BackgroundWidth, BackgroundHeight, 0, &BackgroundTexture)) {
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing backgroundImage"));
		}
		BackgroundImage.setCurrentFrame(0);
		BackgroundImage.setX(0);
		BackgroundImage.setY(0);
		if (numOfPlayers == 1) {
			player1Initialize();
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

			socketData->setIsLoaded(1);
			currentGameState = "WAITING";

		}
		if (receivedJson != "") {
			Document document = tempSocketData->getDocument(receivedJson);
			tempSocketData->setID(document["id"].GetInt());

			if (numOfPlayers == 2) {
				player1 = new Player();
				player2 = new Player();
				if (currentPlayerID == 1) {
					player1->setClientID(currentPlayerID);
					player2->setClientID(2);
				}

				player1Initialize();
				player2Initalize();

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

				socketData->setIsLoaded(1);
				currentGameState = "WAITING";
				int numOfSecondsPassed = int(gameTimer->getCurrentElapsedTime(false));
				int test = timer->getCurrentElapsedTime(false);
				if (numOfSecondsPassed % 1 == 0 && numOfSecondsPassed != currentTime && numOfSecondsPassed != 0) {
					currentTime = numOfSecondsPassed;
					gameClient->sendData(socketData->getJsonData());
				}

			}
			if (numOfPlayers == 3) {
				player1 = new Player();
				player2 = new Player();
				player3 = new Player();

				//initialize the player immediately
				player1Initialize();
				player2Initalize();
				player3Initalize();
				//need to stop the machine for awhile
				socketData->setIsLoaded(1);
				int numOfSecondsPassed = int(gameTimer->getCurrentElapsedTime(false));
				int test = timer->getCurrentElapsedTime(false);
				if (numOfSecondsPassed % 1 == 0 && numOfSecondsPassed != currentTime && numOfSecondsPassed != 0) {
					currentTime = numOfSecondsPassed;
					gameClient->sendData(socketData->getJsonData());
				}
				currentGameState = "WAITING";
			}
		}
	}

	if (currentGameState == "WAITING") 
	{
		// need to assign the player to each respective player object

		//1. Check if there is data coming from other clients
		string receivedJson = gameClient->getCurrentClient()->getData();
		if (numOfPlayers == 1)
			currentGameState = "IN-GAME";
		if (receivedJson != "") 
		{
			// Create a new SocketData object for the received data
			// tempSocketData = new SocketData();
			Document document = tempSocketData->getDocument(receivedJson);
			tempSocketData->setID(document["id"].GetInt());
			//tempSocketData->setIsLoaded(document["check"].GetInt());
			tempSocketData->setIsLoaded(document["isLoaded"].GetInt());
			//tempID is the other player's connection
			int tempID = tempSocketData->getID();
			int tempIsLoaded = tempSocketData->getIsLoaded();
			if (numOfPlayers == 2) {
				if (tempIsLoaded == 1)
					allPlayerLoaded++;
				if (allPlayerLoaded == numOfPlayers) {
					currentGameState = "IN-GAME";
				}
				
				int numOfSecondsPassed = int(gameTimer->getCurrentElapsedTime(false));
				int test = timer->getCurrentElapsedTime(false);
				if (numOfSecondsPassed % 1 == 0 && numOfSecondsPassed != currentTime && numOfSecondsPassed != 0) {
					currentTime = numOfSecondsPassed;
					gameClient->sendData(socketData->getJsonData());
				}
			}
			if (numOfPlayers == 3)	
			{
				if (currentClientIDConnected != tempID) 
				{

					if (tempIsLoaded == 1)
						allPlayerLoaded++;
					if (allPlayerLoaded == numOfPlayers)
						currentGameState = "IN-GAME";
					
					int numOfSecondsPassed = int(gameTimer->getCurrentElapsedTime(false));
					int test = timer->getCurrentElapsedTime(false);
					if (numOfSecondsPassed % 1 == 0 && numOfSecondsPassed != currentTime && numOfSecondsPassed != 0) {
						currentTime = numOfSecondsPassed;
						gameClient->sendData(socketData->getJsonData());
					}				
				}
			}
		}
	}

	else if (currentGameState == "IN-GAME") 
	{
		BackgroundImage.update(frameTime);
		//gameClient->sendData("Haiqel Test");
		/*if (gameClient->getCurrentClient()->getData() != "")
			string test = gameClient->getCurrentClient()->getData();*/
		int numOfSecondsPassed = int(gameTimer->getCurrentElapsedTime(false));
		int test = timer->getCurrentElapsedTime(false);
		if (numOfSecondsPassed % 1 == 0 && numOfSecondsPassed != currentTime && numOfSecondsPassed != 0) {
			currentTime = numOfSecondsPassed;
			gameClient->sendData(socketData->getJsonData());
		}
		
		player1->update(frameTime);
		if (numOfPlayers == 2) {
			player1->update(frameTime);
			player2->update(frameTime);
		}
		if (numOfPlayers == 3) {
			player2->update(frameTime);
			player3->update(frameTime);
		}
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

		//this is where the magic happens
		if (numOfPlayers == 1) {
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

		if (numOfPlayers == 2) 
		{
			//1. Check if there is data coming from other clients
			string receivedJson = gameClient->getCurrentClient()->getData();
			//Create a new SocketData object for the received data
			// tempSocketData = new SocketData();
			Document document = tempSocketData->getDocument(receivedJson);
			tempSocketData->setID(document["id"].GetInt());
			tempSocketData->setXCoordinate(document["XCoordinate"].GetFloat());
			tempSocketData->setYCoordinate(document["YCoordinate"].GetFloat());

			float x = tempSocketData->getXCoordinate();
			float y = tempSocketData->getYCoordinate();
			if (currentPlayerID == 1) {
				player2->setX(x);
				player2->setY(y);
				myPlayerMovement(player1, cameraDifferenceX, cameraDifferenceY);
				socketData->setXCoordinate(player1->getX());
				socketData->setYCoordinate(player1->getY());
				int numOfSecondsPassed = int(gameTimer->getCurrentElapsedTime(false));
				int test = timer->getCurrentElapsedTime(false);
				if (fmod(numOfSecondsPassed, 1) == 0.1 /*&& numOfSecondsPassed != currentTime*/ && numOfSecondsPassed != 0) {
					currentTime = numOfSecondsPassed;
					gameClient->sendData(socketData->getJsonData());
				}
				if (player2->getCurrentFrame() == playerNS::PLAYER_END_FRAME)
				{
					player2->setFrameDelay(AnimationDelayStop);
					player2->setCurrentFrame(0);
				}
				player2->jump(frameTime, cameraDifferenceX, cameraDifferenceY);
			}
			if (currentPlayerID == 2) {
				player1->setX(x);
				player1->setY(y);
				myPlayerMovement(player2, cameraDifferenceX, cameraDifferenceY);
				socketData->setXCoordinate(player2->getX());
				socketData->setYCoordinate(player2->getY());
				int numOfSecondsPassed = int(gameTimer->getCurrentElapsedTime(false));
				int test = timer->getCurrentElapsedTime(false);
				//gameClient->sendData(socketData->getJsonData());
				if (fmod(numOfSecondsPassed, 1) == 0.1 /*&& numOfSecondsPassed != currentTime*/ && numOfSecondsPassed != 0) {
					currentTime = numOfSecondsPassed;
					gameClient->sendData(socketData->getJsonData());
				}
				if (player1->getCurrentFrame() == playerNS::PLAYER_END_FRAME)
				{
					player1->setFrameDelay(AnimationDelayStop);
					player1->setCurrentFrame(0);
				}
				player1->jump(frameTime, cameraDifferenceX, cameraDifferenceY);
			}

			if (camera) {
				camera->Update();
			}


		}

		obstaclesMovement();





		
	} //end of if statement for currentGameState == "IN-GAME"
}

void LastManStanding::myPlayerMovement(Player* player,float cameraDifferenceX, float cameraDifferenceY) {
	if (input->wasKeyPressed(VK_SPACE))
	{
		float currentAngle = player->getRadians();
		player->startJump(currentAngle, frameTime);
		player->setFrameDelay(playerNS::PLAYER_ANIMATION_DELAY);


	}
	if (player->getCurrentFrame() == playerNS::PLAYER_END_FRAME)
	{
		player->setFrameDelay(AnimationDelayStop);
		player->setCurrentFrame(0);
	}

	player->jump(frameTime, cameraDifferenceX, cameraDifferenceY);

}
void LastManStanding::obstaclesMovement() 
{
	Obstacle1->update(frameTime);
	Obstacle2->update(frameTime);


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


	// This is for sine wave

	if (Obstacle2->getX() >= (BackgroundWidth / 20 * 3))
		Obstacle2->setMovementState("LEFT");
	else if (Obstacle2->getX() <= (BackgroundWidth / 20 * 2))
		Obstacle2->setMovementState("RIGHT");


	if (Obstacle2->getMovementState() == "RIGHT") 
	{
		Obstacle2->setX(Obstacle2->getX() + camera->getCameraHorizontalSpeed()*80*frameTime);
		Obstacle2->setY(GAME_HEIGHT / 2 + sin(Obstacle2->getX() * 1 / 10) * 20000 * frameTime);
	}
	else if (Obstacle2->getMovementState() == "LEFT") {
		Obstacle2->setX(Obstacle2->getX() - camera->getCameraHorizontalSpeed()*80*frameTime);
		Obstacle2->setY(sin(Obstacle2->getX() * 1 / 10) * 20000 * frameTime + GAME_HEIGHT / 2);
	}

}
//=============================================================================
// Handle collisions
//=============================================================================
void LastManStanding::collisions(Timer *gameTimer) {
	
	//////////////////////////////////////////////////////////////////////////////////////////////
	if (currentGameState == "IN-GAME") 
	{
		VECTOR2 collisionVector;
		//Event/Scenario:
		// 1) The player collided with Osbtacle1
		if (player1->collidesWith(*Obstacle1, collisionVector))
		{
			//what happens after collision
			player1->setX(player1->getX() - collisionVector.x*frameTime * 2);

		}
	}
}

//=============================================================================
// Render game items
//=============================================================================
void LastManStanding::render()
{
	
	graphics->spriteBegin();                // begin drawing sprites
	if (currentGameState == "IN-LOBBY") {
		LobbyBackgroundImage.draw();
		ID1Image.draw();
		ID2Image.draw();
		ID3Image.draw();
	}
	if (currentGameState == "IN-GAME") {
		BackgroundImage.draw();
		if (numOfPlayers == 1)
			player1->draw();
		if (numOfPlayers == 2) {
			player1->draw();
			player2->draw();
		}
		Obstacle1->draw();
		Obstacle2->draw();
		camera->setCameraState("MOVING");
	}
	if (camera)
	{
		camera->setTransform(graphics);
	}
	for each (Heart *heartTemp in heartList)
	{
		heartTemp->draw();
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

