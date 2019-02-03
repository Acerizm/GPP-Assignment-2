
#ifndef _LASTMANSTANDING_H             // prevent multiple definitions if this 
#define _LASTMANSTANDING_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "player.h"
#include "heart.h"
#include <list>
#include "obstacle.h"
#include <vector>
#include "background.h"
#include "camera.h"
#include "GameClient.h"
#include "SocketData.h"
#include <list>
#include "textDX.h"
using namespace std;

//=============================================================================
// Create game class
//=============================================================================

namespace GameStates {

};

class LastManStanding : public Game
{
protected:
	// variables

	int numOfPlayers = 1;
	int numOfPlayersVoted = 0;
	
	TextureManager BackgroundTexture;
	Image BackgroundImage;
	TextureManager heartTexture;

	TextureManager Player1Texture;
	TextureManager Player2Texture;
	TextureManager Player3Texture;
	Player *player1;
	Player *player2;
	Player *player3;

	list <Heart*>  heartList;
	TextureManager ObsTexture;
	TextureManager Obs2Texture;

	Obstacle *Obstacle1;
	Obstacle *Obstacle2;

	Camera *camera;

	GameClient *gameClient;

	TextureManager LobbyBackgroundTexture;
	Image LobbyBackgroundImage;

	//This is for the in-lobby
	TextureManager ID1Texture;
	Image ID1Image;
	Image ID2Image = {};
	Image ID3Image = {};

	SocketData *socketData;
	SocketData *tempSocketData;

	int currentPlayerID = 1;

	//States
	//I need to store the game's state
	string currentGameState;
	int drawPlayerSelectionBox = 1;

	list<Player *> playerList;
	
	//number of players loaded
	int allPlayerLoaded = 1;
	bool player2IsLoaded = false;
	bool player3IsLoaded = false;
	int player2ID = 0;
	int player3ID = 0;
	int currentClientIDConnected = 0;

	Timer *timer;
	int currentTime = 0;

	TextDX  fontBig;
	//menu options here 
	TextDX *startText;
	TextDX *instructionsText;
	TextDX *quitText;
	TextDX *scoreText;
	int menuOptionNo;
	bool menuOn;
	bool countDownOn;
	const int COUNT_DOWN = 3;
	int countDownTimer;
	time_t t;
	time_t timePassed;
	bool counted = false;
	int i = 0;

public:
	// Constructor
	LastManStanding();

	// Destructor
	virtual ~LastManStanding();

	// Initialize the game
	void initialize(HWND hwnd);
	void lobbyInitialize();
	void player1Initialize();
	void player2Initalize();
	void player3Initalize();
	void myPlayerMovement(Player * player,float cameraDifferenceX, float cameraDifferenceY);
	void obstaclesInitialize(bool value);
	void obstaclesMovement();
	void update(Timer *gameTimer);      // must override pure virtual from Game
	void ai(Timer *gameTimer);          // "
	void collisions(Timer *gameTimer);  // "
	void render();      // "
	void releaseAll();
	void resetAll();
	void MenuInitialize();
	void startGame(float cameraDifferenceX, float cameraDifferenceY);

};

#endif
