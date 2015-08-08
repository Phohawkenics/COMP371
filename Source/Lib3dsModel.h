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
#include "SolidModel.h"

// http://www.turbosquid.com/search/index.cfm?keyword=&max_price=0&min_price=0&file_type=114

class Lib3dsModel : public SolidModel, public PhysicalModel
{
public:
	Lib3dsModel();
	virtual ~Lib3dsModel();

	virtual void Update(float dt);

	void LoadModel();

	virtual q3BoxDef  GetBoxDef();


protected:
	virtual bool ParseLine(const std::vector<ci_string> &token);

private:

	q3Vec3 GetExtents();

	void Reposition(Vertex * v);

	void RenderMesh(Lib3dsMesh * mesh, Lib3dsMaterial ** materials, Vertex * vertexBuffer, int & total_vert_i);
	void RenderFace(
		Lib3dsFace & face,
		Lib3dsMaterial ** materials,
		float(*vertices)[3],
		float (*normal) [3],
		Vertex * vertexBuffer,
		int & total_vert_i,
		glm::vec3 & color);

	unsigned int mNVertices;

	glm::vec3 mBBoxMin;
	glm::vec3 mBBoxMax;

	std::string mFileName;
	std::set<int> mMeshes;
};
