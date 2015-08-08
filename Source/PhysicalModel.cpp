#include "PhysicalModel.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#include "q3_glm_conversions.h"

PhysicalModel::PhysicalModel() : mBody(nullptr)
{
}


PhysicalModel::~PhysicalModel()
{
}

glm::mat4 PhysicalModel::GetWorldMatrix() const{

	if (mBody) {
		q3Transform transform = mBody->GetTransform();

		glm::mat4 t = glm::translate(glm::mat4(1.0f), q2g(transform.position));
		glm::mat4 s = glm::scale(glm::mat4(1.0f), GetScaling());
		glm::mat4 r = q2g_mat3_2_mat4(transform.rotation);

		return t * r * s;
	}
	else{
		return Model::GetWorldMatrix();
	}

}

// body needs to be added in *world* coordinates

q3BodyDef PhysicalModel::GetBodyDef(){
	q3BodyDef def;

	if (GetName().c_str()[0] != '_'){
		def.bodyType = eDynamicBody;
	}

	def.axis = g2q(GetRotationAxis());			// Initial world transformation.
	def.angle = q3PI * (GetRotationAngle() / 180);				// Initial world transformation. Radians.
	def.position = g2q(GetPosition());		// Initial world transformation.
	def.angularVelocity.Set(0, 0, 0);
	def.linearVelocity.Set(0, 0, 0);

	return def;
}




void PhysicalModel::SetBody(q3Body * body){
	mBody = body;
}