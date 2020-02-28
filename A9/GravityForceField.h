#pragma once
#include "ForceField.h"
class GravityForceField :
	public ForceField
{
public:
	GravityForceField (Vec3f gravity);
	Vec3f getAcceleration (const Vec3f &position, float mass, float t);
private:
	Vec3f gravity;
};

