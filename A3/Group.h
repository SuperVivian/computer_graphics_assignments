#pragma once
#include "Object3D.h"
#include<vector>
class Group :	public Object3D
{
public:
	Group (int num_objects);
	~Group ();
	void addObject (int index, Object3D *obj);
	virtual bool intersect (const Ray& r, Hit& h, float tmin);
	int getObjectsNum () {
		return num;
	}
	virtual void paint ();
private:    
	int num;
    Object3D** objects;
};

