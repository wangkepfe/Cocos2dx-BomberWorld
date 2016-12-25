#include "HelloWorldScene.h"
USING_NS_CC;

extern BomberGame * game;
static float unit = 3.f;
#define MAP game->map

void Player::manControl()
{
	if (health == 0)
		return;
	CD_REFRESH(CD);
	if (!remoteBombOn)
	{
		CD_REFRESH(skillCD[0]);
	}
	CD_REFRESH(skillCD[1]);
	CD_REFRESH(skillCD[2]);

	CD_BUTTON_REFRESH(CD, game->labelNormal);
	CD_BUTTON_REFRESH(skillCD[0], game->labelRemoteControl);
	CD_BUTTON_REFRESH(skillCD[1], game->labelTrap);
	CD_BUTTON_REFRESH(skillCD[2], game->labelBounceBomb);

	if (automaticBombOn)
		game->labelTrap->setColor(Color3B::GRAY);
	if (bounceBombOn)
		game->labelBounceBomb->setColor(Color3B::GRAY);

	x = player->getPositionX();
	y = player->getPositionZ();
	i = -x / unit + 10.5;
	j = y / unit + 10.5;

	static int j2 = 0, j3 = 0, i1 = 0, i2 = 0;
	float newX = x, newY = y;

	if (MAP[i][j] < 0)
	{
		getItem(MAP[i][j], i, j);
		getItemNum++;
		game->attributeDisplayUpdate();
		this->displayUpdate(1);
	}

	if (onRush)
	{
		rushCount++;
		if (rushCount == 10)
		{
			rushCount = 0;
			onRush = 0;
			CD = 0;
		}
	}

	if (game->bMove == 1 || onRush)
	{
		Vec2 velocity;
		if (onRush)
		{
			if      (faceTo == 0) game->direction = Vec2(0 ,-1.f);
			else if (faceTo == 1) game->direction = Vec2(0 , 1.f);
			else if (faceTo == 2) game->direction = Vec2( 1.f, 0);
			else if (faceTo == 3) game->direction = Vec2(-1.f, 0);
			velocity = game->direction;
		}
		else
			velocity = game->direction * agility * 0.025f;

		if (game->direction.y >= 0) // up
		{
			i1 = -x / unit + 9.9;
			j3 = y / unit + 10.1;
			i2 = -x / unit + 9.9;
			j2 = y / unit + 10.9;
			if (MAP[i1][j3] <= 0 && MAP[i2][j2] <= 0)
			{
				newX = x + velocity.y;
			}
		}
		else // down
		{
			i1 = -x / unit + 11.1;
			j3 = y / unit + 10.1;
			i2 = -x / unit + 11.1;
			j2 = y / unit + 10.9;
			if (MAP[i1][j3] <= 0 && MAP[i2][j2] <= 0)
			{
				newX = x + velocity.y;
			}
		}

		if (game->direction.x >= 0) // right
		{
			i1 = -x / unit + 10.1;
			j3 = y / unit + 11.1;
			i2 = -x / unit + 10.9;
			j2 = y / unit + 11.1;
			if (MAP[i1][j3] <= 0 && MAP[i2][j2] <= 0)
			{
				newY = y + velocity.x;
			}
		}
		else //left
		{
			i1 = -x / unit + 10.1;
			j3 = y / unit + 9.9;
			i2 = -x / unit + 10.9;
			j2 = y / unit + 9.9;
			if (MAP[i1][j3] <= 0 && MAP[i2][j2] <= 0)
			{
				newY = y + velocity.x;
			}
		}
		player->setPosition3D(Vec3(newX, -2.2f, newY));
		runLength += (float)fabs(newY - y) + (float)fabs(newX - x);
	}

	if (game->direction.x > 0 && game->direction.y > 0)
	{
		if (game->direction.x > game->direction.y)
			faceTo = 2;
		else
			faceTo = 1;
	}
	else if (game->direction.x > 0 && game->direction.y < 0)
	{
		if (game->direction.x > -game->direction.y)
			faceTo = 2;
		else
			faceTo = 0;
	}
	else if (game->direction.x < 0 && game->direction.y < 0)
	{
		if (-game->direction.x > -game->direction.y)
			faceTo = 3;
		else
			faceTo = 0;
	}
	else if (game->direction.x < 0 && game->direction.y > 0)
	{
		if (-game->direction.x > game->direction.y)
			faceTo = 3;
		else
			faceTo = 1;
	}

	x = newX;
	y = newY;
	i = -x / unit + 10.5;
	j = y / unit + 10.5;

	if (game->cameraPosition == 1)
	{
		game->_camera->setPosition3D(Vec3(newX - 15.f, 25.f, newY));
		game->_camera->lookAt(Vec3(newX, -2.2f, newY), Vec3(0.0f, 1.0f, 0.0f));
	}
	else if (game->cameraPosition == 2)
	{
		game->_camera->setPosition3D(Vec3(-7.f - 28.f, 46.f, 0));
		game->_camera->lookAt(Vec3(-7.f, -2.2f, 0), Vec3(0.0f, 1.0f, 0.0f));
	}
	displayUpdate(0);
}