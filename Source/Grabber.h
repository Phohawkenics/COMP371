#include <q3.h>

#pragma once


class GrabberRayCastCallBack : public q3QueryCallback {
	friend class Grabber;

	Grabber & const mGrabber;

	q3RaycastData mData;

	GrabberRayCastCallBack(Grabber & grabber);

	bool ReportShape(q3Box * b);
};

class Grabber
{
public:
	Grabber(q3Scene & physics);
	~Grabber();

	void Update(q3Vec3 & lookAt, q3Vec3 & from);
	void Drop();
	void Throw(q3Vec3 & velocity);
	bool HasObject();

private:

	void TryGrab(q3Vec3 & lookAt, q3Vec3 & from);
	void ApplyForce(q3Vec3 & lookAt, q3Vec3 & from);

	q3Body * mObject;
	q3Vec3   mPoint;

	// control systems yo
	q3Vec3 mIntegralError;

	const q3Scene & const mPhysics;

	friend class GrabberRayCastCallBack;

	GrabberRayCastCallBack mCallback;
};