//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 15/7/15.
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>
#include <vector>
#include <list>

// A billboard has a position and a size in world units, and a texture
struct Billboard
{
    glm::vec3 position;
    glm::vec2 size;
    glm::vec4 color;
};

struct CompareBillboardAlongZ{
    bool operator()(const Billboard *a, const Billboard *b);
};


// We should render billboards in the fewest amount of render calls possible
// Billboards are semi-transparent, so they need to be sorted and rendered from back to front
class BillboardList
{
public:
	BillboardList(unsigned int maxNumBillboards, int textureID,
		float mMaterialAmbient = 0.2f,
		float mMaterialDiffuse = 0.8f,
		float mMaterialSpecular = 0.0f,
		float mMaterialSpecularExponent = 2
		);
    ~BillboardList();
    
    void AddBillboard(Billboard* b);
    void RemoveBillboard(Billboard* b);
    
    void Update(float dt);
    void Draw();
    
private:
    // Each vertex on a billboard
    struct BillboardVertex
    {
        glm::vec3 position;
        glm::vec3 normal;
        glm::vec4 color;
        glm::vec2 textureCoordinate;
    };


	float mMaterialAmbient;
	float mMaterialDiffuse;
	float mMaterialSpecular;
	float mMaterialSpecularExponent;
    
    std::vector<BillboardVertex> mVertexBuffer;
    std::list<Billboard*> mBillboardList;
    
    int mTextureID;
    unsigned int mMaxNumBillboards;

    unsigned int mVertexArrayID;
    unsigned int mVertexBufferID;
};