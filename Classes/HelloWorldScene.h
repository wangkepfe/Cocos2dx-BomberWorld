#ifndef __HELLOWORLD_SCENE_H__
#define __HELLOWORLD_SCENE_H__

//#include "cocos2d.h"
#include "cocos-ext.h"

#include "player.h"

#define CD_REFRESH(__VARIABLE__) \
__VARIABLE__-=0.035f; \
if(__VARIABLE__<0)\
__VARIABLE__ = 0; 

#define CD_BUTTON_REFRESH(__VARIABLE_1__,__VARIABLE_2__) \
if (__VARIABLE_1__ == 0) __VARIABLE_2__->setColor(Color3B::WHITE);\
else __VARIABLE_2__->setColor(Color3B::GRAY);

class BomberGame : public cocos2d::Scene
{
public:
	virtual void onTouchesBegan(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	virtual void onTouchesMoved(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	virtual void onTouchesEnded(const std::vector<cocos2d::Touch*>& touches, cocos2d::Event  *event);
	
	static cocos2d::Scene* scene(int* param, char **paramc);

	static BomberGame * create(int* param, char **paramc);

	void quitMenu();

	void createMap();
	void terrain();
	void bombInit();
	void hurt(Player * player, int i, int j, int initI, int initJ);
	void smallBlast(int i, int j);
	void boom(int i, int j);
	void fireFlame(int direction, int power, int i, int j, bool isRemote);
	void fireOrBoom(int direction, int power, int i, int j, bool isRemote);
	void addThreatArea(int i, int j, int power);
	void minusThreatArea(int i, int j, int power);
	
	void createItems(int code, int x, int y);
	void attributeDisplay();
	void attributeDisplayUpdate();
	void gameOver(char ** name);

	void countDown();

//	CREATE_FUNC(BomberGame);
	BomberGame() {}
	virtual ~BomberGame() {}
	virtual bool init(int *param, char **paramc);
//	void update1(float dt);
	void board();

public:
	bool _menu2init;
	cocos2d::Menu *menu2,*menuQuit;
	cocos2d::MenuItemImage *quitItem;

	bool bfoundSolution;
	int solutionStep;
	short ** map;
	short ** threatMap;
	cocos2d::Vec2 direction;
	bool bMove;
	cocos2d::Sprite *_circle, *health01, *health02, *health03, *health04, *health05;
	cocos2d::Label *labelStrength, *labelAgility, *labelIntelligence;
	cocos2d::Label *labelNormal, *labelBounceBomb, *labelTrap, *labelRemoteControl;
	cocos2d::Label *labelClock;
	int cameraPosition;
	cocos2d::Camera *_camera;
	cocos2d::Vec3 cameraPos, cameraLookPos;

	Player** player;
	int playerNum;
	Player* playerInControl;

	bool gameStart;
	int second;
};


#endif // __HELLOWORLD_SCENE_H__
