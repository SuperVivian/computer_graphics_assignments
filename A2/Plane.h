#pragma once
#include "Object3D.h"
class Plane :
	public Object3D
{
public:
	Plane (Vec3f &normal, float d, Material *mat);
	~Plane ();
	bool intersect (const Ray &r, Hit &h, float tmin);
private:
	Vec3f normal;
	float d;//��origin��ƽ��ľ���
	Material* mat;
	float D;//ƽ�淽�̵ĳ���
};

