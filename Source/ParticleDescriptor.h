//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 15/7/15.
//         with help from Jordan Rutty
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//

#pragma once

#include <glm/glm.hpp>
#include "ParticleSystem.h"

// You can add a lot more to this to create more pleasant effects
// Adding more random parameters will make the particle effects more interesting
class ParticleDescriptor
{
public:
    ParticleDescriptor();
    
    void SetFountainDescriptor();
    void SetFireDescriptor();
    
private:
    
    glm::vec3 velocity;                 // initial speed vector - units per second
    float velocityDeltaAngle;           // angle in degrees from velocity - you can use spherical coordinates to alter velocity
    glm::vec3 acceleration;             // units per second^2
    
    glm::vec2 initialSize;              // initial size of billboard in x and y
    glm::vec2 initialSizeDelta;
    
    glm::vec2 sizeGrowthVelocity;       // growth of the billboard in units per second
    
    glm::vec4 initialColor;                // initial particle color - linearly interpolate towards mid color
    glm::vec4 midColor;                 // mid color - constant during mid stage
    glm::vec4 endColor;                 // end color - interpolates from mid to end color during end stage
    
    float emissionRate;                 // number of particles per second emitted
    float fadeInTime;                   // time from initial to mid stage
    float fadeOutTime;                  // time from mid to end stage
    float totalLifetime;                // amount of time in seconds the particle will remain alive
    float totalLifetimeDelta;
    
    friend class ParticleSystem;
};
