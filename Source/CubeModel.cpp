//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "CubeModel.h"
#include "Renderer.h"

#include "q3_glm_conversions.h"

#include "World.h"

// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>

#include <dynamics/q3Contact.h>


using namespace glm;

static float getRandomAround(float around, float delta){
	return around - delta + 2*delta*(static_cast <float> (rand()) / static_cast <float> (RAND_MAX));
}

CubeModel::CubeModel(vec3 size) : PhysicalModel(), SolidModel(GL_TRIANGLES, 36), mBreakable(false)
{
	// Create Vertex Buffer for all the verices of the Cube
	vec3 halfSize = size * 0.5f;
	
	float colorf = getRandomAround(0.25, 0.05);
	vec3 color(colorf, colorf, colorf);

	Vertex vertexBuffer[] = {  // position,                normal,                  color
								{ vec3(-halfSize.x,-halfSize.y,-halfSize.z), vec3(-1.0f, 0.0f, 0.0f), color }, //left - red
								{ vec3(-halfSize.x,-halfSize.y, halfSize.z), vec3(-1.0f, 0.0f, 0.0f), color },
								{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3(-1.0f, 0.0f, 0.0f), color },

								{ vec3(-halfSize.x,-halfSize.y,-halfSize.z), vec3(-1.0f, 0.0f, 0.0f), color },
								{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3(-1.0f, 0.0f, 0.0f), color },
								{ vec3(-halfSize.x, halfSize.y,-halfSize.z), vec3(-1.0f, 0.0f, 0.0f), color },

								{ vec3( halfSize.x, halfSize.y,-halfSize.z), vec3( 0.0f, 0.0f,-1.0f), color }, // far - blue
								{ vec3(-halfSize.x,-halfSize.y,-halfSize.z), vec3( 0.0f, 0.0f,-1.0f), color },
								{ vec3(-halfSize.x, halfSize.y,-halfSize.z), vec3( 0.0f, 0.0f,-1.0f), color },

								{ vec3( halfSize.x, halfSize.y,-halfSize.z), vec3( 0.0f, 0.0f,-1.0f), color },
								{ vec3( halfSize.x,-halfSize.y,-halfSize.z), vec3( 0.0f, 0.0f,-1.0f), color },
								{ vec3(-halfSize.x,-halfSize.y,-halfSize.z), vec3( 0.0f, 0.0f,-1.0f), color },

								{ vec3( halfSize.x,-halfSize.y, halfSize.z), vec3( 0.0f,-1.0f, 0.0f), color }, // bottom - turquoise
								{ vec3(-halfSize.x,-halfSize.y,-halfSize.z), vec3( 0.0f,-1.0f, 0.0f), color },
								{ vec3( halfSize.x,-halfSize.y,-halfSize.z), vec3( 0.0f,-1.0f, 0.0f), color },
																
								{ vec3( halfSize.x,-halfSize.y, halfSize.z), vec3( 0.0f,-1.0f, 0.0f), color },
								{ vec3(-halfSize.x,-halfSize.y, halfSize.z), vec3( 0.0f,-1.0f, 0.0f), color },
								{ vec3(-halfSize.x,-halfSize.y,-halfSize.z), vec3( 0.0f,-1.0f, 0.0f), color },

								{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3( 0.0f, 0.0f, 1.0f), color }, // near - green
								{ vec3(-halfSize.x,-halfSize.y, halfSize.z), vec3( 0.0f, 0.0f, 1.0f), color },
								{ vec3( halfSize.x,-halfSize.y, halfSize.z), vec3( 0.0f, 0.0f, 1.0f), color },

								{ vec3( halfSize.x, halfSize.y, halfSize.z), vec3( 0.0f, 0.0f, 1.0f), color },
								{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3( 0.0f, 0.0f, 1.0f), color },
								{ vec3( halfSize.x,-halfSize.y, halfSize.z), vec3( 0.0f, 0.0f, 1.0f), color },

								{ vec3( halfSize.x, halfSize.y, halfSize.z), vec3( 1.0f, 0.0f, 0.0f), color }, // right - purple
								{ vec3( halfSize.x,-halfSize.y,-halfSize.z), vec3( 1.0f, 0.0f, 0.0f), color },
								{ vec3( halfSize.x, halfSize.y,-halfSize.z), vec3( 1.0f, 0.0f, 0.0f), color },

								{ vec3( halfSize.x,-halfSize.y,-halfSize.z), vec3( 1.0f, 0.0f, 0.0f), color },
								{ vec3( halfSize.x, halfSize.y, halfSize.z), vec3( 1.0f, 0.0f, 0.0f), color },
								{ vec3( halfSize.x,-halfSize.y, halfSize.z), vec3( 1.0f, 0.0f, 0.0f), color },

								{ vec3( halfSize.x, halfSize.y, halfSize.z), vec3( 0.0f, 1.0f, 0.0f), color }, // top - yellow
								{ vec3( halfSize.x, halfSize.y,-halfSize.z), vec3( 0.0f, 1.0f, 0.0f), color },
								{ vec3(-halfSize.x, halfSize.y,-halfSize.z), vec3( 0.0f, 1.0f, 0.0f), color },

								{ vec3( halfSize.x, halfSize.y, halfSize.z), vec3( 0.0f, 1.0f, 0.0f), color },
								{ vec3(-halfSize.x, halfSize.y,-halfSize.z), vec3( 0.0f, 1.0f, 0.0f), color },
								{ vec3(-halfSize.x, halfSize.y, halfSize.z), vec3( 0.0f, 1.0f, 0.0f), color }
						};

	glBufferData(GL_ARRAY_BUFFER, sizeof(vertexBuffer), vertexBuffer, GL_STATIC_DRAW);
}

CubeModel::~CubeModel()
{
}

// TODO getBoxDef*S*
q3BoxDef CubeModel::GetBoxDef(){
	q3BoxDef def;

	q3Transform tx;
	q3Identity(tx);

	// Set the extents of the box
	def.Set(tx, g2q( GetScaling() ));

	def.SetDensity(mDensity);
	def.SetFriction(mFriction);
	def.SetRestitution(mRestitution);

	return def;
}

void CubeModel::Update(float dt)
{
	Model::Update(dt);
}

// TODO - check kinetic energy of other object and only break above
// some threshold
void CubeModel::handleBeginContact(const q3ContactConstraint *contact){
	//box->body->GetLinearVelocity() * box->body->GetMass();
	
	//auto eKineticA = contact->

	if (mBreakable){
#if 0
		q3Vec3 eKineticA = contact->bodyA->GetLinearVelocity() * contact->bodyA->GetMass();
		q3Vec3 eKineticB = contact->bodyB->GetLinearVelocity() * contact->bodyB->GetMass();

		q3Vec3 eKineticDelta = eKineticA - eKineticB;

		r32 explosion_power = q3Dot(eKineticDelta, eKineticDelta);
		const r32 threshold = 5000;
		/*
		std::cout << "eKineticA: " << std::endl
			<< "   " << eKineticA.x << ", " << eKineticA.y << ", " << eKineticA.z << std::endl
			<< "   " << eKineticB.x << ", " << eKineticB.y << ", " << eKineticB.z << std::endl
			<< "   power: " << explosion_power << std::endl;


		std::cout << "Breakable model contact" << std::endl;
		*/
		if (explosion_power < threshold){
			return;
		}
#endif

		// Queue this object's removal
		World::GetInstance()->RemoveModel(this);

		// break into 8 evenly sized pieces
		vec3 size = mScaling / 2.0f;
		vec3 pos = size / 2.0f;
		size *= 0.75; // add some space for gaps

//		bool sub_breakable = glm::dot(size, size) > 0.2;

		if (true/*sub_breakable*/){

			for (int x = 0; x < 2; ++x){
				for (int y = 0; y < 2; ++y){
					for (int z = 0; z < 2; ++z){
						CubeModel *shard = new CubeModel();
						
						q3Vec3 parentPos = mBody->GetTransform().position;
						
						shard->SetPosition(q2g(parentPos) +
							glm::vec3(
							pos.x * (x ? 1 : -1),
							pos.y * (y ? 1 : -1),
							pos.z * (z ? 1 : -1)
							));
						shard->SetScaling(size);
						shard->SetPhysicsType(Dynamic);
						shard->SetBreakable(false/*sub_breakable*/);

						auto body = new q3BodyDef(shard->GetBodyDef());
						auto box = new q3BoxDef(shard->GetBoxDef());

						body->linearVelocity = q3Vec3(
							float(rand()) / float(RAND_MAX),
							float(rand()) / float(RAND_MAX),
							float(rand()) / float(RAND_MAX)
						);

						auto transform = mBody->GetTransform();
						transform.position = q3Vec3( 0, 0, 0 );

						box->Set(transform, g2q(size));

						World::GetInstance()->AddModel(shard, body, box);
					}
				}
			}
		}

	}
}

void CubeModel::SetBreakable(bool breakable){
	mBreakable = breakable;
}

bool CubeModel::IsBreakable(){
	return mBreakable;
}


bool CubeModel::ParseLine(const std::vector<ci_string> &token)
{
	if (token.empty())
	{
		return true;
	}
	else if (token[0] == "breakable"){
		assert(token[1] == "=");
		assert(token[2] == "true" || token[2] == "false");

		std::cout << "breakable model created" << std::endl;

		mBreakable = token[2] == "true";

		return true;
	}
	else
	{
		return PhysicalModel::ParseLine(token) || SolidModel::ParseLine(token);
	}
}
