#pragma once

#include "Model.h"
#include <q3.h>


class PhysicalModel : virtual public Model
{
public:
	PhysicalModel();
	virtual ~PhysicalModel();

	// ---- Graphics ----
	virtual glm::mat4 GetWorldMatrix() const;


	// ---- Physics ----
	void SetBody(q3Body * body);

	virtual q3BoxDef  GetBoxDef() = 0;
	virtual q3BodyDef GetBodyDef();

protected:
	q3Body * mBody;

};

