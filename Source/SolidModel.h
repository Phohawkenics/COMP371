#pragma once

#include "Model.h"

#include <GL/glew.h>


class SolidModel :
	virtual public Model
{
public:
	SolidModel(GLenum mode, GLsizei size,
		float mMaterialAmbient = 0.2f,
		float mMaterialDiffuse = 0.8f,
		float mMaterialSpecular = 0.2f,
		float mMaterialSpecularExponent = 2
	);
	virtual ~SolidModel();

	virtual void Draw();

protected:
	// The vertex format could be different for different types of models
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
	};

	virtual bool ParseLine(const std::vector<ci_string> &token) = 0;

	bool mVisible;

	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;

	GLsizei mSize;
	GLenum  mMode;

	float mMaterialAmbient;
	float mMaterialDiffuse;
	float mMaterialSpecular;
	float mMaterialSpecularExponent;

};

