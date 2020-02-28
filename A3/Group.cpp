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
void Group::paint () {
	for (int i = 0; i < num; i++)
	{				
		objects[i]->paint ();
	}
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
		//�ڼ����������ཻ�����У�����ཻ�ˣ�hit����ֵ������ж�������ཻ��hit�л�洢������Ǹ�����		
		if (objects[i]->intersect (r, h, tmin))
			intersect = true;
	}
	return intersect;
}
