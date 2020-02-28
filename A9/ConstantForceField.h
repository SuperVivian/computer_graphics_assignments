#pragma once
#include "ForceField.h"
class ConstantForceField :
	public ForceField
{
public:
	ConstantForceField (Vec3f);
	Vec3f getAcceleration (const Vec3f &position, float mass, float t);
private:
	Vec3f force;
};

