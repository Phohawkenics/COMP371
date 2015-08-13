//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "LightModel.h"

#include "ParsingHelper.h"
#include "Billboard.h"
#include <vector>
#include "Lib3dsModel.h"

#include "SphereModel.h"
#include "StaticCamera.h"
#include "PhysicsCamera.h"

#include "Grabber.h"
#include "LightModel.h"

#include <q3.h>

class Camera;
class Model;
class Animation;
class AnimationKey;
class ParticleSystem;
class CubeModel;

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

	void RemoveModel(Model * model);
	void AddModel(Model * model, q3BodyDef *body = NULL, q3BoxDef *box = NULL);

private:
    static World* instance;
    
	PhysicsCamera* player;  //1st person cam
	StaticCamera* bev;		//3rd person cam following player
	 Lib3dsModel * character;
	 float mHorizontalAngle;

	CubeModel * mTeleporter;

	q3Scene * mPhysics;

	Grabber mGrabber;

	void Shoot(float dt);

	void Pickup(float dt);

	void Drop(float dt);

	void Teleport();

	//ADD THIS
	void Rain(float dt);
	void RemoveAllQueuedModels();
	void AddAllQueuedModels();
	LightModel lights;

	std::vector<std::tuple<Model*,q3BodyDef*,q3BoxDef*> > mModelAddQueue;
	std::vector<Model*> mModelRemovalQueue;

	std::vector<Model*> mModel;
    std::vector<Animation*> mAnimation;
    std::vector<AnimationKey*> mAnimationKey;
	std::vector<Camera*> mCamera;
    std::vector<ParticleSystem*> mParticleSystemList;
	unsigned int mCurrentCamera;

	float mShootCooldown;

    BillboardList* mpBillboardList;
};
