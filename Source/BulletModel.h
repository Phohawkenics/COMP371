#pragma once

#include "CubeModel.h"

class BulletModel : public CubeModel
{
public:
	BulletModel();
	virtual ~BulletModel();

	virtual q3BoxDef  GetBoxDef();
	virtual q3BodyDef  GetBodyDef();


	virtual void handleBeginContact(const q3ContactConstraint *contact);


};
