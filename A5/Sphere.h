#pragma once
#include "Object3D.h"
#include"matrix.h"
#include "grid.h"
extern int n_theta;
extern int n_phi;
extern bool gouraud;
class Sphere :	public Object3D
{
public:
	Sphere (Vec3f center,float radius,Material* mat);
	~Sphere ();
	bool intersect (const Ray &r, Hit &h, float tmin);
	virtual void paint ();
	BoundingBox* getBoundingBox ();
	void insertIntoGrid (Grid *g, Matrix *m);
private:
	float radius;
	Vec3f center;
};

