#pragma once
#include "ForceField.h"
class RadialForceField :
	public ForceField
{
public:
	RadialForceField (float);
	Vec3f getAcceleration (const Vec3f &position, float mass, float t);
private:
	float magnitude;
};

