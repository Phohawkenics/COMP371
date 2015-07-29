//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 15/7/15.
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "Billboard.h"
#include "Renderer.h"
#include "World.h"
#include "Camera.h"
#include <GL/glew.h>

using namespace std;
using namespace glm;


// This is used for sorting the list of Billboards
bool CompareBillboardAlongZ::operator()(const Billboard* a, const Billboard* b)
{
    mat4 viewMatrix = World::GetInstance()->GetCurrentCamera()->GetViewMatrix();
    return ((viewMatrix*vec4(a->position, 1.0f)).z < (viewMatrix*vec4(b->position, 1.0f)).z);
}


BillboardList::BillboardList(unsigned int maxNumBillboards, int textureID)
: mTextureID(textureID), mMaxNumBillboards(maxNumBillboards)
{
    // Pre-allocate Vertex Buffer - 6 vertices by billboard (2 triangles)
    mVertexBuffer.resize(maxNumBillboards * 6);
    
    // Initialize vertices to white, with UVs taking all the texture
    for (int i=0; i<mVertexBuffer.size(); ++i)
    {
        // White vertex
        mVertexBuffer[i].color = vec4(1.0f, 1.0f, 1.0f, 1.0f);
        
        // Normal on Z axis
        mVertexBuffer[i].normal = vec3(0.0f, 0.0f, 1.0f);
        
        // Texture coordinates
        switch(i%6)
        {
            case 0: mVertexBuffer[i].textureCoordinate = vec2(0.0f, 1.0f); break;
            case 1: mVertexBuffer[i].textureCoordinate = vec2(0.0f, 0.0f); break;
            case 2: mVertexBuffer[i].textureCoordinate = vec2(1.0f, 1.0f); break;
            case 3: mVertexBuffer[i].textureCoordinate = vec2(1.0f, 1.0f); break;
            case 4: mVertexBuffer[i].textureCoordinate = vec2(0.0f, 0.0f); break;
            case 5: mVertexBuffer[i].textureCoordinate = vec2(1.0f, 0.0f); break;
        }
    }
    
    // Create a vertex array
    glGenVertexArrays(1, &mVertexArrayID);
    
    // Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
    // Note the vertex buffer will change over time, we use GL_DYNAMIC_DRAW
    glGenBuffers(1, &mVertexBufferID);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glBufferData(GL_ARRAY_BUFFER, mVertexBuffer.size() * sizeof(BillboardVertex), &mVertexBuffer[0], GL_DYNAMIC_DRAW);
    Renderer::CheckForErrors();

}

BillboardList::~BillboardList()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);

	mVertexBuffer.resize(0);
	mBillboardList.resize(0);
}

void BillboardList::AddBillboard(Billboard* b)
{
    mBillboardList.push_back(b);
    
    assert(mBillboardList.size() <= mVertexBuffer.size() / 6.0f);
}

void BillboardList::RemoveBillboard(Billboard* b)
{
    list<Billboard*>::iterator it = find(mBillboardList.begin(), mBillboardList.end(), b);
    assert((*it != nullptr));
    mBillboardList.remove(*it);
}

void BillboardList::Update(float dt)
{
    // Sort billboards according to their depth, the functor CompareBillboardAlongZ does the magic!
    CompareBillboardAlongZ comp;
    mBillboardList.sort(comp);
    
    // Set Data in Vertex Buffer
    unsigned long firstVertexIndex = 0;

    // Maybe the view matrix will be useful to align the billboards
    const Camera* cam = World::GetInstance()->GetCurrentCamera();
    mat4 viewMatrix = cam->GetViewMatrix();
    
    // @TODO 4 - Align billboards with Camera plane
    //
    // For each billboard, update each vertex position, color and normals
    // Currently, the only positions are set aligned with the X-Y plane
    for (list<Billboard*>::iterator it = mBillboardList.begin(); it != mBillboardList.end(); ++it)
    {
        const Billboard* b = *it;

        // ... The code below needs to be modified ...
        
        // First triangle
        // Top left
        mVertexBuffer[firstVertexIndex].position.x = b->position.x - 0.5f*b->size.x;
        mVertexBuffer[firstVertexIndex].position.y = b->position.y + 0.5f*b->size.y;
        mVertexBuffer[firstVertexIndex].position.z = b->position.z;
        
        // Bottom Left
        mVertexBuffer[firstVertexIndex + 1].position.x = b->position.x - 0.5f*b->size.x;
        mVertexBuffer[firstVertexIndex + 1].position.y = b->position.y - 0.5f*b->size.y;
        mVertexBuffer[firstVertexIndex + 1].position.z = b->position.z;
        
        // Top Right
        mVertexBuffer[firstVertexIndex + 2].position.x = b->position.x + 0.5f*b->size.x;
        mVertexBuffer[firstVertexIndex + 2].position.y = b->position.y + 0.5f*b->size.y;
        mVertexBuffer[firstVertexIndex + 2].position.z = b->position.z;
        
        // Second Triangle
        // Top Right
        mVertexBuffer[firstVertexIndex + 3].position.x = b->position.x + 0.5f*b->size.x;
        mVertexBuffer[firstVertexIndex + 3].position.y = b->position.y + 0.5f*b->size.y;
        mVertexBuffer[firstVertexIndex + 3].position.z = b->position.z;
        
        // Bottom Left
        mVertexBuffer[firstVertexIndex + 4].position.x = b->position.x - 0.5f*b->size.x;
        mVertexBuffer[firstVertexIndex + 4].position.y = b->position.y - 0.5f*b->size.y;
        mVertexBuffer[firstVertexIndex + 4].position.z = b->position.z;
        
        // Bottom Right
        mVertexBuffer[firstVertexIndex + 5].position.x = b->position.x + 0.5f*b->size.x;
        mVertexBuffer[firstVertexIndex + 5].position.y = b->position.y - 0.5f*b->size.y;
        mVertexBuffer[firstVertexIndex + 5].position.z = b->position.z;
        
        // ... The code above needs to be modified ...

        
        firstVertexIndex += 6;
    }
    
    Renderer::CheckForErrors();
    
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glBufferSubData(GL_ARRAY_BUFFER, 0, 6*sizeof(BillboardVertex)*mBillboardList.size(), (void*)&mVertexBuffer[0]);
}

void BillboardList::Draw()
{
    Renderer::CheckForErrors();

    
    // Set current shader to be the Textured Shader
    ShaderType oldShader = (ShaderType)Renderer::GetCurrentShader();
    
    Renderer::SetShader(SHADER_TEXTURED);
    glUseProgram(Renderer::GetShaderProgramID());

    Renderer::CheckForErrors();

    
    GLuint textureLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "mySamplerTexture");
    glActiveTexture(GL_TEXTURE0);

    Renderer::CheckForErrors();

    
    glBindTexture(GL_TEXTURE_2D, mTextureID);
    glUniform1i(textureLocation, 0);				// Set our Texture sampler to user Texture Unit 0

    
    Renderer::CheckForErrors();

    // This looks for the MVP Uniform variable in the Vertex Program
    GLuint VPMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "ViewProjectionTransform");
    
    // Send the view projection constants to the shader
    const Camera* currentCamera = World::GetInstance()->GetCurrentCamera();
    mat4 VP = currentCamera->GetViewProjectionMatrix();
    glUniformMatrix4fv(VPMatrixLocation, 1, GL_FALSE, &VP[0][0]);

    // Draw the Vertex Buffer
    // Note this draws a unit Cube
    // The Model View Projection transforms are computed in the Vertex Shader
    glBindVertexArray(mVertexArrayID);
    
    GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform");

    // Billboard position are all relative to the origin
    mat4 worldMatrix(1.0f);
    glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &worldMatrix[0][0]);
    
    // 1st attribute buffer : vertex Positions
    glEnableVertexAttribArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glVertexAttribPointer(0,				// attribute. No particular reason for 0, but must match the layout in the shader.
                          3,				// size
                          GL_FLOAT,		// type
                          GL_FALSE,		// normalized?
                          sizeof(BillboardVertex), // stride
                          (void*)0        // array buffer offset
                          );
    
    // 2nd attribute buffer : vertex normal
    glEnableVertexAttribArray(1);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glVertexAttribPointer(1,
                          3,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(BillboardVertex),
                          (void*)sizeof(vec3)    // Normal is Offseted by vec3 (see class Vertex)
                          );
    
    
    // 3rd attribute buffer : vertex color
    glEnableVertexAttribArray(2);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glVertexAttribPointer(2,
                          4,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(BillboardVertex),
                          (void*) (2* sizeof(vec3)) // Color is Offseted by 2 vec3 (see class Vertex)
                          );
    
    // 3rd attribute buffer : texture coordinates
    glEnableVertexAttribArray(3);
    glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
    glVertexAttribPointer(3,
                          2,
                          GL_FLOAT,
                          GL_FALSE,
                          sizeof(BillboardVertex),
                          (void*) (2* sizeof(vec3) + sizeof(vec4)) // texture coordinates are Offseted by 2 vec3 (see class Vertex) and a vec4
                          );
    
    
    // Draw the triangles !
    glDrawArrays(GL_TRIANGLES, 0, mBillboardList.size()*6); // 6 vertices by billboard
    
    glDisableVertexAttribArray(3);
    glDisableVertexAttribArray(2);
    glDisableVertexAttribArray(1);
    glDisableVertexAttribArray(0);
    
    Renderer::CheckForErrors();
    
    Renderer::SetShader(oldShader);
}
