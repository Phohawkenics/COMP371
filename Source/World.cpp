//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "World.h"
#include "Renderer.h"
#include "ParsingHelper.h"

#include "StaticCamera.h"
#include "FirstPersonCamera.h"
#include "PhysicsCamera.h"

#include "CubeModel.h"
#include "SphereModel.h"
#include "Lib3dsModel.h"
#include "Animation.h"
#include "Billboard.h"
#include <GLFW/glfw3.h>
#include "EventManager.h"
#include "TextureLoader.h"

#include "ParticleDescriptor.h"
#include "ParticleEmitter.h"
#include "ParticleSystem.h"

#include "q3_glm_conversions.h"

#include "BulletModel.h"
#include "MazeCube.h"
#include "HalfMCube.h"
#include "RectModel2.h"

#include "ContactListener.h"

#include <tuple>

#include <q3.h>

#include <glm/gtx/rotate_vector.hpp>

using namespace std;
using namespace glm;

World* World::instance;

//ADD THIS
bool rainSwitch = false;

World::World()
 : mPhysics(new q3Scene(1.0f / 200.0f)), // this is the dt of one frame (I guess)
mGrabber(*mPhysics)
{
    instance = this;

	mPhysics->SetContactListener(new ContactListener());

	vec3 startingPosition = vec3(0.0f, 1.0f, 0.0f);
	// Setup Camera
	mCamera.push_back(new PhysicsCamera(startingPosition, *mPhysics));
	mCamera.push_back(new StaticCamera(vec3(3.0f, 30.0f, 5.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f)));
	mCamera.push_back(new PhysicsCamera(startingPosition,*mPhysics));

	mCurrentCamera = 0;

    
    // TODO: You can play with different textures by changing the billboardTest.bmp to another texture
#if defined(PLATFORM_OSX)
//    int billboardTextureID = TextureLoader::LoadTexture("Textures/BillboardTest.bmp");
    int billboardTextureID = TextureLoader::LoadTexture("Textures/Particle.png");
#else
//    int billboardTextureID = TextureLoader::LoadTexture("../Assets/Textures/BillboardTest.bmp");
    int billboardTextureID = TextureLoader::LoadTexture("../Assets/Textures/Particle.png");
#endif
    assert(billboardTextureID != 0);

    mpBillboardList = new BillboardList(2048, billboardTextureID);

}

World::~World()
{
	// Models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		delete *it;
	}

	mModel.clear();

	for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
	{
		delete *it;
	}

	mAnimation.clear();

	for (vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
	{
		delete *it;
	}

	mAnimationKey.clear();

	// Camera
	for (vector<Camera*>::iterator it = mCamera.begin(); it < mCamera.end(); ++it)
	{
		delete *it;
	}
	mCamera.clear();

	delete mpBillboardList;

	delete mPhysics;
}

World* World::GetInstance()
{
    return instance;
}

void World::Update(float dt)
{
	RemoveAllQueuedModels();
	AddAllQueuedModels();
	
	// User Inputs
	// 0 1 2 to change the Camera
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_1 ) == GLFW_PRESS)
	{
		mCurrentCamera = 0;
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_2 ) == GLFW_PRESS)
	{
		if (mCamera.size() > 1)
		{
			mCurrentCamera = 1;
		}
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_3 ) == GLFW_PRESS)
	{
		if (mCamera.size() > 2)
		{
			mCurrentCamera = 2;
		}
	}

	//if (glfwGetKey(EventManager::GetWindow(), GLFW_) == GLFW_PRESS)
	if (glfwGetMouseButton(EventManager::GetWindow(), GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS)
	{
		Pickup(dt);
	}
	else{
		Drop(dt);
	}
	if (glfwGetMouseButton(EventManager::GetWindow(), GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS)
	{
		Shoot(dt);
	}


	// Spacebar to change the shader
	if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_0 ) == GLFW_PRESS)
	{
		//ADD THIS
		Rain(dt);
	}
	else if (glfwGetKey(EventManager::GetWindow(), GLFW_KEY_9 ) == GLFW_PRESS)
	{
		Renderer::SetShader(SHADER_BLUE);
	}

    // Update animation and keys
    for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
    {
        (*it)->Update(dt);
    }
    
    for (vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
    {
        (*it)->Update(dt);
    }


	// Update current Camera
	mCamera[mCurrentCamera]->Update(dt);

	// updatePhysics

	mPhysics->Step();

	// Update models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		(*it)->Update(dt);
	}
    
    // Update billboards
    
    for (vector<ParticleSystem*>::iterator it = mParticleSystemList.begin(); it != mParticleSystemList.end(); ++it)
    {
        (*it)->Update(dt);
    }
    
    mpBillboardList->Update(dt);

	Teleport();

}

void World::Pickup(float dt){
	PhysicsCamera * cam = (PhysicsCamera*)mCamera[0]; // Bad and dangerous but oh well

	vec3 camLookAt = cam->GetLookAt();
	vec3 camPos = cam->GetPos();

	mGrabber.Update(g2q(camLookAt), g2q(camPos));
	
}

void World::Drop(float dt){
	mGrabber.Drop();
}

void World::Shoot(float dt){
	const float COOLDOWN = 0.25;
	static float time = 0;

	time += dt;

	PhysicsCamera * cam = (PhysicsCamera*)mCamera[0]; // Bad and dangerous but oh well

	vec3 camLookAt = cam->GetLookAt();
	vec3 camPos = cam->GetPos();

	if (mGrabber.HasObject()){
		mGrabber.Throw(g2q(camLookAt * 50.0f));
		time = 0;
	}
	else{
		if (time > COOLDOWN){
			std::cout << "Shoot!!!" << std::endl;
			time = 0;
		}
		else{
			return;
		}

		// Box attributes
		BulletModel* bullet = new BulletModel();
		bullet->SetPosition(camPos - vec3(0, 0.5, 0)); // shoot from slightly below the camera
		mModel.push_back(bullet);

		// We associate the Graphical Model to the Physical Body
		q3BodyDef def = bullet->GetBodyDef();
		def.linearVelocity = g2q(50.0f * camLookAt);
		q3Body * body = mPhysics->CreateBody(def);
		q3BoxDef box = bullet->GetBoxDef();
		body->AddBox(box);
		bullet->SetBody(body);
	}
}

void World::Draw()
{
	Renderer::BeginFrame();
	
	// Set shader to use
	glUseProgram(Renderer::GetShaderProgramID());

	// This looks for the MVP Uniform variable in the Vertex Program
	GLuint ViewMatrixID = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewTransform");
	GLuint ProjMatrixID = glGetUniformLocation(Renderer::GetShaderProgramID(), "ProjectionTransform");

	// Send the view projection constants to the shader
	mat4 V = mCamera[mCurrentCamera]->GetViewMatrix();
	mat4 P = mCamera[mCurrentCamera]->GetProjectionMatrix();
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &P[0][0]);

	// Draw models
	for (vector<Model*>::iterator it = mModel.begin(); it < mModel.end(); ++it)
	{
		(*it)->Draw();
	}

	// Draw Path Lines
	
	// Set Shader for path lines
	unsigned int prevShader = Renderer::GetCurrentShader();
	Renderer::SetShader(SHADER_PATH_LINES);
	glUseProgram(Renderer::GetShaderProgramID());

	// Send the view projection constants to the shader
	ViewMatrixID = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewTransform");
	ProjMatrixID = glGetUniformLocation(Renderer::GetShaderProgramID(), "ProjectionTransform");
	glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &V[0][0]);
	glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &P[0][0]);

	for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
	{
		mat4 V = mCamera[mCurrentCamera]->GetViewMatrix();
		mat4 P = mCamera[mCurrentCamera]->GetProjectionMatrix();
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &P[0][0]);

		(*it)->Draw();
	}

	for (vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
	{
		mat4 V = mCamera[mCurrentCamera]->GetViewMatrix();
		mat4 P = mCamera[mCurrentCamera]->GetProjectionMatrix();
		glUniformMatrix4fv(ViewMatrixID, 1, GL_FALSE, &V[0][0]);
		glUniformMatrix4fv(ProjMatrixID, 1, GL_FALSE, &P[0][0]);

		(*it)->Draw();
	}

    Renderer::CheckForErrors();
    
    // Draw Billboards
    mpBillboardList->Draw();

	//ADD THIS
	PhysicsCamera * cam = (PhysicsCamera*)mCamera[0]; // Bad and dangerous but oh well
	vec3 camLookAt = cam->GetLookAt();
	vec3 camPos = cam->GetPos();
	//Update cloud position to camera position
	for (int i = 0; i < mModel.size(); i++){
		if (mModel[i]->GetName() == "cloud"){
			//cout << "update cloud position" << endl;
			mModel[i]->SetPosition(camPos);//does nothing...
		}
	}



	// Restore previous shader
	Renderer::SetShader((ShaderType) prevShader);

	Renderer::EndFrame();
}

void World::LoadScene(const char * scene_path)
{
	// Using case-insensitive strings and streams for easier parsing
	ci_ifstream input;
	input.open(scene_path, ios::in);

	// Invalid file
	if(input.fail() )
	{	 
		fprintf(stderr, "Error loading file: %s\n", scene_path);
		getchar();
		exit(-1);
	}

	ci_string item;
	while( std::getline( input, item, '[' ) )   
	{
        ci_istringstream iss( item );

		ci_string result;
		if( std::getline( iss, result, ']') )
		{
			if (result == "object")
			{
				Lib3dsModel * object = new Lib3dsModel();
				object->Load(iss);
				object->LoadModel();
				mModel.push_back(object);

				if (object->hasPhysics()){
					// We associate the Graphical Model to the Physical Body
					q3BodyDef def = object->GetBodyDef();
					q3Body * body = mPhysics->CreateBody(def);
					body->AddBox(object->GetBoxDef());
					object->SetBody(body);
				}
			}
			else if( result == "cube" )
			{
				// Box attributes
				CubeModel* cube = new CubeModel();
				cube->Load(iss);
				mModel.push_back(cube);

				if (cube->hasPhysics()){
					// We associate the Graphical Model to the Physical Body
					q3BodyDef def = cube->GetBodyDef();
					q3Body * body = mPhysics->CreateBody(def);
					body->AddBox(cube->GetBoxDef());
					cube->SetBody(body);
				}
			}
			else if( result == "mcube" )
			{
				// Box attributes
				MazeCube* cube = new MazeCube();
				cube->Load(iss);
				mModel.push_back(cube);
				cube->setScene(scene_path);

				if (cube->hasPhysics()){
					// We associate the Graphical Model to the Physical Body
					q3BodyDef def = cube->GetBodyDef();
					q3Body * body = mPhysics->CreateBody(def);
					body->AddBox(cube->GetBoxDef());
					cube->SetBody(body);
				}
			}
			else if( result == "mcube2" )
			{
				// Box attributes
				HalfMCube* rectangle = new HalfMCube();
				rectangle->Load(iss);
				mModel.push_back(rectangle);
				rectangle->setScene(scene_path);

				if (rectangle->hasPhysics()){
					// We associate the Graphical Model to the Physical Body
					q3BodyDef def = rectangle->GetBodyDef();
					q3Body * body = mPhysics->CreateBody(def);
					body->AddBox(rectangle->GetBoxDef());
					rectangle->SetBody(body);
				}
			}
			else if( result == "rect2" )
			{
				// Box attributes
				RectModel2* rectangle = new RectModel2();
				rectangle->Load(iss);
				mModel.push_back(rectangle);
				rectangle->setScene(scene_path);

				if (rectangle->hasPhysics()){
					// We associate the Graphical Model to the Physical Body
					q3BodyDef def = rectangle->GetBodyDef();
					q3Body * body = mPhysics->CreateBody(def);
					body->AddBox(rectangle->GetBoxDef());
					rectangle->SetBody(body);
				}
			}
            else if( result == "sphere" )
            {
                SphereModel* sphere = new SphereModel();
                sphere->Load(iss);
                mModel.push_back(sphere);
            }
			else if ( result == "animationkey" )
			{
				AnimationKey* key = new AnimationKey();
				key->Load(iss);
				mAnimationKey.push_back(key);
			}
			else if (result == "animation")
			{
				Animation* anim = new Animation();
				anim->Load(iss);
				mAnimation.push_back(anim);
			}
			else if ( result.empty() == false && result[0] == '#')
			{
				// this is a comment line
			}
			else
			{

				fprintf(stderr, "Error loading scene file... %s !", result.c_str());
				getchar();
				exit(-1);
			}
	    }
	}
	input.close();

	// Set Animation vertex buffers
	for (vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
	{
		// Draw model
		(*it)->CreateVertexBuffer();
	}
}

Animation* World::FindAnimation(ci_string animName)
{
    for(std::vector<Animation*>::iterator it = mAnimation.begin(); it < mAnimation.end(); ++it)
    {
        if((*it)->GetName() == animName)
        {
            return *it;
        }
    }
    return nullptr;
}

AnimationKey* World::FindAnimationKey(ci_string keyName)
{
    for(std::vector<AnimationKey*>::iterator it = mAnimationKey.begin(); it < mAnimationKey.end(); ++it)
    {
        if((*it)->GetName() == keyName)
        {
            return *it;
        }
    }
    return nullptr;
}

const Camera* World::GetCurrentCamera() const
{
     return mCamera[mCurrentCamera];
}

void World::AddBillboard(Billboard* b)
{
    mpBillboardList->AddBillboard(b);
}

void World::RemoveBillboard(Billboard* b)
{
    mpBillboardList->RemoveBillboard(b);
}

void World::AddParticleSystem(ParticleSystem* particleSystem)
{
    mParticleSystemList.push_back(particleSystem);
}

void World::RemoveParticleSystem(ParticleSystem* particleSystem)
{
    vector<ParticleSystem*>::iterator it = std::find(mParticleSystemList.begin(), mParticleSystemList.end(), particleSystem);
    mParticleSystemList.erase(it);
}

void World::AddModel(Model * model, q3BodyDef * body, q3BoxDef * box){
	mModelAddQueue.push_back(  std::make_tuple( model, body, box )  );
}

void World::AddAllQueuedModels(){
	for (
		auto it = mModelAddQueue.begin(), end = mModelAddQueue.end();
		it != end; ++it)
	{
		auto model = std::get<0>(*it);
		auto body = std::get<1>(*it);
		auto box = std::get<2>(*it);

		mModel.push_back(model);

		PhysicalModel * physModel = dynamic_cast<PhysicalModel*> (model);

		if (physModel){
			q3BoxDef  boxDef  = box ? *box : physModel->GetBoxDef();
			q3BodyDef bodyDef = body ? *body : physModel->GetBodyDef();
			q3Body * object = mPhysics->CreateBody(bodyDef);
			object->AddBox(boxDef);
			physModel->SetBody(object);
		}
		if (body) delete body;
		if (box)  delete box;
	}
	mModelAddQueue.clear();
}

void World::RemoveModel(Model * model){
		mModelRemovalQueue.push_back(model);
}

void World::RemoveAllQueuedModels(){

	for (auto it = mModelRemovalQueue.begin(), end = mModelRemovalQueue.end(); it != end; ++it){
		auto erase_it = std::find(mModel.begin(), mModel.end(), *it);
		if (erase_it != mModel.end()) {
			mModel.erase(erase_it);

			PhysicalModel * physModel = dynamic_cast<PhysicalModel *>(*it);

			if (physModel){
				q3Body * body = physModel->GetBody();

				if (body) mPhysics->RemoveBody(body);
			}
			delete *it;
		}
		else{
			std::cout << "Tried to erase non-existant model ...." << std::endl;
		}
	}

	mModelRemovalQueue.clear();
}

//ADD THIS
void World::Teleport(){
	PhysicsCamera * cam = (PhysicsCamera*)mCamera[0]; // Bad and dangerous but oh well
	vec3 camLookAt = cam->GetLookAt();
	vec3 camPos = cam->GetPos();
	vec3 teleportPos;

	for (int i = 0; i < mModel.size(); i++){
		if (mModel[i]->GetName() == "Teleporter"){
			teleportPos = mModel[i]->GetPosition();

			if ((camPos.x >= teleportPos.x - 2 && camPos.x <= teleportPos.x + 2)
				&& (camPos.z >= teleportPos.z - 2 && camPos.z <= teleportPos.z + 2)){
				for (int i = 0; i < mModel.size(); i++){//Empty current scene

					//keep cloud after teleporting
					if (mModel[i]->GetName() != "cloud"){
						RemoveModel(mModel[i]);
					}

				}
				LoadScene("../Assets/Scenes/AnimatedSceneWithParticles.scene");
			}
			return;

		}
	}
}


//ADD THIS
void World::Rain(float dt){
	const float COOLDOWN = 0.25;
	static float time = 0;
	time += dt;

	PhysicsCamera * cam = (PhysicsCamera*)mCamera[0]; // Bad and dangerous but oh well

	vec3 camLookAt = normalize(cam->GetLookAt());
	vec3 camPos = cam->GetPos();

	if (time > COOLDOWN){


		time = 0;
		if (rainSwitch == false){
			CubeModel * cloud = new CubeModel();
			cloud->SetName("cloud");
			cloud->SetPosition(vec3(camPos.x, camPos.y+1, camPos.z-1)); //initial cloud position near camera
			cloud->SetScaling(vec3(0));
			AddModel(cloud);

			ParticleDescriptor * desc = new ParticleDescriptor();
			desc->SetRainDescriptor();
			ParticleEmitter * emitter = new ParticleEmitter(vec3(0), cloud);
			ParticleSystem * rain = new ParticleSystem(emitter, desc);


			std::cout << "Rain On" << std::endl;
			AddParticleSystem(rain);
			rainSwitch = true;
		}
		else{
			for (int i = 0; i < mModel.size(); i++){
				if (mModel[i]->GetName() == "cloud"){
					RemoveParticleSystem( mParticleSystemList[mParticleSystemList.size() - 1]);
					RemoveModel(mModel[i]);
				}
			}
			rainSwitch = false;
			std::cout << "Rain Off" << std::endl;

		}


	}
	else{
		return;
	}
}
