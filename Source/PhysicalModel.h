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
	virtual void SetBody(q3Body * body);
	q3Body * GetBody() { return mBody; }

	virtual q3BoxDef  GetBoxDef() = 0;
	virtual q3BodyDef GetBodyDef();

	bool hasPhysics();

	virtual void handleBeginContact(const q3ContactConstraint *contact){};

	virtual void handleEndContact(const q3ContactConstraint *contact){};

protected:

	virtual bool ParseLine(const std::vector<ci_string> &token) = 0;

	q3Body * mBody;

	float mFriction;
	float mRestitution;
	float mDensity;

};

