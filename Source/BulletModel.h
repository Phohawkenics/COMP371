#pragma once

#include "CubeModel.h"

class BulletModel : public CubeModel
{
public:
	BulletModel();
	virtual ~BulletModel();

	virtual q3BoxDef  GetBoxDef();


	virtual void handleEndContact(q3Box * box);


};
