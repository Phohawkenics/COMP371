#include "BulletModel.h"
#include "World.h"

BulletModel::BulletModel() : CubeModel()
{
}

BulletModel::~BulletModel()
{
}

void BulletModel::handleEndContact(q3Box * body){
	std::cout << "Bullet end contact!!" << std::endl;
	World::GetInstance()->RemoveModel(this);
}