#include "PhysicalModel.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#include "q3_glm_conversions.h"

PhysicalModel::PhysicalModel()
: mBody(nullptr)
, mFriction(0.4f)
, mRestitution(0.2f)
, mDensity(1.0f)
{
}

bool PhysicalModel::ParseLine(const std::vector<ci_string> &token){
	if (token.empty())
	{
		return true;
	}
	else
	{
		if (token[0] == "name")
		{
			assert(token.size() > 2);
			assert(token[1] == "=");
			if (token[2] != "Teleporter"){
				mName = token[2];
			}
		}
		if (token[0] == "restitution"){
			assert(token[1] == "=");

			mRestitution = atof(token[2].c_str());

			return true;
		}
		if (token[0] == "friction"){
			assert(token[1] == "=");

			mFriction = atof(token[2].c_str());

			return true;
		}
		if (token[0] == "density"){
			assert(token[1] == "=");

			mDensity = atof(token[2].c_str());

			return true;
		}
		else{
			return Model::ParseLine(token);
		}
	}

}



PhysicalModel::~PhysicalModel()
{
}

glm::mat4 PhysicalModel::GetWorldMatrix() const{

	if (mBody) {
		q3Transform transform = mBody->GetTransform();

		glm::mat4 t = glm::translate(glm::mat4(1.0f), q2g(transform.position));
		glm::mat4 s = glm::scale(glm::mat4(1.0f), mScaling);
		glm::mat4 r = q2g_mat3_2_mat4(transform.rotation);

		if (mName == "\"couch\""){
			r *= glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(1,0,0));
			t *= glm::translate(glm::mat4(1.0f), glm::vec3(0,-1.75,0));
		}
		else if (mName == "\"table\""){
			r *= glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(1, 0, 0));
			t *= glm::translate(glm::mat4(1.0f), glm::vec3(0, -1, 0));
		}
		else if (mName == "\"bookshelf\""){
			r *= glm::rotate(glm::mat4(1.0f), -90.0f, glm::vec3(1, 0, 0));
			t *= glm::translate(glm::mat4(1.0f), glm::vec3(0, 0.75, 1));
		}
		return t * r * s;
	}
	else{
		return Model::GetWorldMatrix();
	}

}

// TODO, this makes no fkin sense
bool PhysicalModel::hasPhysics(){
	return mPhysicsType != None;
}

// body needs to be added in *world* coordinates

q3BodyDef PhysicalModel::GetBodyDef(){
	q3BodyDef def;

	switch (mPhysicsType){
	case Dynamic:
		def.bodyType = eDynamicBody;
		break;
	case Static:
		def.bodyType = eStaticBody;
		break;
	case Kinematic:
		def.bodyType = eKinematicBody;
		break;
	}

	def.axis = g2q(GetRotationAxis());			// Initial world transformation.
	def.angle = q3PI * (GetRotationAngle() / 180);				// Initial world transformation. Radians.
	def.position = g2q(GetPosition());		// Initial world transformation.
	def.angularVelocity.Set(0, 0, 0);
	def.linearVelocity.Set(0, 0, 0);
	def.userData = this;

	return def;
}




void PhysicalModel::SetBody(q3Body * body){
	mBody = body;
}