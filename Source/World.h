//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once


#include "ParsingHelper.h"
#include "Billboard.h"
#include <vector>

#include "Grabber.h"

#include <q3.h>

class Camera;
class Model;
class Animation;
class AnimationKey;
class ParticleSystem;

class World
{
public:
	World();
	~World();
	
    static World* GetInstance();

	void Update(float dt);
	void Draw();

	void LoadScene(const char * scene_path);
    Animation* FindAnimation(ci_string animName);
	AnimationKey* FindAnimationKey(ci_string keyName);

    const Camera* GetCurrentCamera() const;
    void AddBillboard(Billboard* b);
    void RemoveBillboard(Billboard* b);
    void AddParticleSystem(ParticleSystem* particleSystem);
    void RemoveParticleSystem(ParticleSystem* particleSystem);

	// Light Coefficients
	const glm::vec3 & GetLightColor(){ return lightColor; };
	float GetLightKc() { return lightKc; };
	float GetLightKl() { return lightKl; };
	float GetLightKq() { return lightKq; };
	const glm::vec4 & GetLightPosition() { return lightPosition; }; // If w = 1.0f, we have a point light

	bool RemoveModel(Model * model);
	
private:
    static World* instance;
    
	q3Scene * mPhysics;

	Grabber mGrabber;

	void Shoot(float dt);

	void Pickup(float dt);

	void Drop(float dt);

	// Light Coefficients
	glm::vec3 lightColor;
	float lightKc;
	float lightKl;
	float lightKq;
	glm::vec4 lightPosition; // If w = 1.0f, we have a point light

	void RemoveAllQueuedModels();

	std::vector<Model*> mModelRemovalQueue;

	std::vector<Model*> mModel;
    std::vector<Animation*> mAnimation;
    std::vector<AnimationKey*> mAnimationKey;
	std::vector<Camera*> mCamera;
    std::vector<ParticleSystem*> mParticleSystemList;
	unsigned int mCurrentCamera;

    BillboardList* mpBillboardList;
};
