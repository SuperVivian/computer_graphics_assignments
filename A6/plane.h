#ifndef PLANE_H
#define PLANE_H

#include "object3d.h"
#include "grid.h"

class Plane :public Object3D
{
public:
	Plane(Vec3f &normal, float d, Material *m);
	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	virtual void paint();
	void insertIntoGrid(Grid *g, Matrix *m);
private:
	Vec3f normal;
	float d;
};

#endif