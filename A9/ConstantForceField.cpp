#include "stdafx.h"
#include "ConstantForceField.h"


ConstantForceField::ConstantForceField (Vec3f force)
{
	this->force = force;
}

Vec3f ConstantForceField::getAcceleration (const Vec3f &position, float mass, float t)
{
	//常量力场
	force = force * (1 / mass);
	return force;
}
