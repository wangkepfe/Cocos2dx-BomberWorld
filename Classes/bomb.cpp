#include "HelloWorldScene.h"

#include "VisibleRect.h"
#include "extensions/Particle3D/PU/CCPUParticleSystem3D.h"
USING_NS_CC;

#define MAX_BOMB 30
extern BomberGame * game;
static const float bombAnimationPeriod = 0.5f;
static const float boom3DAnimationPeriod = 1.5f;
static const float multibombPassTime = 0.08f;

static float unit = 3.f;
static struct bombstruct {
	int i;
	int j;
	int power;
}bombs[MAX_BOMB];
static int bombpoint = 0;

void BomberGame::bombInit()
{
	int i,j;
	for (i = 0; i < MAX_BOMB; i++)
	{
		bombs[i].i = 999;
		bombs[i].j = 999;
		bombs[i].power = 999;
	}
	threatMap = new short *[21];
	for (i = 0; i < 21; i++)
	{
		threatMap[i] = new short[21];
	}
	for (i = 0; i < 21; i++)
	{
		for (j = 0; j < 21; j++)
			threatMap[i][j] = 0;
	}
}

void BomberGame::hurt(Player* player, int i, int j, int initI, int initJ)
{
	if (player->i == i && player->j == j)
	{
		player->health--; //hurt
		if (player->health <= 0)
		{
			player->health = 0;
			player->i = 50;
			player->j = 50;
			player->x = (10 - player->i)* unit;
			player->y = (-10 + player->j)* unit;
			player->player->setPositionX(player->x);
			player->player->setPositionY(player->y);
			player->CD = 1;
			player->skillCD[0] = 1;
			player->skillCD[1] = 1;
			player->skillCD[2] = 1;
			player->player->setVisible(0);		
			if (player == playerInControl)
			{
				cameraPosition = 2;
				_camera->setPosition3D(Vec3(-7.f - 28.f, 46.f, 0));
				_camera->lookAt(Vec3(-7.f, -2.2f, 0), Vec3(0.0f, 1.0f, 0.0f));
			}
			player->displayUpdate(3);	
		}
		if (player == playerInControl)
			attributeDisplayUpdate();
		player->displayUpdate(1);
	}
}

void BomberGame::smallBlast(int i, int j)
{	
	for (int k = 0; k < playerNum; k++)
	{
		hurt(player[k], i, j, 1, 1);
	}
//	hurt(player[0], i, j, 1, 1);
//	hurt(player[1], i, j, 1, 1);
//	hurt(player[2], i, j, 1, 1);
//	hurt(player[3], i, j, 1, 1);

	float x = (10 - i)* unit;
	float y = (-10 + j)* unit;
	auto firebill = BillBoard::create();
	firebill->setPosition3D(Vec3(x, -2.2f, y));
	firebill->setCameraMask(2);
	game->addChild(firebill);
	firebill->runAction(Sequence::create(DelayTime::create(bombAnimationPeriod), CallFunc::create([=]() {
		firebill->removeFromParent();
	}), nullptr));

	auto fire = ParticleSystemQuad::create("Particles/SmallSun.plist");
	fire->setPosition(Vec2::ZERO);
	fire->setScale(0.05f);
	fire->setCameraMask(2);
	fire->setPositionType(ParticleSystem::PositionType::RELATIVE);
	firebill->addChild(fire);

	threatMap[i][j] -= 1;
	if (threatMap[i][j] < 0)
		threatMap[i][j] = 0;
}

void Player::createBomb(int i, int j, int power)
{
	if (i < 0 || j < 0 || i > 20 || j > 20)
		return;
	if (game->map[i][j] != 0)
	{
		CD = 0;
		placeBombNum--;
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
					CD = 0;
					placeBombNum--;
					return;				
				}
			}
		}
	}

	bombs[bombpoint].i = i;
	bombs[bombpoint].j = j;
	bombs[bombpoint].power = power;

	bombpoint++;
	if (bombpoint == MAX_BOMB)
		bombpoint = 0;

	game->map[i][j] = 3;
	float x = (10 - i)* unit;
	float y = (-10 + j)* unit;
	auto bomb = Sprite3D::create("box/box.c3t");
	bomb->setTexture("box/CyanSquare2.png");
	bomb->setScale(unit*0.8);
	bomb->setPosition3D(Vec3(x, -2.2f, y));
	game->addChild(bomb);
	bomb->setCameraMask((unsigned short)CameraFlag::USER1);
	bomb->setTag(i * 10 + j * 1000);
	bomb->runAction(Sequence::create(DelayTime::create(4.0f), CallFunc::create([=]()
	{
		game->boom(i, j);
	}), nullptr));
	bomb->runAction(Sequence::create(DelayTime::create(2.0f), CallFunc::create([=]()
	{
		game->addThreatArea(i,j,power);
	}), nullptr));
}
	
void BomberGame::boom(int i, int j)
{
	bool found = 0;
	int k;
	for (k = 0; k < MAX_BOMB; k++)
	{
		if (bombs[k].i == i && bombs[k].j == j)
		{
			found = 1;
			break;
		}
	}
	if (found == 1)
	{
		(game->getChildByTag(i * 10 + j * 1000))->stopAllActions();
		game->removeChildByTag(i * 10 + j * 1000);
		
		map[i][j] = 0;
	/*	float x = (10 - i)* unit;
		float y = (-10 + j)* unit;
		auto ps = PUParticleSystem3D::create("Particle3D/scripts/explosionSystem.pu");
		ps->setPosition3D(Vec3(x, -2.2f, y));
		ps->setScale(2);
		ps->startParticleSystem();
		ps->setCameraMask(2);
		game->addChild(ps);
		ps->runAction(Sequence::create(DelayTime::create(boom3DAnimationPeriod), CallFunc::create([=]() {
			ps->removeFromParent();
		}), nullptr));*/
		fireFlame(5, bombs[k].power, i, j ,0);
		bombs[k].i = 999;
		bombs[k].j = 999;
		bombs[k].power = 999;
	}
}

void BomberGame::fireFlame(int direction, int power, int i, int j, bool isRemote)
{
	if (power <= 0)
		return;
	if (direction == 5)
	{
		smallBlast(i,j);
		game->runAction(Sequence::create(DelayTime::create(multibombPassTime), CallFunc::create([=]()
		{
			fireOrBoom(1, power - 1, i + 1, j, isRemote);
			fireOrBoom(2, power - 1, i, j + 1, isRemote);
			fireOrBoom(3, power - 1, i - 1, j, isRemote);
			fireOrBoom(4, power - 1, i, j - 1, isRemote);
		}), nullptr));
	}
	else if (direction == 1){
		smallBlast(i, j);
		game->runAction(Sequence::create(DelayTime::create(multibombPassTime), CallFunc::create([=](){
			fireOrBoom(1, power - 1, i + 1, j, isRemote);
		}), nullptr));
	}
	else if (direction == 2) {
		smallBlast(i, j);
		game->runAction(Sequence::create(DelayTime::create(multibombPassTime), CallFunc::create([=]() {
			fireOrBoom(2, power - 1, i, j + 1, isRemote);
		}), nullptr));
	}
	else if (direction == 3) {
		smallBlast(i, j);
		game->runAction(Sequence::create(DelayTime::create(multibombPassTime), CallFunc::create([=]() {
			fireOrBoom(3, power - 1, i - 1, j, isRemote);
		}), nullptr));
	}
	else if (direction == 4) {
		smallBlast(i, j);
		game->runAction(Sequence::create(DelayTime::create(multibombPassTime), CallFunc::create([=]() {
			fireOrBoom(4, power - 1, i, j - 1, isRemote);
		}), nullptr));
	}
}

void BomberGame::fireOrBoom(int direction, int power, int i, int j, bool isRemote)
{
	if (i < 0 || j < 0 || i > 20 || j > 20)
		return;
	if (!isRemote)
	{
		if (power <= 0)
			return;
		if (map[i][j] <= 0) // fire
		{
			fireFlame(direction, power, i, j, isRemote);
		}
		if (map[i][j] == 2) // box
		{
			int randNum = rand() % 8;
			if (randNum == 1)createItems(1, i, j);
			else if (randNum == 2)createItems(2, i, j);
			else if (randNum == 3)createItems(3, i, j);
			else if (randNum == 4)createItems(4, i, j);
			else
				map[i][j] = 0;

			game->removeChildByTag(i * 10 + j * 1000);
			fireFlame(direction, 1, i, j, isRemote);
		}
		else if (map[i][j] == 3) // boom
		{
			boom(i, j);
		}
		else if (map[i][j] == 5) // trap
		{
			for (int k = 0; k < playerNum; k++)
			{
				if (game->player[k]->automaticBombX == i && game->player[k]->automaticBombY == j)
					game->player[k]->automaticBombBoom();
			}	
		}
		else if (map[i][j] == 6) // bounce
		{
			for (int k = 0; k < playerNum; k++)
			{
				if (game->player[k]->bounceBombX == i && game->player[k]->bounceBombY == j)
					game->player[k]->bounceBombBoom();
			}
		}
		else if (map[i][j] == 7) // cannon
		{
			for (int k = 0; k < playerNum; k++)
			{
				for (int m = 0; m < 3; m++)
				{
					if (game->player[k]->cannonOn[m] && game->player[k]->cannonX[m] == i && game->player[k]->cannonY[m] == j)
					{
						game->player[k]->cannonHealth[m] -= 40.f;
						if(game->player[k]->cannonHealth[m] <= 0)
							game->player[k]->cannonDestroy(m);
					}
				}
			}
		}
		else if (map[i][j] == 8) // gate
		{
			for (int k = 0; k < playerNum; k++)
			{
				for (int m = 0; m < 3; m++)
				{
					if (game->player[k]->gateOn[m] && game->player[k]->gateX[m] == i && game->player[k]->gateY[m] == j)
					{
						game->player[k]->gateHealth[m] -= 40.f;
						if (game->player[k]->gateHealth[m] <= 0)
							game->player[k]->gateDestroy(m);
					}
				}
			}
		}
	}
	else
	{
		power++;
		if (map[i][j] == 3) // boom
		{
			boom(i, j);
		}
		else if (map[i][j] == 5) // trap
		{
			for (int k = 0; k < playerNum; k++)
			{
				if (game->player[k]->automaticBombX == i && game->player[k]->automaticBombY == j)
					game->player[k]->automaticBombBoom();
			}
		}
		else if (map[i][j] == 6) // bounce
		{
			for (int k = 0; k < playerNum; k++)
			{
				if (game->player[k]->bounceBombX == i && game->player[k]->bounceBombY == j)
					game->player[k]->bounceBombBoom();
			}
		}
		else if (map[i][j] == 7) // cannon
		{
			for (int k = 0; k < playerNum; k++)
			{
				for (int m = 0; m < 3; m++)
				{
					if (game->player[k]->cannonOn[m] && game->player[k]->cannonX[m] == i && game->player[k]->cannonY[m] == j)
					{
						game->player[k]->cannonHealth[m] -= 40.f;
						if (game->player[k]->cannonHealth[m] <= 0)
							game->player[k]->cannonDestroy(m);
					}
				}
			}
		}
		else if (map[i][j] == 8) // gate
		{
			for (int k = 0; k < playerNum; k++)
			{
				for (int m = 0; m < 3; m++)
				{
					if (game->player[k]->gateOn[m] && game->player[k]->gateX[m] == i && game->player[k]->gateY[m] == j)
					{
						game->player[k]->gateHealth[m] -= 40.f;
						if (game->player[k]->gateHealth[m] <= 0)
							game->player[k]->gateDestroy(m);
					}
				}
			}
		}
		if (map[i][j] == 2) // box
		{
			int randNum = rand() % 8;
			if (randNum == 1)createItems(1, i, j);
			else if (randNum == 2)createItems(2, i, j);
			else if (randNum == 3)createItems(3, i, j);
			else if (randNum == 4)createItems(4, i, j);
			else
				map[i][j] = 0;
			game->removeChildByTag(i * 10 + j * 1000);
		}
		fireFlame(direction, power, i, j, isRemote);
	}
}

void BomberGame::addThreatArea(int i, int j, int power)
{
	int k;
	threatMap[i][j] += 1;
	for (k = 1; k < power && map[i + k][j] == 0 && k <= 10; k++)
		threatMap[i + k][j] += 1;
	for (k = 1; k < power && map[i][j + k] == 0 && k <= 10; k++)
		threatMap[i][j + k] += 1;
	for (k = 1; k < power && map[i - k][j] == 0 && k <= 10; k++)
		threatMap[i - k][j] += 1;
	for (k = 1; k < power && map[i][j - k] == 0 && k <= 10; k++)
		threatMap[i][j - k] += 1;
}

void BomberGame::minusThreatArea(int i, int j, int power)
{
	int k;
	if(threatMap[i][j]>0)
		threatMap[i][j] -= 1;

	for (k = 1; k < power && map[i + k][j] == 0 && k <= 10; k++)
	{
		if(threatMap[i + k][j]>0)
			threatMap[i + k][j] -= 1;
	}	
	for (k = 1; k < power && map[i][j + k] == 0 && k <= 10; k++)
	{
		if (threatMap[i][j + k]>0)
			threatMap[i][j + k] -= 1;
	}
	for (k = 1; k < power && map[i - k][j] == 0 && k <= 10; k++)
	{
		if (threatMap[i - k][j]>0)
			threatMap[i - k][j] -= 1;
	}
	for (k = 1; k < power && map[i][j - k] == 0 && k <= 10; k++)
	{
		if (threatMap[i][j - k]>0)
			threatMap[i][j - k] -= 1;
	}
}