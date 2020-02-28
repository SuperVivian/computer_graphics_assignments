#include "stdafx.h"
#include "PerspectiveCamera.h"


PerspectiveCamera::PerspectiveCamera (Vec3f center, Vec3f &direction, Vec3f &up, float angle_radians)
{
	this->direction = direction;  this->direction.Normalize ();
	Vec3f::Cross3 (horizontal, this->direction, up);   horizontal.Normalize ();
	Vec3f::Cross3 (this->up, horizontal, this->direction);  this->up.Normalize ();

	this->center = center;
	this->angle_radians = angle_radians;
}


PerspectiveCamera::~PerspectiveCamera ()
{
}

Ray PerspectiveCamera::generateRay (Vec2f point)
{
	Vec3f origin = center;
	//映射到虚拟image上，然后确定dir
	float  k_size = tan (angle_radians / 2.0f);  if (k_size < 0)k_size = 0;
	Vec3f end = center + (-0.5 + point.x ())*horizontal*k_size
		+ (-0.5 + point.y ())*up*k_size+direction*0.5f	;
	Vec3f dir = end - center; 	dir.Normalize ();
	Ray ray (origin, dir);	
	return  ray;
}

float PerspectiveCamera::getTMin () const
{
	return 0.0f;
}