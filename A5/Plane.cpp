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
	RayTracingStats::IncrementNumIntersections ();
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

void Plane::insertIntoGrid (Grid *g, Matrix *m)
{
	Vec3f m_min = boundingBox->getMin ();
	Vec3f m_max = boundingBox->getMax ();
	Vec3f v = g->getGird ();
	BoundingBox *bb = g->getBoundingBox ();
	Vec3f min = bb->getMin ();
	Vec3f max = Vec3f (bb->getMax ().x () + FLT_EPSILON, bb->getMax ().y () + FLT_EPSILON, bb->getMax ().z () + FLT_EPSILON);
	int x = v.x ();
	int y = v.y ();
	int z = v.z ();
	Vec3f size = max - min;
	float grid_x = size.x () / x;
	float grid_y = size.y () / y;
	float grid_z = size.z () / z;

	int _start_i = (fabs (m_min.x () - min.x ())) * (1 / grid_x);
	int _start_j = (fabs (m_min.y () - min.y ())) * (1 / grid_y);
	int _start_k = (fabs (m_min.z () - min.z ())) * (1 / grid_z);
	int _end_i = (fabs (m_max.x () - min.x ()))*(1 / grid_x);
	int _end_j = (fabs (m_max.y () - min.y ()))*(1 / grid_y);
	int _end_k = (fabs (m_max.z () - min.z ()))*(1 / grid_z);

	if (_start_i == _end_i) _end_i++;
	if (_start_j == _end_j) _end_j++;
	if (_start_k == _end_k) _end_k++;
	if (_start_i > _end_i) swap (_start_i, _end_i);
	if (_start_j > _end_j) swap (_start_j, _end_j);
	if (_start_k > _end_k) swap (_start_k, _end_k);

	assert (_end_i <= x && _end_j <= y && _end_k <= z);

	for (int _i = _start_i; _i < _end_i; _i++)
	{
		for (int _j = _start_j; _j < _end_j; _j++)
		{
			for (int _k = _start_k; _k < _end_k; _k++)
			{
				g->insertIntoThis ((_i * y + _j) * z + _k, true, this);
			}
		}
	}
}