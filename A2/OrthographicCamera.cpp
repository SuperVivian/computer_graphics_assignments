#include "stdafx.h"
#include "OrthographicCamera.h"


OrthographicCamera::OrthographicCamera (Vec3f center, Vec3f direction, Vec3f up, float size)
{	
	this->direction = direction;  this->direction.Normalize ();
	Vec3f::Cross3 (horizontal, this->direction, up);   horizontal.Normalize ();
	Vec3f::Cross3 (this->up, horizontal, this->direction);  this->up.Normalize ();

	this->center = center;	
	this->imgSize = size;
}

OrthographicCamera::~OrthographicCamera ()
{
}

Ray OrthographicCamera::generateRay (Vec2f point)
{
	Vec3f origin=center+ (-0.5f+point.x ())*imgSize*horizontal+ (-0.5f+point.y ())*imgSize*up;
	Ray ray (origin,direction);
	return  ray;
}

float OrthographicCamera::getTMin () const
{
	return -FLT_MAX;
}

