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
		LightSource("Test1", AMBIENT, vec4(0.0f, 10.0f, 0.0f, 1.0f), vec3(2.0f, 0.0f, 0.0f),50)
		);
	LightSources.push_back(
		LightSource("Test2", AMBIENT, vec4(-20.0f, 2.0f, 30.0f, 1.0f), vec3(0.0f, 0.0f, 10.0f), 30)
		);
	LightSources.push_back(
		LightSource("Test3", AMBIENT, vec4(10.0f, 1.0f, -30.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), 40)
		);

}

LightModel::~LightModel()
{
	
}

void LightModel::SwitchLight(int index) {
	LightSource currentLight = GetLightSource(index);
	if (currentLight.mStatus) {
		LightOffStorage.push_back(LightContainer(index, currentLight.mIntensity));
		currentLight.mStatus = 0;
		currentLight.mIntensity = 0;
	} else {
		for (int i = LightOffStorage.size() - 1; i >= 0; i --) {
			if (index == LightOffStorage.at(i).mIndex) {
				currentLight.mIntensity = LightOffStorage.at(i).mIntensity;
				currentLight.mStatus = 1;
				LightOffStorage.erase(LightOffStorage.begin() + i);
				break;
			}
		}
	}
}