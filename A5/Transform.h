#pragma once
#include "Object3D.h"
#include "Matrix.h"
#include "grid.h"
class Transform :
	public Object3D
{
public:
	Transform (Matrix&m, Object3D *obj);
	~Transform ();
	bool intersect (const Ray &r, Hit &h, float tmin);
	virtual void paint ();
	BoundingBox * getBoundingBox ();
	void insertIntoGrid (Grid *g, Matrix *m);
private:
	Object3D * obj;
	Matrix matrix;
};

