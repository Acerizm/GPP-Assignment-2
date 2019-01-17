
#ifndef _LASTMANSTANDING_H             // prevent multiple definitions if this 
#define _LASTMANSTANDING_H             // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include "game.h"
#include "textureManager.h"
#include "image.h"
#include "bullet.h"
#include "player.h"
#include "textDX.h"
#include <list>
#include "zombie.h"
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
	//Bullet *newBullet;

	//the textures & images here
	TextureManager LEVEL1_TILE_TEXTURE;
	//Image LEVEL1_TILE_IMAGE;

	TextureManager PLAYER_SHOOTING_TILE_TEXTURE;
	//Image PLAYER_SHOOTING_TILE_IMAGE;
	
	TextureManager PLAYER_RELOADING_TEXTURE;
	//Image PLAYER_RELOADING_IMAGE;

	//TextureManager healthBarRedTexture;
	//Image healthBarRed;

	TextureManager healthBarGreenTexture;
	Image healthBarGreen;

	TextureManager healthBarBackGroundTexture;
	Image healthBarBackGround;

	TextureManager BULLET_TEXTURE;
	//Image BULLET_IMAGE;

	TextureManager ZOMBIE_MOVING_TEXTURE;

	TextureManager healthBarRedTexture;

	TextureManager enemyHealthBarBackGroundTexture;

	TextureManager barrelExplosionTexture;
	Image barrelExplosionImage;

	TextureManager cursorTexture;
	Image cursorImage;

	TextureManager instructionTexture;
	Image instructionImage;

	//TextureManager ZombieBoss;

	Background backgroundImage;
	
	Player mainPlayer;
	//Bullet testBullet;
	friend class Camera;
	Zombie *testZombie;

	////////////////////////////////////////////////////////////////
	// WX Here

	// this is for the texture/spritesheet of the images(s)
	TextureManager BARREL_TEXTURE;

	Obstacle *tempObstacle;

	list<Obstacle*> obstacleList;

	vector<int> obsTypeList;

	TextureManager OBS1_TEXTURE;




	////////////////////////////////////////////////////////////////
	

	//there is a need for me to store multiple images inside the array
	// <Bullet*> bulletList;
	list<Zombie*> zombieList;
	int nextIntervalValue = 0;
	TextDX  *hpText;
	bool isPaused;
	bool isDead;
	bool isExploded;
	bool showInstruction;
	TextDX *pausedText;
	//menu texts
	TextDX *startText;
	TextDX *instructionsText;
	TextDX *quitText;
	int menuOptionNo;

	TextDX *deadText;
	TextDX *zombieKillCountText;
	TextDX *currentGameTime;
	float nextShootTime;
	float nextHitTime;

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
	void drawZombieAIs();
	bool LastManStanding::checkIsDead();

};

#endif
