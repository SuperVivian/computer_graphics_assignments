#include "stdafx.h"
#include "Sphere.h"


Sphere::Sphere (Vec3f center, float radius, Material* mat)
{
	this->center = center;
	this->radius = radius;
	material = mat;
}


Sphere::~Sphere ()
{
}

bool Sphere::intersect (const Ray &r, Hit &h, float tmin)
{
	Vec3f OC = r.getOrigin () - center;//OC����
	Vec3f dir = r.getDirection ();

	/**************************************������*******************************************************/
	//float a = dir.absLength ();
	//float b = 2.0*Vec3f::Dot (dir, OC);
	//float c = OC.absLength () - pow (this->radius, 2);
	//float discrim = pow (b, 2) - (4 * a*c);
	//float t0=0, t1=0;

	//if (discrim < 0) return false;//�޸�
	//else if (discrim < FLT_EPSILON) { //һ����
	//	t0=t1 = (-1.0*b - sqrt (discrim)) / (2.0*a);
	//}else {//������
	//	t0 = (-1.0*b - sqrt (discrim)) / (2.0*a);
	//	t1 = (-1.0*b + sqrt (discrim)) / (2.0*a);
	//}
/**************************************���η�*******************************************************/
	Vec3f oc = center -r.getOrigin();
	float tp = dir.Dot3(oc);//oc��dir�ϵ�ͶӰ
	if (tp < 0)	return false; //�޽��㣬ͶӰС��0

	float oc2 = oc.Dot3(oc);
	float distance2 = oc2 - tp * tp;//���dƽ��

	if (distance2 > pow(radius,2))	return false;//�޽���

	float discriminant = pow (radius, 2) - distance2;//t'��ƽ��
	float t0=0, t1=0;
	if (discriminant < FLT_EPSILON)	t0 = t1 = tp;
	else	{
		discriminant = sqrt (discriminant);
		t0 = tp - discriminant;
		t1 = tp + discriminant;
	}
	/**************************************�жϸ��Ƿ���Ч*******************************************************/
	if (t0 >= tmin && t0 <= h.getT ()) {
		h.set (t0, this->material, r);
		return true;
	}
	else if (t1 >= tmin && t1 <= h.getT ()) {
		h.set (t1, this->material, r);
		return true;
	}
}



