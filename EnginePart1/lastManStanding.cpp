#include "lastManStanding.h"
#include "player.h"
#include "textDX.h"
#include <string>
#include <Mmsystem.h>
#include <mciapi.h>
#include "zombie.h"
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
	srand(time(NULL));
	hpText = new TextDX();
	isPaused = false;
	showInstruction = false;
	pausedText = new TextDX();
	startText = new TextDX();
	instructionsText = new TextDX();
	quitText = new TextDX();
	isDead = false;
	deadText = new TextDX();
	currentGameTime = new TextDX();
	zombieKillCountText = new TextDX();
	isExploded = false;
	testZombie = NULL;
	nextShootTime = 0;
	menuOptionNo = 2; //2 is start, 1 is instructions 0 is quit game
	for (int i = 0; i < 30; i++) {
		float randomType = static_cast<int>(rand()) / (static_cast<int> (RAND_MAX / 2)) + 1;
		obsTypeList.push_back(randomType);
	}
	nextHitTime = 0;

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
	camera = new Camera(GAME_WIDTH,GAME_HEIGHT,0, DirectX::XMFLOAT3(1.0f, 1.0f, 1.0f),&mainPlayer);

	// main player textures
	if (!PLAYER_SHOOTING_TILE_TEXTURE.initialize(graphics, PLAYER_SHOOTING_TILE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing game textures"));
	if(!mainPlayer.initialize(this, playerNS::PLAYER_SHOOTING_WIDTH, playerNS::PLAYER_SHOOTING_HEIGHT, playerNS::PLAYER_SHOOTING_TEXTURE_COLS, &PLAYER_SHOOTING_TILE_TEXTURE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing player"));
	/*mainPlayer.setPositionVector(GAME_WIDTH / 2, GAME_WIDTH / 2);
	mainPlayer.setSpriteDataXnY(GAME_WIDTH / 2, GAME_WIDTH / 2);*/

	mainPlayer.setPositionVector(LEVEL1_TILE_HEIGHT*LEVEL1_TILE_SCALE / 2, LEVEL1_TILE_WIDTH*LEVEL1_TILE_SCALE / 2);
	mainPlayer.setSpriteDataXnY(LEVEL1_TILE_HEIGHT*LEVEL1_TILE_SCALE / 2, LEVEL1_TILE_WIDTH*LEVEL1_TILE_SCALE / 2);

	mainPlayer.setCollisionRadius(100);
	//initialize bullet texture here
	if (!BULLET_TEXTURE.initialize(graphics, BULLET_TILE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing bullet texture"));
	
	
	
	
	
	if(!instructionTexture.initialize(graphics,INSTRUCTION_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing instruction texture"));


	if(!instructionImage.initialize(graphics,INSTRUCTION_WIDTH,INSTRUCTION_HEIGHT,0,&instructionTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing instructionImage"));



	if(!cursorTexture.initialize(graphics,CURSOR_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing cursorTexture texture"));
	if (!cursorImage.initialize(graphics, cursor_WIDTH, cursor_HEIGHT, 0, &cursorTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing cursorImage"));




	if (!barrelExplosionTexture.initialize(graphics, OBS1_TILE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing barrelExplosion texture"));
	if(!barrelExplosionImage.initialize(graphics,450,450,4,&barrelExplosionTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing explosionImage"));
	
	
	if (!ZOMBIE_MOVING_TEXTURE.initialize(graphics, ZOMBIE_MOVING_TILE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing zombie texture"));

	//implement the LEVEl1_TILE_TEXTURE texture here
	if (!LEVEL1_TILE_TEXTURE.initialize(graphics, LEVEL1_TILE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing texture"));

	////implement the LEVEL1_TILE_IMAGE image here
	//if (!LEVEL1_TILE_IMAGE.initialize(graphics, LEVEL1_TILE_WIDTH, LEVEL1_TILE_HEIGHT, 0, &LEVEL1_TILE_TEXTURE))
	//	throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing "));

	if (!backgroundImage.initialize(this, backgroundNS::LEVEL1_TILE_WIDTH, backgroundNS::LEVEL1_TILE_HEIGHT, 0, &LEVEL1_TILE_TEXTURE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing image"));

	if (!healthBarGreenTexture.initialize(graphics, HEALTHBARGREEN_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing healthBarGreen texture"));

	if (!healthBarGreen.initialize(graphics, 256, 32, 1, &healthBarGreenTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing healthBarGreen"));

	if (!healthBarBackGroundTexture.initialize(graphics, HEALTHBARBACKGROUND_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing healthBarBackGround texture"));

	if (!healthBarBackGround.initialize(graphics, 256, 32, 1, &healthBarBackGroundTexture))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing healthBarBackGround"));

	if (hpText->initialize(graphics, 32, true, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing hpText font"));

	if (pausedText->initialize(graphics, 30, true, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing pausedText font"));

	if (startText->initialize(graphics, 30, false, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing pausedText font"));
	if (instructionsText->initialize(graphics, 30, false, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing pausedText font"));
	if (quitText->initialize(graphics, 30, false, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing pausedText font"));


	if (deadText->initialize(graphics, 30, true, false, "Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing pausedText font"));
	if(currentGameTime->initialize(graphics,20,true,false,"Arial") == false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing currentGameTime font"));

	if(zombieKillCountText->initialize(graphics,20,true,false,"Arial")== false)
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing zombieKillCountText font"));

	if (!healthBarRedTexture.initialize(graphics, HEALTHBARRED_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing healthBarRed texture"));

	if (!enemyHealthBarBackGroundTexture.initialize(graphics, HEALTHBARBACKGROUND_IMAGE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing healthBarBackGround texture"));


	///////////////////////////////////////////////////////////////////////////////////////////////
	// Wx Here

	if(!BARREL_TEXTURE.initialize(graphics,BARREL_TILE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Barrel texture"));
	if(!OBS1_TEXTURE.initialize(graphics,OBS1_TILE))
		throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing OBS1 texture"));

	for (int i = 0; i < obsTypeList.size(); i++) 
	{
		//the address of tempObstacle keeps changing in lastManStanding.h fyi
		tempObstacle = new Obstacle();
		//rng here
		float randomX = static_cast<float>(rand()) / (static_cast<float> (RAND_MAX / (LEVEL1_TILE_HEIGHT*LEVEL1_TILE_SCALE/* - LEVEL1_TILE_HEIGHT*0.1*/)));
		float randomY = static_cast<float>(rand()) / (static_cast<float> (RAND_MAX / (LEVEL1_TILE_WIDTH*LEVEL1_TILE_SCALE/* - LEVEL1_TILE_WIDTH*0.1*/)));
		//check if the type = "1" which represent barrel
		if (obsTypeList[i] == 1) {
			//need to random the position of the barrels
			if(!tempObstacle->initialize(this,&BARREL_TEXTURE, randomX, randomY,1))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Barrel"));

			//then push it back to the obstacleList
			tempObstacle->setCollisionRadius(10.0f);
			obstacleList.push_back(tempObstacle);

			//then gg
		}
		else if (obsTypeList[i] == 2) 
		{
			//need to random the position of the barrels
			if (!tempObstacle->initialize(this, &OBS1_TEXTURE, randomX, randomY, 2))
				throw(GameError(gameErrorNS::FATAL_ERROR, "Error initializing Barrel"));
			//then push it back to the obstacleList
			tempObstacle->setCollisionRadius(10.0f);
			obstacleList.push_back(tempObstacle);
			//then gg
		}
	}

	///////////////////////////////////////////////////////////////////////////////////////////////


	//damn annoying when debug so many times Xddd
	mciSendString("open \"audio\\deathSong.wav\" type waveaudio alias sound", NULL, 0, NULL);
	mciSendString("open \"audio\\backGroundMusic.wav\" type waveaudio alias backGroundMusic", NULL, 0, NULL);

	//set x or set y for the initial position vector of the object
	//	LEVEL1_TILE_IMAGE.setScale(LEVEL1_TILE_SCALE);


	mainPlayer.playerCurrentHp = PLAYER_MAXHP;

	barrelExplosionImage.setFrames(0, 9);
	barrelExplosionImage.setFrameDelay(0.1f);
	barrelExplosionImage.setScale(0.3f);

	healthBarGreen.setX(mainPlayer.getX() - 8);
	healthBarGreen.setY(mainPlayer.getY() - 5);
	healthBarBackGround.setX(mainPlayer.getX() - 8);
	healthBarBackGround.setY(mainPlayer.getY() - 5);
	
	//instructionImage.setX(camera->getCameraX());
	//instructionImage.setY(camera->getCameraY());

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
	if (mainPlayer.getX() <= 0)
	{
		mainPlayer.setPositionVector(mainPlayer.getX() + playerNS::PLAYER_MOVEMENTSPEED * frameTime, mainPlayer.getY());
	}
	else if (mainPlayer.getY() <= 0)
	{
		mainPlayer.setPositionVector(mainPlayer.getX(), mainPlayer.getY() + playerNS::PLAYER_MOVEMENTSPEED *frameTime);
	}
	else if ((mainPlayer.getX() + mainPlayer.getWidth()) >= LEVEL1_TILE_WIDTH*2.2)
	{
		mainPlayer.setPositionVector(mainPlayer.getX() - playerNS::PLAYER_MOVEMENTSPEED * frameTime, mainPlayer.getY());
	}
	else if ((mainPlayer.getY() + mainPlayer.getHeight()) >= LEVEL1_TILE_HEIGHT*2.13)
	{
		mainPlayer.setPositionVector(mainPlayer.getX(), mainPlayer.getY() - playerNS::PLAYER_MOVEMENTSPEED * frameTime);
	}
	
	backgroundImage.update(frameTime);
	barrelExplosionImage.update(frameTime);
	instructionImage.update(frameTime);
	mainPlayer.update(frameTime);
	cursorImage.update(frameTime);
	float test = gameTimer->getCurrentElapsedTime(isPaused);
	
	mciSendString("play backGroundMusic", NULL, 0, NULL);
	if (isPaused)
	{
		if (menuOptionNo == 2)
		{
			startText->setFontColor(graphicsNS::YELLOW);
			instructionsText->setFontColor(graphicsNS::BLACK);
			quitText->setFontColor(graphicsNS::BLACK);
		}
		if (menuOptionNo == 1)
		{
			startText->setFontColor(graphicsNS::BLACK);
			instructionsText->setFontColor(graphicsNS::YELLOW);
			quitText->setFontColor(graphicsNS::BLACK);
		}
		if (menuOptionNo == 0)
		{
			startText->setFontColor(graphicsNS::BLACK);
			instructionsText->setFontColor(graphicsNS::BLACK);
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
		/*if (input->wasKeyPressed(VK_ESCAPE))
		{
			isPaused = !isPaused;
		}*/
		if (input->wasKeyPressed(VK_RETURN))
		{
			if (menuOptionNo == 2)
			{
				isPaused = !isPaused;
			}
			else if (menuOptionNo == 1)
			{
				//show instructions here
				showInstruction = true;
				if ( input->wasKeyPressed(VK_ESCAPE))
				{
					showInstruction = false;
				}
			}
			else if (menuOptionNo == 0)
			{
				PostQuitMessage(0);
			}
		}
		if (showInstruction)
		{
			if (input->wasKeyPressed(VK_ESCAPE))
			{
				showInstruction = false;
			}
		}
	}
	else if (isDead)
	{
		mciSendString("stop backGroundMusic", NULL, 0, NULL);
		mciSendString("play sound", NULL, 0, NULL);
		
		//PlaySound("audio\\deathSong.wav", NULL, SND_FILENAME);
		if (input->wasKeyPressed(VK_ESCAPE))
		{
			PostQuitMessage(0);
		}
		
	}
	else
	{
		auto test = &mainPlayer;
		if (camera) 
		{
			if (input->isKeyDown(VK_F5)) {
				if (!camera->isFollowing()) {
					camera->Follow(&mainPlayer);
				}
			}

			if (input->isKeyDown(VK_F6)) {
				if (camera->isFollowing()) {
					camera->UnFollow();
				}
			}

			camera->Update();
		}
		//update the animation here
		
		healthBarBackGround.update(frameTime);
		//healthBarRed.update(frameTime);
		////PLAYER_SHOOTING_TILE_IMAGE.setFrameDelay(playerNS::PLAYER_SHOOTING_ANIMATION_DELAY);
		if (zombieList.size() != 0) {
			for each (Zombie *zombie in zombieList) {
				zombie->update(frameTime);
			}
		}

		


		//////// Make Player Face Mouse!
		VECTOR2 playerPosition = VECTOR2(mainPlayer.getCenterX(), mainPlayer.getCenterY());
		POINT mousePos;
		GetCursorPos(&mousePos);
		VECTOR2 mousePosVector = VECTOR2(mousePos.x, mousePos.y);
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
		float dx = playerPosition.x - (mousePosVector.x + cameraDifferenceX);
		float dy = playerPosition.y - (mousePosVector.y + cameraDifferenceY);
		float rotation = (atan2(dy, dx)) * 180 / PI;
		mainPlayer.setDegrees(rotation + 180);

		//show new cursor
		ShowCursor(false);
		cursorImage.setX(mousePosVector.x + cameraDifferenceX);
		cursorImage.setY(mousePosVector.y + cameraDifferenceY);

		

		////////////////////////////////////////////////////////////////////////////
		if (input->isKeyDown('A'))  //left arrow key is pressed down
		{
			//PLAYER_SHOOTING_TILE_IMAGE.setX(PLAYER_SHOOTING_TILE_IMAGE.getX() - frameTime * PLAYER_MOVEMENTSPEED);
			//PLAYER_SHOOTING_TILE_IMAGE.setDegrees(180);
			mainPlayer.setX(mainPlayer.getX() - frameTime * playerNS::PLAYER_MOVEMENTSPEED);
			//mainPlayer.setSpriteDataXnY(mainPlayer.getX() - frameTime * playerNS::PLAYER_MOVEMENTSPEED, mainPlayer.getY());
			//mainPlayer.setDegrees(180);

			if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
			{
				//PLAYER_SHOOTING_TILE_IMAGE.setFrameDelay(0.05f);
				mainPlayer.setFrameDelay(0.05f);
				if (nextShootTime < gameTimer->getCurrentElapsedTime(isPaused))
				{
					mainPlayer.shootBullet(BULLET_TEXTURE, &mainPlayer, this, mainPlayer.getDegrees());
					nextShootTime = gameTimer->getCurrentElapsedTime(isPaused) + 0.5;
				}
			}

		}
		else if (input->isKeyDown('D')) //right arrow key is pressed down
		{
			//PLAYER_SHOOTING_TILE_IMAGE.setX(PLAYER_SHOOTING_TILE_IMAGE.getX() + frameTime * PLAYER_MOVEMENTSPEED);
			//PLAYER_SHOOTING_TILE_IMAGE.setDegrees(0);
			mainPlayer.setX(mainPlayer.getX() + frameTime * playerNS::PLAYER_MOVEMENTSPEED);
			//mainPlayer.setSpriteDataXnY(mainPlayer.getX() + frameTime * playerNS::PLAYER_MOVEMENTSPEED, mainPlayer.getY());
			//mainPlayer.setDegrees(0);
			if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
			{
				//PLAYER_SHOOTING_TILE_IMAGE.setFrameDelay(0.05f);
				mainPlayer.setFrameDelay(0.05f);
				if (nextShootTime < gameTimer->getCurrentElapsedTime(isPaused))
				{
					mainPlayer.shootBullet(BULLET_TEXTURE, &mainPlayer, this, mainPlayer.getDegrees());
					nextShootTime = gameTimer->getCurrentElapsedTime(isPaused) + 0.5;
				}
			}
		}
		else if (input->isKeyDown('W')) // up arrow key is pressed down
		{
			//PLAYER_SHOOTING_TILE_IMAGE.setY(PLAYER_SHOOTING_TILE_IMAGE.getY() - frameTime * PLAYER_MOVEMENTSPEED);
			//PLAYER_SHOOTING_TILE_IMAGE.setDegrees(270);
			mainPlayer.setY(mainPlayer.getY() - frameTime * playerNS::PLAYER_MOVEMENTSPEED);
			//mainPlayer.setSpriteDataXnY(mainPlayer.getX(), mainPlayer.getY() - frameTime * playerNS::PLAYER_MOVEMENTSPEED);
			//mainPlayer.setDegrees(270);
			if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
			{
				//PLAYER_SHOOTING_TILE_IMAGE.setFrameDelay(0.05f);
				mainPlayer.setFrameDelay(0.05f);
				if (nextShootTime < gameTimer->getCurrentElapsedTime(isPaused))
				{
					mainPlayer.shootBullet(BULLET_TEXTURE, &mainPlayer, this, mainPlayer.getDegrees());
					nextShootTime = gameTimer->getCurrentElapsedTime(isPaused) + 0.5;
				}
			}
		}
		else if (input->isKeyDown('S'))// down arrow key is pressed down
		{
			//PLAYER_SHOOTING_TILE_IMAGE.setY(PLAYER_SHOOTING_TILE_IMAGE.getY() + frameTime * PLAYER_MOVEMENTSPEED);
			//PLAYER_SHOOTING_TILE_IMAGE.setDegrees(90);
			mainPlayer.setY(mainPlayer.getY() + frameTime * playerNS::PLAYER_MOVEMENTSPEED);
			//mainPlayer.setSpriteDataXnY(mainPlayer.getX(), mainPlayer.getY() + frameTime * playerNS::PLAYER_MOVEMENTSPEED);
			//mainPlayer.setDegrees(90);
			if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
			{
				//PLAYER_SHOOTING_TILE_IMAGE.setFrameDelay(0.05f);
				mainPlayer.setFrameDelay(0.05f);
				if (nextShootTime < gameTimer->getCurrentElapsedTime(isPaused))
				{
					mainPlayer.shootBullet(BULLET_TEXTURE, &mainPlayer, this, mainPlayer.getDegrees());
					nextShootTime = gameTimer->getCurrentElapsedTime(isPaused) + 0.5;
				}
			}
		}
		else if ((GetKeyState(VK_LBUTTON) & 0x100) != 0)
		{
			//Shooting animation
			//PLAYER_SHOOTING_TILE_IMAGE.setFrameDelay(0.05f);
			
			//To minus HP of Player by 5.
			float currentHpBarPercentage = mainPlayer.playerCurrentHp / PLAYER_MAXHP;
			healthBarGreen.setPercentage(currentHpBarPercentage);
			mainPlayer.setFrameDelay(0.05f);
			if (nextShootTime < gameTimer->getCurrentElapsedTime(isPaused))
			{
				mainPlayer.shootBullet(BULLET_TEXTURE, &mainPlayer, this, mainPlayer.getDegrees());
				nextShootTime = gameTimer->getCurrentElapsedTime(isPaused) + 0.5 ;//set 0.5 seconds delay per shot
			}

		}
		else if (input->wasKeyPressed(VK_ESCAPE))
		{
			//To Recover 5 health.
			//mainPlayer.playerCurrentHp = mainPlayer.playerCurrentHp + 5;
			float currentHpBarPercentage = mainPlayer.playerCurrentHp / PLAYER_MAXHP;
			healthBarGreen.setPercentage(currentHpBarPercentage);
			isPaused = !isPaused;
		}
		else
		{
			//PLAYER_SHOOTING_TILE_IMAGE.setFrameDelay(999);
			mainPlayer.setFrameDelay(999);
		}


		if (mainPlayer.playerCurrentHp<= 0)
		{
			isDead = true;
		}
		else
		{
			isDead = false;
		}
		
		healthBarGreen.setX(camera->getCameraX() - GAME_WIDTH/2);
		healthBarGreen.setY(camera->getCameraY() - GAME_HEIGHT/2);
		healthBarBackGround.setX(camera->getCameraX() - GAME_WIDTH/2);
		healthBarBackGround.setY(camera->getCameraY() - GAME_HEIGHT/2);

		healthBarGreen.setRect();
		instructionImage.setX(camera->getCameraX() - GAME_WIDTH / 2);
		instructionImage.setY(camera->getCameraY() - GAME_HEIGHT / 2);
		//edit here to chnage the direction of the bullet
		mainPlayer.moveBullet(frameTime);
	}

	for each (Obstacle *obs in obstacleList) 
	{
		obs->update(frameTime);
	}


	///////////////////////////////////////////////////////////////////////////////
}



//=============================================================================
// Handle collisions
//=============================================================================
void LastManStanding::collisions(Timer *gameTimer) {
	VECTOR2 collisionVector;
	VECTOR2 collisionVector2;
	//this is where the magic happens

	////////////////////////////////////////////////////////////////////////////////////////////////
	// Scenario : Player colliding with other objects
	// LJ
	for each (Obstacle * obs in obstacleList)
	{
		obs->setCollisionRadius(100);
		if (mainPlayer.collidesWith(*obs, collisionVector)) {
			(mainPlayer).bounce(collisionVector, *obs);
			VECTOR2 unitCollisionVector;
			Vector2Normalize(&unitCollisionVector, &collisionVector);
			mainPlayer.setX(mainPlayer.getX() - unitCollisionVector.x * frameTime*100.0f);
			mainPlayer.setY(mainPlayer.getY() - unitCollisionVector.y * frameTime*100.0f);
			mainPlayer.setSpriteDataXnY(mainPlayer.getX(),mainPlayer.getY());
		}
	}

	/////////////////////////////////////////////////////////////////////////////////////////////////
	// Scenario : Zombie boss tossing other zombies to the main player
	// WX
	for each (Zombie *boss in zombieList) 
	{
		if (!boss->getIsBoss())
			continue;
		else 
		{
			// check the surrounding of the zombie boss
			boss->setCollisionRadius(200);
			for each (Zombie *smallZombie in zombieList) 
			{

				if (smallZombie->getIsBoss())
					continue;
				else 
				{
					if (boss->collidesWith(*smallZombie, collisionVector))
					{
						// boost 300 only
						smallZombie->attackPlayer(&mainPlayer, frameTime, 300.0f);
					}
					else
						continue;
				}
			}
			break;
		}
	}
	////////////////////////////////////////////////////////////////////////////////////////////
	//Scenario : When the Boss collides with the environment
	// WX
	if (zombieList.size() > 0 && obstacleList.size() > 0)
	{
		for (list<Zombie*>::iterator zombie = zombieList.begin(); zombie != zombieList.end();) {
			if (!(*zombie)->getIsBoss())
				zombie++;
			else {
				for (list<Obstacle*>::iterator obs = obstacleList.begin(); obs != obstacleList.end();) {
					if ((*zombie)->collidesWith(**obs, collisionVector)) {
						// destroy the objects around the boss
						SAFE_DELETE(*obs);
						obs = obstacleList.erase(obs);
					}
					else
						obs++;
				}

				zombie++;
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////////////

	////////////////////////////////////////////////////////////////////////////////////////////////
	//Scenario : Zombie collide with mainPlayer and vice-versa
	//wx
	////////////////////////////////////////////////////////////////////////////////////////////////
	if (zombieList.size() != 0) {
		for (list<Zombie*>::iterator it = zombieList.begin(); it != zombieList.end(); ) {
			if ((*it)->collidesWith(mainPlayer, collisionVector))
			{
				//the magic is here
				//LOL
				//the zombie has already collided

				// if "" less than the next to kill
				if (nextHitTime < currentGameTimeCpp->getCurrentElapsedTime(isPaused))
				{
					if (mainPlayer.playerCurrentHp > 0)
					{
						mainPlayer.playerCurrentHp -= 10;

					}
					nextHitTime = currentGameTimeCpp->getCurrentElapsedTime(isPaused) + 2; //delay 2 seconds per hit by zombie(s)
					float currentHpBarPercentage = mainPlayer.playerCurrentHp / PLAYER_MAXHP;
					healthBarGreen.setPercentage(currentHpBarPercentage);
					//set the zombie already collided
					(*it)->setIsCollided(true);
				}
				
				else {
					//set the zombie not collided
					(*it)->setIsCollided(false);
					it++;
				}
				
			}
			else {
				//no collision with the player
				it++;
			}
		}
	}
	/////////////////////////////////////////////////////////////////////////////////////////
	// Scenario : Zombie collide with Bullet and vice-versa
	//Darren
	/////////////////////////////////////////////////////////////////////////////////////////
	for (list<Zombie*>::iterator z = zombieList.begin(); z != zombieList.end();)
	{
		for (list<Bullet*>::iterator it = mainPlayer.BULLET_LIST.begin(); it != mainPlayer.BULLET_LIST.end(); )
		{
			Bullet *tempBullet = *it;
			if ((*z)->collidesWith(*tempBullet, collisionVector))
			{
				(*z)->zombieCurrentHP -= 35;
				float currentHpBarPercentage = ((*z)->zombieCurrentHP) / ((*z)->zombieMaxHp);
				(*z)->setPercentage(currentHpBarPercentage);
				(*z)->setIsCollided(true);
				SAFE_DELETE(*it);
				it = mainPlayer.BULLET_LIST.erase(it);
			}
			else
			{
				it++;
			}
		}

		if ((*z)->getIsCollided())
		{
			if ((*z)->zombieCurrentHP <= 0)
			{
				mainPlayer.zombieKillCount += 1;
				SAFE_DELETE(*z);
				z = zombieList.erase(z);
			}
			else
			{
				//need to tweak here as the zobie isCollide need to use in scenario below
				(*z)->setIsCollided(false);
				z++;
			}
		}
		else
		{
			z++;
		}
	}
	///////////////////////////////////////////////////////////////////
	//// Scenario : Bullet Collide with obstacle & obstacle checks for surrounding zombies & surround mainPlayer
	// HQL
	///////////////////////////////////////////////////////////////////
	for (list<Bullet*>::iterator bullet = mainPlayer.BULLET_LIST.begin(); bullet != mainPlayer.BULLET_LIST.end();) 
	{
		for (list<Obstacle*>::iterator obs = obstacleList.begin(); obs != obstacleList.end();) 
		{
			//reduce the collision radius to 10
			//so that zombies/AI can collide with it
			//immitating a pixel perfect scenario
			(*obs)->setCollisionRadius(10.0f);
			if ((*obs)->collidesWith(**bullet, collisionVector)) 
			{
				//add another for loop here to check surrounding zombies near the obstacle
				// check for the radius of the obstacle
				// aka increase the radius of that shit
				// when got time check for the mainPlayer also :D

				//purposely make the radius big for the obs
				//catch any zombie in the circle radius :D
				(*obs)->setCollisionRadius(500);
 				for (list<Zombie*>::iterator zombie = zombieList.begin(); zombie != zombieList.end();) 
				{
					//now check if the zombie is within the radius of the object
					if ((*zombie)->collidesWith(**obs, collisionVector)) 
					{
						// instant death for the damn zombie
						//imba
						(*zombie)->zombieCurrentHP -= (*zombie)->zombieCurrentHP;
						float currentHpBarPercentage = ((*zombie)->zombieCurrentHP) / ((*zombie)->zombieMaxHp);
						(*zombie)->setPercentage(currentHpBarPercentage);
						(*zombie)->setIsCollided(true);
						mainPlayer.zombieKillCount += 1;
						SAFE_DELETE(*zombie);
						zombie = zombieList.erase(zombie);
					}
					else 
					{
						zombie++;
						
					}
				} 
				// end of zombie for-loop
				//zombie near the radius of the barrel is dead

				(*obs)->setIsCollided(true);
				(*bullet)->setIsCollided(true);

			}
			//check if the obs also collides with the player
			//not an else statement as multiple conditions may meet
			if ((*obs)->collidesWith(mainPlayer, collisionVector)) 
			{
				mainPlayer.playerCurrentHp -= 30;
				float currentHpBarPercentage = mainPlayer.playerCurrentHp / PLAYER_MAXHP;
				healthBarGreen.setPercentage(currentHpBarPercentage);
				//after decreasing the player's health
				//remove the objects that has collided
				(*obs)->setIsCollided(true);

			}
			
			//last check if obs has collided and remove them
			if ((*obs)->getIsCollided())
			{
				//if the obstacles collides with the bullet
				barrelExplosionImage.setX((*obs)->getCenterX()- (barrelExplosionImage.getWidth()*0.3/2));
				barrelExplosionImage.setY((*obs)->getCenterY()- (barrelExplosionImage.getHeight()*0.3/2));
				barrelExplosionImage.setToFrame(0);
				isExploded = true;
				SAFE_DELETE(*obs);
				obs = obstacleList.erase(obs);
			}
			else
			{
				//reduce the collision radius to 10
				//so that zombies/AI can collide with it
				//immitating a pixel perfect scenario
				(*obs)->setCollisionRadius(10.0f);
				obs++;
			}
		}
		// end of obs loop

		//check if bullet has collided
		if ((*bullet)->getIsCollided()) 
		{
			SAFE_DELETE(*bullet);
			bullet = mainPlayer.BULLET_LIST.erase(bullet);
		}
		else 
		{
			bullet++;
		}
	}
	//// end of bullet loop


	///////////////////////////////////////////////////////////////////////////////////////////////
	//Scenario : zombie wallked near the obstacles and needs to move away in another direction
	// this should be ez
	// 1. track the previous node/obstacle
	// 2.bounch but not to the opposite side but to an angle;
	// HQL
	///////////////////////////////////////////////////////////////////////////////////////////////
	for (list<Zombie*>::iterator zombie = zombieList.begin(); zombie != zombieList.end();) 
	{
		//check will all obstacles 
		for (list<Obstacle*>::iterator obs = obstacleList.begin(); obs != obstacleList.end();) 
		{
			//check if the zombie collided with the obs
			if ((*zombie)->collidesWith(**obs, collisionVector)) 
			{
				(*zombie)->bounce(collisionVector, **obs);
				//get the unit Vector of the collision Vector
				VECTOR2 unitCollisionVector; 
				//VECTOR2(*unitCollisionVector, *collisionVector);
				Vector2Normalize(&unitCollisionVector, &collisionVector);

				float elapsedTime = gameTimer->getCurrentElapsedTime(isPaused);
				(*zombie)->setX((*zombie)->getX() - unitCollisionVector.x*frameTime * 300.0f);
				(*zombie)->setSpriteDataX((*zombie)->getX());
				(*zombie)->setY((*zombie)->getY() - unitCollisionVector.y*frameTime * 300.0f);
				(*zombie)->setSpriteDataY((*zombie)->getY());
			
				obs++;
			}
			else 
			{
				obs++;
			}

		}

		zombie++;
	}

	//////////////////////////////////////////////////////////////////////////////////////////////////////


	///////////////////////////////////////////////////////////////////////////////////////////////////
	//Scenario : zombie collided with one another
	// Darren
	for each (Zombie *zombie in zombieList) 
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
	}
	//////////////////////////////////////////////////////////////////////////////////////////////
}

//=============================================================================
// Render game items
//=============================================================================
void LastManStanding::render()
{
	// need to de-structure the method here as the player has multiple animations
	// aka need to render again?

	graphics->spriteBegin();                // begin drawing sprites
	backgroundImage.draw();
	if (camera) 
	{
		camera->setTransform(graphics);
	}
	//LEVEL1_TILE_IMAGE.draw();
	//PLAYER_SHOOTING_TILE_IMAGE.draw();
	//PLAYER_RELOADING_IMAGE.draw();
	mainPlayer.draw();
	try {
		mainPlayer.drawBullets();
	}
	catch (exception e) {
		throw(e);
	}

	//testBullet.draw();
	
	//mainPlayer.drawBullets();
	currentGameTime->setFontColor(graphicsNS::BLACK);
	int totalSeconds = this->currentGameTimeCpp->getCurrentElapsedTime(isPaused);
	int hours = totalSeconds / 3600;
	int minutes = (totalSeconds / 60) % 60;
	int seconds = totalSeconds % 60;
	string currentTimeString ="Elapsed Time:" + to_format(hours) + ":" + to_format(minutes) + ":" + to_format(seconds);
	if (totalSeconds == 1)
	{
		isPaused = true;
	}
	hpText->setFontColor(graphicsNS::WHITE);
	

	///////////////////////////////////////////////////////////////////////////////////////////
	//Wx Here

	//tempObstacle->draw();
	//draw each object here
	try {
		for (list<Obstacle*>::iterator it = obstacleList.begin(); it != obstacleList.end();) {
			(*it)->draw();
			it++;
		}
	}
	catch (exception e) {
		throw(e);
	}
	

	///////////////////////////////////////////////////////////////////////////////////////////
	healthBarBackGround.draw();
	healthBarGreen.draw();
	hpText->print(to_string((int)(mainPlayer.playerCurrentHp)) + "/" + to_string((int)(PLAYER_MAXHP)), camera->getCameraX() - (GAME_WIDTH / 2)*0.9, camera->getCameraY() - GAME_HEIGHT / 2);
	
	if (isDead)
	{
		deadText->setFontColor(graphicsNS::RED);
		deadText->print("YOU DIED! Press Esc to close game", camera->getCameraX() - GAME_WIDTH / 2, camera->getCameraY() - GAME_HEIGHT / 2);
	}

	drawZombieAIs();
	
	
	zombieKillCountText->setFontColor(graphicsNS::BLACK);
	zombieKillCountText->print("Zombie Kill Count:"+to_string(mainPlayer.zombieKillCount), camera->getCameraX() - (GAME_WIDTH / 2), camera->getCameraY() + GAME_HEIGHT / 2.2);
	currentGameTime->print(currentTimeString, camera->getCameraX() + GAME_WIDTH / 3.6, camera->getCameraY() + GAME_HEIGHT / 2.2);
	if (isExploded)
	{
		barrelExplosionImage.draw();
		if (barrelExplosionImage.getCurrentFrame() == barrelExplosionImage.getEndFrame())
		{
			isExploded = false;
		}
	}
	cursorImage.setScale(cursor_Scale);
	cursorImage.draw();
	if (isPaused)
	{
		
		backgroundImage.draw();

		//startText->setFontColor(graphicsNS::BLACK);
		//instructionsText->setFontColor(graphicsNS::BLACK);
		//quitText->setFontColor(graphicsNS::BLACK);
		startText->print("START", camera->getCameraX() - (GAME_WIDTH / 2), camera->getCameraY()  - GAME_HEIGHT / 2 );
		instructionsText->print("INSTRUCTIONS", camera->getCameraX() - (GAME_WIDTH / 2), camera->getCameraY() + 30 - GAME_HEIGHT / 2);
		quitText->print("QUIT GAME", camera->getCameraX() - (GAME_WIDTH / 2), camera->getCameraY() + 60 - GAME_HEIGHT / 2);

		if (showInstruction)
		{
			instructionImage.setVisible(true);
			instructionImage.draw();
			pausedText->setFontColor(graphicsNS::WHITE);
			pausedText->print("Press Esc To Go Back To Menu", camera->getCameraX() - GAME_WIDTH / 2, camera->getCameraY() - GAME_HEIGHT / 2);
		}
		else if (showInstruction)
		{
			instructionImage.setVisible(false);

		}
		
		
	}
	
	
	
	
	graphics->spriteEnd();                  // end drawing sprites

}

//=============================================================================
// Artificial Intelligence
//=============================================================================
void LastManStanding::ai(Timer *gameTimer)
{

	if (isPaused)
	{

	}
	else if (isDead)
	{

	}
	else
	{

		int numOfSecondsPassed = int(gameTimer->getCurrentElapsedTime(isPaused));
		// check if the time passed is a multiple of 5
		if (numOfSecondsPassed % 5 == 0 && numOfSecondsPassed != 0 && numOfSecondsPassed != nextIntervalValue) {

			nextIntervalValue = numOfSecondsPassed;
			testZombie = new Zombie();
			//testZombie->initialize(graphics, ZOMBIE_MOVING_TEXTURE, testZombie->ZOMBIE_MOVING_IMAGE);
			if (numOfSecondsPassed % 3 == 0)
			{
				testZombie->initialize(this, zombieNS::ZOMBIE_MOVING_WIDTH, zombieNS::ZOMBIE_MOVING_HEIGHT, zombieNS::ZOMBIE_MOVING_COLS, &ZOMBIE_MOVING_TEXTURE, &healthBarRedTexture, &enemyHealthBarBackGroundTexture, graphics);
				testZombie->setIsBoss();
				testZombie->setScale(1.1);
				testZombie->setZombieMaxHP(300);
			}
			else
				testZombie->initialize(this, zombieNS::ZOMBIE_MOVING_WIDTH, zombieNS::ZOMBIE_MOVING_HEIGHT, zombieNS::ZOMBIE_MOVING_COLS, &ZOMBIE_MOVING_TEXTURE, &healthBarRedTexture, &enemyHealthBarBackGroundTexture, graphics);

			//have to do rng here
			int condition = 0;
			float x;
			float y;

			//redo this algo again when have time
			while (!condition) {
				float x2 = static_cast<float>(rand()) / (static_cast<float> (RAND_MAX / GAME_WIDTH));
				float y2 = static_cast<float>(rand()) / (static_cast<float> (RAND_MAX / GAME_HEIGHT));

				//if ((x2 <= (PLAYER_SHOOTING_TILE_IMAGE.getCenterX() - (PLAYER_SHOOTING_TILE_IMAGE.getWidth() / 2) * 10)) || (x2 >= (PLAYER_SHOOTING_TILE_IMAGE.getCenterX() + (PLAYER_SHOOTING_TILE_IMAGE.getWidth() / 2) * 10)))
				if ((x2 <= (mainPlayer.getCenterX() - (mainPlayer.getWidth() / 2) * 10)) || (x2 >= (mainPlayer.getCenterX() + (mainPlayer.getWidth() / 2) * 10)))
				{
					//continue the loop xd
					int x = 2;
				}
				else
				{
					x = x2;
					y = y2;
					condition = 1;
				}
			};
			//testZombie->setPositionVector(testZombie->ZOMBIE_MOVING_IMAGE, PLAYER_SHOOTING_TILE_IMAGE.getCenterX(), PLAYER_SHOOTING_TILE_IMAGE.getCenterY(), zombieNS::ZOMBIE_MOVING_SCALE, zombieNS::ZOMBIE_MOVING_START_FRAME, zombieNS::ZOMBIE_MOVING_END_FRAME, zombieNS::ZOMBIE_MOVING_ANIMATION_DELAY);
			testZombie->setPositionVector(x, y);

			//each zombie the speed rng

			//add the zombie to the array
			zombieList.push_back(testZombie);
		}

		//then attack the player
		// need to check for collision here
		// let's do a cristmas maneuver
		for each (Zombie * zombie in zombieList) {
			zombie->attackPlayer(&mainPlayer, frameTime, 30.0f);

		}

	}
}

//=============================================================================
// The graphics device was lost.
// Release all reserved video memory so graphics device may be reset.
//=============================================================================
void LastManStanding::releaseAll()
{
	LEVEL1_TILE_TEXTURE.onLostDevice();

	Game::releaseAll();
	return;

}

//=============================================================================
// The grahics device has been reset.
// Recreate all surfaces.
//=============================================================================
void LastManStanding::resetAll()
{
	LEVEL1_TILE_TEXTURE.onResetDevice();

	Game::resetAll();
	return;

}

void LastManStanding::drawZombieAIs() {
	if (zombieList.size() != 0) {
		//for (list<Zombie*>::iterator it = zombieList.begin(); it != zombieList.end(); ) {
		//	// for testing purposes
		//	// destroy the zombie when the zombie is around the player
		//	if ()
		//	{
		//		SAFE_DELETE(*it);
		//		it = BULLET_LIST.erase(it);
		//		//just to check here
		//		int check = BULLET_LIST.size();
		//		//bool test = false;
		//	}
		//	else {
		//		it++;
		//	}
		//}

		//when all okay go and draw the bullet
		for each(Zombie* zombie in zombieList)
		{
			zombie->draw();
			zombie->enemyHealthBarBackGround->draw();
			zombie->healthBarRed->draw();
			//zombie.zombieHpText.print(to_string((int)zombie->zombieCu
			zombie->getZombieHPText()->print(to_string((int) zombie->getCurrentZombieHP())+ "/" + to_string((int)zombie->getZombieMaxHp()),zombie->getX(),zombie->getY()-5);
		}
	}
}
bool LastManStanding::checkIsDead()
{
	return isDead;
}

