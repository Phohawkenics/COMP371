#include "BulletModel.h"
#include "World.h"

BulletModel::BulletModel() : CubeModel()
{
	SetPhysicsType(Model::Dynamic);
	SetScaling(glm::vec3(0.5, 0.5, 0.5));

	mMaxAge = 90; // 90 second exipr date

}

q3BodyDef BulletModel::GetBodyDef(){
	q3BodyDef body = CubeModel::GetBodyDef();

	body.angularVelocity = q3Vec3(200, 200, 200);

	return body;
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