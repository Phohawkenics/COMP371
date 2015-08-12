#include "SolidModel.h"
#include "Renderer.h"
#include "LightModel.h"
#include "World.h"


SolidModel::SolidModel(GLenum mode, GLsizei size, float materialAmbient,
	float materialDiffuse,
	float materialSpecular,
	float materialSpecularExponent)
:	mSize(size),
	mMode(mode),
	mMaterialAmbient(materialAmbient),
	mMaterialDiffuse(materialDiffuse),
	mMaterialSpecular(materialSpecular),
	mMaterialSpecularExponent(materialSpecularExponent),
	mVisible(true)
{
	// Create a vertex array
	glGenVertexArrays(1, &mVertexArrayID);

	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);

	//std::cout << "Solidmodel made" << std::endl;

}


SolidModel::~SolidModel()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}

void SolidModel::Draw()
{

	if (!mVisible) return;

	// Draw the Vertex Buffer
	// Note this draws a unit Cube
	// The Model View Projection transforms are computed in the Vertex Shader
	glBindVertexArray(mVertexArrayID);

	unsigned int programID = Renderer::GetShaderProgramID();

	// Get a handle for our Transformation Matrices uniform
	GLuint WorldMatrixID = glGetUniformLocation(programID, "WorldTransform");

	// Send our transformation to the currently bound shader, 
	// in the "World / View / Projection" uniform
	glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &GetWorldMatrix()[0][0]);

	// Get a handle for Light Attributes uniform
	GLuint LightPositionID1 = glGetUniformLocation(programID, "WorldLightPosition1");
	GLuint LightColorID1 = glGetUniformLocation(programID, "lightColor1");
	GLuint LightPowerID1 = glGetUniformLocation(programID, "lightPower1");
	GLuint LightTypeID1 = glGetUniformLocation(programID, "lightType1");

	GLuint LightPositionID2 = glGetUniformLocation(programID, "WorldLightPosition2");
	GLuint LightColorID2 = glGetUniformLocation(programID, "lightColor2");
	GLuint LightPowerID2 = glGetUniformLocation(programID, "lightPower2");
	GLuint LightTypeID2 = glGetUniformLocation(programID, "lightType2");

	GLuint LightPositionID3 = glGetUniformLocation(programID, "WorldLightPosition3");
	GLuint LightColorID3 = glGetUniformLocation(programID, "lightColor3");
	GLuint LightPowerID3 = glGetUniformLocation(programID, "lightPower3");
	GLuint LightTypeID3 = glGetUniformLocation(programID, "lightType3");

	GLuint LightPositionID4 = glGetUniformLocation(programID, "WorldLightPosition4");
	GLuint LightColorID4 = glGetUniformLocation(programID, "lightColor4");
	GLuint LightPowerID4 = glGetUniformLocation(programID, "lightPower4");
	GLuint LightTypeID4 = glGetUniformLocation(programID, "lightType4");

	GLuint LightAttenuationID = glGetUniformLocation(programID, "lightAttenuation");

	// Get a handle for Material Attributes uniform
	GLuint MaterialAmbientID = glGetUniformLocation(programID, "materialAmbient");
	GLuint MaterialDiffuseID = glGetUniformLocation(programID, "materialDiffuse");
	GLuint MaterialSpecularID = glGetUniformLocation(programID, "materialSpecular");
	GLuint MaterialExponentID = glGetUniformLocation(programID, "materialExponent");

	// Draw the Vertex Buffer
	// Note this draws a unit Sphere
	// The Model View Projection transforms are computed in the Vertex Shader

	// Set shader constants
	glUniform1f(MaterialAmbientID, mMaterialAmbient);
	glUniform1f(MaterialDiffuseID, mMaterialDiffuse);
	glUniform1f(MaterialSpecularID, mMaterialSpecular);
	glUniform1f(MaterialExponentID, mMaterialSpecularExponent);

	LightModel * lights = LightModel::GetInstance();

	auto lightPosition1 = lights->GetLightSource(0).mPosition;
	auto lightColor1 = lights->GetLightSource(0).mColor;
	glUniform4f(LightPositionID1, lightPosition1.x, lightPosition1.y, lightPosition1.z, lightPosition1.w);
	glUniform3f(LightColorID1, lightColor1.r, lightColor1.g, lightColor1.b);
	glUniform1f(LightPowerID1, lights->GetLightSource(0).mIntensity);
	glUniform1i(LightTypeID1, lights->GetLightSource(0).mType);

	auto lightPosition2 = lights->GetLightSource(1).mPosition;
	auto lightColor2 = lights->GetLightSource(1).mColor;
	glUniform4f(LightPositionID2, lightPosition2.x, lightPosition2.y, lightPosition2.z, lightPosition2.w);
	glUniform3f(LightColorID2, lightColor2.r, lightColor2.g, lightColor2.b);
	glUniform1f(LightPowerID2, lights->GetLightSource(1).mIntensity);
	glUniform1i(LightTypeID2, lights->GetLightSource(1).mType);

	auto lightPosition3 = lights->GetLightSource(2).mPosition;
	auto lightColor3 = lights->GetLightSource(2).mColor;
	glUniform4f(LightPositionID3, lightPosition3.x, lightPosition3.y, lightPosition3.z, lightPosition3.w);
	glUniform3f(LightColorID3, lightColor3.r, lightColor3.g, lightColor3.b);
	glUniform1f(LightPowerID3, lights->GetLightSource(2).mIntensity);
	glUniform1i(LightTypeID3, lights->GetLightSource(2).mType);

	auto lightPosition4 = lights->GetLightSource(3).mPosition;
	auto lightColor4 = lights->GetLightSource(3).mColor;
	glUniform4f(LightPositionID4, lightPosition4.x, lightPosition4.y, lightPosition4.z, lightPosition4.w);
	glUniform3f(LightColorID4, lightColor4.r, lightColor4.g, lightColor4.b);
	glUniform1f(LightPowerID4, lights->GetLightSource(3).mIntensity);
	glUniform1i(LightTypeID4, lights->GetLightSource(3).mType);

	glUniform3f(LightAttenuationID, lights->GetLightKc(), lights->GetLightKl(), lights->GetLightKq());

	// 1st attribute buffer : vertex Positions
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(0,				// attribute. No particular reason for 0, but must match the layout in the shader.
		3,				// size
		GL_FLOAT,		// type
		GL_FALSE,		// normalized?
		sizeof(Vertex), // stride
		(void*)0        // array buffer offset
		);

	// 2nd attribute buffer : vertex normal
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(1,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)sizeof(glm::vec3)    // Normal is Offseted by vec3 (see class Vertex)
		);


	// 3rd attribute buffer : vertex color
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(2,
		3,
		GL_FLOAT,
		GL_FALSE,
		sizeof(Vertex),
		(void*)(2 * sizeof(glm::vec3)) // Color is Offseted by 2 vec3 (see class Vertex)
		);

	//std::cout << "Drawing with mode " << mMode << " and size " << mSize << std::endl;

	/*
	int nVerts = mMode == GL_TRIANGLES ? 3 * mSize : mSize;

	Vertex * v = new Vertex[nVerts];

	glGetBufferSubData(mVertexBufferID, 0, nVerts * sizeof(Vertex), v);

	for (int i = 0; i < nVerts; ++i){
		std::cout
			<< v[i].position.x << ", "
			<< v[i].position.y << ", "
			<< v[i].position.z << std::endl;
	}
	*/

	// Draw the triangles !
	glDrawArrays(mMode, 0, mSize);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

bool SolidModel::ParseLine(const std::vector<ci_string> &token){

	if (token.empty())
	{
		return true;
	}
	else
	{
		if (token[0] == "visible"){
			assert(token[1] == "=");

			assert(token[2] == "true" || token[2] == "false");

			mVisible = token[2].c_str() == "true";

			return true;
		}
		else{
			Model::ParseLine(token);
		}
	}

}
