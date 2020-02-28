#pragma once
#include "ForceField.h"
class VerticalForceField :
	public ForceField
{
public:
	VerticalForceField (float magnitude);
	Vec3f getAcceleration (const Vec3f &position, float mass, float t);
private:
	float magnitude;
};

