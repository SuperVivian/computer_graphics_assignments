#pragma once
#include "Object3D.h"
class Sphere :	public Object3D
{
public:
	Sphere (Vec3f center,float radius,Material* mat);
	~Sphere ();
	bool intersect (const Ray &r, Hit &h, float tmin);
private:
	float radius;
	Vec3f center;
};

