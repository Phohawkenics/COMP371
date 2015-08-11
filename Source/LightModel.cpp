#include "LightModel.h"

using namespace glm;

LightModel* LightModel::instance;

LightModel::LightModel():
lightKc(0.0f),
lightKl(0.0f),
lightKq(1.0f) 
{
	instance = this;

	LightSources.push_back(
		LightSource("Test1", AMBIENT, vec4(0.0f, 2.0f, 0.0f, 1.0f))
		);
	LightSources.push_back(
		LightSource("Test2", AMBIENT, vec4(-20.0f, 10.0f, 30.0f, 1.0f))
		);
	LightSources.push_back(
		LightSource("Test3", AMBIENT, vec4(0.0f, 0.0f, 0.0f, 0.0f))
		);

}

LightModel::~LightModel()
{
	
}