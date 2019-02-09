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
#include <iostream>//These 2 for input output of the damn textfile bois
#include <fstream>//
// Need to link with Ws2_32.lib
#pragma comment (lib, "Ws2_32.lib")
#pragma comment(lib, "Winmm.lib")
#include <stdio.h>
#include <iostream>
#include <chrono>
#include <ctime>    

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
	currentGameState = "MENU";
	socketData = new SocketData();
	numOfPlayers = 1;
	timer = new Timer();
	menuOn = true;
	startText = new TextDX();
	instructionsText = new TextDX();
	quitText = new TextDX();
	leaderBoardText = new TextDX();
	nameText = new TextDX();
	scoreText = new TextDX();
	menuOptionNo = 4;
	countDownOn = false;
	camera = new Camera(GAME_WIDTH, GAME_HEIGHT, 0, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f));

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

void LastManStanding::lobbyInitialize() 
{
	obstaclesInitialize(true);

	// Connect to the server //////////////////////////////////////////////////////////////
	gameClient = new GameClient();
	gameClient->createClient();
	connectedToServer = gameClient->ConnectToServer();
	/////////////////////////////////////////////////////////////////////////////////////////


	//Initialize the 1st player's selectiion
	if (!ID1Texture.initialize(graphics, ID1))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing lobbyBackgroundTexture"));
	//ID1Image = new Image();
	if (!ID1Image.initialize(graphics, 426, 720, 0, &ID1Texture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing lobbyBackgroundTexture"));

	ID1Image.setX(0);
	ID1Image.setY(0);
	ID1Image.setCurrentFrame(0);

	//numOfPlayers++;
}

void LastManStanding::MenuInitialize()
{

	fontBig.initialize(graphics, 256, false, false, "Arial Bold");
	fontBig.setFontColor(graphicsNS::RED);
	// init the texts for menu 
	if(!instructionTexture.initialize(graphics,INSTRUCTIONPAGE_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing InstructionsTexture"));

	if (!InstructionImage.initialize(graphics, GAME_WIDTH, GAME_HEIGHT, 0, &instructionTexture))
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing instructionImage"));
	}

	if (startText->initialize(graphics, 30, false, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing pausedText font"));
	if (instructionsText->initialize(graphics, 30, false, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing pausedText font"));
	if (quitText->initialize(graphics, 30, false, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing pausedText font"));
	if (nameText->initialize(graphics,30,false,false,"Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing name font"));
	if (scoreText->initialize(graphics, 30, false, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing score font"));
	if (leaderBoardText->initialize(graphics, 30, false, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing leaderBoardText font"));

}

//=============================================================================
// Initializes the game
// Throws GameError on error
//=============================================================================
void LastManStanding::initialize(HWND hwnd)
{
	//this only happens when the player has joined the lobby
	Game::initialize(hwnd); // throws GameError
	//lobbyInitialize();
	//Initialize the background for the main menu
	if (!cursorTexture.initialize(graphics, CURSO_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing cursorTexture texture"));
	if (!cursorImage.initialize(graphics, cursor_WIDTH, cursor_HEIGHT, 0, &cursorTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing cursorImage"));
	cursorImage.setScale(cursor_Scale);


	if (!LobbyBackgroundTexture.initialize(graphics, LobbyBackground))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing lobbyBackgroundTexture"));
	if (!LobbyBackgroundImage.initialize(graphics, LobbyBackgroundWidth, LobbyBackgroundHeight, 0, &LobbyBackgroundTexture)) {
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing lobbybackgroundImage"));
	}
	LobbyBackgroundImage.setCurrentFrame(0);
	LobbyBackgroundImage.setX(0);
	LobbyBackgroundImage.setY(0);

	if (!BackgroundTexture.initialize(graphics, Background))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing BackgroundTexture"));
	if (!BackgroundImage.initialize(graphics, BackgroundWidth, BackgroundHeight, 0, &BackgroundTexture)) {
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing backgroundImage"));
	}

	if(!DeathImageTexture.initialize(graphics, DEATH_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing BackgroundTexture"));

	if (!DeathImage.initialize(graphics, GAME_WIDTH, GAME_HEIGHT, 0, &DeathImageTexture))
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing deathImage"));
	}
	mciSendString("open \"audio\\deathSong.wav\" type waveaudio alias sound", NULL, 0, NULL);
	BackgroundImage.setCurrentFrame(0);
	BackgroundImage.setX(0);
	BackgroundImage.setY(0);

	countDownTimer = COUNT_DOWN;
	camera->setCameraState("STOP");
	MenuInitialize();
	ifstream leaderBoard;
	leaderBoard.open("scoreBoard.txt");
	if (!leaderBoard)
	{
		throw(GameError(gameErrorNS::FATAL_ERROR, "Failed to open scoreboard text file")); // just some error checking bois.
	}
	//leaderBoard << "DarrenTest786";
	int tempScore = 0;
	string tempPlayerName;
	while (leaderBoard >> tempPlayerName >> tempScore)
	{
		TextDX *tempText = new TextDX();
		if (tempText->initialize(graphics, 30, false, false, "Arial") == false)
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing tempText font"));
		leaderboardTextList.push_back(tempText);
		nameVector.push_back(tempPlayerName);
		scoreVector.push_back(tempScore);

	}

	leaderBoard.close();
	


	return;
}

void LastManStanding::obstaclesInitialize(bool value) 
{
	if (value == true) 
	{
		//Attributes
		// 1) Create the obstacle object first because the pointer is null at first in lastManStanding.h file 
		Obstacle1 = new Obstacle();
		Obstacle2 = new Obstacle();
		Obstacle3 = new Obstacle();
		Obstacle4 = new Obstacle();
		Obstacle5 = new Obstacle();

		// 2) Then add it to the list one by one
		obstacleList.push_back(Obstacle1);
		obstacleList.push_back(Obstacle2);
		obstacleList.push_back(Obstacle3);
		obstacleList.push_back(Obstacle4);
		obstacleList.push_back(Obstacle5);

		//methods || functions
		// 3) Initialize the textures of the obstacles first
		if (!ObsTexture.initialize(graphics, OBS1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Texture"));

		if (!Obs2Texture.initialize(graphics, OBS1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Texture"));

		if (!Obs3Texture.initialize(graphics, OBS1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Texture"));

		if (!Obs4Texture.initialize(graphics, OBS1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Texture"));

		if (!Obs5Texture.initialize(graphics, OBS1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Texture"));


		// 4) Then initialize the obstacle object (this is not an image object)
		// Change the magic numbers to constants in the future
		//backgroundWidth is 12800 pixels
		if (!Obstacle1->initialize(this, &ObsTexture, GAME_WIDTH / 4 * 3, GAME_HEIGHT / 10 * 7.4, 1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Texture"));

		if (!Obstacle2->initialize(this, &Obs2Texture, BackgroundWidth / 20 * 2, GAME_HEIGHT / 10 * 5, 1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Texture"));

		if (!Obstacle3->initialize(this, &Obs3Texture, BackgroundWidth / 20 * 4, GAME_HEIGHT / 10 * 5, 1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Texture"));

		if (!Obstacle4->initialize(this, &Obs3Texture, BackgroundWidth / 20 * 6, GAME_HEIGHT / 10 * 5, 1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Texture"));

		if (!Obstacle5->initialize(this, &Obs3Texture, BackgroundWidth / 20 * 8, GAME_HEIGHT / 10 * 2, 1))
			throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Texture"));


		// 5) Then set the states here
		//set the states here
		Obstacle1->setMovementState("UP");
		Obstacle2->setMovementState("RIGHT");
		Obstacle3->setMovementState("RIGHT");
		Obstacle4->setMovementState("RIGHT");
		Obstacle5->setMovementState("ANTI-CLOCKWISE");
		// 6) Then go to obstaclesMovement() function to make the obstacles move
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
	POINT mousePos;
	GetCursorPos(&mousePos);
	VECTOR2 mousePosVector = VECTOR2(mousePos.x, mousePos.y);
	cursorImage.setX(mousePosVector.x + camera->returnCameraDifferenceX() - cursorImage.getWidth()*cursor_Scale/2);
	cursorImage.setY(mousePosVector.y - cursorImage.getHeight()*cursor_Scale);
	ShowCursor(false);
	//timePassed = std::time(0) - t;
	if (currentGameState == "MENU") {
		BackgroundImage.update(frameTime);

		//KING OF THE SPEGHETTI IS HERE



		int textWidthStart = startText->GetTextWidth("START", startText->getFont());
		int textHeightStart = startText->GetTextHeight("START", startText->getFont());

		int textWidthLeaderBoard = leaderBoardText->GetTextWidth("LEADERBOARD", leaderBoardText->getFont());
		int textHeightLeaderBoard = leaderBoardText->GetTextHeight("LEADERBOARD", leaderBoardText->getFont());

		int textWidthInstructions = instructionsText->GetTextWidth("INSTRUCTIONS", instructionsText->getFont());
		int textHeightInstructions = instructionsText->GetTextHeight("INSTRUCTIONS", instructionsText->getFont());

		int textWidthQuit = quitText->GetTextWidth("QUIT GAME", quitText->getFont());
		int textHeightQuit = quitText->GetTextHeight("QUIT GAME", quitText->getFont());

		





		if (menuOptionNo == 3)
		{
			startText->setFontColor(graphicsNS::YELLOW);
			leaderBoardText->setFontColor(graphicsNS::WHITE);
			instructionsText->setFontColor(graphicsNS::WHITE);
			quitText->setFontColor(graphicsNS::WHITE);
		}
		if (menuOptionNo == 2)
		{
			startText->setFontColor(graphicsNS::WHITE);
			leaderBoardText->setFontColor(graphicsNS::YELLOW);
			instructionsText->setFontColor(graphicsNS::WHITE);
			quitText->setFontColor(graphicsNS::WHITE);
		}
		if (menuOptionNo == 1)
		{
			startText->setFontColor(graphicsNS::WHITE);
			leaderBoardText->setFontColor(graphicsNS::WHITE);
			instructionsText->setFontColor(graphicsNS::YELLOW);
			quitText->setFontColor(graphicsNS::WHITE);
		}
		if (menuOptionNo == 0)
		{
			startText->setFontColor(graphicsNS::WHITE);
			leaderBoardText->setFontColor(graphicsNS::WHITE);
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
			if (menuOptionNo != 3)
			{
				menuOptionNo += 1;
			}
		}
		if (isKeyingInName)
		{
			//player1->setName(input->getTextIn());
			
			if (input->wasKeyPressed(VK_SPACE))
			{
				isKeyingInName = false;
				menuOptionNo = 3;
				tempName = input->getTextIn();
			}

		}
		if (input->wasKeyPressed(VK_RETURN) || (GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			if (menuOptionNo == 3 && !isKeyingInName)
			{
				//clear the damn key first
				input->clearAll();
				//this is where the state is changed to "PRE-LOBBY"
				currentGameState = "PRE-LOBBY";
				menuOn = !menuOn;
				countDownOn = true;
				

				
			}
			else if(menuOptionNo == 2)
			{
				isShowingLeaderBoard = true;
			}

			else if (menuOptionNo == 1)
			{
				//show instructions here
				isShowingInstruction = true;

			}
			else if (menuOptionNo == 0)
			{
				PostQuitMessage(0);
			}
		}
		if (isShowingInstruction &&input->wasKeyPressed(VK_ESCAPE))
		{
			isShowingInstruction = false;
		}
		if (isShowingLeaderBoard &&input->wasKeyPressed(VK_ESCAPE))
		{
			isShowingLeaderBoard = false;
		}

		if (camera) {
			camera->Update();
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
	if (currentGameState == "PRE-LOBBY")
	{
		lobbyInitialize();
		currentGameState = "IN-LOBBY";
	}
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
			/*if (input->wasKeyPressed(0x0D)) {
				if (numOfPlayersVoted == 0)
					numOfPlayersVoted++;
			}*/
			//check if the player is connected to the server first
			if (connectedToServer == 0)
				currentGameState = "LOADING-GAME";
			else 
			{
				if (input->wasKeyPressed(0x0D)) {
					if (numOfPlayersVoted == 0)
						numOfPlayersVoted++;
				}
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
		if (numOfPlayers == 1) {
			t = std::time(0);
			currentGameState = "PRE-GAME";
		}
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
					t = std::time(0);
					currentGameState = "PRE-GAME";
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
					if (allPlayerLoaded == numOfPlayers) {
						t = std::time(0);
						currentGameState = "PRE-GAME";
					}
					
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
	if (currentGameState == "PRE-GAME") 
	{
		//timePassed = std::time(0) - t;
		//this->startGame(cameraDifferenceX, cameraDifferenceY);
		//countDownTimer = COUNT_DOWN;
		timePassed = std::time(0) - t;
		camera->setCameraState("STOP");
		long int timePassedint = static_cast<long int> (timePassed);
		if (countDownOn)
		{
			countDownTimer = COUNT_DOWN - timePassedint;
			if (countDownTimer < 0)
			{
				countDownOn = false;
				//camera->setCameraHorizontalSpeed(0.3f);
				//camera->setCameraState("MOVING");
				//currentGameState = "IN-GAME";
			}
		}
		BackgroundImage.update(frameTime);
		if (camera) {
			camera->Update();
		}

	}
	else if (currentGameState == "IN-GAME") 
	{	
		//darren's start
		if (player1->getNumberOfLifes() < 1)
		{
			isDead = true;
			camera->setCameraState("STOP");
			//darrenishere
			ofstream leaderBoardWrite;
			leaderBoardWrite.open("scoreBoard.txt", std::ios_base::app);
			if (leaderBoardWrite.is_open() && !isRecorded)
			{
				leaderBoardWrite << "\n" + player1->getName() + " " + to_string(player1->getScore()) ;
				isRecorded = true;
				leaderBoardWrite.close();
			}
			// store some data here later.
		}
		if (isDead)
		{
			if (input->wasKeyPressed(VK_ESCAPE))
			{
				PostQuitMessage(0);
			}
		}

		/*long int timePassedint = static_cast<long int> (timePassed);
		if (countDownOn)
		{
			countDownTimer = COUNT_DOWN - timePassedint;
			if (countDownTimer < 0)
			{
				countDownOn = false;
				camera->setCameraHorizontalSpeed(0.3f);
			}
		}*/
		float cameraDifferenceX = 0;
		float cameraDifferenceY = 0;
		int tempScore = 0;

		if ((camera->getCameraX() + GAME_WIDTH / 2) > GAME_WIDTH)
		{
			cameraDifferenceX = (camera->getCameraX() + GAME_WIDTH / 2) - GAME_WIDTH;
		}
		if ((camera->getCameraY() + GAME_HEIGHT / 2) > GAME_HEIGHT)
		{
			cameraDifferenceY = (camera->getCameraY() + GAME_HEIGHT / 2) - GAME_HEIGHT;
		}

		//scoring system created by your boy
		if (i == cameraDifferenceX)
		{
			counted = true;
		}
		if (i < cameraDifferenceX)
		{
			counted = false;
			i = i + 50;  // adding the damn score every 50 pixels bois
		}
		while (counted == false)
		{
			tempScore = player1->getNumberOfLifes();
			player1->setScore(player1->getScore() + tempScore);
			counted = true;
		}

		if (input->wasKeyPressed(VK_ESCAPE))
		{
			//change the state here instead
			menuOn = !menuOn;
			currentGameState = "MENU";
		}

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
	// 7) Update the obstacles here first with frameTime
	for each(Obstacle * obs in obstacleList) {
		obs->update(frameTime);
	}

	// 8) Change the state of the obstacle after a condition you specified
	// 9) change what happens when the obstacle is in that state

	// Obstacle1 moves up and down ez
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

	//this is for cosine wave

	if (Obstacle3->getX() >= (BackgroundWidth / 20 * 5))
		Obstacle3->setMovementState("LEFT");
	else if (Obstacle3->getX() <= (BackgroundWidth / 20 * 4))
		Obstacle3->setMovementState("RIGHT");


	if (Obstacle3->getMovementState() == "RIGHT")
	{
		Obstacle3->setX(Obstacle3->getX() + camera->getCameraHorizontalSpeed() * 80 * frameTime);
		Obstacle3->setY(GAME_HEIGHT / 2 + cos(Obstacle3->getX() * 1 / 10) * 20000 * frameTime);
	}
	else if (Obstacle3->getMovementState() == "LEFT") {
		Obstacle3->setX(Obstacle3->getX() - camera->getCameraHorizontalSpeed() * 80 * frameTime);
		Obstacle3->setY(cos(Obstacle3->getX() * 1 / 10) * 20000 * frameTime + GAME_HEIGHT / 2);
	}

	//this is for tangent wave
	if (Obstacle4->getX() >= (BackgroundWidth / 20 * 7))
		Obstacle4->setMovementState("LEFT");
	else if (Obstacle4->getX() <= (BackgroundWidth / 20 * 6))
		Obstacle4->setMovementState("RIGHT");


	if (Obstacle4->getMovementState() == "RIGHT")
	{
		Obstacle4->setX(Obstacle4->getX() + camera->getCameraHorizontalSpeed() * 80 * frameTime);
		Obstacle4->setY(GAME_HEIGHT / 2 + tan(Obstacle4->getX() * 500) * 1 / 2 * frameTime);
	}
	else if (Obstacle4->getMovementState() == "LEFT") {
		Obstacle4->setX(Obstacle4->getX() - camera->getCameraHorizontalSpeed() * 80 * frameTime);
		Obstacle4->setY(tan(Obstacle4->getX() * 500) * 1/2 * frameTime + GAME_HEIGHT / 2);
	}

	//this is for circular motion wave
	//Formula:
	// 1) X := OriginX + cos(angle)*radius
	// 20 Y := OriginY + sin(angle)*radius

	/*if (Obstacle4->getX() >= (BackgroundWidth / 20 * 7))
		Obstacle4->setMovementState("LEFT");
	else if (Obstacle4->getX() <= (BackgroundWidth / 20 * 6))
		Obstacle4->setMovementState("RIGHT");
*/

	if (Obstacle5->getMovementState() == "ANTI-CLOCKWISE")
	{
		obstacle5Angle += camera->getCameraHorizontalSpeed()*0.005;
		Obstacle5->setX(Obstacle5->getX() + cos(obstacle5Angle)*obstacle5Radius*frameTime);
		Obstacle5->setY(Obstacle5->getY() + sin(obstacle5Angle)*obstacle5Radius*frameTime);
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

void LastManStanding::drawObstacles() 
{
	for each (Obstacle * obs in obstacleList) {
		obs->draw();
	}
}
void LastManStanding::render()
{
	
	graphics->spriteBegin();                // begin drawing sprites
	

	if (currentGameState == "MENU")
	{
		BackgroundImage.draw();

		int textWidthLeaderBoard = leaderBoardText->GetTextWidth("LEADERBOARD", leaderBoardText->getFont());
		int textHeightLeaderBoard = leaderBoardText->GetTextHeight("LEADERBOARD", leaderBoardText->getFont());
		leaderBoardText->print("LEADERBOARD", camera->getCameraX() - textWidthLeaderBoard / 2, camera->getCameraY() + 30 - textHeightLeaderBoard / 2);

		int textWidthStart = startText->GetTextWidth("START", startText->getFont());
		int textHeightStart = startText->GetTextHeight("START", startText->getFont());
		startText->print("START", camera->getCameraX() - textWidthStart / 2, camera->getCameraY() - textHeightStart / 2);

		int textWidthInstructions = instructionsText->GetTextWidth("INSTRUCTIONS", instructionsText->getFont());
		int textHeightInstructions = instructionsText->GetTextHeight("INSTRUCTIONS", instructionsText->getFont());
		instructionsText->print("INSTRUCTIONS", camera->getCameraX() - textWidthInstructions / 2, camera->getCameraY() + 60 - textHeightInstructions / 2);

		int textWidthQuit = quitText->GetTextWidth("QUIT GAME", quitText->getFont());
		int textHeightQuit = quitText->GetTextHeight("QUIT GAME", quitText->getFont());
		quitText->print("QUIT GAME", camera->getCameraX() - (textWidthQuit / 2), camera->getCameraY() + 90 - textHeightQuit / 2);

		POINT mousePos;
		GetCursorPos(&mousePos);
		VECTOR2 mousePosVector = VECTOR2(mousePos.x, mousePos.y);
		mousePos.y -= 28;
		if (startText->getX() < mousePos.x  && (startText->getX() + textWidthStart) > mousePos.x && startText->getY() < mousePos.y && (startText->getY() + textHeightStart) > mousePos.y)
		{
			int test1 = startText->getX();
			int test2 = startText->getY();
			menuOptionNo = 3;
		}
		if (leaderBoardText->getX() < mousePos.x && (leaderBoardText->getX() + textWidthLeaderBoard) > mousePos.x && leaderBoardText->getY() < mousePos.y && (leaderBoardText->getY() + textHeightLeaderBoard) > mousePos.y)
		{
			menuOptionNo = 2;
		}
		if (instructionsText->getX() < mousePos.x && (instructionsText->getX() + textWidthInstructions) > mousePos.x && instructionsText->getY() < mousePos.y && (instructionsText->getY() + textHeightInstructions) > mousePos.y)
		{
			menuOptionNo = 1;
		}
		if (quitText->getX() < mousePos.x && (quitText->getX() + textWidthQuit) > mousePos.x && quitText->getY() < mousePos.y && (quitText->getY() + textHeightQuit) > mousePos.y)
		{
			menuOptionNo = 0;
		}

	}
	if (isKeyingInName)
	{
		BackgroundImage.draw();
		int textWidthName = nameText->GetTextWidth("Please Enter a Name(Press space when done typing):" + input->getTextIn(), nameText->getFont());
		int textHeightName = nameText->GetTextHeight("Please Enter a Name(Press space when done typing):" + input->getTextIn(), nameText->getFont());
		nameText->print("Please Enter a Name(Press space when done typing):" + input->getTextIn(), camera->getCameraX() - textWidthName/2, camera->getCameraY() - textHeightName/2);
	}
	
	if (isShowingInstruction)
	{
		InstructionImage.draw();
		scoreText->print("Please Press Esc to go back to Menu", camera->getCameraX() - (GAME_WIDTH / 2), camera->getCameraY() + GAME_HEIGHT / 2 - 30);
	}
	if (currentGameState == "IN-LOBBY") {
		LobbyBackgroundImage.draw();
		ID1Image.draw();
		ID2Image.draw();
		ID3Image.draw();
	}
	if (currentGameState == "PRE-GAME")
	{
		BackgroundImage.draw();
		if (numOfPlayers == 1)
			player1->draw();
		if (numOfPlayers == 2) {
			player1->draw();
			player2->draw();
		}
		/*Obstacle1->draw();
		Obstacle2->draw();*/
		drawObstacles();
		if (countDownOn)
		{
			if (countDownTimer - timePassed > 0)
			{
				int textWidth = fontBig.GetTextWidth(to_string(countDownTimer - timePassed), fontBig.getFont());
				int textHeight = fontBig.GetTextHeight(to_string(countDownTimer - timePassed), fontBig.getFont());
				fontBig.print(to_string(countDownTimer - timePassed), camera->getCameraX() - textWidth / 2, camera->getCameraY() - textHeight / 2);
			}
			else if (countDownTimer - timePassed <= 0)
			{
				int textWidth = fontBig.GetTextWidth("GO!", fontBig.getFont());
				int textHeight = fontBig.GetTextHeight("GO!", fontBig.getFont());
				fontBig.print("GO!", camera->getCameraX() - textWidth / 2, camera->getCameraY() - textHeight / 2);
				camera->setCameraState("MOVING");
				currentGameState = "IN-GAME";
			}
		}
	}
	if (currentGameState == "IN-GAME") {
		
		BackgroundImage.draw();
		if (numOfPlayers == 1)
			player1->draw();
		if (numOfPlayers == 2) {
			player1->draw();
			player2->draw();
		}
		/*Obstacle1->draw();
		Obstacle2->draw();*/
		drawObstacles();
		player1->setName(tempName);
		int textWidthName = nameText->GetTextWidth(player1->getName(), nameText->getFont());
		nameText->print(player1->getName(), player1->getCenterX() - textWidthName/2, player1->getCenterY());
		scoreText->print("Score: " + to_string(player1->getScore()), camera->getCameraX() - (GAME_WIDTH / 2), camera->getCameraY() + GAME_HEIGHT / 2 - 30);
		camera->setCameraState("MOVING");
	}
	if (camera)
	{
		camera->setTransform(graphics);
	}
	if (isDead)
	{
		mciSendString("stop backGroundMusic", NULL, 0, NULL);
		mciSendString("play sound", NULL, 0, NULL);
		DeathImage.setX(camera->getCameraX() - (GAME_WIDTH / 2));
		DeathImage.setY(camera->getCameraY() - (GAME_HEIGHT / 2));
		DeathImage.draw();
		scoreText->print(to_string(player1->getScore()), camera->getCameraX(), camera->getCameraY() - 25);
	}
	for each (Heart *heartTemp in heartList)
	{
		heartTemp->draw();
	}
	int testX = camera->getCameraX() - GAME_WIDTH / 2;
	int testY = camera->getCameraY();
	if (isShowingLeaderBoard)
	{
		BackgroundImage.draw();
		int trolololol = 0;
		for each(TextDX* tempTextDX in leaderboardTextList)
		{
			nameText->print("Name:", 300, 0);
			quitText->print("Score:", 900, 0);
			tempTextDX->print(nameVector[trolololol], 300, GAME_HEIGHT/20 * (trolololol+1));
			scoreText->print(to_string(scoreVector[trolololol]), 900, GAME_HEIGHT / 20 * (trolololol+1));
			trolololol++;
		}
		scoreText->print("Please Press Esc to go back to Menu" , camera->getCameraX() - (GAME_WIDTH / 2), camera->getCameraY() + GAME_HEIGHT / 2 - 30);
		
	}
	cursorImage.draw();


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

