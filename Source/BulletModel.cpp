#include "BulletModel.h"
#include "World.h"

#include <glm/gtx/rotate_vector.hpp>

#include "q3_glm_conversions.h"

BulletModel::BulletModel(glm::vec3 direction) : Lib3dsModel()
{
	SetPhysicsType(Model::Dynamic);
	SetScaling(glm::vec3(0.5, 0.5, 0.5));

	direction = glm::normalize(direction);

	float verticalAngle = -glm::asin(direction.y); // approximately good
	float horizontalAngle = - ((-3.14159 / 2.0) + glm::atan(direction.z / direction.x));

	// ATAN2
	if (direction.x < 0){
		if (direction.z >= 0){
			horizontalAngle -= 3.14159;
		}
		else{
			horizontalAngle += 3.14159;
		}
	}

	std::cout << "Bullet model vAngle: " << verticalAngle << " hAngle " << horizontalAngle  << " x:  " << direction.x << " z: " << direction.z << std::endl;

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

	//body.angularVelocity = q3Vec3(200, 200, 200);

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

BulletModel::~BulletModel()
{
}

void BulletModel::handleBeginContact(const q3ContactConstraint *contact){
	World::GetInstance()->RemoveModel(this);
}