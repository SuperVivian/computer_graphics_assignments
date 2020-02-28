#pragma once
#include "Object3D.h"
#include"matrix.h"
extern int n_theta;
extern int n_phi;
extern bool gouraud;
class Sphere :	public Object3D
{
public:
	Sphere (Vec3f center,float radius,Material* mat);
	~Sphere ();
	bool intersect (const Ray &r, Hit &h, float tmin);
	void tranform (Matrix m, Object3D&obj);
	virtual void paint ();
private:
	float radius;
	Vec3f center;
};

