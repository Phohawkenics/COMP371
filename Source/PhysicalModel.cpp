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

static  void print(const glm::mat4 & m, const char * name){
	std::cout << "=== " << name << " ===" << std::endl;
	for (int i = 0; i < 4; ++i){
		for (int j = 0; j < 4; ++j){
			std::cout << m[j][i] << ' ';
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;
	std::cout << std::endl;

}

glm::mat4 PhysicalModel::GetWorldMatrix() const{

	if (mBody) {
		q3Transform transform = mBody->GetTransform();

		glm::mat4 t = glm::translate(glm::mat4(1.0f), q2g(transform.position));
		glm::mat4 s = glm::scale(glm::mat4(1.0f), GetScaling());
		glm::mat4 r = q2g_mat3_2_mat4(transform.rotation);

		//print(t, "Translation");
		//print(s, "Scale");
		//print(r, "Rotation");
		//print(t*r*s, "Physics World Matrix");
		//print(Model::GetWorldMatrix(), "Model World Matrix");

		return t * r * s;
	}
	else{
		return Model::GetWorldMatrix();
	}

}


void PhysicalModel::SetBody(q3Body * body){
	mBody = body;
}