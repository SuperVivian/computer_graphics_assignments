#include "stdafx.h"
#include "Group.h"


Group::Group (int num_objects)
{
	num = num_objects;
	objects = new Object3D*[num];
}


Group::~Group ()
{		
}

void Group::addObject (int index, Object3D *obj)
{	
	if (index >= 0 && index < num)
	{
		objects[index] = obj;
	}
}
bool Group::intersect (const Ray& r, Hit& h, float tmin) {
	bool intersect = false;
	for (int i = 0; i < num; i++)
	{
		//在计算这个点的相交过程中，如果相交了，hit会有值。如果有多个物体相交，hit中会存储最近的那个材质		
		if (objects[i]->intersect (r, h, tmin))
			intersect = true;
	}
	return intersect;
}
