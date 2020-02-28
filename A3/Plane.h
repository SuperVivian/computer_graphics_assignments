#pragma once
#include "Object3D.h"
class Plane :
	public Object3D
{
public:
	Plane (Vec3f &normal, float d, Material *mat);
	~Plane ();
	bool intersect (const Ray &r, Hit &h, float tmin);
	virtual void paint ();
private:
	Vec3f normal;
	float d;//点origin到平面的距离
	Material* mat;
	float D;//平面方程的常数
};

