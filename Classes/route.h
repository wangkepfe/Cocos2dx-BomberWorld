#ifndef __ROUTE_H__
#define __ROUTE_H__

#include "HelloWorldScene.h"

struct coordinate
{
	int x;
	int y;
};

class RouteSchedule
{
public:
	RouteSchedule(){}
	~RouteSchedule(){}

	void routeMapInit();
	void routeInit();
	void routeSearch(int x,int y, int func, int direc);
	coordinate* calculateRoute(int x,int y, int func);
	int nearBox(int x,int y);
	bool nearEnemy(int x,int y);
	bool nearFriend(int x, int y);
	bool nearEnemyBuilding(int x, int y);
	void fourDirectionSpread(int x, int y, short compare, int func, int direc);
	int runRoute(int x, int y);

	void getPlayerPosition(Player * _player,int * _friendTeamPosX, int * _friendTeamPosY, int * _emenyTeamPosX, int * _emenyTeamPosY);
	int judge(int x,int y);

	short routeMap[21][21];

	int friendTeamPosX[4];
	int friendTeamPosY[4];
	int emenyTeamPosX[4];
	int emenyTeamPosY[4];
	int friendNum;
	int enemyNum;
	
	coordinate route[50];
	coordinate solutionPosition[30];
	int solutionNum;
	int solutionStep[30];
	int solutionPoint[30];

	int choice;
	bool bFound;
	int searchDeep;

	Player * player;
};

#endif 