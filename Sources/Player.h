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

#include <stdio.h>
#include <iostream>
using namespace std;

#include <vector>
#include "bullet.h"

class Player
{
public:

	vector<Bullet> bulletList;

	string PlayerPath;

	int PlayerNum;

	SDL_Surface *surface;

	SDL_Texture *texture;

	SDL_Rect posRect;

	float xDir;
	float yDir;

	float speed;

	float pos_X, pos_Y;

	Player(SDL_Renderer *renderer, int pNum, string filePath, float x, float y);

	void Update(float deltaTime);

	void Draw(SDL_Renderer *renderer);

	void OnControllerAxis(const SDL_ControllerAxisEvent event);

	void OnControllerButton(const SDL_ControllerButtonEvent event);

	~Player();

private:

	void CreateBullet();
};
