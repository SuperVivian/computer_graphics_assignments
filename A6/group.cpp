
#include "stdafx.h"
#include "group.h"

Group::Group(int num_objects)
{
	_type = ObjectType::GroupObject;
	this->num_objects = num_objects;
	this->object3ds = new Object3D*[num_objects];
	this->boundingBox = new BoundingBox(Vec3f(INT_MAX, INT_MAX, INT_MAX), Vec3f(INT_MIN, INT_MIN, INT_MIN));
}

bool Group::intersect(const Ray &r, Hit &h, float tmin)
{
	bool result = false;
	if (gg){		
		result = grid->intersect(r, h, tmin);
	}
	else {
		for (int i = 0; i < num_objects; i++)
		{
			if (object3ds[i]->intersect(r, h, tmin)){
				result = true;
			}
		}
	}

	return result;
}

void Group::addObject(int index, Object3D *obj)
{
	object3ds[index] = obj;
	if (obj->getBoundingBox() == NULL){
		return;
	}
	boundingBox->Extend(obj->getBoundingBox());
}

void Group::paint()
{
	for (int i = 0; i < num_objects; i++)
	{
		object3ds[i]->paint();
	}
}

void Group::insertIntoGrid(Grid *g, Matrix *m)
{
	grid = g;
	for (int i = 0; i < num_objects; i++)
	{		
		object3ds[i]->insertIntoGrid(g, m);
	}
}

Group::~Group()
{
	delete[]object3ds;
}