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
void printVec3f (Vec3f b) {
	cout << b.x () << " " << b.y () << " " << b.z () << endl;
}
void Plane::paint () {
	mat->glSetMaterial ();
	Vec3f v(0,0,1);
	if (normal.x () == 0 && normal.y () == 0 && normal.z () == 1) {
		v = Vec3f (0,1,0);
	}
	Vec3f b1; Vec3f::Cross3 (b1, normal, v); b1.Normalize ();//x轴
	Vec3f b2; Vec3f::Cross3 (b2,normal, b1); b2.Normalize ();//y轴
	float big=100;
	Vec3f a = b1 * big + b2 * big + this->d*normal;
	Vec3f b = b1*big - 1 * b2*big + this->d * normal;
	Vec3f c = -1*b1 * big - 1 * b2*big + this->d * normal;
	Vec3f d = -1 * b1 * big + 1 * b2*big + this->d * normal;
	glBegin (GL_QUADS);
		glNormal3f (normal.x(), normal.y(), normal.z());
		glVertex3f (a.x (), a.y (), a.z ());
		glVertex3f (b.x (), b.y (), b.z ());		
		glVertex3f (c.x (), c.y (), c.z ()); 
		glVertex3f (d.x (), d.y (), d.z ());
	glEnd ();
}
bool Plane::intersect (const Ray &r, Hit &h, float tmin) {	
	bool intersect = false;
	float t = 0;
	//点到平面的距离，准确讲是origin到平面的距离
	//d^2=(origin·normal+D)/(normal·normal)

	Vec3f origin = r.getOrigin ();
	Vec3f dir = r.getDirection ();
	dir.Normalize ();
	float nDotdir = normal.Dot3 (dir);
	if (nDotdir == 0)return false;
	//和平面的交点，既在射线上，又在平面上，代入参数方程，求t
	//t = -(D + n·Ro) / n·Rd 
	t = -1.0*(-1.0*d + normal.Dot3 (origin)) / nDotdir;
	if (t > tmin &&t < h.getT ()) {
		h.set (t,mat,normal,r);
		intersect = true;
	}
	return intersect;
}
