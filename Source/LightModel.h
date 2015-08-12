#pragma once

#include "Model.h"
#include <GL/glew.h>
#include <string>

class LightModel
{
	static LightModel* instance;
	// Light Coefficients
	float lightKc;
	float lightKl;
	float lightKq;

	// Light Variables: mName, mStatus, mType, mIntensity, mColor, mPosition
	struct LightSource {
		std::string mName;
		bool mStatus;
		int mType;
		float mIntensity;
		glm::vec3 mColor;
		glm::vec4 mPosition;
		LightSource(std::string name, int type, glm::vec4 position,
			glm::vec3 color, float intensity) {
			mStatus = 1; // Default ON
			mColor = color; // Default white
			mIntensity = intensity; // Default
			mName = name;
			mType = type;
			mPosition = position;
		};
	};
	struct LightContainer {
		int mIndex;
		float mIntensity;
		LightContainer(int index, float intensity) {
			mIndex = index;
			mIntensity = intensity;
		}
	};
	std::vector<LightContainer> LightOffStorage;
	std::vector<LightSource> LightSources;
	
public:
	LightModel();
	~LightModel();

	static LightModel* GetInstance() { return instance; };

	// Light Coefficients
	float GetLightKc() { return lightKc; };
	float GetLightKl() { return lightKl; };
	float GetLightKq() { return lightKq; };
	const std::vector<LightSource> GetLightSources() { return LightSources; }; // If w = 1.0f, we have a point light
	const LightSource GetLightSource(int index) { return LightSources[index]; };
	const int GetLightType(int index) { return LightSources[index].mType; };

	void SetLightPosition(glm::vec4 newPosition, int index) { LightSources[index].mPosition = newPosition; };
	void SetLightColor(glm::vec3 newColor, int index) { LightSources[index].mColor = newColor; };
	void SwitchLight(int index);

	enum Types {AMBIENT, CONE};
};