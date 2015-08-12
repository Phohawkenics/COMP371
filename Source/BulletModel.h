#pragma once

#include "Lib3dsModel.h"

#include "q3.h"

class BulletModel : public Lib3dsModel
{
public:
	BulletModel(glm::vec3 direction);
	virtual ~BulletModel();

	virtual q3BoxDef  GetBoxDef();
	virtual q3BodyDef  GetBodyDef();

	virtual void SetBody(q3Body * body);

	virtual void Update(float dt);

	virtual void handleBeginContact(const q3ContactConstraint *contact);
protected:
	q3Quaternion mRotation;
	glm::vec3 mDirection;

	float mAge;
};
