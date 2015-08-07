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


void PhysicalModel::SetBody(q3Body * body){
	mBody = body;
}