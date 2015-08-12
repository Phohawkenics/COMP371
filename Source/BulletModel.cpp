#include "BulletModel.h"
#include "World.h"

#include <iostream>

#include <glm/gtx/rotate_vector.hpp>

#include "q3_glm_conversions.h"

BulletModel::BulletModel(glm::vec3 dir) : Lib3dsModel(), mAge(0)
{
	SetPhysicsType(Model::Dynamic);
	SetScaling(glm::vec3(0.5, 0.5, 0.5));

	mDirection = glm::normalize(dir);

	float verticalAngle = -glm::asin(mDirection.y); // approximately good
	float horizontalAngle = -((-3.14159 / 2.0) + glm::atan(mDirection.z / mDirection.x));

	// ATAN2
	if (mDirection.x < 0){
		if (mDirection.z >= 0){
			horizontalAngle -= 3.14159;
		}
		else{
			horizontalAngle += 3.14159;
		}
	}

	//std::cout << "Bullet model vAngle: " << verticalAngle << " hAngle " << horizontalAngle  << " x:  " << direction.x << " z: " << direction.z << std::endl;

	q3Quaternion rot_vert( q3Vec3(1,0,0), verticalAngle);
	q3Quaternion rot_horiz(q3Vec3(0,1,0), horizontalAngle);

	mRotation = rot_horiz * rot_vert;

	mScaling = glm::vec3(0.01, 0.01, 0.01);
	mMeshes.insert(13);
	mFileName = "../Assets/Objects/bombs.3ds";

	LoadModel();



	//mMaxAge = 90; // 90 second exipr date

}

q3BodyDef BulletModel::GetBodyDef(){
	q3BodyDef body = Lib3dsModel::GetBodyDef();

	body.angularVelocity = g2q(25.0f * mDirection);

	return body;
}

void BulletModel::SetBody(q3Body * body){

	body->SetTransform(mRotation);

	PhysicalModel::SetBody(body);
}


q3BoxDef  BulletModel::GetBoxDef(){
	q3BoxDef box = Lib3dsModel::GetBoxDef();

	box.SetDensity(1000);
	
	return box;
}

void BulletModel::Update(float dt){
	mAge += dt;
	if (mAge > 10){
		std::cout << "Removing bullet " << std::endl;
		World::GetInstance()->RemoveModel(this);
	}
}

BulletModel::~BulletModel()
{
}

void BulletModel::handleBeginContact(const q3ContactConstraint *contact){
	World::GetInstance()->RemoveModel(this);
}