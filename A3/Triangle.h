#pragma once
#include "Object3D.h"
#include"matrix.h"
#include"matrix3f.h"
class Triangle :
	public Object3D
{
public:
	Triangle (Vec3f &a, Vec3f &b, Vec3f &c, Material *m);
	~Triangle ();
	bool intersect (const Ray &r, Hit &h, float tmin);
	Vec3f normals[3];
	virtual void paint ();
private:
	Vec3f a, b, c, ab, ac, bc;
	Material* mat;
};

