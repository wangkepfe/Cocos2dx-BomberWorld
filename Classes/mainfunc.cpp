#include "VisibleRect.h"
#include "HelloWorldScene.h"

//#include "audio/include/AudioEngine.h"

USING_NS_CC;

BomberGame * game = nullptr;

bool BomberGame::init(int* param, char **paramc)
{
	if (!Scene::init())
		return false;
	//******************** create camera *************************
	Size size = Director::getInstance()->getWinSize();
	_camera = Camera::createPerspective(50.0f, size.width / size.height, 1.0f, 200.0f);
	_camera->setCameraFlag(CameraFlag::USER1);
	this->addChild(_camera);

	//****************** listener ***********************
	auto listener = EventListenerTouchAllAtOnce::create();
	listener->onTouchesBegan = CC_CALLBACK_2(BomberGame::onTouchesBegan, this);
	listener->onTouchesMoved = CC_CALLBACK_2(BomberGame::onTouchesMoved, this);
	listener->onTouchesEnded = CC_CALLBACK_2(BomberGame::onTouchesEnded, this);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	//******* switch camera for debug ************
	TTFConfig ttfConfig("fonts/Marker Felt.ttf", 10);
	auto label = Label::createWithTTF(ttfConfig, "Switch Camera");
	auto menuItem = MenuItemLabel::create(label, [=](Ref *ref) {
		if (cameraPosition == 1) cameraPosition = 2;
		else if (cameraPosition == 2)cameraPosition = 1;
	});
	auto menu = Menu::create(menuItem, nullptr);
	menu->setPosition(Vec2::ZERO);
	menuItem->setPosition(Vec2(VisibleRect::right().x - 50, VisibleRect::top().y - 50));
	this->addChild(menu);

	// ******* for global setting ******
	game = this;

	// *********** rand num seed ************8
	srand((unsigned)time(nullptr));

	//***************************** reading data ********************************
	//	_gold = CCUserDefault::sharedUserDefault()->getIntegerForKey("money",0);
	//***************************** fundamental config **************************
	cameraPosition = 2;
	_camera->setPosition3D(Vec3(-7.f - 28.f, 46.f, 0));
	_camera->lookAt(Vec3(-7.f, -2.2f, 0), Vec3(0.0f, 1.0f, 0.0f));

	gameStart = 0;
	//****************************** main scene **********************************
	terrain();
	createMap();
	bombInit();
	// *************************** player setting ***********************
	playerNum = 6;
	playerInControl = nullptr;
	player = new Player*[playerNum];
	player[0] = Player::create(0, param[0], 1);
	player[1] = Player::create(1, param[1], 1);
	player[2] = Player::create(1, param[2], 1);
	player[3] = Player::create(1, param[3], 2);
	player[4] = Player::create(1, param[4], 2);
	player[5] = Player::create(1, param[5], 2);
	/*player[6] = Player::create(1, 1, 4);
	player[7] = Player::create(1, 1, 4);*/

	for (int i = 0; i < playerNum; i++)
	{
		player[i]->teamClassify(playerNum);
		player[i]->name = paramc[i];
	}

	player[0]->playerInit(16, 4);
	player[1]->playerInit(17, 4);
	player[2]->playerInit(16, 5);
	player[3]->playerInit(4, 17);
	player[4]->playerInit(4, 16);
	player[5]->playerInit(5, 16);
	/*player[6]->playerInit(18, 18);
	player[7]->playerInit(18, 19);*/

	//***************************** menu & display ******************************
	countDown();
	gameOver(paramc);
	//***************************** audio system *********************************

	//	AudioEngine::play2d("audio/UFO knight.mp3",true);
	//***************************** quit to menu ************************************
	quitItem = MenuItemImage::create(
		"CloseNormal.png",
		"CloseSelected.png", [=](Ref *ref) {
		quitMenu();
	});
	menuQuit = Menu::create(quitItem, nullptr);
	menuQuit->setPosition(VisibleRect::rightTop() - Vec2(quitItem->getContentSize() / 2));
	addChild(menuQuit);

	_menu2init = 0;
	//*******************************************************************************

/*	this->runAction(Sequence::create(DelayTime::create(10.f), CallFunc::create([=]() {
		player[3]->health = 0;
		player[4]->health = 0;
		player[5]->health = 0;
	}),nullptr));*/

	//
	return true;
}

Scene* BomberGame::scene(int *param, char **paramc)
{
	return BomberGame::create(param, paramc);
}

BomberGame* BomberGame::create(int* param, char **paramc)
{
	BomberGame *pRet = new(std::nothrow) BomberGame();
	if (pRet && pRet->init(param, paramc))
	{
		pRet->autorelease();
		return pRet;
	}
	else
	{
		delete pRet;
		pRet = nullptr;
		return nullptr;
	}
}

void BomberGame::onTouchesBegan(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	if (touches.size())
	{
		auto touch = touches[0];
		auto coordinate = touch->getLocation();
		auto control_circle_x = VisibleRect::right().x * 0.12;
		auto control_circle_y = VisibleRect::top().y * 0.2;
		Vec2 direct = coordinate - Vec2(control_circle_x, control_circle_y);
		
		if ((coordinate.x - control_circle_x)*(coordinate.x - control_circle_x) + (coordinate.y - control_circle_y)*(coordinate.y - control_circle_y) < 115 * 115)
		{
			direction = direct.getNormalized();
			bMove = 1;		
			_circle->setPosition(coordinate.x, coordinate.y);
			_circle->setVisible(1);
		}
		event->stopPropagation();
	}
}

void BomberGame::onTouchesMoved(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	if (touches.size())
	{
		auto touch = touches[0];
		auto coordinate = touch->getLocation();
		auto control_circle_x = VisibleRect::right().x * 0.12;
		auto control_circle_y = VisibleRect::top().y * 0.2;
		Vec2 direct = coordinate - Vec2(control_circle_x, control_circle_y);
		direction = direct.getNormalized();
		if ((coordinate.x - control_circle_x)*(coordinate.x - control_circle_x) + (coordinate.y - control_circle_y)*(coordinate.y - control_circle_y) < 115 * 115)
		{
			_circle->setPosition(coordinate.x, coordinate.y);
		}
		else
		{
			_circle->setPosition(control_circle_x + 115 * direction.x, control_circle_y + 115 * direction.y);
		}
		event->stopPropagation();
	}
}

void BomberGame::onTouchesEnded(const std::vector<Touch*>& touches, cocos2d::Event  *event)
{
	if (!touches.empty())
	{
		bMove = 0;
		event->stopPropagation();
		_circle->setVisible(0);
	}
}