/*
 * main.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: Elijah
 */

#if defined (_WIN32) || (_WIN64)

	#include <SDL.h>
	#include <SDL_image.h>
	#include <SDL_mixer.h>
	#include <direct.h>

#endif

#if defined(__APPLE__)

	#include <SDL2/SDL.h>
	#include <SDL2_image/SDL_image.h>
	#include <SDL2_mixer/SDL_mixer.h>

#endif

#if defined(__linux__)

	#include "SDL2/SDL.h"
	#include "SDL2/SDL_image.h"
	#include "SDL2/SDL_mixer.h"

#endif

#if defined (_linux_)
#include <unistd.h>
#endif

#include <stdio.h>
#include <iostream>
using namespace std;

//#define getcwd _getcwd

//create the SDL_Rectangle for the texture's position and size -x,y,w,h
SDL_Rect bkgd1Pos;

//create the SDL_Rectangle for the texture's position and size -x,y,w,h
SDL_Rect bkgd2Pos;

//set speed for background
int bkgdSpeed = 100;

//set temp variables to hold movement - background 1
float BG1pos_x = 0, BG1pos_y = 0;

//set temp variables to hold movement - background 2
float BG2pos_x = 0, BG2pos_y = -768;

//code for frame rate independence
float deltaTime = 0.0;
int thisTime = 0;
int lastTime = 0;

void UpdateBackground()
{
	//Update background 1
	BG1pos_y += (bkgdSpeed * 1) * deltaTime;

	//set the new bkgd1 position
	bkgd1Pos.y = (int)(BG1pos_y + 0.5f);

	//reset when off the bottom of the screen
	if(bkgd1Pos.y >= 768)
	{
		bkgd1Pos.y =-768;

		BG1pos_y = bkgd1Pos.y;
	}

	//Update background 1
	BG2pos_y += (bkgdSpeed * 1) * deltaTime;

	//set the new bkgd1 position
	bkgd2Pos.y = (int)(BG2pos_y + 0.5f);

	//reset when off the bottom of the screen
	if(bkgd2Pos.y >= 768)
	{
		bkgd2Pos.y =-768;

		BG2pos_y = bkgd2Pos.y;
	}
}

const int JOYSTICK_DEAD_ZONE = 8000;

float xDir, yDir;

float pos_X, pos_Y;

SDL_Rect cursorPos, activePos;

int cursorSpeed = 400;

void moveCursor (const SDL_ControllerAxisEvent event)
{
	if (event.which == 0)
	{
		if(event.axis == 0)
		{
			if (event.value < -JOYSTICK_DEAD_ZONE)
			{
				xDir = -1.0f;
			}
			else if(event.value > JOYSTICK_DEAD_ZONE)
			{
				xDir = 1.0f;
			}
			else
			{
				xDir = 0.0f;
			}
		}

		if(event.axis == 1)
		{
			if (event.value < -JOYSTICK_DEAD_ZONE)
			{
				yDir = -1.0f;
			}
			else if(event.value > JOYSTICK_DEAD_ZONE)
			{
				yDir = 1.0f;
			}
			else
			{
				yDir = 0.0f;
			}
		}
	}
}

void UpdateCursor (float deltaTime)
{
	pos_X += (cursorSpeed * xDir) * deltaTime;
	pos_Y += (cursorSpeed * yDir) * deltaTime;

	cursorPos.x = (int)(pos_X + 0.5f);
	cursorPos.y = (int)(pos_Y + 0.5f);

	activePos.x = cursorPos.x;
	activePos.y = cursorPos.y;

	if(cursorPos.x < 0)
	{
		cursorPos.x = 0;
		pos_X = cursorPos.x;
	}

	if(cursorPos.x > 1024 - cursorPos.w)
	{
		cursorPos.x = 1024 - cursorPos.w;
		pos_X = cursorPos.x;
	}

	if(cursorPos.y < 0)
	{
		cursorPos.y = 0;
		pos_Y = cursorPos.y;
	}

	if(cursorPos.y > 768 - cursorPos.w)
	{
		cursorPos.y = 768 - cursorPos.w;
		pos_Y = cursorPos.y;
	}
}

bool players1Over = false, players2Over = false, instructionsOver = false, quitOver = false, menuOver = false, playOver = false;

#include "Player.h"

#include "enemy.h"
#include <vector>
#include <stdlib.h>
#include <time.h>
#include "explode.h"

vector<Enemy> enemyList;
vector<Explode> explodeList;

void MakeExplosion(int x, int y)
{
	cout << "Explosion being Called" << endl;
	for(int i = 0; i < explodeList.size(); i++)
	{
		if(explodeList[i].active == false)
		{
			explodeList[i].active = true;
			explodeList[i].posRect.x = x;
			explodeList[i].posRect.y = y;
			break;
		}
	}
}

int main(int argc, char* argv[]) {

srand(time(NULL));

#if defined (_WIN32) || (_WIN64)

	cout << "Running on Windows" << endl;

	string s_cwd(getcwd(NULL, 0));

	//create a string linking to the mac's images folder
	string s_cwd_images = s_cwd + "\\Resources\\Images\\";
	string audio_dir = s_cwd + "\\Resources\\Audio\\";

#endif

#if defined(__linux__)

	cout << "Running on Linux" << endl;
	cout << "Added on Linux" << endl;

	string s_cwd(getcwd(NULL, 0));

	//create a string linking to the mac's images folder
	string s_cwd_images = s_cwd + "/Resources/Images";
	string audio_dir = s_cwd + "/Resources/Audio";

#endif

#if defined(__APPLE__)

	cout << "Running on Apple" << endl;
	//get the current working directory
	string s_cwd(getcwd(NULL, 0));

	//create a string linking to the mac's images folder
	string s_cwd_images = s_cwd + "/Resources/Images";

	string audio_dir = s_cwd + "/Resources/Audio";

	//test
	cout << s_cwd_images << endl;

#endif

    SDL_Window *window;                    // Declare a pointer

    SDL_Init(SDL_INIT_EVERYTHING);              // Initialize SDL2

    // Create an application window with the following settings:
    window = SDL_CreateWindow(
        "An SDL2 window",                  // window title
        SDL_WINDOWPOS_UNDEFINED,           // initial x position
        SDL_WINDOWPOS_UNDEFINED,           // initial y position
        1024,                               // width, in pixels
        768,                               // height, in pixels
        SDL_WINDOW_OPENGL                  // flags - see below
    );

    // Check that the window was successfully created
    if (window == NULL) {
        // In the case that the window could not be made...
        printf("Could not create window: %s\n", SDL_GetError());
        return 1;
    }

    //declare renderer
    SDL_Renderer *renderer = NULL;

    //create the renderer
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);

    //******Create BackGround******
    string BKGDpath = s_cwd_images + "/Hyo.png";

    //create a SDL surface to hold the background image
    SDL_Surface *surface = IMG_Load(BKGDpath.c_str());

    //create a SDL texture
    SDL_Texture *bkgd1;

    //place surface info into the texture bkdg1
    bkgd1 = SDL_CreateTextureFromSurface(renderer, surface);

    //create a SDL texture
    SDL_Texture *bkgd2;

    //place surface info into the texture bkdg1
    bkgd2 = SDL_CreateTextureFromSurface(renderer, surface);

    //set the X,Y,W, and H for the Rectangle
    bkgd1Pos.x = 0;
    bkgd1Pos.y = 0;
    bkgd1Pos.w = 1024;
    bkgd1Pos.h = 768;

    //set the X,Y,W, and H for the Rectangle
    bkgd2Pos.x = 0;
    bkgd2Pos.y = -768;
    bkgd2Pos.w = 1024;
    bkgd2Pos.h = 768;

    //free the SDL surface
    //SDL_FreeSurface(surface);


    //********MAke Title***********
   string titlePath = s_cwd_images + "/Title.png";

   surface = IMG_Load(titlePath.c_str());

   SDL_Texture *title;

   title = SDL_CreateTextureFromSurface(renderer, surface);

   SDL_Rect titlePos;

   titlePos.x = 50;
   titlePos.y = 0;
   titlePos.w = 936;
   titlePos.h = 252;

   //********MAke 1 Player***********
  string Play1path = s_cwd_images + "/Player1Button_Hyo.png";

  surface = IMG_Load(Play1path.c_str());

  SDL_Texture *Play1;

  Play1 = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect Play1Pos;

  Play1Pos.x = 200;
  Play1Pos.y = 200;
  Play1Pos.w = 600;
  Play1Pos.h = 100;

   //********MAke 2 Player***********
  string Play2path = s_cwd_images + "/Player2Button_Hyo.png";

    surface = IMG_Load(Play2path.c_str());

    SDL_Texture *Play2;

    Play2 = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect Play2Pos;

    Play2Pos.x = 200;
    Play2Pos.y = 330;
    Play2Pos.w = 600;
    Play2Pos.h = 100;

	//********MAke 1 PlayerOff***********
	string Play1offpath = s_cwd_images + "/Player1off.png";

	surface = IMG_Load(Play1offpath.c_str());

	SDL_Texture *Play1off;

	Play1off = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_Rect Play1offPos;

	Play1offPos.x = 200;
	Play1offPos.y = 200;
	Play1offPos.w = 600;
	Play1offPos.h = 100;

	//********MAke 2 PlayerOff***********
	string Play2offpath = s_cwd_images + "/Player2off.png";

	surface = IMG_Load(Play2offpath.c_str());

	SDL_Texture *Play2off;

	Play2off = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_Rect Play2offPos;

	Play2offPos.x = 200;
	Play2offPos.y = 330;
	Play2offPos.w = 600;
	Play2offPos.h = 100;

	//********MAke Play button***********
	string Playpath = s_cwd_images + "/Play_Hyo.png";

	surface = IMG_Load(Playpath.c_str());

	SDL_Texture *PlayS;

	PlayS = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_Rect PlaySPos;

	PlaySPos.x = 700;
	PlaySPos.y = 680;
	PlaySPos.w = 324;
	PlaySPos.h = 70;

	//********MAke play off button***********
	string Playoffpath = s_cwd_images + "/Playoff_Hyo.png";

	surface = IMG_Load(Playoffpath.c_str());

	SDL_Texture *PlaySoff;

	PlaySoff = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_Rect PlaySoffPos;

	PlaySoffPos.x = 700;
	PlaySoffPos.y = 680;
	PlaySoffPos.w = 324;
	PlaySoffPos.h = 70;

   //********MAke Instructions button***********
  string instructPath = s_cwd_images + "/Instructions_Hyo.png";

  surface = IMG_Load(instructPath.c_str());

  SDL_Texture *instruct;

  instruct = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect inPos;

  inPos.x = 200;
  inPos.y = 460;
  inPos.w = 600;
  inPos.h = 100;

  //********MAke Instructions off button***********
  string instructOffPath = s_cwd_images + "/Instructionsoff_Hyo.png";

  surface = IMG_Load(instructOffPath.c_str());

  SDL_Texture *instructOff;

  instructOff = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect inOffPos;

  inOffPos.x = 200;
  inOffPos.y = 460;
  inOffPos.w = 600;
  inOffPos.h = 100;

  //********MAke Instructions text***********
  string inTextPath = s_cwd_images + "/InstructText.png";

  surface = IMG_Load(inTextPath.c_str());

  SDL_Texture *instructText;

  instructText = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect inTextPos;

  inTextPos.x = 160;
  inTextPos.y = 140;
  inTextPos.w = 720;
  inTextPos.h = 550;

  //********MAke Win text***********
  string winPath = s_cwd_images + "/winText_Hyo.png";

  surface = IMG_Load(winPath.c_str());

  SDL_Texture *winText;

  winText = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect winTextPos;

  winTextPos.x = 200;
  winTextPos.y = 100;
  winTextPos.w = 547;
  winTextPos.h = 115;

  //********MAke Lose text***********
  string losePath = s_cwd_images + "/loseText.png";

  surface = IMG_Load(losePath.c_str());

  SDL_Texture *loseText;

  loseText = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect loseTextPos;

  loseTextPos.x = 200;
  loseTextPos.y = 100;
  loseTextPos.w = 547;
  loseTextPos.h = 115;

   //********MAke Quit***********
  string quitPath = s_cwd_images + "/Quit_Hyo.png";

  surface = IMG_Load(quitPath.c_str());

  SDL_Texture *Quiti;

  Quiti = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect quitPos;

  quitPos.x = 200;
  quitPos.y = 590;
  quitPos.w = 600;
  quitPos.h = 100;

  //********MAke QuitOff***********
  string quitOffPath = s_cwd_images + "/Quitoff_Hyo.png";

  surface = IMG_Load(quitOffPath.c_str());

  SDL_Texture *Quito;

  Quito = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect quitOffPos;

  quitOffPos.x = 200;
  quitOffPos.y = 590;
  quitOffPos.w = 600;
  quitOffPos.h = 100;

  //********MAke menu button***********
  string menuButPath = s_cwd_images + "/Menu_Hyo.png";

  surface = IMG_Load(menuButPath.c_str());

  SDL_Texture *menuBut;

  menuBut = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect menuPos;

  menuPos.x = 5;
  menuPos.y = 680;
  menuPos.w = 324;
  menuPos.h = 70;

  //********MAke menu off***********
  string menuOffPath = s_cwd_images + "/Menuoff_Hyo.png";

  surface = IMG_Load(menuOffPath.c_str());

  SDL_Texture *menuOff;

  menuOff = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect menuOffPos;

  menuOffPos.x = 5;
  menuOffPos.y = 680;
  menuOffPos.w = 324;
  menuOffPos.h = 70;

    //***********Create Cursor*********
    //create cursor
    string CURSORpath = s_cwd_images + "/whatevercursor.png";

    surface = IMG_Load(CURSORpath.c_str());

    SDL_Texture *cursor;

    cursor = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    //create the SDL_Rectangle for the texture's position and size -x,y,w,h
    //SDL_Rect cursorPos, activePos;

    //set the X,Y,W, and H for the Rectangle
    cursorPos.x = 0;
    cursorPos.y = 0;
    cursorPos.w = 50;
    cursorPos.h = 50;

	activePos.x = 10;
	activePos.y = 10;
	activePos.w = 10;
	activePos.h = 10;

    //*****Turn on Game Controller Events
    SDL_GameControllerEventState(SDL_ENABLE);

    SDL_GameController* gGameController0 = NULL;

    //****Open Game Controller
    gGameController0 = SDL_GameControllerOpen(0);

    SDL_GameController* gGameController1 = NULL;

    //****Open Game Controller
    gGameController1 = SDL_GameControllerOpen(1);

    //*****SDL Event to handle input
    SDL_Event event;

    //*****set up variables for game states
    enum GameState{MENU, INSTRUCTIONS, PLAYERS1, PLAYERS2, WIN, LOSE};

    //set up the initial state
    GameState gameState = MENU;

    //boolean values to control movement through states
    bool menu, instructions, players1, players2, win, lose, quit = false;

    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048);

//    Mix_Music *bgm = Mix_LoadMUS((audio_dir + "background.mp3").c_str());
//
//    if(!Mix_PlayingMusic())
//    {
//    	Mix_PlayMusic(bgm, -1);
//    }

    Mix_Chunk *overSound = Mix_LoadWAV((audio_dir + "/over.wav").c_str());
    Mix_Chunk *pressedSound = Mix_LoadWAV((audio_dir + "/pressed.wav").c_str());
    Mix_Chunk *explosionSound = Mix_LoadWAV((audio_dir + "/explosion.wav").c_str());

    bool alreadyOver = false;

    Player player1 = Player(renderer, 0, s_cwd_images.c_str(), audio_dir.c_str(), 250.0, 500.0);
    Player player2 = Player(renderer, 1, s_cwd_images.c_str(), audio_dir.c_str(), 750.0, 500.0);

    for(int i = 0; i < 20; i++)
    {
    	Explode tmpExplode(renderer, s_cwd_images, 500, 500);
    	explodeList.push_back(tmpExplode);
    }

    while(!quit)
    {
    	switch (gameState)
    	{
    	case MENU:
    		alreadyOver = false;
    		menu = true;
    		cout << "The GameState is Menu" << endl;
    		cout << "Press the A button for instructions" << endl;
    		cout << "Press the B button for 1 Player game" << endl;
    		cout << "Press the X button for 2 Player game" << endl;
    		cout << "Press the Y button for Quit game" << endl;
    		cout << endl;

    		while (menu)
    		{
    			//set up frame rate for the section or CASE
    			thisTime = SDL_GetTicks();
    			deltaTime = (float)(thisTime - lastTime)/1000;
    			lastTime = thisTime;

    			//check for input events
    			if (SDL_PollEvent(&event))
    			{
    				//check to see if the SDL Window is closed - player clicks X in window
    				if(event.type == SDL_QUIT)
    				{
    					quit = true;
    					menu = false;
    					break;
    				}

    				switch (event.type)
    				{
    				case SDL_CONTROLLERBUTTONDOWN:
    					if (event.cdevice.which == 0)

    					{
    						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
    						{
    							if(players1Over)
    							{
    								Mix_PlayChannel(-1, pressedSound, 0);
    								menu = false;
    								gameState = PLAYERS1;
    								players1Over = false;
    							}

    							if(players2Over)
    							{
    								Mix_PlayChannel(-1, pressedSound, 0);
    								menu = false;
    								gameState = PLAYERS2;
    								players2Over = false;
    							}

    							if(instructionsOver)
    							{
    								Mix_PlayChannel(-1, pressedSound, 0);
    								menu = false;
    								gameState = INSTRUCTIONS;
    								players1Over = false;
    							}

    							if(quitOver)
    							{
    								Mix_PlayChannel(-1, pressedSound, 0);
    								SDL_Delay(200);
    								menu = false;
    								quit = true;
    								quitOver = false;
    							}
    						}
    					}
    					break;

    				case SDL_CONTROLLERAXISMOTION:
    					moveCursor(event.caxis);
    					break;
    				}
    			}

    			//Update
    			UpdateBackground();
    			UpdateCursor(deltaTime);

    			players1Over = SDL_HasIntersection(&activePos, &Play1Pos);
    			players2Over = SDL_HasIntersection(&activePos, &Play2Pos);
    			instructionsOver = SDL_HasIntersection(&activePos, &inPos);
    			quitOver = SDL_HasIntersection(&activePos, &quitPos);

    			if(players1Over || players2Over || instructionsOver || quitOver)
    			{
    				if(alreadyOver == false)
    				{
    					Mix_PlayChannel(-1, overSound, 0);
    					alreadyOver = true;
    				}
    			}

    			if(!players1Over && !players2Over && !instructionsOver && !quitOver)
    			{
    				alreadyOver = false;
    			}

    			//Start Drawing
    			//Clear SDL renderer
    			SDL_RenderClear(renderer);
    			//Draw the bkgd1 image
    			SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
    			//Draw the bkgd2 image
    			SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

    			SDL_RenderCopy(renderer, title, NULL, &titlePos);

    			if(players1Over)
    			{
    			SDL_RenderCopy(renderer, Play1off, NULL, &Play1offPos);
    			}
    			else
    			{
    			SDL_RenderCopy(renderer, Play1, NULL, &Play1Pos);
    			}

    			if(players2Over)
    			{
        		SDL_RenderCopy(renderer, Play2off, NULL, &Play2offPos);
    			}
    			else
    			{
        		SDL_RenderCopy(renderer, Play2, NULL, &Play2Pos);
    			}

    			if(instructionsOver)
    			{
    			SDL_RenderCopy(renderer, instructOff, NULL, &inOffPos);
    			}
    			else
    			{
				SDL_RenderCopy(renderer, instruct, NULL, &inPos);
    			}

    			if(quitOver)
    			{
    			SDL_RenderCopy(renderer, Quito, NULL, &quitOffPos);
    			}
    			else
    			{
				SDL_RenderCopy(renderer, Quiti, NULL, &quitPos);
    			}

    			SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

    			//SDL Render present
    			SDL_RenderPresent(renderer);

    		}
    		break; //end main menu case

    	case INSTRUCTIONS:
    		alreadyOver = false;
    		instructions = true;
    		cout << "The GameState is Instructions" << endl;
    		cout << "Press the A button for Main Menu" << endl;
    		cout << endl;

    		while (instructions)
    		{
    			thisTime = SDL_GetTicks();
    			deltaTime = (float)(thisTime - lastTime) / 1000;
    			lastTime = thisTime;
    			//check for input events
    			if (SDL_PollEvent(&event))
    			{
    				//check to see if the SDL Window is closed - player clicks X in window
    				if(event.type == SDL_QUIT)
    				{
    					quit = true;
    					instructions = false;
    					break;
    				}

    				switch (event.type)
    				{
    				case SDL_CONTROLLERBUTTONDOWN:
    					if (event.cdevice.which == 0)

    					{
    						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
    						{
    							if(menuOver)
    							{
    								Mix_PlayChannel(-1, pressedSound, 0);
    								instructions = false;
    								gameState = MENU;
    								menuOver = false;
    							}
    						}
    					}
    					break;

    				case SDL_CONTROLLERAXISMOTION:
    					moveCursor(event.caxis);
    					break;
    				}
    			}

    			//Update
    			UpdateBackground();
    			UpdateCursor(deltaTime);

    			menuOver = SDL_HasIntersection(&activePos, &menuPos);

    			if(menuOver)
    			{
    				if(alreadyOver == false)
    				{
    					Mix_PlayChannel(-1, overSound, 0);
    					alreadyOver = true;
    				}
    			}

    			if(!menuOver)
    			{
    				alreadyOver = false;
    			}

    			//Start Drawing
    			//Clear SDL renderer
    			SDL_RenderClear(renderer);
    			//Draw the bkgd1 image
    			SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
    			//Draw the bkgd2 image
    			SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

    			SDL_RenderCopy(renderer, title, NULL, &titlePos);

    			SDL_RenderCopy(renderer, instructText, NULL, &inTextPos);

    			if(menuOver)
    			{
    				SDL_RenderCopy(renderer, menuOff, NULL, &menuOffPos);
    			}
    			else
    			{
    				SDL_RenderCopy(renderer, menuBut, NULL, &menuPos);
    			}

    			SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

    			//SDL Render present
    			SDL_RenderPresent(renderer);
    		}
    		break; //end instructions case

    	case PLAYERS1:

    		enemyList.clear();
    		player1.Reset();
    		alreadyOver = false;
    		players1 = true;

    		for(int i = 0; i < 6; i++)
    		{
    			Enemy tmpEnemy(renderer, s_cwd_images);

    			enemyList.push_back(tmpEnemy);
    		}

    		while (players1)
    		{
    			thisTime = SDL_GetTicks();
    			deltaTime = (float)(thisTime - lastTime) / 1000;
    			lastTime = thisTime;
    			//check for input events
    			if (SDL_PollEvent(&event))
    			{
    				//check to see if the SDL Window is closed - player clicks X in window
    				if(event.type == SDL_QUIT)
    				{
    					quit = true;
    					players1 = false;
    					break;
    				}

    				switch (event.type)
    				{
    				case SDL_CONTROLLERBUTTONDOWN:
    					if (event.cdevice.which == 0)

    					{
    						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
    						{
    							players1 = false;
    							gameState = WIN;
    						}

    						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
    						{
    							players1 = false;
    							gameState = LOSE;
    						}
    						if(player1.active)
    						{
    							player1.OnControllerButton(event.cbutton);
    						}
    					}
    					break;

    				case SDL_CONTROLLERAXISMOTION:
    					if(player1.active)
    					{
    						player1.OnControllerAxis(event.caxis);
    					}
    					break;
    				}
    			}
    			//Update
    			UpdateBackground();

    			if(player1.active)
    			{
    				player1.Update(deltaTime, renderer);
    			}

    			for(int i = 0; i < enemyList.size(); i++)
    			{
    				enemyList[i].Update(deltaTime);
    			}

    			//Check to see if Player 1 is active
    			if(player1.active == true)
    			{
    				for (int i = 0; i < player1.bulletList.size(); i ++)
    				{
    					if(player1.bulletList[i].active == true)
    					{
    						for (int j = 0; j < enemyList.size(); j++)
    						{
    							if (SDL_HasIntersection(&player1.bulletList[i].posRect, &enemyList[j].posRect))
    							{
    								Mix_PlayChannel(-1, explosionSound, 0);
    								MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);
    								enemyList[j].Reset();
    								player1.bulletList[i].Reset();
    								player1.playerScore += 50;

    								if(player1.playerScore >= 1000)
    								{
    									players1 = false;
    									gameState = WIN;
    								}
    							}
    						}
    					}
    				}

    				for(int i = 0; i < enemyList.size(); i++)
    				{
    					if(SDL_HasIntersection(&player1.posRect, &enemyList[i].posRect))
    					{
    						Mix_PlayChannel(-1, explosionSound, 0);
    						MakeExplosion(player1.posRect.x-32, player1.posRect.y-32);
    						enemyList[i].Reset();
    						player1.playerLives -= 1;

    						if(player1.playerLives <= 0)
    						{
    							players1 = false;
    							gameState = LOSE;
    							break;
    						}
    					}
    				}
    			}
    			//Check end for PLAYER 1

    			for(int i = 0; i < explodeList.size(); i++)
    			{
    				if(explodeList[i].active == true)
    				{
    					explodeList[i].Update(deltaTime);
    				}
    			}

    			//Start Drawing
    			//Clear SDL renderer
    			SDL_RenderClear(renderer);
    			//Draw the bkgd1 image
    			SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
    			//Draw the bkgd2 image
    			SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

    			SDL_RenderCopy(renderer, Play1, NULL, &Play1Pos);

    			for(int i = 0; i < enemyList.size(); i++)
    			{
    				enemyList[i].Draw(renderer);
    			}

    			player1.Draw(renderer);

    			//SDL Render present
    			SDL_RenderPresent(renderer);
    		}
    		break; //end players1 case

    	case PLAYERS2:
    		enemyList.clear();
    		player1.Reset();
    		player2.Reset();
    		alreadyOver = false;
    		players2 = true;

    		for (int i = 0; i < 12; i++)
    		{
    			Enemy tmpEnemy(renderer, s_cwd_images);
    			enemyList.push_back(tmpEnemy);
    		}

    		while (players2)
    		{

    			thisTime = SDL_GetTicks();
    			deltaTime = (float)(thisTime - lastTime) / 1000;
    			lastTime = thisTime;
    			//check for input events
    			if (SDL_PollEvent(&event))
    			{
    				//check to see if the SDL Window is closed - player clicks X in window
    				if(event.type == SDL_QUIT)
    				{
    					quit = true;
    					players2 = false;
    					break;
    				}

    				switch (event.type)
    				{
    				case SDL_CONTROLLERBUTTONDOWN:
    					if (event.cdevice.which == 0 || event.cdevice.which == 1)

    					{
    						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
    						{
    							players2 = false;
    							gameState = WIN;
    						}

    						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
    						{
    							players2 = false;
    							gameState = LOSE;
    						}
    					}

    					if(player1.active)
    					{
    						player1.OnControllerButton(event.cbutton);
    					}

    					if(player2.active)
    					{
    						player2.OnControllerButton(event.cbutton);
    					}

    					break;

    				case SDL_CONTROLLERAXISMOTION:

    					if(player1.active)
    					{
    						player1.OnControllerAxis(event.caxis);
    					}

    					if(player2.active)
    					{
    						player2.OnControllerAxis(event.caxis);
    					}
    				}
    			}

    			//Update
    			UpdateBackground();

    			if(player1.active)
    			{
    				player1.Update(deltaTime, renderer);
    			}

    			if(player2.active)
    			{
    				player2.Update(deltaTime, renderer);
    			}

    			for (int i = 0; i < enemyList.size(); i++)
    			{
    				enemyList[i].Update(deltaTime);
    			}

    			//Check to see if Player 1 is active
    			if(player1.active == true)
    			{
    				for (int i = 0; i < player1.bulletList.size(); i ++)
    				{
    					if(player1.bulletList[i].active == true)
    					{
    						for (int j = 0; j < enemyList.size(); j++)
    						{
    							if (SDL_HasIntersection(&player1.bulletList[i].posRect, &enemyList[j].posRect))
    							{
    								Mix_PlayChannel(-1, explosionSound, 0);
    								enemyList[j].Reset();
    								player1.bulletList[i].Reset();
    								player1.playerScore += 50;
    								if(player1.playerScore >= 1000)
    								{
    									players2 = false;
    									gameState = WIN;
    								}
    							}
    						}
    					}
    				}

    				for(int i = 0; i < enemyList.size(); i++)
    				{
    					if(SDL_HasIntersection(&player1.posRect, &enemyList[i].posRect))
    					{
    						Mix_PlayChannel(-1, explosionSound, 0);
    						MakeExplosion(player1.posRect.x-32, player1.posRect.y-32);
    						enemyList[i].Reset();
    						player1.playerLives -= 1;

    						if(player1.playerLives <= 0 && player2.playerLives <= 0)
    						{
    							players2 = false;
    							gameState = LOSE;
    							break;
    						}
    					}
    				}
    			}
    			//End check of PLAYER 1

    			//Check to see if Player 2 is active
    			if(player2.active == true)
    			{
    				for (int i = 0; i < player2.bulletList.size(); i ++)
    				{
    					if(player2.bulletList[i].active == true)
    					{
    						for (int j = 0; j < enemyList.size(); j++)
    						{
    							if (SDL_HasIntersection(&player2.bulletList[i].posRect, &enemyList[j].posRect))
    							{
    								Mix_PlayChannel(-1, explosionSound, 0);
    								MakeExplosion(enemyList[j].posRect.x, enemyList[j].posRect.y);
    								enemyList[j].Reset();
    								player2.bulletList[i].Reset();
    								player2.playerScore += 50;
    								if(player2.playerScore >= 1000)
    								{
    									players2 = false;
    									gameState = WIN;
    								}
    							}
    						}
    					}
    				}

    				for(int i = 0; i < enemyList.size(); i++)
    				{
    					if(SDL_HasIntersection(&player2.posRect, &enemyList[i].posRect))
    					{
    						Mix_PlayChannel(-1, explosionSound, 0);
    						MakeExplosion(player2.posRect.x-32, player2.posRect.y-32);
    						enemyList[i].Reset();
    						player2.playerLives -= 1;

    						if(player1.playerLives <= 0 && player2.playerLives <= 0)
    						{
    							players2 = false;
    							gameState = LOSE;
    							break;
    						}
    					}
    				}
    			}
    			//End check of PLAYER 2

    			for(int i = 0; i < explodeList.size(); i++)
    			{
    				if(explodeList[i].active == true)
    				{
    					explodeList[i].Update(deltaTime);
    				}
    			}

    			//Start Drawing
    			//Clear SDL renderer
    			SDL_RenderClear(renderer);
    			//Draw the bkgd1 image
    			SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
    			//Draw the bkgd2 image
    			SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

    			SDL_RenderCopy(renderer, Play2, NULL, &Play2Pos);

    			for (int i = 0; i < enemyList.size(); i++)
    			{
    				enemyList[i].Draw(renderer);
    			}

    			player1.Draw(renderer);

    			player2.Draw(renderer);

    			//SDL Render present
    			SDL_RenderPresent(renderer);
    		}
    		break; //end players2 case

    	case WIN:
    		alreadyOver = false;
    		win = true;
    		cout << "The GameState is Win Screen" << endl;
    		cout << "Press the A button for Main Menu" << endl;
    		cout << "Press the B button to Quit" << endl;
    		cout << endl;

    		while (win)
    		{
    			thisTime = SDL_GetTicks();
    			deltaTime = (float)(thisTime - lastTime) / 1000;
    			lastTime = thisTime;
    			//check for input events
    			if (SDL_PollEvent(&event))
    			{
    				//check to see if the SDL Window is closed - player clicks X in window
    				if(event.type == SDL_QUIT)
    				{
    					quit = true;
    					win = false;
    					break;
    				}

    				switch (event.type)
    				{
    				case SDL_CONTROLLERBUTTONDOWN:
    					if (event.cdevice.which == 0)

    					{
    						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
    						{
    							if(menuOver)
    							{
    								Mix_PlayChannel(-1, pressedSound, 0);
    								win = false;
    								gameState = MENU;
    							}

    							if(playOver)
    							{
    								Mix_PlayChannel(-1, pressedSound, 0);
    								win = false;
    								gameState = PLAYERS1;
    								playOver = false;
    							}
    						}

    						//							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
    						//							{
    						//								win = false;
    						//								quit = true;
    						//							}
    					}
    					break;

    				case SDL_CONTROLLERAXISMOTION:
    					moveCursor(event.caxis);
    					break;
    				}
    			}

    			//Update
    			UpdateBackground();
    			UpdateCursor(deltaTime);

    			menuOver = SDL_HasIntersection(&activePos, &menuPos);
    			playOver = SDL_HasIntersection(&activePos, &PlaySPos);

    			if(menuOver || playOver)
    			{
    				if(alreadyOver == false)
    				{
    					Mix_PlayChannel(-1, overSound, 0);
    					alreadyOver = true;
    				}
    			}

    			if(!menuOver && !playOver)
    			{
    				alreadyOver = false;
    			}

    			//Start Drawing
    			//Clear SDL renderer
    			SDL_RenderClear(renderer);
    			//Draw the bkgd1 image
    			SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
    			//Draw the bkgd2 image
    			SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

    			SDL_RenderCopy(renderer, winText, NULL, &winTextPos);

    			if(menuOver)
    			{
    				SDL_RenderCopy(renderer, menuOff, NULL, &menuOffPos);
    			}
    			else
    			{
    				SDL_RenderCopy(renderer, menuBut, NULL, &menuPos);
    			}

    			if(playOver)
    			{
    				SDL_RenderCopy(renderer, PlaySoff, NULL, &PlaySoffPos);

    			}
    			else
    			{
    				SDL_RenderCopy(renderer, PlayS, NULL, &PlaySPos);
    			}

    			SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

    			//SDL Render present
    			SDL_RenderPresent(renderer);
    		}
    		break; //end win case

    	case LOSE:
    		alreadyOver = false;
    		lose = true;
    		cout << "The GameState is Lose Screen" << endl;
    		cout << "Press the A button for Main Menu" << endl;
    		cout << "Press the B button to Quit" << endl;
    		cout << endl;

    		while (lose)
    		{
    			thisTime = SDL_GetTicks();
    			deltaTime = (float)(thisTime - lastTime) / 1000;
    			lastTime = thisTime;
    			//check for input events
    			if (SDL_PollEvent(&event))
    			{
    				//check to see if the SDL Window is closed - player clicks X in window
    				if(event.type == SDL_QUIT)
    				{
    					quit = true;
    					lose = false;
    					break;
    				}

    				switch (event.type)
    				{
    				case SDL_CONTROLLERBUTTONDOWN:
    					if (event.cdevice.which == 0)

    					{
    						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
    						{
    							if(menuOver)
    							{
    								Mix_PlayChannel(-1, pressedSound, 0);
    								lose = false;
    								gameState = MENU;
    								menuOver = false;
    							}

    							if(playOver)
    							{
    								Mix_PlayChannel(-1, pressedSound, 0);
    								lose = false;
    								gameState = PLAYERS1;
    								playOver = false;
    							}
    						}

    						//							if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
    						//							{
    						//								lose = false;
    						//								quit = true;
    						//							}
    					}
    					break;

    				case SDL_CONTROLLERAXISMOTION:
    					moveCursor(event.caxis);
    					break;
    				}
    			}

    			//Update
    			UpdateBackground();
    			UpdateCursor(deltaTime);

    			menuOver = SDL_HasIntersection(&activePos, &menuPos);
    			playOver = SDL_HasIntersection(&activePos, &PlaySPos);

    			if(menuOver || playOver)
    			{
    				if(alreadyOver == false)
    				{
    					Mix_PlayChannel(-1, overSound, 0);
    					alreadyOver = true;
    				}
    			}

    			if(!menuOver && !playOver)
    			{
    				alreadyOver = false;
    			}

    			//Start Drawing
    			//Clear SDL renderer
    			SDL_RenderClear(renderer);
    			//Draw the bkgd1 image
    			SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);
    			//Draw the bkgd2 image
    			SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

    			SDL_RenderCopy(renderer, loseText, NULL, &loseTextPos);

    			if(menuOver)
    			{
    				SDL_RenderCopy(renderer, menuOff, NULL, &menuOffPos);
    			}
    			else
    			{
    				SDL_RenderCopy(renderer, menuBut, NULL, &menuPos);
    			}

    			if(playOver)
    			{
    				SDL_RenderCopy(renderer, PlaySoff, NULL, &PlaySoffPos);

    			}
    			else
    			{
    				SDL_RenderCopy(renderer, PlayS, NULL, &PlaySPos);
    			}

    			SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

    			//SDL Render present
    			SDL_RenderPresent(renderer);
    		}
    		break; //end lose case
    	}
    }

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}



