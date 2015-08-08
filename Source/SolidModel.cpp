#include "SolidModel.h"
#include "Renderer.h"

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
	mMaterialSpecularExponent(materialSpecularExponent)
{
	// Create a vertex array
	glGenVertexArrays(1, &mVertexArrayID);

	// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
	glGenBuffers(1, &mVertexBufferID);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);

	std::cout << "Solidmodel made" << std::endl;

}


SolidModel::~SolidModel()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}

void SolidModel::Draw()
{
	// Draw the Vertex Buffer
	// Note this draws a unit Cube
	// The Model View Projection transforms are computed in the Vertex Shader
	glBindVertexArray(mVertexArrayID);

	unsigned int programID = Renderer::GetShaderProgramID();

	// Get a handle for our Transformation Matrices uniform
	GLuint WorldMatrixID = glGetUniformLocation(programID, "WorldTransform");

	// Get a handle for Light Attributes uniform
	GLuint LightPositionID = glGetUniformLocation(programID, "WorldLightPosition");
	GLuint LightColorID = glGetUniformLocation(programID, "lightColor");
	GLuint LightAttenuationID = glGetUniformLocation(programID, "lightAttenuation");

	// Get a handle for Material Attributes uniform
	GLuint MaterialAmbientID = glGetUniformLocation(programID, "materialAmbient");
	GLuint MaterialDiffuseID = glGetUniformLocation(programID, "materialDiffuse");
	GLuint MaterialSpecularID = glGetUniformLocation(programID, "materialSpecular");
	GLuint MaterialExponentID = glGetUniformLocation(programID, "materialExponent");

	// Send our transformation to the currently bound shader, 
	// in the "World / View / Projection" uniform
	glUniformMatrix4fv(WorldMatrixID, 1, GL_FALSE, &GetWorldMatrix()[0][0]);


	// Draw the Vertex Buffer
	// Note this draws a unit Sphere
	// The Model View Projection transforms are computed in the Vertex Shader

	// Set shader constants
	glUniform1f(MaterialAmbientID, mMaterialAmbient);
	glUniform1f(MaterialDiffuseID, mMaterialDiffuse);
	glUniform1f(MaterialSpecularID, mMaterialSpecular);
	glUniform1f(MaterialExponentID, mMaterialSpecularExponent);

	World * // hip hop
		world = World::GetInstance();

	auto lightPosition = world->GetLightPosition();
	auto lightColor = world->GetLightColor();

	glUniform4f(LightPositionID, lightPosition.x, lightPosition.y, lightPosition.z, lightPosition.w);
	glUniform3f(LightColorID, lightColor.r, lightColor.g, lightColor.b);
	glUniform3f(LightAttenuationID, world->GetLightKc(), world->GetLightKl(), world->GetLightKq());


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
