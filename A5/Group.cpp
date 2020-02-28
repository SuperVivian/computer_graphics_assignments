#include "stdafx.h"
#include "Group.h"


Group::Group (int num_objects)
{
	this->num_objects = num_objects;
	this->object3ds = new Object3D*[num_objects];
	this->boundingBox = new BoundingBox (
		Vec3f (INT_MAX, INT_MAX, INT_MAX),
		Vec3f (INT_MIN, INT_MIN, INT_MIN)
	);
}


Group::~Group ()
{
	delete[]object3ds;
}
void Group::paint () {
	for (int i = 0; i < num_objects; i++)
	{
		object3ds[i]->paint ();
	}
}
void Group::addObject (int index, Object3D *obj)
{	
	object3ds[index] = obj;
	if (obj->getBoundingBox () == NULL) {
		return;
	}
	boundingBox->Extend (obj->getBoundingBox ());
}
bool Group::intersect (const Ray& r, Hit& h, float tmin) {
	bool result = false;
	if (isGrid) {
		result = grid->intersect (r, h, tmin);
	}
	else {
		for (int i = 0; i < num_objects; i++)
		{
			if (object3ds[i]->intersect (r, h, tmin)) {
				result = true;
			}
		}
	}

	return result;
}

BoundingBox* Group::getBoundingBox ()
{
	return boundingBox;
}
void Group::insertIntoGrid (Grid *g, Matrix *m) {
	grid = g;
	for (int i = 0; i < num_objects; i++)
	{
		if (object3ds[i]->getBoundingBox () == NULL)
			continue;
		object3ds[i]->insertIntoGrid (g, m);
	}
}

