#pragma once

#include "CubeModel.h"

class BulletModel : public CubeModel
{
public:
	BulletModel();
	virtual ~BulletModel();

	virtual q3BoxDef  GetBoxDef();


	virtual void handleBeginContact(const q3ContactConstraint *contact);


};
