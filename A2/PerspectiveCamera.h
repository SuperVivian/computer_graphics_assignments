#pragma once
#include "Camera.h"
class PerspectiveCamera :
	public Camera
{
public:
    PerspectiveCamera (Vec3f center, Vec3f &direction, Vec3f &up, float angle_radians);
	~PerspectiveCamera ();
	virtual Ray generateRay (Vec2f point);
	virtual float getTMin () const;
private:
	Vec3f center;
	Vec3f direction;
	Vec3f up;
	float angle_radians;
	Vec3f horizontal;
};

