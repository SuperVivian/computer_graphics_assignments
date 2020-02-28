#ifndef TRANSFORM_H
#define TRANSFORM_H

#include "matrix.h"
#include "object3d.h"
#include "grid.h"
#include"triangle.h"
class Transform : public Object3D
{
public:
	Transform(Matrix &m, Object3D *o);
	virtual bool intersect(const Ray &r, Hit &h, float tmin);
	BoundingBox*TransformBoundingBox ();
	virtual void paint();
	void insertIntoGrid(Grid *g, Matrix *m);
	void setFlattened () { _flattened = true; }
private:
	Object3D *object3d;
	Matrix transform_matrix;
	bool _flattened = false;
};

#endif