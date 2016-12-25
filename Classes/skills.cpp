#include "HelloWorldScene.h"

#define MAP game->map
#define THREAT_MAP game->threatMap
static const float boom3DAnimationPeriod = 1.5f;

USING_NS_CC;
extern BomberGame * game;
static float unit = 3.f;

void Player::createRemoteBomb(int i, int j, int power)
{
	if (MAP[i][j] != 0)
		return;
	for (int n = 0; n < game->playerNum; n++)
	{
		if (game->player[n]->occupation == 3)
		{
			for (int m = 0; m < 3; m++)
			{
				if (game->player[n]->gateOn[m] && game->player[n]->gateX[m] == i && game->player[n]->gateY[m] == j)
					return;
			}
		}
	}
	MAP[i][j] = 4;
	float x = (10 - i)* unit;
	float y = (-10 + j)* unit;
	auto bomb = Sprite3D::create("box/box.c3t");
	bomb->setTexture("box/c4.png");
	bomb->setScale(unit*0.8);
	bomb->setPosition3D(Vec3(x, -2.2f, y));
	game->addChild(bomb);
	bomb->setCameraMask((unsigned short)CameraFlag::USER1);
	bomb->setTag(i * 10 + j * 1000);
	remoteBombPower = power;
	remoteBombX = i;
	remoteBombY = j;

	bomb->runAction(Sequence::create(DelayTime::create(30.0f), CallFunc::create([=]()
	{
		trigerRemoteBomb();
	}), nullptr));
	
	game->addThreatArea(i, j, power * 1.5f);

	remoteBombOn = 1;
}

void Player::trigerRemoteBomb()
{
	int i = remoteBombX;
	int j = remoteBombY;
	(game->getChildByTag(i * 10 + j * 1000))->stopAllActions();
	game->removeChildByTag(i * 10 + j * 1000);

	MAP[i][j] = 0;
	float x = (10 - i)* unit;
	float y = (-10 + j)* unit;
	auto ps = PUParticleSystem3D::create("Particle3D/scripts/explosionSystem.pu");
	ps->setPosition3D(Vec3(x, -2.2f, y));
	ps->setScale(2);
	ps->startParticleSystem();
	ps->setCameraMask(2);
	game->addChild(ps);
	ps->runAction(Sequence::create(DelayTime::create(boom3DAnimationPeriod), CallFunc::create([=]() {
		ps->removeFromParent();
	}), nullptr));
	game->fireFlame(5,remoteBombPower , i, j, 1);
	remoteBombOn = 0;
}

void Player::createAutomaticBomb(int i, int j, int power)
{
	if (MAP[i][j] != 0)
	{
		skillCD[1] = 0;
		return;
	}
	for (int n = 0; n < game->playerNum; n++)
	{
		if (game->player[n]->occupation == 3)
		{
			for (int m = 0; m < 3; m++)
			{
				if (game->player[n]->gateOn[m] && game->player[n]->gateX[m] == i && game->player[n]->gateY[m] == j)
				{
					skillCD[1] = 0;
					return;
				}
			}
		}
	}
	MAP[i][j] = 5;
	float x = (10 - i)* unit;
	float y = (-10 + j)* unit;
	auto bomb = Sprite3D::create("box/box.c3t");
	bomb->setTexture("box/plane.png");
	bomb->setScale(unit*0.8);
	bomb->setPosition3D(Vec3(x, -2.2f, y));
	game->addChild(bomb);
	bomb->setCameraMask((unsigned short)CameraFlag::USER1);
	bomb->setTag(i * 10 + j * 1000);
	automaticBombPower = power;
	automaticBombX = i;
	automaticBombY = j;

	bomb->runAction(Sequence::create(DelayTime::create(1.f),RepeatForever::create(Sequence::create(DelayTime::create(0.3f), CallFunc::create([=]() {
		int k;
		bool boom = 0;
		for (int m = 0; m < emenyNum; m++)
		{
			int enemyI = emenyTeamPosX[m];
			int enemyJ = emenyTeamPosY[m];
			for (k = 1; k < power && MAP[i + k][j] == 0; k++)
			{
				if (enemyI == i + k && enemyJ == j)
					boom = 1;
			}
			for (k = 1; k < power && MAP[i][j + k] == 0; k++)
			{
				if (enemyI == i && enemyJ + k == j)
					boom = 1;
			}
			for (k = 1; k < power && MAP[i - k][j] == 0; k++)
			{
				if (enemyI == i - k && enemyJ == j)
					boom = 1;
			}
			for (k = 1; k < power && MAP[i][j - k] == 0; k++)
			{
				if (enemyI == i && enemyJ - k == j)
					boom = 1;
			}
		}
		if (this->i == i || this->j == j)
			boom = 0;
		if (boom)
		{
			automaticBombBoom();
		}
	}), nullptr)), nullptr));

	bomb->runAction(Sequence::create(DelayTime::create(10.0f), CallFunc::create([=]()
	{
		automaticBombBoom();
	}), nullptr));

	game->addThreatArea(i, j, power * 0.8f);

	automaticBombOn = 1;
}

void Player::automaticBombBoom()
{
	int i = automaticBombX;
	int j = automaticBombY;
	int power = automaticBombPower;
	(game->getChildByTag(i * 10 + j * 1000))->stopAllActions();
	game->removeChildByTag(i * 10 + j * 1000);

	MAP[i][j] = 0;
	float x = (10 - i)* unit;
	float y = (-10 + j)* unit;
	auto ps = PUParticleSystem3D::create("Particle3D/scripts/explosionSystem.pu");
	ps->setPosition3D(Vec3(x, -2.2f, y));
	ps->setScale(2);
	ps->startParticleSystem();
	ps->setCameraMask(2);
	game->addChild(ps);
	ps->runAction(Sequence::create(DelayTime::create(boom3DAnimationPeriod), CallFunc::create([=]() {
		ps->removeFromParent();
	}), nullptr));
	game->fireFlame(5, power, i, j, 0);
	automaticBombOn = 0;
}

static const float speed = 0.5f;
static const float Vball = 0.5f;

void Player::createBounceBomb(int i, int j, int power)
{
	if (MAP[i][j] != 0)
	{
		skillCD[2] = 0;
		return;
	}
	for (int n = 0; n < game->playerNum; n++)
	{
		if (game->player[n]->occupation == 3)
		{
			for (int m = 0; m < 3; m++)
			{
				if (game->player[n]->gateOn[m] && game->player[n]->gateX[m] == i && game->player[n]->gateY[m] == j)
				{
					skillCD[2] = 0;
					return;
				}
			}
		}
	}
	MAP[i][j] = 6;
	float x = (10 - i)* unit;
	float y = (-10 + j)* unit;
	auto bomb = Sprite3D::create("box/box.c3t");
	bomb->setTexture("box/CyanSquare2.png");
	bomb->setScale(unit*0.8);
	bomb->setPosition3D(Vec3(x, -2.2f, y));
	game->addChild(bomb);
	bomb->setCameraMask((unsigned short)CameraFlag::USER1);
	bomb->setTag(i * 10 + j * 1000);
	game->addThreatArea(i, j, power);
	bounceBombPower = power;
	bounceBombX = i;
	bounceBombY = j;
	bombFaceTo = faceTo;

	bomb->runAction(RepeatForever::create(Sequence::create(CallFunc::create([=]() {
		this->calCurrentPlayerPosition();
		int m, enemyI, enemyJ;
		int bombJ, bombI;
		int distance[10] = { 100,100,100,100,100,100,100,100,100,100 };
		int leastDistance = 100, choice = 0;
		bombI = -bomb->getPositionX() / unit + 10.5f;
		bombJ = bomb->getPositionZ() / unit + 10.5f;
		for (m = 0; m < emenyNum; m++)
		{
			enemyI = emenyTeamPosX[m];
			enemyJ = emenyTeamPosY[m];
			
			if (enemyI > bombI)
				distance[m] = enemyI - bombI;
			else
				distance[m] = bombI - enemyI;
			if (enemyJ > bombJ)
				distance[m] += enemyJ - bombJ;
			else
				distance[m] += bombJ - enemyJ;

			if (distance[m] < leastDistance)
			{
				leastDistance = distance[m];
				choice = m;
			}
		}
		enemyI = emenyTeamPosX[choice];
		enemyJ = emenyTeamPosY[choice];

		if (bombFaceTo == 0 && MAP[bounceBombX + 1][bounceBombY] == 0) {
			bomb->runAction(JumpBy::create(speed, Vec3(-3.f, 0, 0), 1.f, 1)); // 0
			MAP[bounceBombX][bounceBombY] = 0;
			game->minusThreatArea(bounceBombX, bounceBombY, power);
			bounceBombX++;
			bomb->setTag(bounceBombX * 10 + bounceBombY * 1000);
			MAP[bounceBombX][bounceBombY] = 6;
			game->addThreatArea(bounceBombX, bounceBombY, power);
		}
		else if (bombFaceTo == 1 && MAP[bounceBombX - 1][bounceBombY] == 0) {
			bomb->runAction(JumpBy::create(speed, Vec3(3.f, 0, 0), 1.f, 1));  // 1
			MAP[bounceBombX][bounceBombY] = 0;
			game->minusThreatArea(bounceBombX, bounceBombY, power);
			bounceBombX--;
			bomb->setTag(bounceBombX * 10 + bounceBombY * 1000);
			MAP[bounceBombX][bounceBombY] = 6;
			game->addThreatArea(bounceBombX, bounceBombY, power);
		}
		else if (bombFaceTo == 2 && MAP[bounceBombX][bounceBombY + 1] == 0) {
			bomb->runAction(JumpBy::create(speed, Vec3(0, 0, 3.f), 1.f, 1)); // 2
			MAP[bounceBombX][bounceBombY] = 0;
			game->minusThreatArea(bounceBombX, bounceBombY, power);
			bounceBombY++;
			bomb->setTag(bounceBombX * 10 + bounceBombY * 1000);
			MAP[bounceBombX][bounceBombY] = 6;
			game->addThreatArea(bounceBombX, bounceBombY, power);
		}
		else if (bombFaceTo == 3 && MAP[bounceBombX][bounceBombY - 1] == 0) {
			bomb->runAction(JumpBy::create(speed, Vec3(0, 0, -3.f), 1.f, 1)); // 3
			MAP[bounceBombX][bounceBombY] = 0;
			game->minusThreatArea(bounceBombX, bounceBombY, power);
			bounceBombY--;
			bomb->setTag(bounceBombX * 10 + bounceBombY * 1000);
			MAP[bounceBombX][bounceBombY] = 6;
			game->addThreatArea(bounceBombX, bounceBombY, power);
		}
		else 
			bomb->runAction(JumpBy::create(speed, Vec3(0, 0,0), 1.f, 1));

		if (enemyI > bombI && ((enemyI - bombI) / (float)(enemyJ - bombJ) > 1 || (enemyI - bombI) / (float)(enemyJ - bombJ) < -1))
			bombFaceTo = 0;
		else if (enemyI < bombI && ((enemyI - bombI) / (float)(enemyJ - bombJ) > 1 || (enemyI - bombI) / (float)(enemyJ - bombJ) < -1))
			bombFaceTo = 1;
		else if (enemyJ > bombJ && ((enemyI - bombI) / (float)(enemyJ - bombJ) < 1 || (enemyI - bombI) / (float)(enemyJ - bombJ) > -1))
			bombFaceTo = 2;
		else if (enemyJ < bombJ && ((enemyI - bombI) / (float)(enemyJ - bombJ) < 1 || (enemyI - bombI) / (float)(enemyJ - bombJ) > -1))
			bombFaceTo = 3;
	}), DelayTime::create(speed),nullptr)));
	
	bomb->runAction(Sequence::create(DelayTime::create(4.0f), CallFunc::create([=]()
	{
		bounceBombBoom();
	}), nullptr));		

	bounceBombOn = 1;
}

void Player::bounceBombBoom()
{
	int i = bounceBombX;
	int j = bounceBombY;
	int power = bounceBombPower;
	(game->getChildByTag(i * 10 + j * 1000))->stopAllActions();
	game->removeChildByTag(i * 10 + j * 1000);

	MAP[i][j] = 0;
	float x = (10 - i)* unit;
	float y = (-10 + j)* unit;
	auto ps = PUParticleSystem3D::create("Particle3D/scripts/explosionSystem.pu");
	ps->setPosition3D(Vec3(x, -2.2f, y));
	ps->setScale(2);
	ps->startParticleSystem();
	ps->setCameraMask(2);
	game->addChild(ps);
	ps->runAction(Sequence::create(DelayTime::create(boom3DAnimationPeriod), CallFunc::create([=]() {
		ps->removeFromParent();
	}), nullptr));
	game->fireFlame(5, power, i, j, 0);
	bounceBombOn = 0;
}

void Player::throw3Bombs(int i, int j, int power)
{
	int m, enemyI, enemyJ;
	int distance[10] = { 100,100,100,100,100,100,100,100,100,100 };
	int leastDistance = 100, choice = 0;
	for (m = 0; m < emenyNum; m++)
	{
		enemyI = emenyTeamPosX[m];
		enemyJ = emenyTeamPosY[m];

		if (enemyI > i)
			distance[m] = enemyI - i;
		else
			distance[m] = i - enemyI;
		if (enemyJ > j)
			distance[m] += enemyJ - j;
		else
			distance[m] += j - enemyJ;

		if (distance[m] < leastDistance)
		{
			leastDistance = distance[m];
			choice = m;
		}
	}

	if (distance[choice] < 10)
	{
		int p, q, rx, ry;
		onThrowing = 1;
		for (p = 0; p < 3; p++)
		{
			throwTargetPosX[p] = 0;
			throwTargetPosY[p] = 0;
			for (q = 5; q < 100; q++)
			{
				rx = emenyTeamPosX[choice] - q / 5 + rand() % (2 * (q / 5) + 1);
				ry = emenyTeamPosY[choice] - q / 5 + rand() % (2 * (q / 5) + 1);
				if (rx > 0 && ry > 0 && rx < 21 && ry < 21 && MAP[rx][ry] == 0)
				{
					throwTargetPosX[p] = rx;
					throwTargetPosY[p] = ry;
					break;
				}
			}
		}
	}
	
	if (!onThrowing)
	{
		skillCD[1] = 0;
		return;
	}
	player->runAction(Sequence::create(CallFunc::create([=]() {
		float x = (10 - i)* unit;
		float y = (-10 + j)* unit;
		auto bomb = Sprite3D::create("box/box.c3t");
		bomb->setTexture("box/CyanSquare2.png");
		bomb->setScale(unit*0.8);
		bomb->setPosition3D(Vec3(x, -2.2f, y));
		game->addChild(bomb);
		bomb->setCameraMask((unsigned short)CameraFlag::USER1);
		Vec3 throwDirection = Vec3(-3.f * (throwTargetPosX[0] - i), 0, 3.f * (throwTargetPosY[0] - j));
		bomb->runAction(Sequence::create(JumpBy::create(1.f, throwDirection, 5.f, 1), CallFunc::create([=]() {
			bomb->removeFromParent();
			createBomb(throwTargetPosX[0], throwTargetPosY[0], power);
		}),nullptr));
	}), DelayTime::create(0.3f), CallFunc::create([=]() {
		float x = (10 - i)* unit;
		float y = (-10 + j)* unit;
		auto bomb = Sprite3D::create("box/box.c3t");
		bomb->setTexture("box/CyanSquare2.png");
		bomb->setScale(unit*0.8);
		bomb->setPosition3D(Vec3(x, -2.2f, y));
		game->addChild(bomb);
		bomb->setCameraMask((unsigned short)CameraFlag::USER1);
		Vec3 throwDirection = Vec3(-3.f * (throwTargetPosX[1] - i), 0, 3.f * (throwTargetPosY[1] - j));
		bomb->runAction(Sequence::create(JumpBy::create(1.f, throwDirection, 5.f, 1), CallFunc::create([=]() {
			bomb->removeFromParent();
			createBomb(throwTargetPosX[1], throwTargetPosY[1], power);
		}), nullptr));
	}), DelayTime::create(0.3f), CallFunc::create([=]() {
		float x = (10 - i)* unit;
		float y = (-10 + j)* unit;
		auto bomb = Sprite3D::create("box/box.c3t");
		bomb->setTexture("box/CyanSquare2.png");
		bomb->setScale(unit*0.8);
		bomb->setPosition3D(Vec3(x, -2.2f, y));
		game->addChild(bomb);
		bomb->setCameraMask((unsigned short)CameraFlag::USER1);
		Vec3 throwDirection = Vec3(-3.f * (throwTargetPosX[2] - i), 0, 3.f * (throwTargetPosY[2] - j));
		bomb->runAction(Sequence::create(JumpBy::create(1.f, throwDirection, 5.f, 1), CallFunc::create([=]() {
			bomb->removeFromParent();
			createBomb(throwTargetPosX[2], throwTargetPosY[2], power);
		}), nullptr));
	}),CallFunc::create([=]() {
		onThrowing = 0;
	}),nullptr));
}

void Player::bombWall(int i, int j, int power)
{
	int m;
	if (faceTo == 0)
	{
		for (m = 1; m <= 5; m++)
			createBomb(i + m, j, power);
	}
	else if (faceTo == 1)
	{
		for (m = 1; m <= 5; m++)
			createBomb(i - m, j, power);
	}
	else if (faceTo == 2)
	{
		for (m = 1; m <= 5; m++)
			createBomb(i, j + m, power);
	}
	else if (faceTo == 3)
	{
		for (m = 1; m <= 5; m++)
			createBomb(i, j - m, power);
	}
}

void Player::createCannon(int i, int j, int power)
{
	if (MAP[i][j] != 0)
	{
		useSkillNum--;
		skillCD[2] = 0;
		return;
	}
	for (int n = 0; n < game->playerNum; n++)
	{
		if (game->player[n]->occupation == 3)
		{
			for (int m = 0; m < 3; m++)
			{
				if (game->player[n]->gateOn[m] && game->player[n]->gateX[m] == i && game->player[n]->gateY[m] == j)
				{			
					useSkillNum--;
					skillCD[2] = 0;
					return;
				}
			}
		}
	}
	MAP[i][j] = 7;

	int m;
	for (m = 0; m < 3; m++)
	{
		if (!cannonOn[m])
		{
			cannonOn[m] = 1;
			cannonPower[m] = power;
			cannonX[m] = i;
			cannonY[m] = j;
			cannonFaceTo[m] = faceTo;
			cannonHealth[m] = 100.f;
			cannonNum++;
			break;
		}
	}
	float x = (10 - i)* unit;
	float y = (-10 + j)* unit;
	auto cannon = Sprite3D::create("box/box.c3t");
	cannon->setTexture("box/Cannon.png");
	cannon->setScale(unit*0.8);
	cannon->setPosition3D(Vec3(x, -2.2f, y));
	game->addChild(cannon);
	cannon->setCameraMask((unsigned short)CameraFlag::USER1);
	cannon->setTag(i * 10 + j * 1000);

	cannon->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.3f), CallFunc::create([=]() {
		cannonHealth[m] -= 1.5f;
		if (cannonHealth[m] <= 0)
			cannonDestroy(m);
	}), nullptr)));

	cannon->runAction(RepeatForever::create(Sequence::create(DelayTime::create(4.f), CallFunc::create([=]() {
		float bx = cannon->getPositionX();
		float by = cannon->getPositionZ();
		int bi = -bx / unit + 10.5;
		int bj = by / unit + 10.5;

		if (cannonFaceTo[m] == 0) bx -= unit / 3;
		else if (cannonFaceTo[m] == 1) bx += unit / 3;
		else if (cannonFaceTo[m] == 2) by += unit / 3;
		else if (cannonFaceTo[m] == 3) by -= unit / 3;

		auto cannonBall = Sprite3D::create("box/sphere.c3b");
		cannonBall->setTexture("box/CyanSquare2.png");
		cannonBall->setScale(unit*0.04);
		cannonBall->setPosition3D(Vec3(bx, -2.2f, by));
		game->addChild(cannonBall);
		cannonBall->setCameraMask((unsigned short)CameraFlag::USER1);

		cannonBall->runAction(Sequence::create(DelayTime::create(2.f), CallFunc::create([=]() {
			float bx = cannonBall->getPositionX();
			float by = cannonBall->getPositionZ();

			game->fireFlame(5, cannonPower[m], (int)(10.5f - bx / unit), (int)(10.5f + by / unit), 0);
			cannonBall->stopAllActions();
			cannonBall->removeFromParent();
		}), nullptr));

		cannonBall->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.035f), CallFunc::create([=]() {
			float bx = cannonBall->getPositionX();
			float by = cannonBall->getPositionZ();
			bool boom = 0;

			if ((cannonFaceTo[m] == 0 &&
				(MAP[(int)(11.1f - bx / unit)][(int)(10.1f + by / unit)] > 0 ||
					MAP[(int)(11.1f - bx / unit)][(int)(10.9f + by / unit)] > 0)) ||
				((cannonFaceTo[m] == 1 &&
					(MAP[(int)(9.9f - bx / unit)][(int)(10.1f + by / unit)] > 0 ||
						MAP[(int)(9.9f - bx / unit)][(int)(10.9f + by / unit)] > 0))) ||
				((cannonFaceTo[m] == 2 &&
					(MAP[(int)(10.1f - bx / unit)][(int)(11.1f + by / unit)] > 0 ||
						MAP[(int)(10.9f - bx / unit)][(int)(11.1f + by / unit)] > 0))) ||
				((cannonFaceTo[m] == 3 &&
					(MAP[(int)(10.1f - bx / unit)][(int)(9.9f + by / unit)] > 0 ||
						MAP[(int)(10.9f - bx / unit)][(int)(9.9f + by / unit)] > 0))))
				boom = 1;

			for (int n = 0; n < game->playerNum; n++)
			{
				int pi = game->player[n]->i;
				int pj = game->player[n]->j;

				if (pi == (int)(10.5f - bx / unit) && pj == (int)(10.5f + by / unit))
					boom = 1;
			}

			if (boom)
			{
				auto ps = PUParticleSystem3D::create("Particle3D/scripts/explosionSystem.pu");
				ps->setPosition3D(Vec3(bx, -2.2f, by));
				ps->setScale(2);
				ps->startParticleSystem();
				ps->setCameraMask(2);
				game->addChild(ps);
				ps->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=]() {
					ps->removeFromParent();
				}), nullptr));
				game->fireFlame(5, cannonPower[m], (int)(10.5f - bx / unit), (int)(10.5f + by / unit), 0);
				cannonBall->stopAllActions();
				cannonBall->removeFromParent();
			}
			else
			{
				if (cannonFaceTo[m] == 0) bx -= Vball;
				else if (cannonFaceTo[m] == 1) bx += Vball;
				else if (cannonFaceTo[m] == 2) by += Vball;
				else if (cannonFaceTo[m] == 3) by -= Vball;

				cannonBall->setPosition3D(Vec3(bx, -2.2f, by));
			}
		}), nullptr)));
	}), nullptr)));

	cannon->runAction(Sequence::create(DelayTime::create(1.f), RepeatForever::create(Sequence::create(DelayTime::create(2.f), CallFunc::create([=]() {
		int k;
		THREAT_MAP[i][j] += 1;
		for (k = 1; cannonFaceTo[m] == 0 && k < power && MAP[i + k][j] == 0 && k <= 10; k++)
			THREAT_MAP[i + k][j] += 1;
		for (k = 1; cannonFaceTo[m] == 2 && k < power && MAP[i][j + k] == 0 && k <= 10; k++)
			THREAT_MAP[i][j + k] += 1;
		for (k = 1; cannonFaceTo[m] == 1 && k < power && MAP[i - k][j] == 0 && k <= 10; k++)
			THREAT_MAP[i - k][j] += 1;
		for (k = 1; cannonFaceTo[m] == 3 && k < power && MAP[i][j - k] == 0 && k <= 10; k++)
			THREAT_MAP[i][j - k] += 1;
	}), DelayTime::create(2.f), CallFunc::create([=]() {
		int k;
		THREAT_MAP[i][j] -= 1;
		for (k = 1; cannonFaceTo[m] == 0 && k < power && MAP[i + k][j] == 0 && k <= 10; k++)
			THREAT_MAP[i + k][j] -= 1;
		for (k = 1; cannonFaceTo[m] == 2 && k < power && MAP[i][j + k] == 0 && k <= 10; k++)
			THREAT_MAP[i][j + k] -= 1;
		for (k = 1; cannonFaceTo[m] == 1 && k < power && MAP[i - k][j] == 0 && k <= 10; k++)
			THREAT_MAP[i - k][j] -= 1;
		for (k = 1; cannonFaceTo[m] == 3 && k < power && MAP[i][j - k] == 0 && k <= 10; k++)
			THREAT_MAP[i][j - k] -= 1;
	}), nullptr)), nullptr));
}

void Player::createGate(int i, int j)
{
	if (MAP[i][j] != 0)
	{
		useSkillNum--;
		skillCD[1] = 0;
		return;
	}
	for (int n = 0; n < game->playerNum; n++)
	{
		if (game->player[n]->occupation == 3)
		{
			for (int m = 0; m < 3; m++)
			{
				if (game->player[n]->gateOn[m] && game->player[n]->gateX[m] == i && game->player[n]->gateY[m] == j)
				{
					useSkillNum--;
					skillCD[1] = 0;
					return;
				}
			}
		}
	}
	MAP[i][j] = 8;

	int m;
	for (m = 0; m < 3; m++)
	{
		if (!gateOn[m])
		{
			gateOn[m] = 1;
			gateX[m] = i;
			gateY[m] = j;
			gateHealth[m] = 150.f;
			gateNum++;
			break;
		}
	}

	float x = (10 - i)* unit;
	float y = (-10 + j)* unit;
	auto gate = Sprite3D::create("box/box.c3t");
	gate->setTexture("box/Gate.png");
	gate->setScale(unit*0.8);
	gate->setPosition3D(Vec3(x, -2.2f, y));
	game->addChild(gate);
	gate->setCameraMask((unsigned short)CameraFlag::USER1);
	gate->setTag(i * 10 + j * 1000);
	gateX[m] = i;
	gateY[m] = j;

	gate->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.3f), CallFunc::create([=]() {
		int n, friendI, friendJ, enemyI, enemyJ, p, q;
		bool detectFriend = 0;
		for (n = 0; n < friendNum; n++)
		{
			friendI = friendTeamPosX[n];
			friendJ = friendTeamPosY[n];
			for (p = i - 1; p <= i + 1; p++)
			{
				for (q = j - 1; q <= j + 1; q++)
				{
					if ((friendI == p && friendJ == q) || (this->i == p && this->j == q))
					{
						detectFriend = 1;
						break;
					}
				}
			}
		}
		bool detectEnemy = 0;
		for (n = 0; n < emenyNum; n++)
		{
			enemyI = emenyTeamPosX[n];
			enemyJ = emenyTeamPosY[n];
			for (p = i - 1; p <= i + 1; p++)
			{
				for (q = j - 1; q <= j + 1; q++)
				{
					if ((enemyI == p && enemyJ == q))
					{
						detectEnemy = 1;
						break;
					}
				}
			}
		}
		if (MAP[i][j] == 8 && detectFriend)
		{
			MAP[i][j] = 9;
			int tag = 10000;
			auto action = RepeatForever::create(Sequence::create(CallFunc::create([=]() {
				gate->setPositionY(gate->getPositionY() - 0.5f);
				if (gate->getPositionY()<= -4.5f)
				{
					gate->setPositionY(-4.5f);
					gate->stopActionByTag(tag);
					MAP[i][j] = 0;
				}
			}), DelayTime::create(0.035f), nullptr));
			action->setTag(tag);
			gate->runAction(action);
		}
		if (MAP[i][j] == 0 && !detectFriend && !detectEnemy)
		{
			MAP[i][j] = 9;
			int tag = 10001;
			auto action = RepeatForever::create(Sequence::create(CallFunc::create([=]() {
				gate->setPositionY(gate->getPositionY() + 0.5f);
				if (gate->getPositionY()>= -2.2f)
				{
					gate->setPositionY(-2.2f);
					gate->stopActionByTag(tag);
					MAP[i][j] = 8;
				}
			}), DelayTime::create(0.035f), nullptr));
			action->setTag(tag);
			gate->runAction(action);
		}

		gateHealth[m] -= 1.5f;
		if (gateHealth[m] <= 0)
			gateDestroy(m);
	}), nullptr)));
}

void Player::createMotar(int i, int j, int power)
{
	if (MAP[i][j] != 0)
	{
		useSkillNum--;
		skillCD[0] = 0;
		return;
	}
	for (int n = 0; n < game->playerNum; n++)
	{
		if (game->player[n]->occupation == 3)
		{
			for (int m = 0; m < 3; m++)
			{
				if (game->player[n]->gateOn[m] && game->player[n]->gateX[m] == i && game->player[n]->gateY[m] == j)
				{
					useSkillNum--;
					skillCD[0] = 0;
					return;
				}
			}
		}
	}
	MAP[i][j] = 10;

	motarOn = 1;
	motarPower = power;
	motarX = i;
	motarY = j;
	motarHealth = 200.f;

	float x = (10 - i)* unit;
	float y = (-10 + j)* unit;
	auto motar = Sprite3D::create("box/box.c3t");
	motar->setTexture("box/Motar.png");
	motar->setScale(unit*0.8);
	motar->setPosition3D(Vec3(x, -2.2f, y));
	game->addChild(motar);
	motar->setCameraMask((unsigned short)CameraFlag::USER1);
	motar->setTag(i * 10 + j * 1000);

	motar->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.3f), CallFunc::create([=]() {
		motarHealth -= 1.5f;
		if (motarHealth <= 0)
			motarDestroy();
	}), nullptr)));

	motar->runAction(RepeatForever::create(Sequence::create(DelayTime::create(6.f), CallFunc::create([=]() {
		int m, enemyI, enemyJ;
		int distance;
		bool throwBall = 0;
		this->calCurrentPlayerPosition();
		for (m = 0; m < emenyNum; m++)
		{
			enemyI = emenyTeamPosX[m];
			enemyJ = emenyTeamPosY[m];

			if (enemyI > i)
				distance = enemyI - i;
			else
				distance = i - enemyI;
			if (enemyJ > j)
				distance += enemyJ - j;
			else
				distance += j - enemyJ;

			if (distance < 20 && distance > 5)
			{
				throwBall = 1;
				break;
			}
		}
		if (enemyI == 0 || enemyJ == 0)
			throwBall = 0;
		if (throwBall)
		{
			auto motarBall = Sprite3D::create("box/sphere.c3b");
			motarBall->setTexture("box/c4.png");
			motarBall->setScale(unit*0.04);
			motarBall->setPosition3D(Vec3(x, -1.f, y));
			game->addChild(motarBall);
			motarBall->setCameraMask((unsigned short)CameraFlag::USER1);
			Vec3 throwDirection = Vec3(-3.f * (enemyI - i), 0, 3.f * (enemyJ - j));
			motarBall->runAction(RotateBy::create(3.f, Vec3(rand() % 100, 700.f, rand() % 100)));
			motarBall->runAction(Sequence::create(JumpBy::create(3.f, throwDirection, 10.f, 1), CallFunc::create([=]() {
				motarBall->removeFromParent();
				game->fireFlame(5, power, enemyI, enemyJ, 0);
			}), nullptr));
			motarBall->runAction(Sequence::create(DelayTime::create(1.5f), CallFunc::create([=]() {
				game->addThreatArea(enemyI, enemyJ, power);
			}), nullptr));
		}
	}), nullptr)));
}

void Player::cannonDestroy(int index)
{
	cannonOn[index] = 0;
	cannonNum--;

	int i = cannonX[index];
	int j = cannonY[index];
	
	MAP[i][j] = 0;
	float x = (10 - i)* unit;
	float y = (-10 + j)* unit;
	auto ps = PUParticleSystem3D::create("Particle3D/scripts/explosionSystem.pu");
	ps->setPosition3D(Vec3(x, -2.2f, y));
	ps->setScale(2);
	ps->startParticleSystem();
	ps->setCameraMask(2);
	game->addChild(ps);
	ps->runAction(Sequence::create(DelayTime::create(boom3DAnimationPeriod), CallFunc::create([=]() {
		ps->removeFromParent();
	}), nullptr));
	game->fireFlame(5, 1, i, j, 0);

	if (game->getChildByTag(i * 10 + j * 1000) == nullptr)
		return;
	(game->getChildByTag(i * 10 + j * 1000))->stopAllActions();
	game->removeChildByTag(i * 10 + j * 1000);
}

void Player::motarDestroy()
{
	motarOn = 0;

	int i = motarX;
	int j = motarY;

	MAP[i][j] = 0;
	float x = (10 - i)* unit;
	float y = (-10 + j)* unit;
	auto ps = PUParticleSystem3D::create("Particle3D/scripts/explosionSystem.pu");
	ps->setPosition3D(Vec3(x, -2.2f, y));
	ps->setScale(2);
	ps->startParticleSystem();
	ps->setCameraMask(2);
	game->addChild(ps);
	ps->runAction(Sequence::create(DelayTime::create(boom3DAnimationPeriod), CallFunc::create([=]() {
		ps->removeFromParent();
	}), nullptr));
	game->fireFlame(5, 1, i, j, 0);

	if (game->getChildByTag(i * 10 + j * 1000) == nullptr)
		return;
	(game->getChildByTag(i * 10 + j * 1000))->stopAllActions();
	game->removeChildByTag(i * 10 + j * 1000);
}

void Player::gateDestroy(int index)
{
	gateOn[index] = 0;
	gateNum--;

	int i = gateX[index];
	int j = gateY[index];

	MAP[i][j] = 0;
	game->minusThreatArea(i, j, 10);
	float x = (10 - i)* unit;
	float y = (-10 + j)* unit;
	auto ps = PUParticleSystem3D::create("Particle3D/scripts/explosionSystem.pu");
	ps->setPosition3D(Vec3(x, -2.2f, y));
	ps->setScale(2);
	ps->startParticleSystem();
	ps->setCameraMask(2);
	game->addChild(ps);
	ps->runAction(Sequence::create(DelayTime::create(boom3DAnimationPeriod), CallFunc::create([=]() {
		ps->removeFromParent();
	}), nullptr));
	game->fireFlame(5, 1, i, j, 0);

	if (game->getChildByTag(i * 10 + j * 1000) == nullptr)
		return;
	(game->getChildByTag(i * 10 + j * 1000))->stopAllActions();
	game->removeChildByTag(i * 10 + j * 1000);
}

