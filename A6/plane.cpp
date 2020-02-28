
#include "stdafx.h"
#include "plane.h"
#include "grid.h"

Plane::Plane(Vec3f &normal, float d, Material *m)
{
	_type = ObjectType::PlaneObject;
	this->material = m;
	this->normal = normal;
	this->normal.Normalize();
	this->d = d;
}

bool Plane::intersect(const Ray &r, Hit &h, float tmin)
{
	RayTracingStats::IncrementNumIntersections ();
	bool result = false;
	if (normal.Dot3(r.getDirection()) == 0)
	{
		result = false;
	}
	float t = (d - normal.Dot3(r.getOrigin())) / normal.Dot3(r.getDirection());
	if (t > tmin)
	{
		if (t < h.getT())
		{
			result = true;
			h.set(t, material, normal, r);
		}
	}
	return result;
}

void Plane::paint()
{
	Vec3f d1, d2, v(1, 0, 0);
	if (normal == v)
	{
		v = Vec3f(0, 1, 0);
	}
	Vec3f::Cross3(d1, v, normal);
	d1.Normalize();
	Vec3f::Cross3(d2, normal, d1);
	d2.Normalize();
	d1 *= 10000;
	d2 *= 10000;
	Vec3f e1 = d1 + d2, e2 = d1*(-1) + d2, e3 = d1*(-1) - d2, e4 = d1 - d2;
	e1 += d * normal;
	e2 += d * normal;
	e3 += d * normal;
	e4 += d * normal;
	material->glSetMaterial();
	glBegin(GL_QUADS);
	glNormal3f(normal.x(), normal.y(), normal.z());
	glVertex3f(e1.x(), e1.y(), e1.z());
	glVertex3f(e2.x(), e2.y(), e2.z());
	glVertex3f(e3.x(), e3.y(), e3.z());
	glVertex3f(e4.x(), e4.y(), e4.z());
	glEnd();
}

void Plane::insertIntoGrid(Grid *g, Matrix *m)
{
	g->addPlanes (this);
}