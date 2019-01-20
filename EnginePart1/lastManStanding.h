
#ifndef _LASTMANSTANDING_H             // prevent multiple definitions if this 
#define _LASTMANSTANDING_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "player.h"
#include "textDX.h"
#include <list>
#include "obstacle.h"
#include <vector>
#include "camera.h"
#include "background.h"
using namespace std;

//=============================================================================
// Create game class
//=============================================================================
class LastManStanding : public Game
{
protected:
	// variables

	//the textures & images here
	/*TextureManager LEVEL1_TILE_TEXTURE;*/
	//Image LEVEL1_TILE_IMAGE;

	TextureManager PLAYER_SHOOTING_TILE_TEXTURE;
	//Image PLAYER_SHOOTING_TILE_IMAGE;
	
	Camera *camera;

public:
	// Constructor
	LastManStanding();

	// Destructor
	virtual ~LastManStanding();

	// Initialize the game
	void initialize(HWND hwnd);
	void update(Timer *gameTimer);      // must override pure virtual from Game
	void ai(Timer *gameTimer);          // "
	void collisions(Timer *gameTimer);  // "
	void render();      // "
	void releaseAll();
	void resetAll();

};

#endif
