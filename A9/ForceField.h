#pragma once
#include"vectors.h"
class ForceField
{
public:
	ForceField ();
	~ForceField ();
	virtual Vec3f getAcceleration (const Vec3f &position, float mass, float t) = 0;
};

