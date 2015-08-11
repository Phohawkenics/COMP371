//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 8/7/14.
// Updated by Gary Chang on 14/1/15
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include "PhysicalModel.h"
#include "SolidModel.h"

class RectModel2 : public PhysicalModel, public SolidModel
{
public:
	RectModel2(glm::vec3 size = glm::vec3(1.0f, 1.0f, 1.0f));
    virtual ~RectModel2(void);

    virtual void Update(float dt);

	virtual q3BoxDef GetBoxDef();

	virtual void handleBeginContact(const q3ContactConstraint * contact);

	void SetBreakable(bool breakable);

	bool IsBreakable();
    
protected:
    virtual bool ParseLine(const std::vector<ci_string> &token);

	bool mBreakable;

};


