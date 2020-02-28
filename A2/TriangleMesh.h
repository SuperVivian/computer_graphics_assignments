#pragma once
#include "Object3D.h"
#include <vector>
#include "Object3D.h"
#include "Triangle.h"
//by default counterclockwise winding is front face


class TriangleMesh :	public Object3D
{
public:
	TriangleMesh (const char * filename, Material* m);
	~TriangleMesh ();
	virtual bool intersect (const Ray &r, Hit &h, float tmin);
protected:
	Material * mat;
	void compute_norm ();
};

