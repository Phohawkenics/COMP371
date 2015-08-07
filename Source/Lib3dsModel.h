//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include <lib3ds.h>
#include <string>
#include <set>

#include "PhysicalModel.h"

// http://www.turbosquid.com/search/index.cfm?keyword=&max_price=0&min_price=0&file_type=114

class Lib3dsModel : public PhysicalModel
{
public:
	Lib3dsModel();
	virtual ~Lib3dsModel();

	virtual void Update(float dt);
	virtual void Draw();

	void LoadModel();

	virtual q3BoxDef  GetBoxDef();
	virtual q3BodyDef GetBodyDef();

protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:
	// The vertex format could be different for different types of models
	struct Vertex
	{
		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
	};

	void RenderMesh(Lib3dsMesh * mesh, Vertex * vertexBuffer, int & total_vert_i);
	void RenderFace(
		Lib3dsFace & face,
		float(*vertices)[3],
		float (*normal) [3],
		Vertex * vertexBuffer,
		int & total_vert_i);

	unsigned int mVertexArrayID;
	unsigned int mVertexBufferID;

	unsigned int mNVertices;

	std::string mFileName;
	std::set<int> mMeshes;
};
