
#ifndef _CONSTANTS_H            // prevent multiple definitions if this 
#define _CONSTANTS_H            // ..file is included in more than one place
#define WIN32_LEAN_AND_MEAN

#include <windows.h>

//-----------------------------------------------
// Useful macros
//-----------------------------------------------
// Safely delete pointer referenced item
#define SAFE_DELETE(ptr)       { if (ptr) { delete (ptr); (ptr)=NULL; } }
// Safely release pointer referenced item
#define SAFE_RELEASE(ptr)      { if(ptr) { (ptr)->Release(); (ptr)=NULL; } }
// Safely delete pointer referenced array
#define SAFE_DELETE_ARRAY(ptr) { if(ptr) { delete [](ptr); (ptr)=NULL; } }
// Safely call onLostDevice
#define SAFE_ON_LOST_DEVICE(ptr)    { if(ptr) { ptr->onLostDevice(); } }
// Safely call onResetDevice
#define SAFE_ON_RESET_DEVICE(ptr)   { if(ptr) { ptr->onResetDevice(); } }
#define TRANSCOLOR  SETCOLOR_ARGB(0,255,0,255)  // transparent color (magenta)


//-----------------------------------------------
//                  Constants
//-----------------------------------------------
// window
const char CLASS_NAME[] = "LastManStanding";
const char GAME_TITLE[] = "LastManStanding";
const bool FULLSCREEN = false;              // windowed or fullscreen
const UINT GAME_WIDTH =  1280;               // width of game in pixels
const UINT GAME_HEIGHT = 720;               // height of game in pixels
 
// game
const double PI = 3.14159265;
const float FRAME_RATE  = 200.0f;               // the target frame rate (frames/sec)
const float MIN_FRAME_RATE = 10.0f;             // the minimum frame rate
const float MIN_FRAME_TIME = 1.0f/FRAME_RATE;   // minimum desired time for 1 frame
const float MAX_FRAME_TIME = 1.0f/MIN_FRAME_RATE; // maximum time used in calculations
const int AnimationDelayStop = 999;
// key mappings
// In this game simple constants are used for key mappings. If variables were used
// it would be possible to save and restore key mappings from a data file.
const UCHAR ESC_KEY      = VK_ESCAPE;   // escape key
const UCHAR ALT_KEY      = VK_MENU;     // Alt key
const UCHAR ENTER_KEY    = VK_RETURN;   // Enter key
const UCHAR SHIP_LEFT_KEY = VK_LEFT;
const UCHAR SHIP_RIGHT_KEY = VK_RIGHT;
const UCHAR SHIP_UP_KEY = VK_UP;
const UCHAR SHIP_DOWN_KEY = VK_DOWN;

// graphic images
// add the tile heres
const char Background[] = "pictures\\main.jpg";
const int backgroundHeight = 720;
const int backgroundWidth = 12800;

//audio files here
// audio files required by audio.cpp
// WAVE_BANK must be location of .xwb file.
const char WAVE_BANK[] = "audio\\Win\\WavesExample1.xwb";
// SOUND_BANK must be location of .xsb file.
const char SOUND_BANK[] = "audio\\Win\\SoundsExample1.xsb";

// audio cues
const char BEEP1[] = "beep1";
const char BEEP2[] = "beep2";
const char BEEP3[] = "beep3";
const char BEEP4[] = "beep4";
const char HIT[] = "hit";


//const float LEVEL1_TILE_SCALE = 2.0;

//instruction image
//const char INSTRUCTION_IMAGE[] = "pictures\\GameInstruction.png";
//const int INSTRUCTION_HEIGHT = 600;
//const int INSTRUCTION_WIDTH = 800;

//cursor Image
//const char CURSOR_IMAGE[] = "pictures\\NewCursor.png";
//const int cursor_HEIGHT = 397;
//const int cursor_WIDTH = 416;
//const float cursor_Scale = 0.15f;


//Health Bar
//const char HEALTHBARGREEN_IMAGE[] = "pictures\\healthbarGreen.png";
//const char HEALTHBARRED_IMAGE[] = "pictures\\healthbarRed.png";
//const char HEALTHBARBACKGROUND_IMAGE[] = "pictures\\healthbarBackGround.png";
//const char PLAYER_SHOOTING_TILE[] = "pictures\\PlayerShooting.png";
//

//player reloading
const char PLAYER[] = "pictures\\Player.png";
//const int PLAYER_RELOADING_START_FRAME = 0;
//const int PLAYER_RELOADING_END_FRAME = 19;
//const float PLAYER_RELOADING_ANIMATION_DELAY = 0.2f;
//const int PLAYER_RELOADING_COLS = 3;
//const int PLAYER_RELOADING_HEIGHT = 217;
//const int PLAYER_RELOADING_WIDTH = 322;
//const float PLAYER_RELOADING_SCALE = 0.3f;



//HP Bar
//const float PLAYER_MAXHP = 100.0f;




#endif
