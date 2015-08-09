#include "BulletModel.h"
#include "World.h"

BulletModel::BulletModel() : CubeModel()
{
	SetPhysicsType(Model::Dynamic);
	SetScaling(glm::vec3(0.5, 0.5, 0.5));

}

q3BoxDef  BulletModel::GetBoxDef(){
	q3BoxDef box = CubeModel::GetBoxDef();

	box.SetDensity(1000);

	return box;
}

BulletModel::~BulletModel()
{
}

void BulletModel::handleBeginContact(const q3ContactConstraint *contact){
	World::GetInstance()->RemoveModel(this);
}