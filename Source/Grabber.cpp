#include "Grabber.h"

void Grabber::Update(q3Vec3 & lookAt, q3Vec3 & from){
	if (HasObject()){
		ApplyForce(lookAt, from);
	}
	else{
		TryGrab(lookAt, from);
	}
}

GrabberRayCastCallBack::GrabberRayCastCallBack(Grabber & grabber)
: mGrabber(grabber)
{
}

bool GrabberRayCastCallBack::ReportShape(q3Box * b){
	// Save the body
	mGrabber.mObject = b->body;

	// Retrieve the point of contact
	q3Vec3 worldPoint = mData.start + (mData.dir * mData.toi);

	// Save the point of contact in model space
	mGrabber.mPoint = b->body->GetLocalPoint(worldPoint);

	mGrabber.mIntegralError = { 0, 0, 0 };

	// TODO could for example return false if you grab a bullet or something else not grabbable
	return true;
}


void Grabber::TryGrab(q3Vec3 & lookAt, q3Vec3 & from) {
	// We query the world using a raycast
	//Set up the info of the ray
	mCallback.mData.dir = lookAt;                   // The direction of the ray
	mCallback.mData.start = from + (lookAt * 0.5f); // The start point of the ray
	mCallback.mData.t = 4;                          // The max length of the ray

	// Perform the query
	// The callback object will modify us if something is found
	mPhysics.RayCast(&mCallback, mCallback.mData);
}

void Grabber::ApplyForce(q3Vec3 & lookAt, q3Vec3 & from){
	assert(HasObject());

	q3Vec3 objectPoint = mObject->GetWorldPoint(mPoint);

	q3Vec3 force = (from + (lookAt * mCallback.mData.t)) - objectPoint;

	// Also integrate the error over time
	mIntegralError += force * 0.1;

	r32 distance = q3Distance({0,0,0},force);

	q3Vec3 velocity = mObject->GetLinearVelocity();

	if (q3Dot(velocity, force) < 0.0f){
		// If the object is moving away from us, damp its velocity
		mObject->SetLinearVelocity(velocity * 0.95f);
	}
	// Always pull it very hard toward us
	force *= distance * distance;
	force += mIntegralError;

	mObject->ApplyForceAtWorldPoint(force, objectPoint);
}

void Grabber::Drop(){
	if (HasObject()){
		mObject = NULL;
	}
}
void Grabber::Throw(q3Vec3 & velocity){
	if (HasObject()){
		mObject->SetLinearVelocity(velocity);
		mObject = NULL;
	}
}
bool Grabber::HasObject(){
	return mObject != NULL;
}

Grabber::Grabber(q3Scene & physics)
: mPhysics(physics)
, mObject(NULL)
, mCallback(*this)
, mIntegralError(0,0,0)
{
}

Grabber::~Grabber()
{
}
