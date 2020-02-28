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
bool Transform::intersect (const Ray &r, Hit &h, float tmin) {
    //对发出的射线进行逆向变换
	Vec4f o4 (r.getOrigin (),1.0f);
	Vec4f d4 (r.getDirection (),0.0f);
	Vec4f o4_trans= matrix.inverse()*o4;
	Vec4f d4_trans = matrix.inverse()*d4;
	Vec3f o3_trans = o4_trans.xyz();
	Vec3f d3_trans = d4_trans.xyz();
	Ray newRay (o3_trans, d3_trans);
	bool intersect = false;
	if (obj->intersect (newRay, h, tmin)) {
	//	对法线进行变换
		Vec4f normalTrans4 = Vec4f (h.getNormal (), 0.0f);
		Vec4f normal4 = (this->matrix.inverse ().transposed ()*normalTrans4);
		Vec3f normal = normal4.xyz ().Normalized();
		h.set (h.getT (), h.getMaterial (), normal,r);		
		intersect = true;
	}
	return intersect;
}
