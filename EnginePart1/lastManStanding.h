
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

	int numOfPlayers = 0;
	
	TextureManager BackgroundTexture;
	Image BackgroundImage;
	TextureManager heartTexture;

	TextureManager Player1Texture;
	list <Heart*>  heartList;
	Player *player1;

	TextureManager ObsTexture;
	//Image Obs1Image;

	Obstacle *Obstacle1;

	Camera *camera;

	GameClient *gameClient;

	//I need to store the game's state
	string currentGameState;

	TextureManager LobbyBackgroundTexture;
	Image LobbyBackgroundImage;

	TextureManager ID1Texture;
	Image ID1Image;

	SocketData *socketData;
	SocketData *tempSocketData;

public:
	// Constructor
	LastManStanding();

	// Destructor
	virtual ~LastManStanding();

	// Initialize the game
	void initialize(HWND hwnd);
	void obstaclesInitialize(bool value);
	void obstaclesMovement();
	void update(Timer *gameTimer);      // must override pure virtual from Game
	void ai(Timer *gameTimer);          // "
	void collisions(Timer *gameTimer);  // "
	void render();      // "
	void releaseAll();
	void resetAll();

};

#endif
