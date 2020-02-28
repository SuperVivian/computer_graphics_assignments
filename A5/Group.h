#pragma once
#include "Object3D.h"
#include<vector>
#include "grid.h"
class Group :	public Object3D
{
public:
	Group (int num_objects);
	virtual bool intersect (const Ray &r, Hit &h, float tmin);
	void addObject (int index, Object3D *obj);
	virtual void paint ();
	BoundingBox* getBoundingBox ();
	void insertIntoGrid (Grid *g, Matrix *m);
	void setIsGrid (bool g) { isGrid = g; };
	~Group ();
protected:
	Object3D * * object3ds;
	int num_objects;
	bool isGrid;
	Grid *grid;
};

