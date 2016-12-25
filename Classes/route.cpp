#include "HelloWorldScene.h"

#define MAP game->map
#define THREAT_MAP game->threatMap
extern BomberGame * game;

void RouteSchedule::routeMapInit()
{
	int i, j;
	for (i = 0; i < 21; i++)
	{
		for (j = 0; j < 21; j++)
		{
			routeMap[i][j] = 100;
		}
	}
}

void RouteSchedule::routeInit()
{
	int i;
	for (i = 0; i < 25; i++)
	{
		route[i].x = 0;
		route[i].y = 0;
	}
	for (i = 0; i < 30; i++)
	{
		solutionPoint[i] = 0;
	}
}

int RouteSchedule::nearBox(int x, int y)
{
	int count = 0;
	if (MAP[x + 1][y] == 2 && MAP[x + 2][y] != 3 && MAP[x + 1][y + 1] != 3 && MAP[x + 1][y - 1] != 3 && MAP[x][y] != 3)
		count++;
	if (MAP[x - 1][y] == 2 && MAP[x - 2][y] != 3 && MAP[x - 1][y + 1] != 3 && MAP[x - 1][y - 1] != 3 && MAP[x][y] != 3)
		count++;
	if (MAP[x][y + 1] == 2 && MAP[x][y + 2] != 3 && MAP[x + 1][y + 1] != 3 && MAP[x - 1][y + 1] != 3 && MAP[x][y] != 3)
		count++;
	if (MAP[x][y - 1] == 2 && MAP[x][y - 2] != 3 && MAP[x + 1][y - 1] != 3 && MAP[x - 1][y - 1] != 3 && MAP[x][y] != 3)
		count++;
	return count;
}

void RouteSchedule::routeSearch(int x, int y, int func, int direc)
{
	if (searchDeep < routeMap[x][y])
		return;
	if ((func == 1 && THREAT_MAP[x][y] == 0)
		|| (routeMap[x][y] == 12 && rand() % 3 == 0)
		|| (func != 1 && MAP[x][y] < 0)
		|| (func == 2 && nearBox(x, y))
		|| (func == 3 && nearEnemy(x, y))
		|| (func == 4 && (nearBox(x, y) || nearEnemy(x, y) || nearEnemyBuilding(x, y))))
	{
		bFound = 1;

		bool foundSolution = 1;
		int i;
		if (func == 1 && solutionNum > 0 && solutionStep[solutionNum - 1] < routeMap[x][y])
			foundSolution = 0;
		for (i = 0; i < solutionNum; i++)
		{
			if (solutionPosition[i].x == x && solutionPosition[i].y == y)
			{
				foundSolution = 0;
				break;
			}
		}

		if (foundSolution)
		{
			solutionStep[solutionNum] = routeMap[x][y];
			solutionPosition[solutionNum].x = x;
			solutionPosition[solutionNum].y = y;
			solutionNum++;
		}
	}
	if (direc != 1)fourDirectionSpread(x + 1, y, routeMap[x][y], func, 0);
	if (direc != 0)fourDirectionSpread(x - 1, y, routeMap[x][y], func, 1);
	if (direc != 3)fourDirectionSpread(x, y + 1, routeMap[x][y], func, 2);
	if (direc != 2)fourDirectionSpread(x, y - 1, routeMap[x][y], func, 3);
	return;
}

void RouteSchedule::fourDirectionSpread(int x, int y, short compare, int func, int direc)
{
	if (x < 0 && y < 0 && x > 20 && y > 20)
		return;
	if (func == 1 && MAP[x][y] > 0)
		return;
	if (func == 2 && (MAP[x][y] > 0 || THREAT_MAP[x][y] != 0))
		return;
	if (func == 3 && (MAP[x][y] > 0 || THREAT_MAP[x][y] != 0))
		return;
	if (func == 4 && (MAP[x][y] > 0 || THREAT_MAP[x][y] != 0))
		return;

	if (routeMap[x][y] == 100)
	{
		routeMap[x][y] = compare + 1;
		routeSearch(x, y, func, direc);
	}
	else if (routeMap[x][y] == compare + 1)
	{
		routeSearch(x, y, func, direc);
	}
	else if (routeMap[x][y] > compare + 1)
	{
		routeMap[x][y] = compare + 1;
		routeSearch(x,y, func, direc);
	}
	else if (routeMap[x][y] == compare || routeMap[x][y] == compare - 1)
	{
	}
	else if (routeMap[x][y] < compare - 1)
	{
		routeSearch(x, y, func, direc);
	}
}

coordinate* RouteSchedule::calculateRoute(int x, int y, int func)
{
	routeMapInit();
	routeInit();

	routeMap[x][y] = 0;

	bFound = 0;
	searchDeep = 1;
	solutionNum = 0;
	choice = 0;
	int i, j;
	int maxPoint;
	int anotherChoice[10];
	int anotherChoiceNum = 0;
	for (i = 0; i < 12; i++)
	{
		routeSearch(x, y, func, 4);
		if (i>7 && bFound)
		{
			maxPoint = -10000;
			for (j = 0; j < solutionNum; j++)
			{
				if (func!=1 )
				{
					if(MAP[solutionPosition[j].x][solutionPosition[j].y] < 0)
						solutionPoint[j] += 1000;
					solutionPoint[j] += 300 * nearBox(solutionPosition[j].x, solutionPosition[j].y);
					solutionPoint[j] -= solutionStep[j] * 20;
					if (nearFriend(solutionPosition[j].x, solutionPosition[j].y))
						solutionPoint[j] -= 1000;
				}
				else
				{
					solutionPoint[j] += judge(solutionPosition[j].x, solutionPosition[j].y);
				}

				if (solutionPoint[j] > maxPoint)
				{
					maxPoint = solutionPoint[j];
					choice = j;
					anotherChoiceNum = 0;
				}
				if (solutionPoint[j] == maxPoint && anotherChoiceNum<10)
				{
					anotherChoice[anotherChoiceNum] = j;
					anotherChoiceNum++;
				}
			}

			if (rand() % (anotherChoiceNum + 1) != 0)
				choice = anotherChoice[rand() % anotherChoiceNum];

			route[solutionStep[choice]].x = solutionPosition[choice].x;
			route[solutionStep[choice]].y = solutionPosition[choice].y;

			int m, n;
			for (j = 1; j <= solutionStep[choice]; j++)
			{
				m = route[solutionStep[choice] - j + 1].x;
				n = route[solutionStep[choice] - j + 1].y;
				if (routeMap[m + 1][n] == routeMap[m][n] - 1)
				{
					route[solutionStep[choice] - j].x = m + 1;
					route[solutionStep[choice] - j].y = n;
				}
				if (routeMap[m - 1][n] == routeMap[m][n] - 1)
				{
					route[solutionStep[choice] - j].x = m - 1;
					route[solutionStep[choice] - j].y = n;
				}
				if (routeMap[m][n + 1] == routeMap[m][n] - 1)
				{
					route[solutionStep[choice] - j].x = m;
					route[solutionStep[choice] - j].y = n + 1;
				}
				if (routeMap[m][n - 1] == routeMap[m][n] - 1)
				{
					route[solutionStep[choice] - j].x = m;
					route[solutionStep[choice] - j].y = n - 1;
				}
			}
			return route;
		}
		else
		{
			searchDeep += 1;	
		}
	}
	return nullptr;
}

int RouteSchedule::runRoute(int x, int y)
{
	auto solutionStep = this->solutionStep[choice];
	auto route = this->route;
	for (int i = 0; i <= solutionStep; i++)
	{
		if (route[i].x == x && route[i].y == y)
		{
			if (i == solutionStep && MAP[x][y] < 0)
				return 6;
			if (i == solutionStep)
				return 4;
			if (route[i + 1].x > route[i].x)
				return 0;
			if (route[i + 1].x < route[i].x)
				return 1;
			if (route[i + 1].y > route[i].y)
				return 2;
			if (route[i + 1].y < route[i].y)
				return 3;
			break;
		}	
	}
	return 5;
}

void RouteSchedule::getPlayerPosition(Player * _player, int * _friendTeamPosX, int * _friendTeamPosY, int * _emenyTeamPosX, int * _emenyTeamPosY)
{
	int i;
	i = 0;
	friendNum = 0;
	while(_friendTeamPosX[i]!= 0)
	{
		friendTeamPosX[i] = _friendTeamPosX[i];
		friendTeamPosY[i] = _friendTeamPosY[i];
		friendNum++;
		i++;
	}
	i = 0;
	enemyNum = 0;
	while (_emenyTeamPosX[i] != 0)
	{
		emenyTeamPosX[i] = _emenyTeamPosX[i];
		emenyTeamPosY[i] = _emenyTeamPosY[i];
		enemyNum++;
		i++;
	}
	player = _player;
}

bool RouteSchedule::nearEnemy(int x, int y)
{
	bool value = 0;
	int i,manx,many;
	for (i = 0; i < enemyNum; i++)
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

bool RouteSchedule::nearFriend(int x, int y)
{
	bool value = 0;
	int i, manx, many, m, n;
	for (i = 0; i < friendNum; i++)
	{
		manx = friendTeamPosX[i];
		many = friendTeamPosY[i];
		for (m = x - 3; m <= x + 3 && value == 0; m++)
		{
			for (n = y - 3; n <= y + 3 && value == 0; n++)
			{
				if(manx == m && many == n)
					value = 1;
			}
		}
	}
	return value;
}

bool RouteSchedule::nearEnemyBuilding(int x, int y)
{
	bool value = 0;
	int n, m;
	for (n = 0; n < game->playerNum; n++)
	{
		if (game->player[n]->team != player->team && game->player[n]->occupation == 3)
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

int RouteSchedule::judge(int x, int y)
{
	int point = 0, i, j,distance;
	for (i = x - 3; i <= x + 3; i++)
	{
		for (j = y - 3; j <= y + 3; j++)
		{
			if (i >= 0 && j >= 0 && i <= 20 && j <= 20)
			{
				if (x > i)
					distance = x - i;
				else
					distance = i - x;
				if (y > j)
					distance += y - j;
				else
					distance += j - y;
				if (distance <= 3)
				{
					if (MAP[i][j] == 0 && THREAT_MAP[i][j]==0)
					{
						point += 4 - distance;
					}
				}	
			}
		}
	}
	return point;
}