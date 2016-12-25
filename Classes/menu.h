#ifndef __MENU_H__
#define __MENU_H__

#include "cocos2d.h"

class MainInterface : public cocos2d::Scene
{
public:
	CREATE_FUNC(MainInterface);

	static cocos2d::Scene* scene();

	MainInterface() {};
	virtual ~MainInterface() {};

	virtual bool init() override;
};

class LoadInterface : public cocos2d::Scene
{
public:
	CREATE_FUNC(LoadInterface);

	static cocos2d::Scene* scene();

	LoadInterface();
	
	void loadingCallBack(cocos2d::Texture2D *texture);
	void loadingCallBack2();

private:
	cocos2d::Label *_labelLoading;
	cocos2d::Label *_labelPercent;
	int _numberOfSprites;
	int _numberOfLoadedSprites;
};

class WaitingScene : public cocos2d::Scene
{
public:
	static WaitingScene * create(int param);

	static cocos2d::Scene* scene(int param);

	WaitingScene() {};
	virtual ~WaitingScene() {};

	virtual bool init(int param);
};

class CharacterChoose : public cocos2d::Scene
{
public:
	CREATE_FUNC(CharacterChoose);

	static cocos2d::Scene* scene();

	CharacterChoose() {};
	virtual ~CharacterChoose() {};

	virtual bool init() override;

private:
	int choice;
	cocos2d::Camera *_camera;
};

class WinScene : public cocos2d::Scene
{
public:
	static WinScene * create(int* param,char **paramc);

	static cocos2d::Scene* scene(int* param, char **paramc);

	WinScene() {};
	virtual ~WinScene() {};

	virtual bool init(int* param, char **paramc);
};

class ControlMenu : public cocos2d::Scene
{
public:
	CREATE_FUNC(ControlMenu);

	static cocos2d::Scene* scene();

	ControlMenu() {};
	virtual ~ControlMenu() {};

	virtual bool init() override;
};

#endif
