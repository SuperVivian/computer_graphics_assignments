#ifndef TRIANGLE
#define TRIANGLE

#include "object3d.h"
#include "grid.h"
#include"plane.h"
class Triangle :public Object3D
{
public:
	Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m);
	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	virtual void paint();
	void insertIntoGrid(Grid *g, Matrix *m);
	bool inBound (Vec3f othermin,Vec3f othermax);
	Matrix* getTransformMatrix () { return m; }
	Vec3f getVertex (int index) const {
		switch (index) {
		case 0: return a;
		case 1: return b;
		case 2: return c;
		default: assert (0); return Vec3f ();
		}
	}
private:
	Vec3f normal;
	Vec3f ca;
	Vec3f cb;
	Vec3f m_min;
	Vec3f m_max;
	Matrix* m;
};

#endif