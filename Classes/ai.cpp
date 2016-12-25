#include "HelloWorldScene.h"

#define MAP game->map

USING_NS_CC;
extern BomberGame * game;
static const float unit = 3.f;

void Player::ai()
{
	if (health == 0)
		return;
	CD_REFRESH(CD);
	if (!remoteBombOn)
	{
		CD_REFRESH(skillCD[0]);
	}
	CD_REFRESH(skillCD[1]);
	CD_REFRESH(skillCD[2]);
	x = player->getPositionX();
	y = player->getPositionZ();

	if (aiState == 10) //judge state
	{
		i = -x / unit + 10.5f;
		j = y / unit + 10.5f;

		int move = 5; // 0,1,2,3:four direction; 4: action; 5: no move
		bool placeBomb = 0;
		bool canMove = 1;

		bool nearEnemy = 0;
		bool nearEnemyStraight = 0;
		bool nearEnemyFaceToFace = 0;

		calCurrentPlayerPosition();

		int m, n, p;
		// near enemy straight
		for (m = 1; !nearEnemyStraight && m <= 5; m++)
		{
			for (p = 0; p < emenyNum; p++)
			{
				if (emenyTeamPosX[p] == i + m && emenyTeamPosY[p] == j)
				{
					faceTo = 0;
					nearEnemyStraight = 1;
					break;
				}
				else if (emenyTeamPosX[p] == i - m && emenyTeamPosY[p] == j)
				{
					faceTo = 1;
					nearEnemyStraight = 1;
					break;
				}
				else if (emenyTeamPosX[p] == i && emenyTeamPosY[p] == j + m)
				{
					faceTo = 2;
					nearEnemyStraight = 1;
					break;
				}
				else if (emenyTeamPosX[p] == i && emenyTeamPosY[p] == j - m)
				{
					faceTo = 3;
					nearEnemyStraight = 1;
					break;
				}
			}
		}
		// near enemy
		for (m = i - 4; !nearEnemy && m <= i + 4; m++)
		{
			for (n = j - 4; !nearEnemy &&  n <= j + 4; n++)
			{
				for (p = 0; p < emenyNum; p++)
				{
					if (emenyTeamPosX[p] == m && emenyTeamPosY[p] == n)
					{
						nearEnemy = 1;
						break;
					}
				}
			}
		}
		// near enemy face to face
		for (p = 0; !nearEnemyFaceToFace && p < emenyNum; p++)
		{
			for (m = 1; MAP[i + m][j] <= 0; m++)
			{
				if (emenyTeamPosX[p] == i + m && emenyTeamPosY[p] == j)
				{
					faceTo = 0;
					nearEnemyFaceToFace = 1;
					break;
				}
			}
			for (m = 1; MAP[i - m][j] <= 0; m++)
			{
				if (emenyTeamPosX[p] == i - m && emenyTeamPosY[p] == j)
				{
					faceTo = 1;
					nearEnemyFaceToFace = 1;
					break;
				}
			}
			for (m = 1; MAP[i][j + m] <= 0; m++)
			{
				if (emenyTeamPosX[p] == i && emenyTeamPosY[p] == j + m)
				{
					faceTo = 2;
					nearEnemyFaceToFace = 1;
					break;
				}
			}
			for (m = 1; MAP[i][j - m] <= 0; m++)
			{
				if (emenyTeamPosX[p] == i && emenyTeamPosY[p] == j - m)
				{
					faceTo = 3;
					nearEnemyFaceToFace = 1;
					break;
				}
			}	
		}
		
		if (occupation == 1)
		{
			if (remoteBombOn)
			{
				for (p = 0; p < emenyNum; p++)
				{
					if ((remoteBombX == emenyTeamPosX[p] || remoteBombY == emenyTeamPosY[p]) &&
						(remoteBombX != i && remoteBombY != j))
					{
						trigerRemoteBomb();
						useSkillNum++;
						skillCD[0] = (200.f - (float)intelligence) / 20.f;
						break;
					}
				}
			}

			if (nearEnemyFaceToFace && !bounceBombOn && skillCD[2] == 0 && rand() % 2 == 0)
			{
				createBounceBomb(i, j, strength);
				skillCD[2] = (200.f - (float)intelligence) / 20.f;
				useSkillNum++;
				inRoute = 0;
			}
			else if (nearEnemy && !automaticBombOn && skillCD[1] == 0 && rand() % 2 == 0)
			{
				createAutomaticBomb(i, j, strength);
				skillCD[1] = (200.f - (float)intelligence) / 20.f;
				useSkillNum++;
				inRoute = 0;
			}
			else if (nearEnemy && !remoteBombOn && skillCD[0] == 0 && rand() % 2 == 0)
			{
				createRemoteBomb(i, j, strength);
				inRoute = 0;
			}

		}
		else if (occupation == 2)
		{
			if (nearEnemyStraight && !onRush && skillCD[2] == 0 && rand() % 2 == 0)
			{
				onRush = 1;
				canControl = 0;
				aiState = 8;
				skillCD[2] = (200.f - (float)intelligence) / 20.f;
				useSkillNum++;
				inRoute = 0;
			}
			else if (nearEnemy && !onThrowing && skillCD[1] == 0 && rand() % 2 == 0)
			{
				skillCD[1] = (200.f - (float)intelligence) / 20.f;
				useSkillNum++;
				placeBombNum += 3;
				throw3Bombs(i, j, strength);
				inRoute = 0;
			}
			else if (nearEnemyStraight && skillCD[0] == 0 && rand() % 2 == 0)
			{
				bombWall(i, j, strength);
				skillCD[0] = (200.f - (float)intelligence) / 20.f;
				placeBombNum += 5;
				useSkillNum++;
				inRoute = 0;
			}
		}
		else if (occupation == 3)
		{
			if (nearEnemyFaceToFace && cannonNum <= 3 && skillCD[2] == 0 && rand() % 2 == 0)
			{
				bool canPlace = 1;
				for (int n = 0; n < game->playerNum; n++)
				{
					int pi = game->player[n]->i;
					int pj = game->player[n]->j;

					if ((faceTo == 0 && pi == i + 1 && pj == j) ||
						(faceTo == 1 && pi == i - 1 && pj == j) ||
						(faceTo == 2 && pi == i && pj == j + 1) ||
						(faceTo == 3 && pi == i && pj == j - 1))
						canPlace = 0;
				}
				if (canPlace)
				{
					skillCD[2] = (200.f - (float)intelligence) / 20.f;
					useSkillNum++;
					if (faceTo == 0)createCannon(i + 1, j, strength);
					else if (faceTo == 1)createCannon(i - 1, j, strength);
					else if (faceTo == 2)createCannon(i, j + 1, strength);
					else if (faceTo == 3)createCannon(i, j - 1, strength);
					inRoute = 0;
				}
			}
			else if (gateNum <= 3 && skillCD[1] == 0 && rand() % 3)
			{
				bool canPlace = 1;
				bool rightPlace = 0;
				for (int n = 0; n < game->playerNum; n++)
				{
					int pi = game->player[n]->i;
					int pj = game->player[n]->j;

					if ((faceTo == 0 && pi == i + 1 && pj == j) ||
						(faceTo == 1 && pi == i - 1 && pj == j) ||
						(faceTo == 2 && pi == i && pj == j + 1) ||
						(faceTo == 3 && pi == i && pj == j - 1))
						canPlace = 0;
				}			
				if (canPlace)
				{
					int wallX, wallY;
					wallX = i;
					wallY = j;
					if (faceTo == 0)wallX += 1;
					else if (faceTo == 1)wallX -= 1;
					else if (faceTo == 2)wallY += 1;
					else if (faceTo == 3)wallY -= 1;

					if (wallX > 1 && wallY > 1 && wallX < 20 && wallY < 20 && MAP[wallX][wallY] <= 0)
					{
						if (MAP[wallX + 1][wallY] > 0 && MAP[wallX - 1][wallY] > 0 && MAP[wallX][wallY + 1] <= 0 && MAP[wallX][wallY - 1] <= 0)
							rightPlace = 1;
						else if (MAP[wallX + 1][wallY] <= 0 && MAP[wallX - 1][wallY] <= 0 && MAP[wallX][wallY + 1] > 0 && MAP[wallX][wallY - 1] > 0)
							rightPlace = 1;
						else if (MAP[wallX + 1][wallY] > 0 && MAP[wallX][wallY + 1] <= 0 && MAP[wallX][wallY - 1] <= 0 && MAP[wallX + 1][wallY + 1] <= 0 && MAP[wallX + 1][wallY - 1] <= 0)
							rightPlace = 1;
						else if (MAP[wallX - 1][wallY] > 0 && MAP[wallX][wallY + 1] <= 0 && MAP[wallX][wallY - 1] <= 0 && MAP[wallX - 1][wallY + 1] <= 0 && MAP[wallX - 1][wallY - 1] <= 0)
							rightPlace = 1;
						else if (MAP[wallX][wallY + 1] > 0 && MAP[wallX + 1][wallY] <= 0 && MAP[wallX - 1][wallY] <= 0 && MAP[wallX + 1][wallY + 1] <= 0 && MAP[wallX - 1][wallY + 1] <= 0)
							rightPlace = 1;
						else if (MAP[wallX][wallY - 1] > 0 && MAP[wallX + 1][wallY] <= 0 && MAP[wallX - 1][wallY] <= 0 && MAP[wallX + 1][wallY - 1] <= 0 && MAP[wallX - 1][wallY - 1] <= 0)
							rightPlace = 1;
					}
						
					if (rightPlace)
					{
						useSkillNum++;
						skillCD[1] = (200.f - (float)intelligence) / 20.f;
						inRoute = 0;
						createGate(wallX, wallY);
					}
				}	
			}
			else if (!motarOn && skillCD[0] == 0 && rand() % 10 == 0)
			{
				bool canPlace = 1;
				for (int n = 0; n < game->playerNum; n++)
				{
					int pi = game->player[n]->i;
					int pj = game->player[n]->j;

					if ((faceTo == 0 && pi == i + 1 && pj == j) ||
						(faceTo == 1 && pi == i - 1 && pj == j) ||
						(faceTo == 2 && pi == i && pj == j + 1) ||
						(faceTo == 3 && pi == i && pj == j - 1))
						canPlace = 0;
				}
				if (canPlace)
				{
					useSkillNum++;
					skillCD[0] = (200.f - (float)intelligence) / 20.f;
					if (faceTo == 0)createMotar(i + 1, j, strength);
					else if (faceTo == 1)createMotar(i - 1, j, strength);
					else if (faceTo == 2)createMotar(i, j + 1, strength);
					else if (faceTo == 3)createMotar(i, j - 1, strength);
					inRoute = 0;
				}
			}
		}
		if (canControl)
		{
			if (game->threatMap[i][j] == 0)
			{
				if (inRoute != 2)
				{
					inRoute = 2;
					routeSchedule->getPlayerPosition(this,friendTeamPosX, friendTeamPosY, emenyTeamPosX, emenyTeamPosY);
					route = routeSchedule->calculateRoute(i, j, 4);
					if (route == nullptr)
					{
						inRoute = 0;
						move = rand() % 4;
					}
				}
				if (inRoute == 2 && canMove)
				{
					move = routeSchedule->runRoute(i, j);
					if (move == 4)
					{
						if(routeSchedule->nearBox(i, j) || this->nearEnemy() || this->nearEnemyBuilding() )
							placeBomb = 1;
					}
				}
			}
			else if (game->threatMap[i][j] != 0) // in danger
			{
				if (inRoute != 1) // must change the road to avoid dangers
				{
					inRoute = 1;
					//calCurrentPlayerPosition();
					//routeSchedule->getPlayerPosition(friendTeamPosX, friendTeamPosY, emenyTeamPosX, emenyTeamPosY);
					route = routeSchedule->calculateRoute(i, j, 1);
					if (route == nullptr)
					{
						move = rand() % 4;
						inRoute = 0;
					}
				}
				if (inRoute == 1 && canMove)
				{
					move = routeSchedule->runRoute(i, j);
				}
			}
			if (placeBomb && CD == 0)
			{
				CD = (100.f - (float)intelligence) / 25.f;
				placeBombNum++;
				createBomb(i, j, strength);
				
			}
			if (placeBomb && CD != 0)
			{
				move = 5;
			}
			if (MAP[i][j] < 0)
			{
				getItem(MAP[i][j], i, j);
				getItemNum++;
				displayUpdate(1);
			}

			if (move == 5 || !canMove) aiState = 4;
			else if (move == 7) aiState = 4;
			else if (move == 4) {
				aiState = 10; inRoute = 0; move = rand() % 4;
				if (move == 0 && MAP[i + 1][j] <= 0) { aiState = 0; }
				else if (move == 1 && MAP[i - 1][j] <= 0) { aiState = 1; }
				else if (move == 2 && MAP[i][j + 1] <= 0) { aiState = 2; }
				else if (move == 3 && MAP[i][j - 1] <= 0) { aiState = 3; }
			}
			else if (move == 6) { aiState = 10; inRoute = 0; }		
			else if (move == 0 && MAP[i + 1][j] <= 0) { aiState = 0; }
			else if (move == 1 && MAP[i - 1][j] <= 0) { aiState = 1; }
			else if (move == 2 && MAP[i][j + 1] <= 0) { aiState = 2; }
			else if (move == 3 && MAP[i][j - 1] <= 0) { aiState = 3; }
			else
				aiState = 10;


			if((move == 0 && MAP[i + 1][j] > 0)||
				(move == 1 && MAP[i - 1][j] > 0) || 
				(move == 2 && MAP[i][j + 1] > 0) || 
				(move == 3 && MAP[i][j - 1] > 0))
				inRoute = 0;
		}
	}
	else if (aiState == 0 || aiState == 1 || aiState == 2 || aiState == 3) // walk state
	{
		if (aiCount == 0)
		{
			previousX = x;
			previousY = y;
		}

		if (aiState == 0)
		{		
			if (x - 0.025 * agility <= previousX - unit)
			{
				player->setPositionX(previousX - unit);
				aiCount = 100;
			}
			else
				player->setPositionX(x - 0.025 * agility);
			faceTo = 0;
		}
		else if (aiState == 1)
		{
			if (x + 0.025 * agility >= previousX + unit)
			{
				player->setPositionX(previousX + unit);
				aiCount = 100;
			}
			else
				player->setPositionX(x + 0.025 * agility);
			faceTo = 1;
		}
		else if (aiState == 2)
		{
			if (y + 0.025 * agility >= previousY + unit)
			{
				player->setPositionZ(previousY + unit);
				aiCount = 100;
			}
			else
				player->setPositionZ(y + 0.025 * agility);
			faceTo = 2;
		}
		else if (aiState == 3)
		{
			if (y - 0.025 * agility <= previousY - unit)
			{
				player->setPositionZ(previousY - unit);
				aiCount = 100;
			}
			else
				player->setPositionZ(y - 0.025 * agility);
			faceTo = 3;
		}
		runLength += (float)fabs(0.025 * agility);
		aiCount++;
		if (aiCount > 99)
		{
			aiCount = 0;
			aiState = 10;
		}
		displayUpdate(0);
	}
	else if (aiState == 4) // wait state
	{
		aiCount++;
		if (aiCount == 10)
		{
			aiCount = 0;
			aiState = 10;
			inRoute = 0;
		}	
	}
	else if (aiState == 8) // rush state
	{
		int m, n1, n2;
		aiCount++;
		if (aiCount == 10)
		{
			aiCount = 0;
			aiState = 10;
			onRush = 0;
			canControl = 1;
			CD = 0;
		}
		else
		{
			if (faceTo == 0)
			{
				m = -x / unit + 11.1;
				n1 = y / unit + 10.1;
				n2 = y / unit + 10.9;
				if (MAP[m][n1] <= 0 && MAP[m][n2] <= 0)
					player->setPositionX(x - 1.f);
				else
					aiCount = 9;
			}
			else if (faceTo == 1)
			{
				m = -x / unit + 9.9;
				n1 = y / unit + 10.1;
				n2 = y / unit + 10.9;
				if (MAP[m][n1] <= 0 && MAP[m][n2] <= 0)
					player->setPositionX(x + 1.f);
				else
					aiCount = 9;
			}
			else if (faceTo == 2)
			{
				n1 = -x / unit + 10.1;
				n2 = -x / unit + 10.9;
				m = y / unit + 11.1;
				if (MAP[n1][m] <= 0 && MAP[n2][m] <= 0)
					player->setPositionZ(y + 1.f);
				else
					aiCount = 9;
			}
			else if (faceTo == 3)
			{
				n1 = -x / unit + 10.1;
				n2 = -x / unit + 10.9;
				m = y / unit + 9.9;
				if (MAP[n1][m] <= 0 && MAP[n2][m] <= 0)
					player->setPositionZ(y - 1.f);
				else
					aiCount = 9;
			}
		}
	}
}

