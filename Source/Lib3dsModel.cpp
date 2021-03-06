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
#include <glm/gtc/matrix_transform.hpp>
#include <glm/common.hpp>

#include <lib3ds.h>

#include <memory>
#include <algorithm>
#include <limits.h>

using namespace glm;

float inf = std::numeric_limits<float>::infinity();

Lib3dsModel::Lib3dsModel()
	: SolidModel(GL_TRIANGLES, 0)
	, mBBoxMin(inf, inf, inf)
	, mBBoxMax(-inf, -inf, -inf)
{
}

Lib3dsModel::~Lib3dsModel()
{
}

void Lib3dsModel::Update(float dt){}

q3Vec3 Lib3dsModel::GetExtents(){

       // Object scale is {1,1,1} at this point

return q3Vec3(
		(mBBoxMax.x - mBBoxMin.x),
		(mBBoxMax.y - mBBoxMin.y),
		(mBBoxMax.z - mBBoxMin.z)
	);
}

q3BoxDef  Lib3dsModel::GetBoxDef(){
	q3BoxDef def;

	q3Transform tx;
	q3Identity(tx);

	def.Set(tx, GetExtents());

	def.SetDensity(mDensity);
	def.SetFriction(mFriction);
	def.SetRestitution(mRestitution);

	return def;
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
			PhysicalModel::ParseLine(token) || SolidModel::ParseLine(token);
		}
	}
}

/**
 *  Process the set of mesh indices generated from the config file.
 *
 *  If the set is empty, then fill it with every valid mesh index.
 *
 *  If the set is non-empty remove any invalid index from it.
 *
 *  Valid indices are those in the range [0, nmeshes)
 *
 */
static void preprocess_mesh_indices(int nmeshes, std::set<int> & meshIndices){
	// If no meshes were specified, then we will render them all
	if (meshIndices.size() == 0){
		for (int i = 0; i < nmeshes; ++i){
			meshIndices.insert(i);
		}
	}
	else{
		// Build the set of invalid indices to remove
		std::set<int> toRemove;
		std::copy_if(
			meshIndices.begin(), meshIndices.end(),
			std::inserter(toRemove, toRemove.end()),
			[nmeshes](int v){ return v >= nmeshes; });
		
		// Build the output as the set difference between the original set and the
		// elements to remove
		std::set<int> output;
		std::set_difference(
			meshIndices.begin(), meshIndices.end(),
			toRemove.begin(), toRemove.end(),
			std::inserter(output, output.end()));

		meshIndices = output;
	}
}

static int count_vertices(Lib3dsMesh ** meshes, const std::set<int> & meshIndices){

	int total_nvertices = 0;
	// Count up all the vertices that we will need
	for (auto m = meshIndices.begin(); m != meshIndices.end(); ++m){
		total_nvertices += meshes[*m]->nfaces * 3;
	}

	return total_nvertices;

}

static void prescale_vertices(float(*vertices)[3], int nvertices, const glm::vec3 & scale){
	for (int i = 0; i < nvertices; ++i){
		vertices[i][0] *= scale.x;
		vertices[i][1] *= scale.y;
		vertices[i][2] *= scale.z;
	}
}

void Lib3dsModel::LoadModel(){
	//std::cout << "Loading 3DS object from file: '" << mFileName << "' ";

	Lib3dsFile * f =  lib3ds_file_open(mFileName.c_str());

	if ( ! f ){
		std::cout << "Loading 3DS object from file: '" << mFileName << "' "
		 << "failed. :(" << std::endl;
		assert(!"Loading file failed");
	}
	else{
		/*std::cout
			<< "Successful!" << std::endl
			<< "  name: " << f->name << std::endl
			<< "  meshes: " << f->nmeshes << std::endl;
			*/
		int nmeshes = f->nmeshes;
		Lib3dsMesh ** meshes = f->meshes;

		int nmaterials = f->nmaterials;
		Lib3dsMaterial ** materials = f->materials;

		// Verify the set of mesh indices which we will draw
		preprocess_mesh_indices(nmeshes, mMeshes);
		// Count the number of vertices needed for the meshes
		mNVertices = count_vertices(meshes, mMeshes);
		
		//std::cout << "  vertices: " << mNVertices << std::endl;

		// Allocate a vertexBuffer big enough for all of those vertices
		std::unique_ptr<Vertex[]>  vertexBuffer (new Vertex[mNVertices]);
		
		// global index into the vertex buffer
		int total_vert_i = 0;

		// Write all of the vertices into the vertexBuffer
		for (auto m = mMeshes.begin(); m != mMeshes.end(); ++m){
			//std::cout << "    mesh " << *m << std::endl;
			
			RenderMesh(
				meshes[*m],         // the m-th mesh
				materials,
				vertexBuffer.get(), // the raw pointer to the vertex buffer
				total_vert_i);      // the current vertex index, by reference
		}
		// Assert that we exactly filled the vertxBuffer
		assert(total_vert_i == mNVertices);

		mSize = mNVertices;

		// Set unity scaling: at this point all the vertices have been pre-scaled
		SetScaling(vec3(1, 1, 1));

		// Center the object around the origin in modelspace
		Reposition(vertexBuffer.get());

		glBufferData(GL_ARRAY_BUFFER, mNVertices * sizeof(Vertex), vertexBuffer.get(), GL_STATIC_DRAW);

		lib3ds_file_free(f);

	}
}

void Lib3dsModel::Reposition(Vertex * v){
	

	// create an offset that will translate the object so
	// that it is centered 
	glm::vec3 offset = - 0.5f * (mBBoxMax + mBBoxMin);

	for (int i = 0; i < mNVertices; ++i){
		v[i].position += offset;
	}
}

void Lib3dsModel::RenderMesh(Lib3dsMesh * mesh, Lib3dsMaterial ** materials, Vertex * vertexBuffer, int &total_vert_i){

	// The array of vertices in the mesh
	float(*vertices)[3] = mesh->vertices;
	int nvertices = mesh->nvertices;

	// This simplifies life for calculating the bounding box
	// for physics later
	prescale_vertices(vertices, nvertices, GetScaling());

	// The array of faces in the mesh, each face refers to its 3 vertices by their
	// index in the vertex array
	Lib3dsFace * faces = mesh->faces;
	int nfaces = mesh->nfaces;

	//std::cout << "      name: " << mesh->name << std::endl;
	//std::cout << "      faces: " << nfaces << std::endl;
	//std::cout << "      transform:" << std::endl;

	// Allocate an arry to store the vertex normals
	std::unique_ptr<float[][3]> normals(new float[nfaces * 3][3]);
	// Ensure that we were able to allocate it
	assert(normals.get());
	// generate the normals for the mesh
	lib3ds_mesh_calculate_vertex_normals(mesh, normals.get());

#if 1
	// debugging - keep track of the min, max and average vertex position

	// debugging - alternate colors of faces
	glm::vec3 colors[] = {
		glm::vec3(1, 0, 0),
		glm::vec3(1, 1, 0),
		glm::vec3(0, 1, 0),
		glm::vec3(0, 1, 1),
		glm::vec3(0, 0, 1),
		glm::vec3(1, 0, 1),
	};
#endif
	for (int face_i = 0; face_i < nfaces; ++face_i){
		// Draw out each face
		RenderFace(
			faces[face_i],       // The face to draw
			materials,
			vertices,            // The mesh's vertex array
			&normals[3*face_i],  // An offset into the normal array for the current face
			vertexBuffer,        // The global vertex buffer
			total_vert_i,        // The global vertex buffer index by reference
			colors[face_i % 6]); // the color to use
#if 0

		Lib3dsFace face = faces[face_i];
		x += vertices[face.index[0]][0];
		y += vertices[face.index[0]][1];
		z += vertices[face.index[0]][2];

		xm = min(xm, vertices[face.index[0]][0]);
		ym = min(ym, vertices[face.index[0]][1]);
		zm = min(zm, vertices[face.index[0]][2]);

		xM = max(xM, vertices[face.index[0]][0]);
		yM = max(yM, vertices[face.index[0]][1]);
		zM = max(zM, vertices[face.index[0]][2]);
#endif
	}
#if 0
	std::cout << x / nfaces << " " << y / nfaces << " " << z / nfaces << std::endl;
	std::cout << xm << " " << ym << " " << zm << std::endl;
	std::cout << xM << " " << yM << " " << zM << std::endl;
#endif
}


void Lib3dsModel::RenderFace(
	Lib3dsFace & face,
	Lib3dsMaterial ** materials,
	float(*vertices)[3],
	float(*normal)[3],
	Vertex * vertexBuffer,
	int &total_vert_i,
	glm::vec3 & color)
{

	Lib3dsMaterial * material = materials[face.material];
	//std::cout << "Face:" << std::endl;
	for (int i = 0; i < 3; ++i, ++total_vert_i){
		float x = vertices[face.index[i]][0];
		float y = vertices[face.index[i]][1];
		float z = vertices[face.index[i]][2];

		mBBoxMax.x = max(mBBoxMax.x, x);
		mBBoxMin.x = min(mBBoxMin.x, x);

		mBBoxMax.y = max(mBBoxMax.y, y);
		mBBoxMin.y = min(mBBoxMin.y, y);

		mBBoxMax.z = max(mBBoxMax.z, z);
		mBBoxMin.z = min(mBBoxMin.z, z);

		vertexBuffer[total_vert_i].position =
			vec3(x, y, z);

		vertexBuffer[total_vert_i].normal =
			vec3(
				normal[i][0],
				normal[i][1],
				normal[i][2]);

		// TODO store all of the zillion parameters of the material
		// with each vertex
		vertexBuffer[total_vert_i].color =
			vec3(
			material->diffuse[0],
			material->diffuse[1],
			material->diffuse[2]);
	}
}