#include "stdafx.h"
#include "VerticalForceField.h"


VerticalForceField::VerticalForceField (float magnitude) {
	this->magnitude = magnitude;
}
Vec3f VerticalForceField::getAcceleration (const Vec3f &position, float mass, float t) {
	//´¹Ö±Á¦³¡
	Vec3f force;
	force.Set (0, position.y ()*magnitude, 0);
	force /= mass;
	Vec3f a = force * -1.0f;

	return a;
}
