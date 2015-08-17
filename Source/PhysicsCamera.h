#include "Camera.h"
#include "Lib3dsModel.h"
#include "CameraRayCastCallBack.h"

#pragma once

class PhysicsCamera : public Camera
{
public:
	PhysicsCamera(glm::vec3 position , q3Scene & physics);

	virtual void Update(float dt);

	virtual glm::mat4 GetViewMatrix() const;
	virtual glm::vec3 GetPos() { return mPosition; };
	virtual glm::vec3 GetLookAt() { return mLookAt; };

	void checkCollision(q3Vec3 & dir, q3Vec3 & from, r32 dist);

	virtual void SetPosition(glm::vec3 & pos);

private:
	glm::vec3 mPosition;	
	float mHorizontalAngle; // horizontal angle
	float mVerticalAngle;   // vertical angle
	float mSpeed;			// World units per second
	float mAngularSpeed;    // Degrees per pixel
	glm::vec3 mLookAt;
	glm::vec3 mForward;

	const q3Scene & const mPhysics;

	friend class CameraRayCastCallBack;

	CameraRayCastCallBack mCallBack;
};
