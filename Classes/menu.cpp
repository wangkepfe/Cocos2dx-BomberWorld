#include "VisibleRect.h"
#include "HelloWorldScene.h"
#include "menu.h"
#include "SimpleAudioEngine.h"

USING_NS_CC;
using namespace CocosDenshion;

char* itoa2(int num, char*str, int radix);

//******************************************** MainInterface ********************************************
Scene* MainInterface::scene()
{
	return MainInterface::create();
}

bool MainInterface::init()
{
	if (!Scene::init())
		return false;

	auto background = Sprite::create("image/main.png");
	background->setPosition(VisibleRect::center());
	addChild(background);

	TTFConfig ttfConfig("fonts/Marker Felt.ttf", 50);
	TTFConfig ttfConfig2("fonts/Marker Felt.ttf", 30);

	auto label = Label::createWithTTF(ttfConfig, "Game Start");
	label->setColor(Color3B::BLACK);
	auto item = MenuItemLabel::create(label, [=](Ref *ref) {
		auto scene = CharacterChoose::scene();
		Director::getInstance()->replaceScene(TransitionCrossFade::create(0.2f, scene));
	});
	auto menu = Menu::create(item, nullptr);
	addChild(menu);

	label = Label::createWithTTF(ttfConfig2, "Config");
	label->setColor(Color3B::BLACK);
	item = MenuItemLabel::create(label, [=](Ref *ref) {
		auto scene = ControlMenu::scene();
		Director::getInstance()->replaceScene(TransitionCrossFade::create(0.2f, scene));
	});
	menu = Menu::create(item, nullptr);
	menu->setPosition(VisibleRect::rightTop() - item->getContentSize());
	addChild(menu);

	/*
	//music system
	//预加载背景音乐
	SimpleAudioEngine::sharedEngine()->preloadBackgroundMusic(music / xxxx.mp3);
	//开始播放背景音乐，true表示循环
	SimpleAudioEngine::sharedEngine()->playBackgroundMusic(music / xxxx.mp3, true);
	//停止背景音乐，这是一个缺省参数函数，传参表示是否释放音乐文件
	SimpleAudioEngine::sharedEngine()->stopBackgroundMusic();
	//暂停背景音乐
	SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
	//重头调用背景音乐
	SimpleAudioEngine::sharedEngine()->rewindBackgroundMusic();
	//返回布尔型参数，表示是否在放着背景音乐
	SimpleAudioEngine::sharedEngine()->isBackgroundMusicPlaying();
	//设置音量0.0-1.0
	SimpleAudioEngine::sharedEngine()->setBackgroundMusicVolume(0.5);

	//预加载音效
	SimpleAudioEngine::sharedEngine()->preloadEffect(music / xxxx.mp3);
	//开始播放背景音效，false表示不循环
	SimpleAudioEngine::sharedEngine()->playEffect(music / xxxx.mp3, false);
	//停止音效，可以选择单独停掉一个音效，这个值是由playEffect返回的
	SimpleAudioEngine::sharedEngine()->stopEffect(m_nSoundId);
	//停止全部音效
	SimpleAudioEngine::sharedEngine()->stopAllEffects();
	//暂停单个音效
	SimpleAudioEngine::sharedEngine()->pauseEffect(m_nSoundId);
	//重新开始音效
	SimpleAudioEngine::sharedEngine()->resumeEffect(m_nSoundId);
	//暂停全部音效
	SimpleAudioEngine::sharedEngine()->pauseAllEffects();
	//重新开始全部音效
	SimpleAudioEngine::sharedEngine()->resumeAllEffects();
	//设置音效音量0.0-1.0
	SimpleAudioEngine::sharedEngine()->setEffectsVolume(0.5);
	//卸载音效
	SimpleAudioEngine::sharedEngine()->unloadEffect(music / xxxx.mp3);
	*/

	return true;
}

//******************************************** LoadInterface ********************************************
Scene* LoadInterface::scene()
{
	return LoadInterface::create();
}

LoadInterface::LoadInterface()
	: _numberOfSprites(21)
	, _numberOfLoadedSprites(0)
{
	auto size = Director::getInstance()->getWinSize();

	_labelLoading = Label::createWithTTF("loading...", "fonts/arial.ttf", 20);
	_labelPercent = Label::createWithTTF("%0", "fonts/arial.ttf", 20);

	_labelLoading->setPosition(Vec2(size.width / 2, size.height / 2 - 20));
	_labelPercent->setPosition(Vec2(size.width / 2, size.height / 2 + 20));

	this->addChild(_labelLoading);
	this->addChild(_labelPercent);

	// load textrues
	Director::getInstance()->getTextureCache()->addImageAsync("box/c4.png", CC_CALLBACK_1(LoadInterface::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("box/Cannon.png", CC_CALLBACK_1(LoadInterface::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("box/CyanSquare.png", CC_CALLBACK_1(LoadInterface::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("box/CyanSquare2.png", CC_CALLBACK_1(LoadInterface::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("box/CyanSquare3.png", CC_CALLBACK_1(LoadInterface::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("box/CyanSquare4.png", CC_CALLBACK_1(LoadInterface::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("box/CyanSquare5.png", CC_CALLBACK_1(LoadInterface::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("box/CyanSquare6.png", CC_CALLBACK_1(LoadInterface::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("box/CyanSquare7.png", CC_CALLBACK_1(LoadInterface::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("box/Gate.png", CC_CALLBACK_1(LoadInterface::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("box/Grass1.jpg", CC_CALLBACK_1(LoadInterface::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("box/Motar.png", CC_CALLBACK_1(LoadInterface::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("box/pu_hart.png", CC_CALLBACK_1(LoadInterface::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("box/plane.png", CC_CALLBACK_1(LoadInterface::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("box/road.jpg", CC_CALLBACK_1(LoadInterface::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("box/pu_hart.png", CC_CALLBACK_1(LoadInterface::loadingCallBack, this));

	Director::getInstance()->getTextureCache()->addImageAsync("image/glass_button.png", CC_CALLBACK_1(LoadInterface::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("image/main.png", CC_CALLBACK_1(LoadInterface::loadingCallBack, this));
	Director::getInstance()->getTextureCache()->addImageAsync("image/round_button.png", CC_CALLBACK_1(LoadInterface::loadingCallBack, this));

	this->runAction(Sequence::create(CallFunc::create([=]() {
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Particles/SmallSun.plist"); 
	}), CallFunc::create([=]() {loadingCallBack2(); }), nullptr));
	this->runAction(Sequence::create(CallFunc::create([=]() {
		SpriteFrameCache::getInstance()->addSpriteFramesWithFile("Particle3D/scripts/explosionSystem.pu");
	}), CallFunc::create([=]() {loadingCallBack2(); }), nullptr));
}

void LoadInterface::loadingCallBack(Texture2D *texture)
{
	++_numberOfLoadedSprites;
	char tmp[10];
	sprintf(tmp, "%%%d", (int)(((float)_numberOfLoadedSprites / _numberOfSprites) * 100));
	_labelPercent->setString(tmp);

	if (_numberOfLoadedSprites == _numberOfSprites)
	{
		this->removeChild(_labelLoading, true);
		this->removeChild(_labelPercent, true);
		//************* change scene here! ***************
		auto scene = MainInterface::scene();
		Director::getInstance()->replaceScene(TransitionCrossFade::create(0.2f, scene));
	}
}

void LoadInterface::loadingCallBack2()
{
	++_numberOfLoadedSprites;
	char tmp[10];
	sprintf(tmp, "%%%d", (int)(((float)_numberOfLoadedSprites / _numberOfSprites) * 100));
	_labelPercent->setString(tmp);

	if (_numberOfLoadedSprites == _numberOfSprites)
	{
		this->removeChild(_labelLoading, true);
		this->removeChild(_labelPercent, true);
		//************* change scene here! ***************
		auto scene = MainInterface::scene();
		Director::getInstance()->replaceScene(TransitionCrossFade::create(0.2f, scene));
	}
}

//******************************************** ControlMenu ********************************************
Scene* ControlMenu::scene()
{
	return ControlMenu::create();
}

bool ControlMenu::init()
{
	if (!Scene::init())
		return false;

	auto background = Sprite::create("image/main.png");
	background->setPosition(VisibleRect::center());
	addChild(background);

	Size size = Director::getInstance()->getWinSize();
	float x = size.width;
	float y = size.height;

	TTFConfig ttfConfig("fonts/Marker Felt.ttf", 40);
	TTFConfig ttfConfig2("fonts/Marker Felt.ttf", 30);

	auto label = Label::createWithTTF(ttfConfig, "Music");
	label->setColor(Color3B::BLACK);
	auto item = MenuItemLabel::create(label, [=](Ref *ref) {
		// music 
	});
	auto menu = Menu::create(item, nullptr);
	menu->setPosition(x * 0.5f, y * 0.5f);
	addChild(menu);
	
	label = Label::createWithTTF(ttfConfig, "Sound Effect");
	label->setColor(Color3B::BLACK);
	item = MenuItemLabel::create(label, [=](Ref *ref) {
		// sound effect
	});
	menu = Menu::create(item, nullptr);
	menu->setPosition(x * 0.5f, y * 0.3f);
	addChild(menu);

	label = Label::createWithTTF(ttfConfig2, "Main Interface");
	label->setColor(Color3B::BLACK);
	item = MenuItemLabel::create(label, [=](Ref *ref) {
		auto scene = MainInterface::scene();
		Director::getInstance()->replaceScene(TransitionCrossFade::create(0.2f, scene));
	});
	menu = Menu::create(item, nullptr);
	menu->setPosition(x * 0.8f, y * 0.3f);
	addChild(menu);

	return true;
}

//******************************************** CharacterChoose ********************************************
Scene * CharacterChoose::scene()
{
	return CharacterChoose::create();
}

bool CharacterChoose::init()
{
	if (!Scene::init())
		return false;

	auto background = Sprite::create("image/main.png");
	background->setPosition(VisibleRect::center());
	addChild(background);
	Size size = Director::getInstance()->getWinSize();

	_camera = Camera::createPerspective(50.0f, size.width / size.height, 1.0f, 50.0f);
	_camera->setCameraFlag(CameraFlag::USER1);
	_camera->setDepth(1);
	_camera->setPosition3D(Vec3(0, 0, 10.f));
	_camera->lookAt(Vec3::ZERO);
	addChild(_camera);

	Sprite3D *characer[3];
	Node *characterDisplay = Node::create();
	addChild(characterDisplay);

	int i;
	const float stepDistamce = 10.f;
	choice = 0;

	// Engineer 
	characer[0] = Sprite3D::create("box/box.c3t");
	characer[0]->setTexture("box/CyanSquare.png");
	// Bomber 
	characer[1] = Sprite3D::create("box/box.c3t");
	characer[1]->setTexture("box/CyanSquare4.png");
	// Architect
	characer[2] = Sprite3D::create("box/box.c3t");
	characer[2]->setTexture("box/CyanSquare5.png");

	for (i = 0; i < 3; i++)
	{
		characterDisplay->addChild(characer[i]);
		characer[i]->setScale(1.5f);
		characer[i]->setPosition3D(Vec3(i * stepDistamce, 0, 0));
		characer[i]->setCameraMask((unsigned short)CameraFlag::USER1);
		characer[i]->runAction(RepeatForever::create(RotateBy::create(4.f, Vec3(0, 360, 0))));
	}

	TTFConfig ttfConfig("fonts/Marker Felt.ttf", 50);

	auto label = Label::createWithTTF(ttfConfig, "Game Start");
	label->setColor(Color3B::BLACK);
	auto item = MenuItemLabel::create(label, [=](Ref *ref) {
		auto scene = WaitingScene::scene(choice + 1);
		Director::getInstance()->replaceScene(TransitionFade::create(0.2f, scene));
		//		Director::getInstance()->pushScene(scene);
	});
	auto menu = Menu::create(item, nullptr);
	menu->setPosition(Vec2(size.width * 0.5f, size.height * 0.25f));
	addChild(menu);

	label = Label::createWithTTF(ttfConfig, "Next");
	label->setColor(Color3B::BLACK);
	item = MenuItemLabel::create(label, [=](Ref *ref) {
		if (choice != 2)
		{
			characterDisplay->runAction(EaseExponentialOut::create(MoveBy::create(2.f, Vec3(-stepDistamce, 0, 0))));
			choice++;
		}
	});
	menu = Menu::create(item, nullptr);
	menu->setPosition(Vec2(size.width * 0.75, size.height * 0.25));
	addChild(menu);

	label = Label::createWithTTF(ttfConfig, "Last");
	label->setColor(Color3B::BLACK);
	item = MenuItemLabel::create(label, [=](Ref *ref) {
		if (choice != 0)
		{
			characterDisplay->runAction(EaseExponentialOut::create(MoveBy::create(2.f, Vec3(stepDistamce, 0, 0))));
			choice--;
		}
	});
	menu = Menu::create(item, nullptr);
	menu->setPosition(Vec2(size.width * 0.25, size.height * 0.25));
	addChild(menu);

	label = Label::createWithTTF(ttfConfig, "Back to Menu");
	label->setColor(Color3B::BLACK);
	item = MenuItemLabel::create(label, [=](Ref *ref) {
		auto scene = MainInterface::scene();
		Director::getInstance()->replaceScene(TransitionCrossFade::create(0.2f, scene));
	});
	menu = Menu::create(item, nullptr);
	menu->setPosition(Vec2(size.width - 1.2 * label->getContentSize().width, 1.2 * label->getContentSize().height));
	addChild(menu);

	return true;
}

//******************************************** WaitingScene ********************************************
Scene * WaitingScene::scene(int param)
{
	return WaitingScene::create(param);
}

WaitingScene * WaitingScene::create(int param)
{
	WaitingScene *pRet = new(std::nothrow) WaitingScene();
	if (pRet && pRet->init(param))
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

bool WaitingScene::init(int param)
{
	if (!Scene::init())
		return false;

	auto background = Sprite::create("image/main.png");
	background->setPosition(VisibleRect::center());
	addChild(background);

	TTFConfig ttfConfig("fonts/Marker Felt.ttf", 100);
	TTFConfig ttfConfig2("fonts/Marker Felt.ttf", 50);
	TTFConfig ttfConfig3("fonts/Marker Felt.ttf", 40);

	auto label = Label::createWithTTF(ttfConfig, "VS");
	label->setPosition(VisibleRect::center());
	label->setColor(Color3B::BLACK);
	this->addChild(label);

	Size size = Director::getInstance()->getWinSize();
	float _x = size.width;
	float _y = size.height;
	float x, y;
	int randNum;
	int *role;
	char ** name;
	role = new int[6];
	name = new char*[6];

	const std::string strRole1 = "Engineer";
	const std::string strRole2 = "Mad Bomber";
	const std::string strRole3 = "Architect";

	const std::string strPlayer1 = "Player";
	const std::string strPlayer2 = "Smart Friend";
	const std::string strPlayer3 = "Helpful Friend";
	const std::string strPlayer4 = "Tough Enemy";
	const std::string strPlayer5 = "Sneaky Enemy";
	const std::string strPlayer6 = "Frenzy Enemy";

	name[0] = "Player";
	name[1] = "Smart Friend";
	name[2] = "Helpful Friend";
	name[3] = "Tough Enemy";
	name[4] = "Sneaky Enemy";
	name[5] = "Frenzy Enemy";
	/*********************************************** name ***************************************************/
	/**********************  1 ************************/
	x = _x * .25f;
	y = _y * .6f;
	label = Label::createWithTTF(ttfConfig2, strPlayer1);
	label->setPosition(Vec2(x, y));
	label->setColor(Color3B::BLUE);
	this->addChild(label);
	/**********************  2 ************************/
	x = _x * .25f;
	y = _y * .4f;
	label = Label::createWithTTF(ttfConfig2, strPlayer2);
	label->setPosition(Vec2(x, y));
	label->setColor(Color3B::BLUE);
	this->addChild(label);
	/**********************  3 ************************/
	x = _x * .25f;
	y = _y * .2f;
	label = Label::createWithTTF(ttfConfig2, strPlayer3);
	label->setPosition(Vec2(x, y));
	label->setColor(Color3B::BLUE);
	this->addChild(label);
	/**********************  4 ************************/
	x = _x * .75f;
	y = _y * .6f;
	label = Label::createWithTTF(ttfConfig2, strPlayer4);
	label->setPosition(Vec2(x, y));
	label->setColor(Color3B::RED);
	this->addChild(label);
	/**********************  5 ************************/
	x = _x * .75f;
	y = _y * .4f;
	label = Label::createWithTTF(ttfConfig2, strPlayer5);
	label->setPosition(Vec2(x, y));
	label->setColor(Color3B::RED);
	this->addChild(label);
	/**********************  6 ************************/
	x = _x * .75f;
	y = _y * .2f;
	label = Label::createWithTTF(ttfConfig2, strPlayer6);
	label->setPosition(Vec2(x, y));
	label->setColor(Color3B::RED);
	this->addChild(label);
	/************************************************ role **************************************************/
	/**********************  1 ************************/
	x = _x * .15f;
	y = _y * .66f;
	role[0] = param;
	if (param == 1)
		label = Label::createWithTTF(ttfConfig3, strRole1);
	else if (param == 2)
		label = Label::createWithTTF(ttfConfig3, strRole2);
	else if (param == 3)
		label = Label::createWithTTF(ttfConfig3, strRole3);
	label->setPosition(Vec2(x, y));
	label->setColor(Color3B::BLUE);
	this->addChild(label);
	/**********************  2 ************************/
	x = _x * .15f;
	y = _y * .46f;
	randNum = rand() % 3 + 1;
	role[1] = randNum;
	if (randNum == 1)
		label = Label::createWithTTF(ttfConfig3, strRole1);
	else if (randNum == 2)
		label = Label::createWithTTF(ttfConfig3, strRole2);
	else if (randNum == 3)
		label = Label::createWithTTF(ttfConfig3, strRole3);
	label->setPosition(Vec2(x, y));
	label->setColor(Color3B::BLUE);
	this->addChild(label);
	/**********************  3 ************************/
	x = _x * .15f;
	y = _y * .26f;
	randNum = rand() % 3 + 1;
	role[2] = randNum;
	if (randNum == 1)
		label = Label::createWithTTF(ttfConfig3, strRole1);
	else if (randNum == 2)
		label = Label::createWithTTF(ttfConfig3, strRole2);
	else if (randNum == 3)
		label = Label::createWithTTF(ttfConfig3, strRole3);
	label->setPosition(Vec2(x, y));
	label->setColor(Color3B::BLUE);
	this->addChild(label);
	/**********************  4 ************************/
	x = _x * .85f;
	y = _y * .66f;
	randNum = rand() % 3 + 1;
	role[3] = randNum;
	if (randNum == 1)
		label = Label::createWithTTF(ttfConfig3, strRole1);
	else if (randNum == 2)
		label = Label::createWithTTF(ttfConfig3, strRole2);
	else if (randNum == 3)
		label = Label::createWithTTF(ttfConfig3, strRole3);
	label->setPosition(Vec2(x, y));
	label->setColor(Color3B::RED);
	this->addChild(label);
	/**********************  5 ************************/
	x = _x * .85f;
	y = _y * .46f;
	randNum = rand() % 3 + 1;
	role[4] = randNum;
	if (randNum == 1)
		label = Label::createWithTTF(ttfConfig3, strRole1);
	else if (randNum == 2)
		label = Label::createWithTTF(ttfConfig3, strRole2);
	else if (randNum == 3)
		label = Label::createWithTTF(ttfConfig3, strRole3);
	label->setPosition(Vec2(x, y));
	label->setColor(Color3B::RED);
	this->addChild(label);
	/**********************  6 ************************/
	x = _x * .85f;
	y = _y * .26f;
	randNum = rand() % 3 + 1;
	role[5] = randNum;
	if (randNum == 1)
		label = Label::createWithTTF(ttfConfig3, strRole1);
	else if (randNum == 2)
		label = Label::createWithTTF(ttfConfig3, strRole2);
	else if (randNum == 3)
		label = Label::createWithTTF(ttfConfig3, strRole3);
	label->setPosition(Vec2(x, y));
	label->setColor(Color3B::RED);
	this->addChild(label);
	/********************************************************************************************************/

	this->runAction(Sequence::create(DelayTime::create(1.f), CallFunc::create([=]() {
		auto scene = BomberGame::scene(role, name);
		Director::getInstance()->replaceScene(TransitionFade::create(0.2f, scene));
	}), nullptr));

	return true;
}

//******************************************** WinScene ********************************************
Scene * WinScene::scene(int * param, char **paramc)
{
	return WinScene::create(param, paramc);
}

WinScene * WinScene::create(int * param, char **paramc)
{
	WinScene *pRet = new(std::nothrow) WinScene();
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

bool WinScene::init(int * param, char **paramc)
{
	if (!Scene::init())
		return false;

	auto background = Sprite::create("image/main.png");
	background->setPosition(VisibleRect::center());
	addChild(background);

	int i, j;
	Size size = Director::getInstance()->getWinSize();
	float x = size.width;
	float y = size.height;
	TTFConfig ttfConfig2("fonts/Marker Felt.ttf", 30);
	TTFConfig ttfConfig3("fonts/Marker Felt.ttf", 20);

	//******************************* row *************************************
	// colonm 1 survive
	auto label = Label::createWithTTF(ttfConfig2, "Survive");
	label->setPosition(Vec2(0.4f * x,0.6f * y));
	label->setColor(Color3B::BLACK);
	this->addChild(label);
	// colonm 2 Run Distance
	label = Label::createWithTTF(ttfConfig2, "    Run\nDistance");
	label->setPosition(Vec2(0.5f * x, 0.6f * y));
	label->setColor(Color3B::BLACK);
	label->setScale(.8f);
	this->addChild(label);
	// colonm 3 bomb num
	label = Label::createWithTTF(ttfConfig2, "Place\nBomb");
	label->setPosition(Vec2(0.6f * x, 0.6f * y));
	label->setColor(Color3B::BLACK);
	this->addChild(label);
	// colonm 4 skill num
	label = Label::createWithTTF(ttfConfig2, "Use\nSkill");
	label->setPosition(Vec2(0.7f * x, 0.6f * y));
	label->setColor(Color3B::BLACK);
	this->addChild(label);
	// colonm 5 item
	label = Label::createWithTTF(ttfConfig2, "Buff\nGet");
	label->setPosition(Vec2(0.8f * x, 0.6f * y));
	label->setColor(Color3B::BLACK);
	this->addChild(label);

	//******************************* colomn *************************************
	const std::string strRole1 = "Engineer";
	const std::string strRole2 = "Mad Bomber";
	const std::string strRole3 = "Architect";
	for (i = 0; i < 6; i++)
	{
		if (param[30 + i] == 1)
			label = Label::createWithTTF(ttfConfig3, strRole1);
		else if (param[30 + i] == 2)
			label = Label::createWithTTF(ttfConfig3, strRole2);
		else if (param[30 + i] == 3)
			label = Label::createWithTTF(ttfConfig3, strRole3);

		label->setPosition(Vec2(0.18f * x, (0.56f - i * 0.08f) * y));
		if (i < 3)
			label->setColor(Color3B::BLUE);
		else
			label->setColor(Color3B::RED);
		this->addChild(label);

		const std::string strName = paramc[i];
		label = Label::createWithTTF(ttfConfig2, strName);
		label->setPosition(Vec2(0.25f * x, (0.52f - i * 0.08f) * y));
		if (i < 3)
			label->setColor(Color3B::BLUE);
		else
			label->setColor(Color3B::RED);
		this->addChild(label);
	}

	//******************************* chart ***************************************
	char* str = new char[5];
	for (i = 0; i < 6; i++)
	{
		if (param[i * 5] == 1)
			label = Label::createWithTTF(ttfConfig2, "Yes");
		else 
			label = Label::createWithTTF(ttfConfig2, "No");
		label->setPosition(Vec2(0.4f * x, (0.52f - i * 0.08f) * y));
		label->setColor(Color3B::BLACK);
		this->addChild(label);
	}
	for (j = 1; j < 5; j++)
	{
		for (i = 0; i < 6; i++)
		{
			const std::string strNum = itoa2(param[i * 5 + j], str, 10);
			label = Label::createWithTTF(ttfConfig2, strNum);
			label->setPosition(Vec2((0.4f + j * 0.1f) * x, (0.52f - i * 0.08f) * y));
			label->setColor(Color3B::BLACK);
			this->addChild(label);
		}
	}

	TTFConfig ttfConfig("fonts/Marker Felt.ttf", 50);
	label = Label::createWithTTF(ttfConfig, "Back To Menu");
	label->setColor(Color3B::BLACK);
	auto item = MenuItemLabel::create(label, [=](Ref *ref) {
		auto scene = MainInterface::scene();
		Director::getInstance()->replaceScene(TransitionCrossFade::create(0.2f, scene));
	});
	auto menu = Menu::create(item, nullptr);
	menu->setPosition(Vec2(size.width - label->getContentSize().width, label->getContentSize().height));
	addChild(menu);

	return true;
}

//******************************************** main game ********************************************
void BomberGame::quitMenu()
{
	float x = VisibleRect::center().x, y = VisibleRect::center().y;

	if (!_menu2init)
	{
		TTFConfig ttfConfig("fonts/Marker Felt.ttf", 50);
		auto labelAsk = Label::createWithTTF(ttfConfig, "Do you really want to quit ?");
		auto labelY = Label::createWithTTF(ttfConfig, "Yes");
		auto labelN = Label::createWithTTF(ttfConfig, "No");
		menu2 = Menu::create();
		auto menuItemAsk = MenuItemLabel::create(labelAsk);
		auto menuItemY = MenuItemLabel::create(labelY, [=](Ref *ref) {
			menu2->runAction(EaseExponentialOut::create(MoveTo::create(2, Vec2(3 * x, y))));
			menuQuit->runAction(EaseExponentialOut::create(MoveBy::create(2, -Vec2(quitItem->getContentSize().width, 0))));
			//menu
			auto scene = MainInterface::scene();
			Director::getInstance()->replaceScene(TransitionFade::create(0.2f, scene));
		});
		auto menuItemN = MenuItemLabel::create(labelN, [=](Ref *ref) {
			menu2->runAction(EaseExponentialOut::create(MoveTo::create(2, Vec2(3 * x, y))));
			menuQuit->runAction(EaseExponentialOut::create(MoveBy::create(2, -Vec2(quitItem->getContentSize().width, 0))));
		});
		menu2->addChild(menuItemAsk);
		menu2->addChild(menuItemY);
		menu2->addChild(menuItemN);
		menu2->setPosition(Vec2(3 * x, y));
		menuItemAsk->setPosition(Vec2(0, 50));
		menuItemY->setPosition(Vec2(-100, -50));
		menuItemN->setPosition(Vec2(100, -50));
		this->addChild(menu2);
		_menu2init = 1;
	}
	menu2->runAction(EaseExponentialOut::create(MoveTo::create(2, Vec2(x, y))));
	menuQuit->runAction(EaseExponentialOut::create(MoveBy::create(2, Vec2(quitItem->getContentSize().width, 0))));
}

void BomberGame::countDown()
{
	TTFConfig ttfConfig("fonts/Marker Felt.ttf", 100);
	auto label = Label::create();
	label->setTTFConfig(ttfConfig);

	Size size = Director::getInstance()->getWinSize();

	this->runAction(Sequence::create(CallFunc::create([=]() {
		auto easeMove = EaseExponentialOut::create(MoveBy::create(0.4f, Vec2(size.width, 0)));
		auto delayMove = DelayTime::create(0.2f);
		auto sequenceMove = Sequence::create(easeMove, delayMove, easeMove, nullptr);
		label->setString("3");
		label->setPosition(Vec2(-0.5f * size.width, 0.5f * size.height));
		label->runAction(sequenceMove);
	}), DelayTime::create(1.f), CallFunc::create([=]() {
		auto easeMove = EaseExponentialOut::create(MoveBy::create(0.4f, Vec2(size.width, 0)));
		auto delayMove = DelayTime::create(0.2f);
		auto sequenceMove = Sequence::create(easeMove, delayMove, easeMove, nullptr);
		label->setString("2");
		label->setPosition(Vec2(-0.5f * size.width, 0.5f * size.height));
		label->runAction(sequenceMove);
	}), DelayTime::create(1.f), CallFunc::create([=]() {
		auto easeMove = EaseExponentialOut::create(MoveBy::create(0.4f, Vec2(size.width, 0)));
		auto delayMove = DelayTime::create(0.2f);
		auto sequenceMove = Sequence::create(easeMove, delayMove, easeMove, nullptr);
		label->setString("1");
		label->setPosition(Vec2(-0.5f * size.width, 0.5f * size.height));
		label->runAction(sequenceMove);
	}), DelayTime::create(1.f), CallFunc::create([=]() {
		auto easeMove = EaseExponentialOut::create(MoveBy::create(0.4f, Vec2(size.width, 0)));
		auto delayMove = DelayTime::create(0.2f);
		auto sequenceMove = Sequence::create(easeMove, delayMove, easeMove, nullptr);
		label->setString("Fight!");
		label->setPosition(Vec2(-0.5f * size.width, 0.5f * size.height));
		label->runAction(sequenceMove);
	}), DelayTime::create(0.5f), CallFunc::create([=]() {
		gameStart = 1;
	}), nullptr));

	cameraPos = _camera->getPosition3D();
	cameraLookPos = Vec3(-7.f, -2.2f, 0);
	Vec3 deltacameraPos = (playerInControl->player->getPosition3D() + Vec3(-15.f, 25.f,0) - _camera->getPosition3D()) / 42.f;
	Vec3 deltacameraLookPos = (playerInControl->player->getPosition3D() - Vec3(-7.f, -2.2f, 0)) / 42.f;
	this->runAction(Sequence::create(DelayTime::create(2.f), Repeat::create(Sequence::create(DelayTime::create(0.035f), CallFunc::create([=]() {
		cameraPos = cameraPos + deltacameraPos;
		cameraLookPos = cameraLookPos + deltacameraLookPos;
		_camera->setPosition3D(cameraPos);
		_camera->lookAt(cameraLookPos, Vec3(0.0f, 1.0f, 0.0f));
	}), nullptr), 42), CallFunc::create([=]() {
		cameraPosition = 1;
	}), nullptr));
	
	this->addChild(label);
}

void BomberGame::gameOver(char ** name)
{
	Size size = Director::getInstance()->getWinSize();
	TTFConfig ttfConfig("fonts/Marker Felt.ttf", 120);
	auto label = Label::create();
	label->setTTFConfig(ttfConfig);

	label->runAction(RepeatForever::create(Sequence::create(DelayTime::create(5.f), CallFunc::create([=]() {
		if (player[0]->health == 0 && player[1]->health == 0 && player[2]->health == 0
			&& player[3]->health == 0 && player[4]->health == 0 && player[5]->health == 0)
		{
			//draw
			auto easeMove = EaseExponentialOut::create(MoveBy::create(0.8f, Vec2(size.width, 0)));
			auto delayMove = DelayTime::create(2.f);
			auto sequenceMove = Sequence::create(easeMove, delayMove, easeMove, nullptr);
			label->setString("Draw");
			label->setPosition(Vec2(-0.5f * size.width, 0.5f * size.height));
			label->runAction(sequenceMove);

			this->runAction(Sequence::create(DelayTime::create(5.f), CallFunc::create([=]() {
				int *param = new int[30];
				for (int i = 0; i < 6; i++)
				{
					//************ 5 digits ************
					// 1 for alive
					if (player[i]->health == 0)
						param[i * 5] = 0;
					else
						param[i * 5] = 1;
					// 2 for run distance
					param[i * 5 + 1] = (int)player[i]->runLength;
					// 3 for bomb num
					param[i * 5 + 2] = player[i]->placeBombNum;
					// 4 for skill num
					param[i * 5 + 3] = player[i]->useSkillNum;
					// 5 for item num
					param[i * 5 + 4] = player[i]->getItemNum;
					// 30 -- 35 occupation
					param[30 + i] = player[i]->occupation;
				}
				auto scene = WinScene::scene(param, name);
				Director::getInstance()->replaceScene(TransitionFade::create(0.2f, scene));
			}),nullptr));

			label->stopAllActions();
		}
		else if (player[0]->health == 0 && player[1]->health == 0 && player[2]->health == 0)
		{
			//lose
			auto easeMove = EaseExponentialOut::create(MoveBy::create(0.8f, Vec2(size.width, 0)));
			auto delayMove = DelayTime::create(2.f);
			auto sequenceMove = Sequence::create(easeMove, delayMove, easeMove, nullptr);
			label->setString("Lose");
			label->setPosition(Vec2(-0.5f * size.width, 0.5f * size.height));
			label->runAction(sequenceMove);

			this->runAction(Sequence::create(DelayTime::create(5.f), CallFunc::create([=]() {
				int *param = new int[30];
				for (int i = 0; i < 6; i++)
				{
					//************ 5 digits ************
					// 1 for alive
					if (player[i]->health == 0)
						param[i * 5] = 0;
					else
						param[i * 5] = 1;
					// 2 for run distance
					param[i * 5 + 1] = (int)player[i]->runLength;
					// 3 for bomb num
					param[i * 5 + 2] = player[i]->placeBombNum;
					// 4 for skill num
					param[i * 5 + 3] = player[i]->useSkillNum;
					// 5 for item num
					param[i * 5 + 4] = player[i]->getItemNum;
					// 30 -- 35 occupation
					param[30 + i] = player[i]->occupation;
				}
				auto scene = WinScene::scene(param, name);
				Director::getInstance()->replaceScene(TransitionFade::create(0.2f, scene));
			}), nullptr));

			label->stopAllActions();
		}
		else if (player[3]->health == 0 && player[4]->health == 0 && player[5]->health == 0)
		{
			//win
			auto easeMove = EaseExponentialOut::create(MoveBy::create(0.8f, Vec2(size.width, 0)));
			auto delayMove = DelayTime::create(2.f);
			auto sequenceMove = Sequence::create(easeMove, delayMove, easeMove, nullptr);
			label->setString("Win");
			label->setPosition(Vec2(-0.5f * size.width, 0.5f * size.height));
			label->runAction(sequenceMove);

			this->runAction(Sequence::create(DelayTime::create(5.f), CallFunc::create([=]() {
				int *param = new int[30];
				for (int i = 0; i < 6; i++)
				{
					//************ 5 digits ************
					// 1 for alive
					if (player[i]->health == 0)
						param[i * 5] = 0;
					else
						param[i * 5] = 1;
					// 2 for run distance
					param[i * 5 + 1] = (int)player[i]->runLength;
					// 3 for bomb num
					param[i * 5 + 2] = player[i]->placeBombNum;
					// 4 for skill num
					param[i * 5 + 3] = player[i]->useSkillNum;
					// 5 for item num
					param[i * 5 + 4] = player[i]->getItemNum;
					// 30 -- 35 occupation
					param[30 + i] = player[i]->occupation;
				}
				auto scene = WinScene::scene(param, name);
				Director::getInstance()->replaceScene(TransitionFade::create(0.2f, scene));
			}), nullptr));

			label->stopAllActions();
		}
	}), nullptr)));

	TTFConfig ttfConfig2("fonts/Marker Felt.ttf", 20);
	labelClock = Label::create();
	labelClock->setTTFConfig(ttfConfig2);
	labelClock->setPosition(Vec2(0.5f * size.width, 0.95f * size.height));

	second = 183;
	//second = 13;
	labelClock->runAction(RepeatForever::create(Sequence::create(DelayTime::create(1.f), CallFunc::create([=]() {
		if (second > 0)
			second--;
		int minute = second / 60;
		int second2 = second % 60;

		char* str = new char[3];
		
		std::string strSecond;
		if(second2<10)
			strSecond = "0" + std::string(itoa2(second2, str, 10));
		else
			strSecond = std::string(itoa2(second2, str, 10));

		const std::string strTime = std::string(itoa2(minute, str, 10)) + " : " + strSecond;
		labelClock->setString(strTime);

		if (second == 0)
		{
			//draw
			auto easeMove = EaseExponentialOut::create(MoveBy::create(0.8f, Vec2(size.width, 0)));
			auto delayMove = DelayTime::create(2.f);
			auto sequenceMove = Sequence::create(easeMove, delayMove, easeMove, nullptr);
			label->setString("Draw");
			label->setPosition(Vec2(-0.5f * size.width, 0.5f * size.height));
			label->runAction(sequenceMove);

			this->runAction(Sequence::create(DelayTime::create(5.f), CallFunc::create([=]() {
				int *param = new int[36];
				for (int i = 0; i < 6; i++)
				{
					//************ 5 digits ************
					// 1 for alive
					if (player[i]->health == 0)
						param[i * 5] = 0;
					else
						param[i * 5] = 1;
					// 2 for run distance
					param[i * 5 + 1] = (int)(player[i]->runLength / 3.f);
					// 3 for bomb num
					param[i * 5 + 2] = player[i]->placeBombNum;
					// 4 for skill num
					param[i * 5 + 3] = player[i]->useSkillNum;
					// 5 for item num
					param[i * 5 + 4] = player[i]->getItemNum;
					// 30 -- 35 occupation
					param[30 + i] = player[i]->occupation;
				}
				auto scene = WinScene::scene(param, name);
				Director::getInstance()->replaceScene(TransitionFade::create(0.2f, scene));
			}), nullptr));

			labelClock->stopAllActions();
		}
	}), nullptr)));

	this->addChild(label);
	this->addChild(labelClock);
}
