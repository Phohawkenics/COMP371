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
		LightSource("GunFlash", AMBIENT, vec4(0.0f, 0.0f, 0.0f, 0.0f), vec3(0.0f, 0.0f, 10.0f), 0)
		);
	LightSources.push_back(
		LightSource("RedLight", AMBIENT, vec4(10.0f, 40.0f, 30.0f, 1.0f), vec3(15.0f, 0.0f, 0.0f),50)
		);
	LightSources.push_back(
		LightSource("DoubleSided", LAMP, vec4(5.0f, 1.0f, 10.0f, 1.0f), vec3(1.0f, 1.0f, 1.0f), 4)
		);
	LightSources.push_back(
		LightSource("GreenLight", AMBIENT,vec4(24.0f, 12.0f, 24.0f, 1.0f), vec3(0.0f, 3.0f, 0.0f), 100)
		);
}

LightModel::~LightModel()
{
	
}

void LightModel::varyingFlicker(int index) {
	// Get Random float between a range
	if (LightSources[index].mIntensity != 0) {
		float min = 1;
		float max = 5;
		float random = ((float)rand()) / (float)RAND_MAX;
		float range = max - min;
		random = (random*range) + min;

		LightSources[index].mIntensity = random;
	}
}

void LightModel::SwitchLight(int index) {
	if (LightSources[index].mStatus) {
		LightOffStorage.push_back(LightContainer(index, LightSources[index].mIntensity));
		LightSources[index].mStatus = 0;
		LightSources[index].mIntensity = 0;
	} else {
		for (int i = LightOffStorage.size() - 1; i >= 0; i --) {
			if (index == LightOffStorage.at(i).mIndex) {
				LightSources[index].mIntensity = LightOffStorage.at(i).mIntensity;
				LightSources[index].mStatus = 1;
				LightOffStorage.erase(LightOffStorage.begin() + i);
				break;
			}
		}
	}
}