#include "HelloWorldScene.h"

#define MAP game->map

USING_NS_CC;

extern BomberGame * game;
static const float unit = 3.f;

Player* Player::create(bool is_com, int _occupation, int _team)
{
	auto _player = new (std::nothrow) Player();
	if (_player)
	{
		_player->occupation = _occupation;
		_player->team = _team;
		_player->controlByMan = !is_com;
		if (_player->occupation == 1)
		{
			_player->player = Sprite3D::create("box/box.c3t");
			if (_player->team == 1)
				_player->player->setTexture("box/CyanSquare.png");
			else if (_player->team == 2)
				_player->player->setTexture("box/CyanSquare3.png");
			_player->player->setScale(2.5f);
			game->addChild(_player->player);
			_player->player->setCameraMask(2);
		}
		else if (_player->occupation == 2)
		{
			_player->player = Sprite3D::create("box/box.c3t");
			if (_player->team == 1)
				_player->player->setTexture("box/CyanSquare4.png");
			else if (_player->team == 2)
				_player->player->setTexture("box/CyanSquare7.png");
			_player->player->setScale(2.5f);
			game->addChild(_player->player);
			_player->player->setCameraMask(2);
		}
		else if (_player->occupation == 3)
		{
			_player->player = Sprite3D::create("box/box.c3t");
			if (_player->team == 1)
				_player->player->setTexture("box/CyanSquare5.png");
			else if (_player->team == 2)
				_player->player->setTexture("box/CyanSquare6.png");
			_player->player->setScale(2.5f);
			game->addChild(_player->player);
			_player->player->setCameraMask(2);
		}
		return _player;
	}
	CC_SAFE_DELETE(_player);
	return nullptr;
}

void Player::playerInit(int i, int j)
{
	this->i = i;
	this->j = j;
	x = (10 - i)* unit;
	y = (-10 + j)* unit;
	player->setPosition3D(Vec3(x, -2.2f, y));
	//	game->_camera->setPosition3D(cocos2d::Vec3(x - 11.f, 18.f, y));
	//	game->_camera->lookAt(cocos2d::Vec3(x, -2.2f, y), cocos2d::Vec3(0.0f, 1.0f, 0.0f));
	CD = 0;
	skillCD[0] = 0;
	skillCD[1] = 0;
	skillCD[2] = 0;
	//basic
	health = 3;	
	strength = 3;
	agility = 10;
	intelligence = 10;
	faceTo = 0;
	canControl = 1;
	//engineer
	remoteBombOn = 0;
	automaticBombOn = 0;
	bounceBombOn = 0;
	//mad bomber
	rushCount = 0;
	onRush = 0;
	onThrowing = 0;
	//architect
	gateNum = 0;
	cannonNum = 0;
	motarOn = 0;
	gateOn[0] = gateOn[1] = gateOn[2] = 0;
	cannonOn[0] = cannonOn[1] = cannonOn[2] = 0;
	//data init
//	name = "name";
	runLength = 0;
	placeBombNum = 0;
	useSkillNum = 0;
	getItemNum = 0;
	//
	displayUpdate(2);
	if (!controlByMan)
	{
		routeSchedule = new RouteSchedule;
		route = nullptr;
		inRoute = 0;
		aiState = 10;
		aiCount = 0;
		player->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.035f),CallFunc::create([=]() {
			if(game->gameStart)
				ai();
		}), nullptr)));
	}
	else
	{
		game->playerInControl = this;
		game->board();
		game->attributeDisplay();
		player->runAction(RepeatForever::create(Sequence::create(DelayTime::create(0.035f), CallFunc::create([=]() {
			if (game->gameStart)
				manControl();
		}), nullptr)));
	}
}

void Player::teamClassify(int playerNum)
{
	int i, j, k;
	_friendTeamPosX = new int *[playerNum];
	_friendTeamPosY = new int *[playerNum];
	_emenyTeamPosX = new int *[playerNum];
	_emenyTeamPosY = new int *[playerNum];
	friendTeamPosX = new int[playerNum];
	friendTeamPosY = new int[playerNum];
	emenyTeamPosX = new int[playerNum];
	emenyTeamPosY = new int[playerNum];

	for (i = 0; i < playerNum; i++)
	{
		_friendTeamPosX[i] = new int;
		_friendTeamPosY[i] = new int;
		_emenyTeamPosX[i] = new int;
		_emenyTeamPosY[i] = new int;

		_friendTeamPosX[i] = nullptr;
		_friendTeamPosY[i] = nullptr;
		_emenyTeamPosX[i] = nullptr;
		_emenyTeamPosY[i] = nullptr;

		friendTeamPosX[i] = 0;
		friendTeamPosY[i] = 0;
		emenyTeamPosX[i] = 0;
		emenyTeamPosY[i] = 0;
	}

	i = 0;
	j = 0;
	for (k = 0; k < playerNum; k++)
	{
		if (this != game->player[k])
		{
			if (team == game->player[k]->team)
			{
				_friendTeamPosX[i] = &game->player[k]->i;
				_friendTeamPosY[i] = &game->player[k]->j;
				i++;
			}
			else
			{
				_emenyTeamPosX[j] = &game->player[k]->i;
				_emenyTeamPosY[j] = &game->player[k]->j;
				j++;
			}
		}
	}

	friendNum = i;
	emenyNum = j;
}

void Player::calCurrentPlayerPosition()
{
	int i, j, k;
	i = 0;
	j = 0;
	for (k = 0; k < game->playerNum; k++)
	{
		friendTeamPosX[k] = 0;
		friendTeamPosY[k] = 0;
		emenyTeamPosX[k] = 0;
		emenyTeamPosY[k] = 0;

		if (_friendTeamPosX[k] != nullptr)
		{
			friendTeamPosX[i] = *_friendTeamPosX[k];
			friendTeamPosY[i] = *_friendTeamPosY[k];
			i++;
		}
		if (_emenyTeamPosX[k] != nullptr)
		{
			emenyTeamPosX[j] = *_emenyTeamPosX[k];
			emenyTeamPosY[j] = *_emenyTeamPosY[k];
			j++;
		}
	}
	friendNum = i;
	emenyNum = j;
}

bool Player::nearEnemy()
{
	bool value = 0;
	int i, manx, many, x, y;
	x = this->i;
	y = this->j;
	for (i = 0; i < emenyNum; i++)
	{
		manx = emenyTeamPosX[i];
		many = emenyTeamPosY[i];
		if ((x + 1 == manx && y == many)
			|| (x - 1 == manx && y == many)
			|| (x == manx && y + 1 == many)
			|| (x == manx && y - 1 == many))
			value = 1;
	}
	return value;
}

bool Player::nearEnemyBuilding()
{
	bool value = 0;
	int n, m, x, y;
	x = this->i;
	y = this->j;
	for (n = 0; n < game->playerNum; n++)
	{
		if (game->player[n]->team != team && game->player[n]->occupation == 3)
		{
			for (m = 0; m < 3; m++)
			{
				if (game->player[n]->gateOn[m])
				{
					int px = game->player[n]->gateX[m];
					int py = game->player[n]->gateY[m];
					if ((x + 1 == px && y == py)
						|| (x - 1 == px && y == py)
						|| (x == px && y + 1 == py)
						|| (x == px && y - 1 == py))
						value = 1;
				}
				if (game->player[n]->cannonOn[m])
				{
					int px = game->player[n]->cannonX[m];
					int py = game->player[n]->cannonY[m];
					if ((x + 1 == px && y == py)
						|| (x - 1 == px && y == py)
						|| (x == px && y + 1 == py)
						|| (x == px && y - 1 == py))
						value = 1;
				}
			}
			if (game->player[n]->motarOn)
			{
				int px = game->player[n]->motarX;
				int py = game->player[n]->motarY;
				if ((x + 1 == px && y == py)
					|| (x - 1 == px && y == py)
					|| (x == px && y + 1 == py)
					|| (x == px && y - 1 == py))
					value = 1;
			}
		}
	}
	return value;
}

void Player::displayUpdate(int func)
{
	if (func == 0)
	{
		boardDisplay->setPosition3D(Vec3(x, 1.f, y));
	}
	else if (func == 1)
	{
		boardDisplay->removeAllChildren();

		if (health != 0)
		{
			char* str = new char[10];
			const std::string strName = name;
			TTFConfig ttfConfig("fonts/Marker Felt.ttf", 20);
			labelName = Label::createWithTTF(ttfConfig, strName);
			labelName->setScale(0.04f);
			if (team == 1)
				labelName->setColor(Color3B::BLUE);
			if (team == 2)
				labelName->setColor(Color3B::RED);
			labelName->setPosition3D(Vec3(0, 0.5f, 0));
			labelName->setCameraMask(2);
			boardDisplay->addChild(labelName);
		}
		if (health == 1)
		{
			auto heart = Sprite::create("box/pu_hart.png");
			heart->setScale(0.003f);
			heart->setPosition3D(Vec3::ZERO);
			heart->setCameraMask(2);
			boardDisplay->addChild(heart);
		}
		else if (health == 2)
		{
			auto heart = Sprite::create("box/pu_hart.png");
			heart->setScale(0.003f);
			heart->setPosition3D(Vec3(0.25, 0, 0));
			heart->setCameraMask(2);
			boardDisplay->addChild(heart);

			heart = Sprite::create("box/pu_hart.png");
			heart->setScale(0.003f);
			heart->setPosition3D(Vec3(-0.25, 0, 0));
			heart->setCameraMask(2);
			boardDisplay->addChild(heart);
		}
		else if (health == 3)
		{
			auto heart = Sprite::create("box/pu_hart.png");
			heart->setScale(0.003f);
			heart->setPosition3D(Vec3::ZERO);
			heart->setCameraMask(2);
			boardDisplay->addChild(heart);

			heart = Sprite::create("box/pu_hart.png");
			heart->setScale(0.003f);
			heart->setPosition3D(Vec3(0.5, 0, 0));
			heart->setCameraMask(2);
			boardDisplay->addChild(heart);

			heart = Sprite::create("box/pu_hart.png");
			heart->setScale(0.003f);
			heart->setPosition3D(Vec3(-0.5, 0, 0));
			heart->setCameraMask(2);
			boardDisplay->addChild(heart);
		}
		else if (health == 4)
		{
			auto heart = Sprite::create("box/pu_hart.png");
			heart->setScale(0.003f);
			heart->setPosition3D(Vec3(0.25, 0, 0));
			heart->setCameraMask(2);
			boardDisplay->addChild(heart);

			heart = Sprite::create("box/pu_hart.png");
			heart->setScale(0.003f);
			heart->setPosition3D(Vec3(-0.25, 0, 0));
			heart->setCameraMask(2);
			boardDisplay->addChild(heart);

			heart = Sprite::create("box/pu_hart.png");
			heart->setScale(0.003f);
			heart->setPosition3D(Vec3(0.75, 0, 0));
			heart->setCameraMask(2);
			boardDisplay->addChild(heart);

			heart = Sprite::create("box/pu_hart.png");
			heart->setScale(0.003f);
			heart->setPosition3D(Vec3(-0.75, 0, 0));
			heart->setCameraMask(2);
			boardDisplay->addChild(heart);
		}
		else if (health == 5)
		{
			auto heart = Sprite::create("box/pu_hart.png");
			heart->setScale(0.003f);
			heart->setPosition3D(Vec3::ZERO);
			heart->setCameraMask(2);
			boardDisplay->addChild(heart);

			heart = Sprite::create("box/pu_hart.png");
			heart->setScale(0.003f);
			heart->setPosition3D(Vec3(0.5, 0, 0));
			heart->setCameraMask(2);
			boardDisplay->addChild(heart);

			heart = Sprite::create("box/pu_hart.png");
			heart->setScale(0.003f);
			heart->setPosition3D(Vec3(-0.5, 0, 0));
			heart->setCameraMask(2);
			boardDisplay->addChild(heart);

			heart = Sprite::create("box/pu_hart.png");
			heart->setScale(0.003f);
			heart->setPosition3D(Vec3(1, 0, 0));
			heart->setCameraMask(2);
			boardDisplay->addChild(heart);

			heart = Sprite::create("box/pu_hart.png");
			heart->setScale(0.003f);
			heart->setPosition3D(Vec3(-1, 0, 0));
			heart->setCameraMask(2);
			boardDisplay->addChild(heart);
		}
	}
	else if (func == 2)
	{
		boardDisplay = BillBoard::create();
		boardDisplay->setPosition3D(Vec3(x, 1.f, y));
		boardDisplay->setCameraMask(2);
		boardDisplay->setMode(BillBoard::Mode::VIEW_PLANE_ORIENTED);
		game->addChild(boardDisplay);

		char* str = new char[10];
		const std::string strName = name;
		TTFConfig ttfConfig("fonts/Marker Felt.ttf", 20);
		labelName = Label::createWithTTF(ttfConfig, strName);
		labelName->setScale(0.04f);
		if (team == 1)
			labelName->setColor(Color3B::BLUE);
		if (team == 2)
			labelName->setColor(Color3B::RED);
		labelName->setPosition3D(Vec3(0, 0.5f, 0));
		labelName->setCameraMask(2);
		boardDisplay->addChild(labelName);

		auto heart = Sprite::create("box/pu_hart.png");
		heart->setScale(0.003f);
		heart->setPosition3D(Vec3::ZERO);
		heart->setCameraMask(2);
		boardDisplay->addChild(heart);

		heart = Sprite::create("box/pu_hart.png");
		heart->setScale(0.003f);
		heart->setPosition3D(Vec3(0.5, 0, 0));
		heart->setCameraMask(2);
		boardDisplay->addChild(heart);

		heart = Sprite::create("box/pu_hart.png");
		heart->setScale(0.003f);
		heart->setPosition3D(Vec3(-0.5, 0, 0));
		heart->setCameraMask(2);
		boardDisplay->addChild(heart);
	}
	else if (func == 3)
	{
		boardDisplay->removeAllChildren();
	}
}

