#include "HelloWorldScene.h"
USING_NS_CC;

void BomberGame::terrain()
{
	Terrain::DetailMap r("TerrainTest/dirt.jpg"), g("TerrainTest/Grass2.jpg", 10), b("TerrainTest/road.jpg"), a("TerrainTest/Grass1.jpg", 40);

	Terrain::TerrainData data("TerrainTest/heightmap0.jpg", "TerrainTest/alphamap0.png", r, g, b, a, Size(32, 32), 7.0f, 1.f);
	auto terrain = Terrain::create(data, Terrain::CrackFixedType::SKIRT);
	terrain->setMaxDetailMapAmount(4);
	terrain->setDrawWire(false);

	terrain->setSkirtHeightRatio(0);
	terrain->setLODDistance(64, 128, 192);
	terrain->setCameraMask((unsigned short)CameraFlag::USER1);
	//create terrain
	std::vector<float> heidata = terrain->getHeightData();
	auto size = terrain->getTerrainSize();
	this->addChild(terrain);
}

short a[21][21] = {
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,
	1,2,0,0,0,0,0,0,2,0,0,0,0,0,0,0,0,2,2,2,1,
	1,0,0,0,2,0,0,0,0,0,0,0,2,0,0,0,0,0,2,2,1,
	1,0,0,1,1,2,2,1,1,2,2,1,1,2,2,0,0,0,0,2,1,
	1,0,2,1,0,2,0,2,0,0,0,2,1,2,0,0,0,0,0,0,1,
	1,0,0,2,2,1,2,1,2,1,2,2,1,0,0,0,0,0,0,0,1,
	1,0,0,2,0,2,0,2,0,0,2,1,0,2,0,0,0,2,0,0,1,
	1,0,0,1,2,1,2,1,2,2,1,2,0,0,0,0,2,2,2,0,1,
	1,2,0,1,0,2,0,2,2,2,0,0,0,0,2,1,1,1,0,0,1,
	1,0,0,2,0,1,0,2,2,0,0,0,0,2,1,2,2,1,0,0,1,
	1,0,0,2,0,2,2,1,0,0,2,0,0,1,0,0,0,1,0,0,1,
	1,0,0,1,2,2,1,2,0,0,0,0,2,0,0,0,0,1,0,0,1,
	1,0,2,1,1,1,0,0,2,0,0,2,0,0,0,0,0,1,0,2,1,
	1,0,0,2,2,0,0,0,0,0,1,0,0,2,2,2,0,2,0,0,1,
	1,0,0,2,0,0,0,0,2,1,0,0,0,2,2,2,0,2,0,0,1,
	1,0,0,0,0,0,0,0,1,2,0,0,0,2,2,2,0,1,0,0,1,
	1,0,0,0,0,0,0,2,1,2,0,0,0,0,0,0,0,1,2,0,1,
	1,2,0,0,0,0,2,2,1,1,1,1,1,2,2,1,1,1,0,0,1,
	1,2,2,0,0,0,0,0,0,0,0,0,2,0,0,0,0,0,0,0,1,
	1,2,2,2,0,0,0,0,0,2,0,0,0,0,0,0,2,0,0,2,1,
	1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1
};

void BomberGame::createMap()
{
	int i, j;
	float unit = 3.f;
	float x, y;
	map = new short *[21];
	for (i = 0; i < 21; i++)
	{
		map[i] = new short[21];
	}
	for (i = 0; i <21; i++)
	{
		for (j = 0; j < 21; j++)
		{
			map[i][j] = a[i][j];
			if (map[i][j] == 1)
			{
				x = (10 - i)* unit;
				y = (-10 + j)* unit;
				auto wall = Sprite3D::create("box/box.c3t");
				wall->setTexture("box/road.jpg");
				wall->setScale(unit);
				wall->setPosition3D(Vec3(x, -2.f, y));
				this->addChild(wall);
				wall->setCameraMask((unsigned short)CameraFlag::USER1);
			}
			if (map[i][j] == 2)
			{
				x = (10 - i)* unit;
				y = (-10 + j)* unit;
				auto box = Sprite3D::create("box/box.c3t");
				box->setTexture("box/plane.png");
				box->setScale(unit*0.8);
				box->setPosition3D(Vec3(x, -2.2f, y));
				this->addChild(box);
				box->setCameraMask((unsigned short)CameraFlag::USER1);
				box->setTag(i * 10 + j * 1000);
			}
		}
	}
}