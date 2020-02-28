#pragma once
#include "Object3D.h"
#include "grid.h"
class Plane :
	public Object3D
{
public:
	Plane (Vec3f &normal, float d, Material *mat);
	~Plane ();
	bool intersect (const Ray &r, Hit &h, float tmin);
	virtual void paint ();
	void insertIntoGrid (Grid *g, Matrix *m);
	BoundingBox * getBoundingBox () { return NULL; };
private:
	Vec3f normal;
	float d;//点origin到平面的距离
	Material* mat;
	float D;//平面方程的常数
};

