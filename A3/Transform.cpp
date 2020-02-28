#include "stdafx.h"
#include "Transform.h"


Transform::Transform (Matrix&matrix, Object3D* obj)
{
	this->obj = obj;
	this->matrix = matrix.transposed ();
}

Transform::~Transform ()
{
}
void Transform::paint () {
	glPushMatrix ();
	GLfloat m[16] = {
		this->matrix.Get (0,0),	this->matrix.Get (1,0),	this->matrix.Get (2,0),	this->matrix.Get (3,0),
		this->matrix.Get (0,1),	this->matrix.Get (1,1),	this->matrix.Get (2,1),	this->matrix.Get (3,1),
		this->matrix.Get (0,2),	this->matrix.Get (1,2),	this->matrix.Get (2,2),	this->matrix.Get (3,2),
		this->matrix.Get (0,3),	this->matrix.Get (1,3),	this->matrix.Get (2,3),	this->matrix.Get (3,3)
	};
	GLfloat *glMatrix = m;
	glMultMatrixf (glMatrix);
	obj->paint ();
	glPopMatrix ();
}
bool Transform::intersect (const Ray &r, Hit &h, float tmin) {
	//�Է��������߽�������任
	Vec4f o4 (r.getOrigin (), 1.0f);
	Vec4f d4 (r.getDirection (), 0.0f);
	Vec4f o4_trans = matrix.inverse ()*o4;
	Vec4f d4_trans = matrix.inverse ()*d4;
	Vec3f o3_trans = o4_trans.xyz ();
	Vec3f d3_trans = d4_trans.xyz ();
	Ray newRay (o3_trans, d3_trans);
	bool intersect = false;
	if (obj->intersect (newRay, h, tmin)) {
		//	�Է��߽��б任
		Vec4f normalTrans4 = Vec4f (h.getNormal (), 0.0f);
		Vec4f normal4 = (this->matrix.inverse ().transposed ()*normalTrans4);
		Vec3f normal = normal4.xyz ().Normalized ();
		h.set (h.getT (), h.getMaterial (), normal, r);
		intersect = true;
	}
	return intersect;
}
