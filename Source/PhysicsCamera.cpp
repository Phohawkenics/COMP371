#include "PhysicsCamera.h"
#include "EventManager.h"
#include <glm/gtc/matrix_transform.hpp>
#include <dynamics/q3Body.h>
#include "q3_glm_conversions.h"
#include <GLFW/glfw3.h>

using namespace glm;

void PhysicsCamera::checkCollision(q3Vec3 & dir, q3Vec3 & from)
{
	mCallBack.mData.dir = q3Normalize(dir);
	mCallBack.mData.start = from;
	mCallBack.mData.t = 0.5;

	mPhysics.RayCast(&mCallBack, mCallBack.mData);
}

void PhysicsCamera::Update(float dt)
{
	// Prevent from having the camera move only when the cursor is within the windows
	EventManager::DisableMouseCursor();

	// The Camera moves based on the User inputs
	// - You can access the mouse motion with EventManager::GetMouseMotionXY()
	// - For mapping A S D W, you can look in World.cpp for an example of accessing key states
	// - Don't forget to use dt to control the speed of the camera motion

	// Mouse motion to get the variation in angle
	mHorizontalAngle -= EventManager::GetMouseMotionX() * mAngularSpeed * dt;
	//mVerticalAngle   -= EventManager::GetMouseMotionY() * mAngularSpeed * dt;

	// Clamp vertical angle to [-85, 85] degrees
	/*mVerticalAngle = std::max(-85.0f, std::min(85.0f, mVerticalAngle));
	if (mHorizontalAngle > 360)
	{
		mHorizontalAngle -= 360;
	}
	else if (mHorizontalAngle < -360)
	{
		mHorizontalAngle += 360;
	}*/

	float theta = radians(mHorizontalAngle);
	float phi = radians(mVerticalAngle);

	mLookAt = vec3(cosf(phi)*cosf(theta), sinf(phi), -cosf(phi)*sinf(theta));
	
	vec3 sideVector = glm::cross(mLookAt, vec3(0.0f, 1.0f, 0.0f));
	glm::normalize(sideVector);
	
	//Starting position
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_B ) == GLFW_PRESS)
	{
			mPosition=vec3(-14.0f, 0.0f, 5.0f);
			mLookAt=vec3(0.0f, 0.0f, 1.0f);
			mVerticalAngle=0.0f;
			mHorizontalAngle=0.0f;
	}
	//Top View
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_X ) == GLFW_PRESS)
	{
		mPosition=vec3(3.0f, 30.0f, 5.0f);
		mLookAt=vec3(0.0f, -1.0f, 0.0f);
		mVerticalAngle=-90.0f;
		mHorizontalAngle=0.0f;
	}

	// A S D W for motion along the camera basis vectors
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_W ) == GLFW_PRESS)
	{
		mCallBack.impactBody = NULL;
		checkCollision(g2q(mLookAt), g2q(mPosition));
		if(mCallBack.impactBody==NULL)
			mPosition += mLookAt * dt * mSpeed;
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_S ) == GLFW_PRESS)
	{
		mCallBack.impactBody = NULL;
		checkCollision(g2q(-mLookAt), g2q(mPosition));
		if(mCallBack.impactBody==NULL)
			mPosition -= mLookAt * dt * mSpeed;
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_D ) == GLFW_PRESS)
	{
		mCallBack.impactBody = NULL;
		checkCollision(g2q(sideVector), g2q(mPosition));
		if(mCallBack.impactBody==NULL)
			mPosition += sideVector * dt * mSpeed;
	}

	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_A ) == GLFW_PRESS)
	{
		mCallBack.impactBody = NULL;
		checkCollision(g2q(-sideVector), g2q(mPosition));
		if(mCallBack.impactBody==NULL)
			mPosition -= sideVector * dt * mSpeed;
	}
}

PhysicsCamera::PhysicsCamera(vec3 position, q3Scene & physics) 
	: Camera(), 
	mPhysics(physics),
	mPosition(position),
	mVerticalAngle(0.0f),
	mHorizontalAngle(0.0f),
	mSpeed(20.0f),
	mAngularSpeed(50.0f),
	mLookAt(0.0f,0.0f,0.0f)
{
}

mat4 PhysicsCamera::GetViewMatrix() const
{
	return glm::lookAt(	mPosition, mPosition + mLookAt, vec3(0.0f, 1.0f, 0.0f) );
}
