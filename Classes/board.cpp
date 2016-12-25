#include "HelloWorldScene.h"
#include "VisibleRect.h"

USING_NS_CC;

void BomberGame::board()
{
	auto control_circle_x = VisibleRect::right().x * 0.12;
	auto control_circle_y = VisibleRect::top().y * 0.2;
	auto control_circle = Sprite::create("image/round_button.png");
	control_circle->setPosition(control_circle_x, control_circle_y);
	control_circle->setScale(1.5f);
	this->addChild(control_circle);

	_circle = Sprite::create("image/round_button.png");
	_circle->setVisible(0);
	_circle->setScale(0.6f);
	this->addChild(_circle);

	auto button_x = VisibleRect::right().x * 0.88;
	auto button_y = VisibleRect::top().y * 0.2;

	auto glass = Sprite::create("image/glass_button.png");
	glass->setPosition(button_x, button_y);
	glass->setScaleX(1.6f);
	glass->setScaleY(0.8f);
	this->addChild(glass);

	auto glass2 = Sprite::create("image/glass_button.png");
	glass2->setPosition(button_x + 50, button_y + 120);
	glass2->setScaleX(0.8f);
	glass2->setScaleY(0.4f);
	this->addChild(glass2);

	auto glass3 = Sprite::create("image/glass_button.png");
	glass3->setPosition(button_x + 50, button_y + 240);
	glass3->setScaleX(0.8f);
	glass3->setScaleY(0.4f);
	this->addChild(glass3);

	auto glass4 = Sprite::create("image/glass_button.png");
	glass4->setPosition(button_x + 50, button_y + 360);
	glass4->setScaleX(0.8f);
	glass4->setScaleY(0.4f);
	this->addChild(glass4);

	TTFConfig ttfConfig("fonts/Marker Felt.ttf", 90);
	TTFConfig ttfConfig2("fonts/Marker Felt.ttf", 25);

	if (playerInControl->occupation == 1)
	{
		labelNormal = Label::createWithTTF(ttfConfig, "BOMB");
		labelBounceBomb = Label::createWithTTF(ttfConfig2, "BOUNCE\n  BOMB");
		labelTrap = Label::createWithTTF(ttfConfig2, "TRAP\nBOMB");
		labelRemoteControl = Label::createWithTTF(ttfConfig2, "REMOTE\nCONTROL");
		auto bombButton = MenuItemLabel::create(labelNormal, [=](Ref *ref)
		{
			if (playerInControl->CD == 0)
			{
				playerInControl->CD = (100.f - (float)playerInControl->intelligence) / 25.f;
				playerInControl->placeBombNum++;
				playerInControl->createBomb(playerInControl->i, playerInControl->j, playerInControl->strength);				
			}
		});
		auto bombButton2 = MenuItemLabel::create(labelBounceBomb, [=](Ref *ref)
		{
			if (playerInControl->skillCD[2] == 0 && !playerInControl->bounceBombOn)
			{
				playerInControl->useSkillNum++;
				playerInControl->skillCD[2] = (200.f - (float)playerInControl->intelligence) / 20.f;
				playerInControl->createBounceBomb(playerInControl->i, playerInControl->j, playerInControl->strength);		
			}
		});
		auto bombButton3 = MenuItemLabel::create(labelTrap, [=](Ref *ref)
		{
			if (playerInControl->skillCD[1] == 0 && !playerInControl->automaticBombOn)
			{
				playerInControl->useSkillNum++;
				playerInControl->skillCD[1] = (200.f - (float)playerInControl->intelligence) / 20.f;
				playerInControl->calCurrentPlayerPosition();
				playerInControl->createAutomaticBomb(playerInControl->i, playerInControl->j, playerInControl->strength);				
			}
		});
		auto bombButton4 = MenuItemLabel::create(labelRemoteControl, [=](Ref *ref)
		{
			if (playerInControl->remoteBombOn)
			{
				playerInControl->useSkillNum++;
				playerInControl->trigerRemoteBomb();
				playerInControl->skillCD[0] = (200.f - (float)playerInControl->intelligence) / 20.f;
			}
			else if (playerInControl->skillCD[0] == 0)
				playerInControl->createRemoteBomb(playerInControl->i, playerInControl->j, playerInControl->strength);
		});

		auto buttonMenu = Menu::create(bombButton, bombButton2, bombButton3, bombButton4, nullptr);
		buttonMenu->setPosition(button_x, button_y);
		bombButton2->setPosition(50, 120);
		bombButton3->setPosition(50, 240);
		bombButton4->setPosition(50, 360);
		this->addChild(buttonMenu);
	}
	else if (playerInControl->occupation == 2)
	{
		labelNormal = Label::createWithTTF(ttfConfig, "BOMB");
		labelBounceBomb = Label::createWithTTF(ttfConfig2, "RUSH");
		labelTrap = Label::createWithTTF(ttfConfig2, "THROW\n3 BOMBS");
		labelRemoteControl = Label::createWithTTF(ttfConfig2, "BOMB\nWALL");
		auto bombButton = MenuItemLabel::create(labelNormal, [=](Ref *ref)
		{
			if (playerInControl->CD == 0)
			{
				playerInControl->placeBombNum++;
				playerInControl->CD = (100.f - (float)playerInControl->intelligence) / 25.f;
				playerInControl->createBomb(playerInControl->i, playerInControl->j, playerInControl->strength);			
			}
		});
		auto bombButton2 = MenuItemLabel::create(labelBounceBomb, [=](Ref *ref)
		{
			if (playerInControl->skillCD[2] == 0 && !playerInControl->onRush)
			{
				playerInControl->useSkillNum++;
				playerInControl->skillCD[2] = (200.f - (float)playerInControl->intelligence) / 20.f;
				playerInControl->onRush = 1;	
			}
		});
		auto bombButton3 = MenuItemLabel::create(labelTrap, [=](Ref *ref)
		{
			if (playerInControl->skillCD[1] == 0 && !playerInControl->onThrowing)
			{
				playerInControl->useSkillNum++;
				playerInControl->skillCD[1] = (200.f - (float)playerInControl->intelligence) / 20.f;
				playerInControl->calCurrentPlayerPosition();	
				playerInControl->placeBombNum += 3;
				playerInControl->throw3Bombs(playerInControl->i, playerInControl->j, playerInControl->strength);		
			}
		});
		auto bombButton4 = MenuItemLabel::create(labelRemoteControl, [=](Ref *ref)
		{
			if (playerInControl->skillCD[0] == 0)
			{
				playerInControl->useSkillNum++;
				playerInControl->placeBombNum += 5;
				playerInControl->skillCD[0] = (200.f - (float)playerInControl->intelligence) / 20.f;
				playerInControl->bombWall(playerInControl->i, playerInControl->j, playerInControl->strength);
			}
		});

		auto buttonMenu = Menu::create(bombButton, bombButton2, bombButton3, bombButton4, nullptr);
		buttonMenu->setPosition(button_x, button_y);
		bombButton2->setPosition(50, 120);
		bombButton3->setPosition(50, 240);
		bombButton4->setPosition(50, 360);
		this->addChild(buttonMenu);
	}
	else if (playerInControl->occupation == 3)
	{
		labelNormal = Label::createWithTTF(ttfConfig, "BOMB");
		labelBounceBomb = Label::createWithTTF(ttfConfig2, "CANNON");
		labelTrap = Label::createWithTTF(ttfConfig2, "SAFETY\n  GATE");
		labelRemoteControl = Label::createWithTTF(ttfConfig2, "MOTAR");
		auto bombButton = MenuItemLabel::create(labelNormal, [=](Ref *ref)
		{
			if (playerInControl->CD == 0)
			{
				playerInControl->placeBombNum++;
				playerInControl->CD = (100.f - (float)playerInControl->intelligence) / 25.f;
				playerInControl->createBomb(playerInControl->i, playerInControl->j, playerInControl->strength);		
			}
		});
		auto bombButton2 = MenuItemLabel::create(labelBounceBomb, [=](Ref *ref)
		{
			if (playerInControl->skillCD[2] == 0 && playerInControl->cannonNum <= 3)
			{
				bool canPlace = 1;
				for (int n = 0; n < playerNum; n++)
				{
					int pi = player[n]->i;
					int pj = player[n]->j;

					if ((playerInControl->faceTo == 0 && pi == playerInControl->i + 1 && pj == playerInControl->j) ||
						(playerInControl->faceTo == 1 && pi == playerInControl->i - 1 && pj == playerInControl->j) ||
						(playerInControl->faceTo == 2 && pi == playerInControl->i && pj == playerInControl->j + 1) ||
						(playerInControl->faceTo == 3 && pi == playerInControl->i && pj == playerInControl->j - 1))
						canPlace = 0;
				}
				if (canPlace)
				{
					playerInControl->useSkillNum++;
					playerInControl->skillCD[2] = (200.f - (float)playerInControl->intelligence) / 20.f;
					if (playerInControl->faceTo == 0)playerInControl->createCannon(playerInControl->i + 1, playerInControl->j, playerInControl->strength);
					else if (playerInControl->faceTo == 1)playerInControl->createCannon(playerInControl->i - 1, playerInControl->j, playerInControl->strength);
					else if (playerInControl->faceTo == 2)playerInControl->createCannon(playerInControl->i, playerInControl->j + 1, playerInControl->strength);
					else if (playerInControl->faceTo == 3)playerInControl->createCannon(playerInControl->i, playerInControl->j - 1, playerInControl->strength);
				}
			}
		});
		auto bombButton3 = MenuItemLabel::create(labelTrap, [=](Ref *ref)
		{
			if (playerInControl->skillCD[1] == 0 && playerInControl->gateNum <= 3)
			{
				bool canPlace = 1;
				for (int n = 0; n < playerNum; n++)
				{
					int pi = player[n]->i;
					int pj = player[n]->j;

					if ((playerInControl->faceTo == 0 && pi == playerInControl->i + 1 && pj == playerInControl->j) ||
						(playerInControl->faceTo == 1 && pi == playerInControl->i - 1 && pj == playerInControl->j) ||
						(playerInControl->faceTo == 2 && pi == playerInControl->i && pj == playerInControl->j + 1) ||
						(playerInControl->faceTo == 3 && pi == playerInControl->i && pj == playerInControl->j - 1))
						canPlace = 0;
				}
				if (canPlace)
				{
					playerInControl->useSkillNum++;
					playerInControl->skillCD[1] = (200.f - (float)playerInControl->intelligence) / 20.f;
					if      (playerInControl->faceTo == 0)playerInControl->createGate(playerInControl->i + 1, playerInControl->j);
					else if (playerInControl->faceTo == 1)playerInControl->createGate(playerInControl->i - 1, playerInControl->j);
					else if (playerInControl->faceTo == 2)playerInControl->createGate(playerInControl->i, playerInControl->j + 1);
					else if (playerInControl->faceTo == 3)playerInControl->createGate(playerInControl->i, playerInControl->j - 1);
				}
			}
		});
		auto bombButton4 = MenuItemLabel::create(labelRemoteControl, [=](Ref *ref)
		{
			if (playerInControl->skillCD[0] == 0 && !playerInControl->motarOn)
			{
				bool canPlace = 1;
				for (int n = 0; n < playerNum; n++)
				{
					int pi = player[n]->i;
					int pj = player[n]->j;

					if ((playerInControl->faceTo == 0 && pi == playerInControl->i + 1 && pj == playerInControl->j) ||
						(playerInControl->faceTo == 1 && pi == playerInControl->i - 1 && pj == playerInControl->j) ||
						(playerInControl->faceTo == 2 && pi == playerInControl->i && pj == playerInControl->j + 1) ||
						(playerInControl->faceTo == 3 && pi == playerInControl->i && pj == playerInControl->j - 1))
						canPlace = 0;
				}
				if (canPlace)
				{
					playerInControl->useSkillNum++;
					playerInControl->skillCD[0] = (200.f - (float)playerInControl->intelligence) / 20.f;
					if (playerInControl->faceTo == 0)playerInControl->createMotar(playerInControl->i + 1, playerInControl->j, playerInControl->strength);
					else if (playerInControl->faceTo == 1)playerInControl->createMotar(playerInControl->i - 1, playerInControl->j, playerInControl->strength);
					else if (playerInControl->faceTo == 2)playerInControl->createMotar(playerInControl->i, playerInControl->j + 1, playerInControl->strength);
					else if (playerInControl->faceTo == 3)playerInControl->createMotar(playerInControl->i, playerInControl->j - 1, playerInControl->strength);
				}
			}
		});

		auto buttonMenu = Menu::create(bombButton, bombButton2, bombButton3, bombButton4, nullptr);
		buttonMenu->setPosition(button_x, button_y);
		bombButton2->setPosition(50, 120);
		bombButton3->setPosition(50, 240);
		bombButton4->setPosition(50, 360);
		this->addChild(buttonMenu);
	}
}