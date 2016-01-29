/*
 * main.cpp
 *
 *  Created on: Jan 20, 2016
 *      Author: Elijah
 */

#if defined (_WIN32) || (_WIN64)

	#include "SDL.h"
	#include "SDL_image.h"

#endif

#if defined(__APPLE__)

	#include "SDL2/SDL.h"
	#include "SDL2_image/SDL_image.h"

#endif

#if defined(__linux__)

	#include "SDL2/SDL.h"
	#include "SDL2/SDL_image.h"

#endif

#if defined (_WIN32) || (_WIN64)
#include <direct.h>
#define getcwd _getcwd
#endif

#if defined (_linux_)
#include <unistd.h>
#endif

#include <stdio.h>
#include <iostream>
using namespace std;

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

int main(int argc, char* argv[]) {

#if defined (_WIN32) || (_WIN64)

	cout << "Running on Windows" << endl;

	string s_cwd(getcwd(NULL, 0));

	//create a string linking to the mac's images folder
	string s_cwd_images = s_cwd + "\\Resources\\Images\\";

	//test
	//cout << s_cwd_images << endl;

#endif

#if defined(__linux__)

	cout << "Running on Linux" << endl;
	cout << "Added on Linux" << endl;

	string s_cwd(getcwd(NULL, 0));

	//create a string linking to the mac's images folder
	string s_cwd_images = s_cwd + "\\Resources\\Images\\";

#endif

#if defined(__APPLE__)

	cout << "Running on Apple" << endl;
	//get the current working directory
	string s_cwd(getcwd(NULL, 0));

	//create a string linking to the mac's images folder
	string s_cwd_images = s_cwd + "/Resources/Images";

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
  string Play1path = s_cwd_images + "/Player1.png";

  surface = IMG_Load(Play1path.c_str());

  SDL_Texture *Play1;

  Play1 = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect Play1Pos;

  Play1Pos.x = 200;
  Play1Pos.y = 200;
  Play1Pos.w = 648;
  Play1Pos.h = 144;

   //********MAke 2 Player***********
  string Play2path = s_cwd_images + "/Player2.png";

    surface = IMG_Load(Play2path.c_str());

    SDL_Texture *Play2;

    Play2 = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_Rect Play2Pos;

    Play2Pos.x = 200;
    Play2Pos.y = 370;
    Play2Pos.w = 648;
    Play2Pos.h = 144;

   //********MAke Instructions button***********
  /*string titlePath = s_cwd_images + "/Title.png";

  surface = IMG_Load(titlePath.c_str());

  SDL_Texture *title;

  title = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect titlePos;

  titlePos.x = 50;
  titlePos.y = 0;
  titlePos.w = 936;
  titlePos.h = 252;*/

   //********MAke Quit***********
  /*string titlePath = s_cwd_images + "/Title.png";

  surface = IMG_Load(titlePath.c_str());

  SDL_Texture *title;

  title = SDL_CreateTextureFromSurface(renderer, surface);

  SDL_Rect titlePos;

  titlePos.x = 50;
  titlePos.y = 0;
  titlePos.w = 936;
  titlePos.h = 252;*/

    //***********Create Cursor*********
    //create cursor
    string CURSORpath = s_cwd_images + "/whatevercursor.png";

    surface = IMG_Load(CURSORpath.c_str());

    SDL_Texture *cursor;

    cursor = SDL_CreateTextureFromSurface(renderer, surface);

    SDL_FreeSurface(surface);

    //create the SDL_Rectangle for the texture's position and size -x,y,w,h
    SDL_Rect cursorPos;

    //set the X,Y,W, and H for the Rectangle
    cursorPos.x = 0;
    cursorPos.y = 0;
    cursorPos.w = 50;
    cursorPos.h = 50;

    SDL_GameController* gGameController = NULL;

    //****Open Game Controller
    gGameController = SDL_GameControllerOpen(0);

    //*****Turn on Game Controller Events
    SDL_GameControllerEventState(SDL_ENABLE);

    //*****SDL Event to handle input
    SDL_Event event;

    //*****set up variables for game states
    enum GameState{MENU, INSTRUCTIONS, PLAYERS1, PLAYERS2, WIN, LOSE};

    //set up the initial state
    GameState gameState = MENU;

    //boolean values to control movement through states
    bool menu, instructions, players1, players2, win, lose, quit = false;

    // The window is open: could enter program loop here (see SDL_PollEvent())
    while(!quit)
    {
    	switch (gameState)
    	{
    	case MENU:
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
    							menu = false;
    							gameState = INSTRUCTIONS;
    						}

    						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
    						{
    						    menu = false;
    						    gameState = PLAYERS1;
    						}

    						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_X)
    						{
    						    menu = false;
    						    gameState = PLAYERS2;
    						}

    						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
    						{
    						    menu = false;
    						    quit = true;
    						}
    					}
    					break;
    				}
    			}

    			//Update
    			UpdateBackground();


    			//Start Drawing
    			//Clear SDL renderer
    			SDL_RenderClear(renderer);

    			//Draw the bkgd1 image
    			SDL_RenderCopy(renderer, bkgd1, NULL, &bkgd1Pos);

    			//Draw the bkgd2 image
    			SDL_RenderCopy(renderer, bkgd2, NULL, &bkgd2Pos);

    			SDL_RenderCopy(renderer, title, NULL, &titlePos);

    			SDL_RenderCopy(renderer, Play1, NULL, &Play1Pos);

    			SDL_RenderCopy(renderer, Play2, NULL, &Play2Pos);

    			SDL_RenderCopy(renderer, cursor, NULL, &cursorPos);

    			//SDL Render present
    			SDL_RenderPresent(renderer);

    		}
    		break; //end main menu case

    	case INSTRUCTIONS:
    	    	instructions = true;
    	    	cout << "The GameState is Instructions" << endl;
    	    	cout << "Press the A button for Main Menu" << endl;
    	    	cout << endl;

    	    	while (instructions)
    	    	{
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
    	    						instructions = false;
    	    						gameState = MENU;
    	    					}

    	    					if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
    	    					{
    	    					    instructions = false;
    	    					    quit = true;
    	    					}
    	    				}
    	    				break;
    	    			}
    	    		}
    	    	}
    	    	break; //end instructions case

    	case PLAYERS1:
     	    		players1 = true;
 	   	    		cout << "The GameState is 1 Player Game" << endl;
   	   	    		cout << "Press the A button for Win screen" << endl;
   	   	    		cout << "Press the B button for Lose screen" << endl;
   	   	    		cout << endl;

   	   	    		while (players1)
   	   	    		{
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
        	    						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
        	    						{
        	    							players1 = false;
        	    							gameState = WIN;
        	    						}

        	    						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
        	    						{
        	    						    players1 = false;
        	    						    gameState = LOSE;
        	    						}

        	    						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
        	    						{
        	    						    players1 = false;
        	    						    quit = true;
        	    						}
        	    					}
        	    					break;
        	    				}
        	    			}
        	    		}
        	    		break; //end players1 case

    	case PLAYERS2:
 	     	    		players2 = true;
         	    		cout << "The GameState is 2 Player Game" << endl;
         	    		cout << "Press the A button for Win screen" << endl;
         	    		cout << "Press the B button for Lose screen" << endl;
         	    		cout << endl;

    	   	    		while (players2)
    	   	    		{
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
    	    	    	    					if (event.cdevice.which == 0)

    	    	    	    					{
    	    	    	    						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_A)
    	    	    	    						{
    	    	    	    							players2 = false;
    	    	    	    							gameState = WIN;
    	    	    	    						}

    	    	    	    						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
    	    	    	    						{
    	    	    	    						    players2 = false;
    	    	    	    						    gameState = LOSE;
    	    	    	    						}

    	    	    	    						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_Y)
    	    	    	    						{
    	    	    	    						    players2 = false;
    	    	    	    						    quit = true;
    	    	    	    						}
    	    	    	    					}
    	    	    	    					break;
    	    	    	    				}
    	    	    	    			}
    	    	    	    		}
    	    	    	    		break; //end players2 case

    	case WIN:
    	    	    	    	    		win = true;
    	    	    	    	    		cout << "The GameState is Win Screen" << endl;
    	    	    	    	    		cout << "Press the A button for Main Menu" << endl;
    	    	    	    	    		cout << "Press the B button to Quit" << endl;
    	    	    	    	    		cout << endl;

    	    	    	    	    		while (win)
    	    	    	    	    		{
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
    	    	    	    	    							win = false;
    	    	    	    	    							gameState = MENU;
    	    	    	    	    						}

    	    	    	    	    						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
    	    	    	    	    						{
    	    	    	    	    						    win = false;
    	    	    	    	    						    quit = true;
    	    	    	    	    						}
    	    	    	    	    					}
    	    	    	    	    					break;
    	    	    	    	    				}
    	    	    	    	    			}
    	    	    	    	    		}
    	    	    	    	    		break; //end win case

    	case LOSE:
    	    	    	    	    	    		lose = true;
    	    	    	    	    	    		cout << "The GameState is Lose Screen" << endl;
    	    	    	    	    	    		cout << "Press the A button for Main Menu" << endl;
    	    	    	    	    	    		cout << "Press the B button to Quit" << endl;
    	    	    	    	    	    		cout << endl;

    	    	    	    	    	    		while (lose)
    	    	    	    	    	    		{
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
    	    	    	    	    	    							lose = false;
    	    	    	    	    	    							gameState = MENU;
    	    	    	    	    	    						}

    	    	    	    	    	    						if(event.cbutton.button == SDL_CONTROLLER_BUTTON_B)
    	    	    	    	    	    						{
    	    	    	    	    	    						    lose = false;
    	    	    	    	    	    						    quit = true;
    	    	    	    	    	    						}
    	    	    	    	    	    					}
    	    	    	    	    	    					break;
    	    	    	    	    	    				}
    	    	    	    	    	    			}
    	    	    	    	    	    		}
    	    	    	    	    	    		break; //end lose case
    	}
    }

    //SDL_Delay(3000);  // Pause execution for 3000 milliseconds, for example

    // Close and destroy the window
    SDL_DestroyWindow(window);

    // Clean up
    SDL_Quit();
    return 0;
}



