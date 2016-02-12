/*
 * bullet.cpp
 *
 *  Created on: Feb 3, 2016
 *      Author: Elijah
 */

#include "bullet.h"


Bullet::Bullet(SDL_Renderer *renderer, string filepath, float x, float y)
{

	active = false;

	speed = 800;

	SDL_Surface *surface = IMG_Load(filepath.c_str());

	texture = SDL_CreateTextureFromSurface(renderer, surface);

    if (texture == NULL) {
        // In the case that the window could not be made...
        printf("Could not create bullet: %s\n", SDL_GetError());
    }

	SDL_FreeSurface(surface);

	int w, h;
	SDL_QueryTexture(texture, NULL, NULL, &w, &h);
	posRect.w = w;
	posRect.h = h;
	posRect.x = x - (posRect.w/2);
	posRect.y = y;

	pos_X = x;
	pos_Y = y;

	xDir = 0;
	yDir = -1;
}

void Bullet::Reset()
{
	posRect.x = -1000;
	pos_X = posRect.x;
	active = false;
}

void Bullet::Update(float deltaTime)
{
	if (active)
	{
		pos_Y += (speed * yDir) * deltaTime;

		posRect.y = (int)(pos_Y + 0.05f);

		if(posRect.y < (0 - posRect.h))
		{
			posRect.x = -1000;
			pos_X = posRect.x;
			active = false;
		}
	}
}

void Bullet::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);
}

Bullet::~Bullet()
{
	//SDL_DestroyTexture(texture);
}

