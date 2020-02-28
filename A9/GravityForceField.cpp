#include "stdafx.h"
#include "GravityForceField.h"


GravityForceField::GravityForceField (Vec3f gravity)
{
	this->gravity = gravity;
}

Vec3f GravityForceField::getAcceleration (const Vec3f &position, float mass, float t)
{
	Vec3f a = gravity * (1.0f / mass);
	return a;
}
