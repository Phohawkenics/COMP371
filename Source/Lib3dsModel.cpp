//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#include "Lib3dsModel.h"
#include "Renderer.h"

#include "q3_glm_conversions.h"

// Include GLEW - OpenGL Extension Wrangler
#include <GL/glew.h>

#include <lib3ds.h>

#include <memory>

using namespace glm;

Lib3dsModel::Lib3dsModel() : PhysicalModel()
{
}

Lib3dsModel::~Lib3dsModel()
{
	// Free the GPU from the Vertex Buffer
	glDeleteBuffers(1, &mVertexBufferID);
	glDeleteVertexArrays(1, &mVertexArrayID);
}

q3BodyDef Lib3dsModel::GetBodyDef(){
	q3BodyDef def;

	if (GetName().c_str()[0] != '_'){
		def.bodyType = eDynamicBody;
	}

	def.axis     = g2q(GetRotationAxis());			// Initial world transformation.
	def.angle    = q3PI * (GetRotationAngle()/180);				// Initial world transformation. Radians.
	def.position = g2q(GetPosition());		// Initial world transformation.
		
	def.angularVelocity.Set(0, 0, 0);
	def.linearVelocity.Set(0, 0, 0);
	
	return def;
}

// TODO getBoxDef*S*
q3BoxDef Lib3dsModel::GetBoxDef(){
	q3BoxDef def;

	if (GetName().c_str()[0] == '_'){
		def.SetRestitution(0);
	}

	q3Transform tx;
	q3Identity(tx);

	// Set the extents of the box
	def.Set(tx, g2q( GetScaling() ));

	return def;
}

void Lib3dsModel::Update(float dt)
{
	// If you are curious, un-comment this line to have spinning cubes!
	// That will only work if your world transform is correct...
	// mRotationAngleInDegrees += 90 * dt; // spins by 90 degrees per second

	Model::Update(dt);
}

void Lib3dsModel::Draw()
{

	// Draw the Vertex Buffer
	// Note this draws a unit Cube
	// The Model View Projection transforms are computed in the Vertex Shader
	glBindVertexArray(mVertexArrayID);

	GLuint WorldMatrixLocation = glGetUniformLocation(Renderer::GetShaderProgramID(), "WorldTransform"); 
	glUniformMatrix4fv(WorldMatrixLocation, 1, GL_FALSE, &GetWorldMatrix()[0][0]);
	
	// 1st attribute buffer : vertex Positions
	glEnableVertexAttribArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(	0,				// attribute. No particular reason for 0, but must match the layout in the shader.
							3,				// size
							GL_FLOAT,		// type
							GL_FALSE,		// normalized?
							sizeof(Vertex), // stride
							(void*)0        // array buffer offset
						);

	// 2nd attribute buffer : vertex normal
	glEnableVertexAttribArray(1);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(	1,
							3,
							GL_FLOAT,
							GL_FALSE,
							sizeof(Vertex),
							(void*)sizeof(vec3)    // Normal is Offseted by vec3 (see class Vertex)
						);


	// 3rd attribute buffer : vertex color
	glEnableVertexAttribArray(2);
	glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
	glVertexAttribPointer(	2,
							3,
							GL_FLOAT,
							GL_FALSE,
							sizeof(Vertex),
							(void*) (2* sizeof(vec3)) // Color is Offseted by 2 vec3 (see class Vertex)
						);

	// Draw the triangles !
	glDrawArrays(GL_TRIANGLES, 0, mNVertices);

	glDisableVertexAttribArray(2);
	glDisableVertexAttribArray(1);
	glDisableVertexAttribArray(0);
}

bool Lib3dsModel::ParseLine(const std::vector<ci_string> &token)
{
	if (token.empty())
	{
		return true;
	}
	else
	{
		if (token[0] == "3dsfile"){
			assert(token[1] == "=");

			mFileName = token[2].c_str();
			
			return true;
		}
		else if (token[0] == "meshes"){
			assert(token[1] == "=");

			for (int i = 2; i < token.size(); ++i){
				mMeshes.insert(std::stoi(token[i].c_str()));
			}

			return true;
		}
		else{
			return PhysicalModel::ParseLine(token);
		}
	}
}

void Lib3dsModel::LoadModel(){
	std::cout << "Loading 3DS object from file: '" << mFileName << "' ";

	Lib3dsFile * f =  lib3ds_file_open(mFileName.c_str());

	if ( ! f ){
		std::cout << "failed. :(" << std::endl;
		assert(!"Loading file failed");
	}
	else{
		std::cout
			<< "Successful!" << std::endl
			<< "  name: " << f->name << std::endl
			<< "  meshes: " << f->nmeshes << std::endl;

		int total_nvertices = 0;
		
		int nmeshes = f->nmeshes;
		
		// If no meshes were specified, then we will render them all
		if (mMeshes.size() == 0){
			for (int i = 0; i < nmeshes; ++i){
				mMeshes.insert(i);
			}
		}
		
		Lib3dsMesh ** meshes = f->meshes;
		
		// Count up all the vertices that we will need
		for (auto m = mMeshes.begin(); m != mMeshes.end(); ++m){
			total_nvertices += meshes[*m]->nfaces * 3;
		}

		std::cout << "  vertices: " << total_nvertices << std::endl;

		mNVertices = total_nvertices;

		std::unique_ptr<Vertex[]> vertexBuffer(new Vertex[total_nvertices]);

		std::cout << "  Processing Meshes" << std::endl;
				
		int total_vert_i = 0;

		for (auto m = mMeshes.begin(); m != mMeshes.end(); ++m){
			std::cout << "    mesh " << *m << std::endl;
			RenderMesh(meshes[*m], vertexBuffer.get(), total_vert_i);
		}

		assert(total_vert_i == total_nvertices);

		// Create a vertex array
		glGenVertexArrays(1, &mVertexArrayID);

		// Upload Vertex Buffer to the GPU, keep a reference to it (mVertexBufferID)
		glGenBuffers(1, &mVertexBufferID);
		glBindBuffer(GL_ARRAY_BUFFER, mVertexBufferID);
		glBufferData(GL_ARRAY_BUFFER, mNVertices, vertexBuffer.get(), GL_STATIC_DRAW);

		lib3ds_file_free(f);

	}
}

static void transform_vertices(float t[4][4], float(*vertices)[3], int nvertices){
	
	glm::mat4 T(
		t[0][0], t[0][1], t[0][2], t[0][3],
		t[1][0], t[1][1], t[1][2], t[1][3],
		t[2][0], t[2][1], t[2][2], t[2][3],
		t[3][0], t[3][1], t[3][2], t[3][3]);

	for (int i = 0; i < nvertices; ++i){
		glm::vec4 v(
			vertices[i][0],
			vertices[i][1],
			vertices[i][2],
			1);
		v = T*v;

		vertices[i][0] = v.x;
		vertices[i][1] = v.y;
		vertices[i][2] = v.z;
	}
}

void Lib3dsModel::RenderMesh(Lib3dsMesh * mesh, Vertex * vertexBuffer, int &total_vert_i){

	int nfaces = mesh->nfaces;
	float(*vertices)[3] = mesh->vertices;
	int nvertices = mesh->nvertices;
	Lib3dsFace * faces = mesh->faces;

	std::cout << "      name: " << mesh->name << std::endl;
	std::cout << "      faces: " << nfaces << std::endl;
	std::cout << "      transform:" << std::endl;

	for (int i = 0; i < 4; ++i){
		std::cout << "      ";
		for (int j = 0; j < 4; ++j){
			std::cout << "  " << mesh->matrix[i][j];
		}
		std::cout << std::endl;
	}
	std::cout << std::endl;

	//transform_vertices(mesh->matrix, vertices, nvertices);

	// Build normals
	std::unique_ptr<float[][3]> normals(new float[nfaces * 3][3]);

	assert(normals.get());
	lib3ds_mesh_calculate_vertex_normals(mesh, normals.get());

	float x = 0, y = 0, z = 0;
	float xm = INFINITY, ym = INFINITY, zm = INFINITY;
	float xM = -INFINITY, yM = -INFINITY, zM = -INFINITY;

	for (int face_i = 0; face_i < nfaces; ++face_i){

		Lib3dsFace face = faces[face_i];

		RenderFace(face, vertices, &normals[3*face_i], vertexBuffer, total_vert_i);
		x += vertices[face.index[0]][0];
		y += vertices[face.index[0]][1];
		z += vertices[face.index[0]][2];

		xm = min(xm, vertices[face.index[0]][0]);
		ym = min(ym, vertices[face.index[0]][1]);
		zm = min(zm, vertices[face.index[0]][2]);

		xM = max(xM, vertices[face.index[0]][0]);
		yM = max(yM, vertices[face.index[0]][1]);
		zM = max(zM, vertices[face.index[0]][2]);
	}

	std::cout << x / nfaces << " " << y / nfaces << " " << z / nfaces << std::endl;
	std::cout << xm << " " << ym << " " << zm << std::endl;
	std::cout << xM << " " << yM << " " << zM << std::endl;
}


void Lib3dsModel::RenderFace(
	Lib3dsFace & face,
	float(*vertices)[3],
	float(*normal)[3],
	Vertex * vertexBuffer,
	int &total_vert_i)
{
	//std::cout << "Face:" << std::endl;
	for (int i = 0; i < 3; ++i, ++total_vert_i){
		// TODO apply file-level transform to each vertex?
		vertexBuffer[total_vert_i].position =
			vec3(
			vertices[face.index[i]][0], // / 1000.0f,
			vertices[face.index[i]][1], // / 1000.0f,
			vertices[face.index[i]][2]);// / 1000.0f);

		/*std::cout << "    "
			<< vertices[face.index[i]][0] << " "
			<< vertices[face.index[i]][1] << " "
			<< vertices[face.index[i]][2] << std::endl;*/

		vertexBuffer[total_vert_i].normal =
			vec3(0, 0, 0);
//				normal[i][0],
//				normal[i][1],
//				normal[i][2]);

		vertexBuffer[total_vert_i].color =
			vec3(0.9f, 0.5f, 0.7f); // purplish
	}
}