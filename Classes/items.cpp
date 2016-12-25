#include "HelloWorldScene.h"
#include "VisibleRect.h"

USING_NS_CC;

static float unit = 3.f;
extern BomberGame * game;

void BomberGame::createItems(int code,int x,int y)
{
	auto item = Sprite3D::create("box/box.c3t");
	item->setScale(0.8f);
	item->setPosition3D(Vec3((10 - x)* unit, -0.5f, (-10 + y)* unit));
	item->setCameraMask((unsigned short)CameraFlag::USER1);
	item->setTag(x * 10 + y * 1000);
	game->addChild(item);

	item->runAction(RepeatForever::create(RotateBy::create(4.f, Vec3(0.f, 360.f, 0.f))));
	item->runAction(Sequence::create(DelayTime::create(40.0f), CallFunc::create([=]()
	{
		map[x][y] = 0;
		item->stopAllActions();
		item->removeFromParent();
	}), nullptr));

	switch (code)
	{
	case 1: // health recover
		map[x][y] = -1;
		item->setTexture("box/pu_hart.png");
		break;
	case 2: // power increase
		map[x][y] = -2;
		item->setTexture("box/CyanSquare2.png");
		break;
	case 3: // agility increase
		map[x][y] = -3;
		item->setTexture("box/CyanSquare3.png");
		break;
	case 4: // intelligence increase
		map[x][y] = -4;
		item->setTexture("box/CyanSquare.png");
		break;
	}
}

void Player::getItem(int code,int x,int y)
{
	if (code == -1 && health != 5)
		health++;
	else if (code == -2 && strength <= 10)
		strength++;
	else if (code == -3 && agility < 20)
		agility++;
	else if (code == -4 && intelligence < 100)
		intelligence += 10;

	(game->getChildByTag(x * 10 + y * 1000))->stopAllActions();
	game->removeChildByTag(x * 10 + y * 1000);
	game->map[x][y] = 0;
}

char* itoa2(int num, char*str, int radix)
{/*索引表*/
	char index[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";
	unsigned unum;/*中间变量*/
	int i = 0, j, k;
	/*确定unum的值*/
	if (radix == 10 && num<0)/*十进制负数*/
	{
		unum = (unsigned)-num;
		str[i++] = '-';
	}
	else unum = (unsigned)num;/*其他情况*/
							  /*转换*/
	do {
		str[i++] = index[unum % (unsigned)radix];
		unum /= radix;
	} while (unum);
	str[i] = '\0';
	/*逆序*/
	if (str[0] == '-')k = 1;/*十进制负数*/
	else k = 0;
	char temp;
	for (j = k; j <= (i - 1) / 2; j++)
	{
		temp = str[j];
		str[j] = str[i - 1 + k - j];
		str[i - 1 + k - j] = temp;
	}
	return str;
}

void BomberGame::attributeDisplay()
{
	if (playerInControl == nullptr)
		return;
	char* str = new char[10];
	const std::string strStrength = "Strength : " + std::string(itoa2(playerInControl->strength, str, 10));
	const std::string strAgility = "Agility : " + std::string(itoa2(playerInControl->agility, str, 10));
	const std::string strIntelligence = "Intelligence : " + std::string(itoa2(playerInControl->intelligence, str, 10));
	
	TTFConfig ttfConfig("fonts/Marker Felt.ttf", 30);
	auto labelHealth = Label::createWithTTF(ttfConfig,"Health Point");
	labelStrength = Label::createWithTTF(ttfConfig, strStrength);
	labelAgility = Label::createWithTTF(ttfConfig, strAgility);
	labelIntelligence = Label::createWithTTF(ttfConfig, strIntelligence);
	auto menuItem00 = MenuItemLabel::create(labelHealth, [=](Ref *ref) {

	});
	auto menuItem01 = MenuItemLabel::create(labelStrength, [=](Ref *ref) {
		
	});
	auto menuItem02 = MenuItemLabel::create(labelAgility, [=](Ref *ref) {

	});
	auto menuItem03 = MenuItemLabel::create(labelIntelligence, [=](Ref *ref) {

	});
	menuItem00->setPositionY(0);
	menuItem01->setPositionY(-50);
	menuItem02->setPositionY(-100);
	menuItem03->setPositionY(-150);

	auto menu = Menu::create(menuItem00, menuItem01, menuItem02, menuItem03, nullptr);
	menu->setPosition(Vec2(VisibleRect::left().x + 100, VisibleRect::top().y - 30));
	game->addChild(menu);

	health01 = Sprite::create("box/pu_hart.png");
	health02 = Sprite::create("box/pu_hart.png");
	health03 = Sprite::create("box/pu_hart.png");
	health04 = Sprite::create("box/pu_hart.png");
	health05 = Sprite::create("box/pu_hart.png");

	health01->setPosition(VisibleRect::left().x + 200, VisibleRect::top().y - 30);
	health02->setPosition(health01->getPositionX() + 40, health01->getPositionY());
	health03->setPosition(health02->getPositionX() + 40, health01->getPositionY());
	health04->setPosition(health03->getPositionX() + 40, health01->getPositionY());
	health05->setPosition(health04->getPositionX() + 40, health01->getPositionY());

	health01->setScale(0.2f);
	health02->setScale(0.2f);
	health03->setScale(0.2f);
	health04->setScale(0.2f);
	health05->setScale(0.2f);

	health01->setVisible(1);
	health02->setVisible(1);
	health03->setVisible(1);
	health04->setVisible(0);
	health05->setVisible(0);

	game->addChild(health01);
	game->addChild(health02);
	game->addChild(health03);
	game->addChild(health04);
	game->addChild(health05);
}

void BomberGame::attributeDisplayUpdate()
{
	if (playerInControl == nullptr)
		return;
	char* str = new char[10];
	const std::string strStrength = "Strength : " + std::string(itoa2(playerInControl->strength, str, 10));
	const std::string strAgility = "Agility : " + std::string(itoa2(playerInControl->agility, str, 10));
	const std::string strIntelligence = "Intelligence : " + std::string(itoa2(playerInControl->intelligence, str, 10));

	labelStrength->setString(strStrength);
	labelAgility->setString(strAgility);
	labelIntelligence->setString(strIntelligence);

	if (playerInControl->health == 0)
	{
		health01->setVisible(0);
		health02->setVisible(0);
		health03->setVisible(0);
		health04->setVisible(0);
		health05->setVisible(0);
	}
	else if (playerInControl->health == 1)
	{
		health01->setVisible(1);
		health02->setVisible(0);
		health03->setVisible(0);
		health04->setVisible(0);
		health05->setVisible(0);
	}
	else if (playerInControl->health == 2)
	{
		health01->setVisible(1);
		health02->setVisible(1);
		health03->setVisible(0);
		health04->setVisible(0);
		health05->setVisible(0);
	}
	else if (playerInControl->health == 3)
	{
		health01->setVisible(1);
		health02->setVisible(1);
		health03->setVisible(1);
		health04->setVisible(0);
		health05->setVisible(0);
	}
	else if (playerInControl->health == 4)
	{
		health01->setVisible(1);
		health02->setVisible(1);
		health03->setVisible(1);
		health04->setVisible(1);
		health05->setVisible(0);
	}
	else if (playerInControl->health == 5)
	{
		health01->setVisible(1);
		health02->setVisible(1);
		health03->setVisible(1);
		health04->setVisible(1);
		health05->setVisible(1);
	}
}


