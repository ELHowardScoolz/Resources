#include "Player.h"

const int JOYSTICK_DEAD_ZONE = 8000;

Player::Player(SDL_Renderer *renderer, int pNum, string filePath, float x, float y)
{
	PlayerNum = pNum;

	speed = 500.0f;

	if(PlayerNum == 0)
	{
		PlayerPath = filePath + "/TestPlayer1.png";
	}
	else
	{
		PlayerPath = filePath + "/TestPlayer2.png";
	}

	surface = IMG_Load(PlayerPath.c_str());

	texture = SDL_CreateTextureFromSurface(renderer, surface);

	SDL_FreeSurface(surface);

	posRect.x = x;

	posRect.y = y;

	int w,h;

	SDL_QueryTexture(texture, NULL, NULL, &w, &h);

	posRect.w = w;

	posRect.h = h;

	pos_X = x;
	pos_Y = y;

	xDir = 0;
	yDir = 0;

	string bulletPath;

	if(pNum ==0)
	{
		bulletPath = filePath + "/bulletTest.png";
	}
	else
	{
		bulletPath = filePath + "/bulletTest2.png";
	}

	for(int i = 0; i < 10; i++)
	{
		Bullet tmpBullet(renderer, bulletPath, -1000, -1000);

		bulletList.push_back(tmpBullet);
	}
}

void Player::Update(float deltaTime)
{
	pos_X += (speed *xDir) * deltaTime;
	pos_Y += (speed *yDir) * deltaTime;

	posRect.x = (int)(pos_X + 0.05f);
	posRect.y = (int)(pos_Y + 0.05f);

	if(posRect.x < 0)
	{
		posRect.x = 0;
		pos_X = posRect.x;
	}

	if(posRect.x > 1024 - posRect.w)
	{
		posRect.x = 1024 - posRect.w;
		pos_X = posRect.x;
	}

	if(posRect.y > 768 - posRect.h)
	{
		posRect.y = 768 - posRect.h;
		pos_Y = posRect.y;
	}

	for(int i = 0; i < bulletList.size(); i++)
	{
		if(bulletList[i].active)
		{
			bulletList[i].Update(deltaTime);
		}
	}
}

void Player::Draw(SDL_Renderer *renderer)
{
	SDL_RenderCopy(renderer, texture, NULL, &posRect);

	for(int i = 0; i < bulletList.size(); i++)
	{
		if(bulletList[i].active)
		{
			bulletList[i].Draw(renderer);
		}
	}
}

void Player::CreateBullet()
{
	for(int i = 0; i < bulletList.size(); i++)
	{
		if(bulletList[i].active == false)
		{
			bulletList[i].active = true;
			bulletList[i].posRect.x = (pos_X + (posRect.w/2));
			bulletList[i].posRect.x = (bulletList[i].posRect.x - (bulletList[i].posRect.w/2));

			bulletList[i].posRect.y = posRect.y;

			bulletList[i].pos_X = pos_X;
			bulletList[i].pos_X = pos_Y;

			break;
		}
	}
}

void Player::OnControllerButton(const SDL_ControllerButtonEvent event)
{
	if(event.which == 0 && PlayerNum == 0)
	{
		if (event.button == 0)
		{
			cout << "Player 1 - Button A" << endl;

			CreateBullet();
		}
	}

	if(event.which == 1 && PlayerNum == 1)
	{
		if (event.button == 0)
		{
			cout << "Player 2 - Button A" << endl;

			CreateBullet();
		}
	}
}

void Player::OnControllerAxis(const SDL_ControllerAxisEvent event)
{
	if(event.which == 0 && PlayerNum == 0)
	{
		if (event.axis == 0)
		{
			if(event.value < -JOYSTICK_DEAD_ZONE)
			{
				xDir = -1.0f;
			}
			else if (event.value > JOYSTICK_DEAD_ZONE)
			{
				xDir = 1.0f;
			}
			else
			{
				xDir = 0.0f;
			}
		}

			if (event.axis == 1)
			{
				if(event.value < -JOYSTICK_DEAD_ZONE)
				{
					yDir = -1.0f;
				}
				else if (event.value > JOYSTICK_DEAD_ZONE)
				{
					yDir = 1.0f;
				}
				else
				{
					yDir = 0.0f;
				}
			}
	}

	if(event.which == 1 && PlayerNum == 1)
	{
		if (event.axis == 0)
		{
			if(event.value < -JOYSTICK_DEAD_ZONE)
			{
				xDir = -1.0f;
			}
			else if (event.value > JOYSTICK_DEAD_ZONE)
			{
				xDir = 1.0f;
			}
			else
			{
				xDir = 0.0f;
			}
		}

			if (event.axis == 1)
			{
				if(event.value < -JOYSTICK_DEAD_ZONE)
				{
					yDir = -1.0f;
				}
				else if (event.value > JOYSTICK_DEAD_ZONE)
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


Player::~Player()
{
	SDL_DestroyTexture(texture);
}