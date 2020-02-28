#include "stdafx.h"
#include "Plane.h"


Plane::Plane (Vec3f &normal, float d, Material *mat)
{
	this->normal = normal; this->normal.Normalize ();
	this->d = d;	
	this->mat = mat;
}


Plane::~Plane ()
{

}

bool Plane::intersect (const Ray &r, Hit &h, float tmin) {	
	bool intersect = false;
	float t = 0;
	//�㵽ƽ��ľ��룬׼ȷ����origin��ƽ��ľ���
	//d^2=(origin��normal+D)/(normal��normal)

	Vec3f origin = r.getOrigin ();
	Vec3f dir = r.getDirection ();
	dir.Normalize ();
	float nDotdir = normal.Dot3 (dir);
	if (nDotdir == 0)return false;
	//��ƽ��Ľ��㣬���������ϣ�����ƽ���ϣ�����������̣���t
	//t = -(D + n��Ro) / n��Rd 
	t = -1.0*(-1.0*d + normal.Dot3 (origin)) / nDotdir;
	if (t > tmin &&t < h.getT ()) {
		h.set (t,mat,normal,r);
		intersect = true;
	}
	return intersect;
}
