//
// COMP 371 Assignment Framework
//
// Created by Nicolas Bergeron on 15/7/15.
//         with help from Jordan Rutty
//
// Copyright (c) 2014-2015 Concordia University. All rights reserved.
//
#include "ParticleDescriptor.h"


using namespace glm;

ParticleDescriptor::ParticleDescriptor()
{
    SetFountainDescriptor();
}

//void ParticleDescriptor::SetFountainDescriptor()
//{
//    velocity = vec3(0.0f, 10.0f, 0.0f);
//    velocityDeltaAngle = 15;
//    
//    acceleration = vec3(0.0f, -9.8f, 0.0f);
//    
//    initialSize = vec2(0.1f, 0.1f);
//    initialSizeDelta = vec2(0.1f, 0.1f);
//    
//    sizeGrowthVelocity = vec2(0.7f, 0.7f);
//    
//    initialColor = vec4(0.5f, 0.5f, 0.5f, 0.0f);
//    midColor = vec4(0.8f, 0.8f, 1.0f, 1.0f);
//    endColor = vec4(0.5f, 0.5f, 0.5f, 0.0f);
//
//    emissionRate = 50.0f;
//    fadeInTime = 0.2f;
//    fadeOutTime = 0.7f;
//    totalLifetime = 2.0f;
//    totalLifetimeDelta = 0.3f;
//}
//
//void ParticleDescriptor::SetFireDescriptor()
//{
//    velocity = vec3(0.0f, 0.1f, 0.0f);
//    velocityDeltaAngle = 4.0f;
//    
//    acceleration = vec3(0.0f, 0.0f, 0.0f);
//    
//    initialSize = vec2(0.25f, 0.25f);
//    initialSizeDelta = vec2(0.02f, 0.02f);
//    
//    sizeGrowthVelocity = vec2(0.2f, 0.2f);
//    
//    initialColor = vec4(1.0f, 0.0f, 0.0f, 0.1f);
//    midColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
//    endColor = vec4(0.0f, 0.0f, 0.8f, 0.0f);
//
//    emissionRate = 25.0f;
//    fadeInTime = 0.5f;
//    fadeOutTime = 1.5f;
//    totalLifetime = 2.5f;
//    totalLifetimeDelta = 0.0f;
//}
void ParticleDescriptor::SetFountainDescriptor()
{
	velocity = vec3(0.0f, 17.0f, 0.0f);
	velocityDeltaAngle = 15;

	acceleration = vec3(0.0f, -7.0f, 0.0f);

	initialSize = vec2(1.4f, 1.4f);
	initialSizeDelta = vec2(0.1f, 0.1f);
	sizeGrowthVelocity = vec2(1.4f, 1.4f);

	initialColor = vec4(1.0f, 1.0f, 1.0f, 0.0f);
	midColor = vec4(0.9f, 0.9f, 1, 1.0f);
	endColor = vec4(0.5f, 0.6f, 1.0f, 0.0f);

	emissionRate = 25.0f;
	fadeInTime = 0.2f;
	fadeOutTime = 4.5f;
	totalLifetime = 5.0f;
	totalLifetimeDelta = 0.3f;
}

void ParticleDescriptor::SetFireDescriptor()
{
	velocity = vec3(0.0f, 2.4f, 0.0f);
	velocityDeltaAngle = 10.0f;

	acceleration = vec3(0, 0, 0);

	initialSize = vec2(1.0f, 1.0f);
	initialSizeDelta = vec2(0.02f, 0.02f);

	sizeGrowthVelocity = vec2(-1.4f, -1.4f);

	initialColor = vec4(1.0f, 0.0f, 0.0f, 0.2f);
	midColor = vec4(1.0f, 1.0f, 0.0f, 1.0f);
	endColor = vec4(0.5f, 0.5f, 0.8f, 0.0f);

	emissionRate = 25.0f;
	fadeInTime = 0.5f;
	fadeOutTime = 0.2f;
	totalLifetime = 0.7f;
	totalLifetimeDelta = 0.0f;
}

//ADD THIS
void ParticleDescriptor::SetTeleportDescriptor()
{
	velocity = vec3(0.0f, 5.0f, 0.0f);
	velocityDeltaAngle = 15;

	acceleration = vec3(0.0f, 0.1f, 0.0f);

	initialSize = vec2(0.01f, 1.4f);
	initialSizeDelta = vec2(0.1f, 0.1f);
	sizeGrowthVelocity = vec2(0);

	initialColor = vec4(0.5f, 1.0f, 0.0f, 1.0f);
	midColor = vec4(0.25f, 1.0f, 0.0f, 1.0f);
	endColor = vec4(0.75f, 1.0f, 0.0f, 1.0f);

	emissionRate = 25.0f;
	fadeInTime = 0.2f;
	fadeOutTime = 3.5f;
	totalLifetime = 4.0f;
	totalLifetimeDelta = 0.3f;
}

//ADD THIS
void ParticleDescriptor::SetRainDescriptor()
{
	velocity = vec3(0.0f, 1.0f, 0.0f);
	velocityDeltaAngle = 90;

	acceleration = vec3(0.0f, -7.0f, 0.0f);

	initialSize = vec2(0.01f, 0.5f);
	initialSizeDelta = vec2(0);
	sizeGrowthVelocity = vec2(0);

	initialColor = vec4(0.0f, 0.0f, 1.0f, 0.0f);
	midColor = vec4(0.0f, 1.0f, 1.0f, 1.0f);
	endColor = vec4(0.0f, 0.0f, 1.0f, 0.0f);

	emissionRate = 50.0f;
	fadeInTime = 0.2f;
	fadeOutTime = 4.5f;
	totalLifetime = 5.0f;
	totalLifetimeDelta = 0.3f;
}