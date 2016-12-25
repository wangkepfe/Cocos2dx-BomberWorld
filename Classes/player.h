#ifndef __PLAYER_H__
#define __PLAYER_H__

#include "cocos2d.h"

class Player;

struct coordinate
{
	int x;
	int y;
};

class RouteSchedule
{
public:
	RouteSchedule() {}
	~RouteSchedule() {}

	void routeMapInit();
	void routeInit();
	void routeSearch(int x, int y, int func, int direc);
	coordinate* calculateRoute(int x, int y, int func);
	int nearBox(int x, int y);
	bool nearEnemy(int x, int y);
	bool nearFriend(int x, int y);
	bool nearEnemyBuilding(int x, int y);
	void fourDirectionSpread(int x, int y, short compare, int func, int direc);
	int runRoute(int x, int y);

	void getPlayerPosition(Player * _player, int * _friendTeamPosX, int * _friendTeamPosY, int * _emenyTeamPosX, int * _emenyTeamPosY);
	int judge(int x, int y);

	short routeMap[21][21];

	int friendTeamPosX[4];
	int friendTeamPosY[4];
	int emenyTeamPosX[4];
	int emenyTeamPosY[4];
	int friendNum;
	int enemyNum;

	coordinate route[25];
	coordinate solutionPosition[30];
	int solutionNum;
	int solutionStep[30];
	int solutionPoint[30];

	int choice;
	bool bFound;
	int searchDeep;

	Player * player;
};

class Player
{
public:
	Player(){}
	~Player(){}

	//model
	cocos2d::Sprite3D *player;
	static Player* create(bool is_com,int _occupation, int _team);

	//fundamental attribute
	int health;
	int strength;
	int agility;
	int intelligence;
	cocos2d::BillBoard * boardDisplay;
	cocos2d::Sprite *healthDisplay[5];

	//data
	char *name;
	cocos2d::Label *labelName;
	float runLength;
	int placeBombNum;
	int useSkillNum;
	int getItemNum;

	// position
	int i, j;
	float x, y;
	
	// ai
	bool controlByMan;
	int occupation; // 1 for ME
	int team; // 1 or 2
	int aiState;
	int aiCount;
	int faceTo;
	coordinate *route;
	int inRoute;
	float previousX;
	float previousY;
	RouteSchedule* routeSchedule;

	int **_friendTeamPosX;
	int **_friendTeamPosY;
	int **_emenyTeamPosX;
	int **_emenyTeamPosY;
	int *friendTeamPosX;
	int *friendTeamPosY;
	int *emenyTeamPosX;
	int *emenyTeamPosY;
	int friendNum;
	int emenyNum;

    void ai();
	void teamClassify(int playerNum);

	void calCurrentPlayerPosition();

	// CD
	float CD;
	float skillCD[3];

	// skill parameters for engineer
	bool canControl;

	bool remoteBombOn, automaticBombOn, bounceBombOn;

	int remoteBombPower;
	int remoteBombX, remoteBombY;

	int automaticBombPower;
	int automaticBombX, automaticBombY;

	int bounceBombPower;
	int bounceBombX, bounceBombY;
	int bombFaceTo;

	// skill parameters for mad bomber
	bool onRush;
	int rushCount;
	bool onThrowing;
	int throwTargetPosX[3], throwTargetPosY[3];

	// skill parameters for architect
	int gateNum;
	bool cannonOn[3];
	int cannonNum;
	bool gateOn[3];
	bool motarOn;
	
	int gateX[3], gateY[3];
	int cannonX[3], cannonY[3], cannonPower[3], cannonFaceTo[3];
	float cannonHealth[3],gateHealth[3];
	int motarX, motarY, motarPower,motarHealth;
	//
	void playerInit(int i, int j);

	// skills for engineer
	void createBomb(int i, int j, int power);

	void createRemoteBomb(int i, int j, int power);
	void trigerRemoteBomb();

	void createAutomaticBomb(int i, int j, int power);
	void automaticBombBoom();
	
	void createBounceBomb(int i, int j, int power);
	void bounceBombBoom();

	//skills for mad bomber
	void throw3Bombs(int i,int j,int power);
	void bombWall(int i, int j, int power);

	//skills for architect
	void createCannon(int i, int j, int power);
	void createGate(int i, int j);
	void createMotar(int i, int j, int power);

	void cannonDestroy(int index);
	void motarDestroy();
	void gateDestroy(int index);
	//
	void manControl();
	void getItem(int code, int x, int y);
	bool nearEnemy();
	bool nearEnemyBuilding();
	void displayUpdate(int func);
};



#endif 