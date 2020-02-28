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
void Sphere::paint () {
	material->glSetMaterial ();
	int M = n_phi; int N = n_theta;
	float step_y = PI / M;  //y����ÿ�β����ĽǶ�
	float step_xz = 2 * PI / N;//x,zƽ��ÿ�β����ĽǶ�
	float x[4], y[4], z[4];//�ĸ���������

	float angle_y = 0.0;//��ʼ�Ƕ�
	float angle_xz = 0.0;
	int i = 0, j = 0;
	glBegin (GL_QUADS);
	for (i = 0; i < M; i++)
	{
		angle_y = i * step_y; //ÿ�β���step_y
		for (j = 0; j < N; j++)
		{
			angle_xz = j * step_xz; //ÿ�β���step_xz

			x[0] = radius * sin (angle_y) * cos (angle_xz);//      :  :   ���Ͻ�
			z[0] = radius * sin (angle_y) * sin (angle_xz);
			y[0] = radius * cos (angle_y);

			x[1] = radius * sin (angle_y + step_y) * cos (angle_xz); //      :  :   ���½�
			z[1] = radius * sin (angle_y + step_y) * sin (angle_xz);
			y[1] = radius * cos (angle_y + step_y);

			x[2] = radius * sin (angle_y + step_y)*cos (angle_xz + step_xz);//      :  :   ���½�
			z[2] = radius * sin (angle_y + step_y)*sin (angle_xz + step_xz);
			y[2] = radius * cos (angle_y + step_y);

			x[3] = radius * sin (angle_y) * cos (angle_xz + step_xz);//      :  :   ���Ͻ�
			z[3] = radius * sin (angle_y) * sin (angle_xz + step_xz);
			y[3] = radius * cos (angle_y);

			Vec3f a(center.x () + x[0], center.y () + y[0], center.z () + z[0]);
			Vec3f b (center.x () + x[1], center.y () + y[1], center.z () + z[1]);
			Vec3f c (center.x () + x[2], center.y () + y[2], center.z () + z[2]);
			Vec3f d (center.x () + x[3], center.y () + y[3], center.z () + z[3]);
			Vec3f ab = a - b;  Vec3f bc = b - c;  
			Vec3f normal; Vec3f::Cross3 (normal, bc, ab); normal.Normalize ();

			Vec3f n_a = a - center;  Vec3f n_b = b - center;
			Vec3f n_c = c - center; Vec3f n_d = d - center;

			//�������ƽ��
			if (!gouraud) {
				glNormal3f (normal.x (), normal.y (), normal.z ());
				glVertex3f (a.x (), a.y (), a.z ()); 				glVertex3f (b.x (), b.y (), b.z ());
				glVertex3f (c.x (), c.y (), c.z ());   				glVertex3f (d.x (), d.y (), d.z ());
			}
			else {
				glNormal3f (n_a.x (), n_a.y (), n_a.z ());				glVertex3f (a.x (), a.y (), a.z ()); 
				glNormal3f (n_b.x (), n_b.y (), n_b.z ());			glVertex3f (b.x (), b.y (), b.z ());
				glNormal3f (n_c.x (), n_c.y (), n_c.z ());           glVertex3f (c.x (), c.y (), c.z ());
				glNormal3f (n_d.x (), n_d.y (), n_d.z ());           glVertex3f (d.x (), d.y (), d.z ());
			}
		}//ѭ��������һ���ƽ�棬���һ����
	}//y��++������ʣ���
	glEnd ();
}
void Sphere::tranform (Matrix m, Object3D&obj) {
	Vec4f centerV4 (center,1);
	centerV4 = m * centerV4;
	Vec3f centerNew (centerV4.x(),centerV4.y(),centerV4.z());
	Sphere sphere (centerNew,radius,material);
	obj = sphere;
}
bool Sphere::intersect (const Ray &r, Hit &h, float tmin)
{
	Vec3f OC = r.getOrigin () - center;//OC����
	Vec3f dir = r.getDirection ();

	/**************************************������*******************************************************/
	float a = dir.absLength ();
	float b = 2.0*dir.Dot3(OC);
	float c = OC.absLength () - pow (this->radius, 2);
	float discrim = pow (b, 2) - (4 * a*c);
	float t0=0, t1=0;

	if (discrim < 0) return false;//�޸�
	else if (discrim < FLT_EPSILON) { //һ����
		t0=t1 = (-1.0*b - sqrt (discrim)) / (2.0*a);
	}else {//������
		t0 = (-1.0*b - sqrt (discrim)) / (2.0*a);
		t1 = (-1.0*b + sqrt (discrim)) / (2.0*a);
	}

	/**************************************���η�*******************************************************/
	//Vec3f oc = center - r.getOrigin ();
	//float tp = dir.Dot3 (oc);//oc��dir�ϵ�ͶӰ
	//if (tp < 0)	return false; //�޽��㣬ͶӰС��0

	//float oc2 = oc.Dot3 (oc);
	//float distance2 = oc2 - tp * tp;//���dƽ��

	//if (distance2 > pow (radius, 2))	return false;//�޽���

	//float discriminant = pow (radius, 2) - distance2;//t'��ƽ��
	//float t0 = 0, t1 = 0;
	//if (discriminant < FLT_EPSILON)	t0 = t1 = tp;
	//else {
	//	discriminant = sqrt (discriminant);
	//	t0 = tp - discriminant;
	//	t1 = tp + discriminant;
	//}

	///**************************************�жϸ��Ƿ���Ч*******************************************************/
	if (t0 >= tmin && t0 <= h.getT ()) {
		Vec3f normalNew = (r.getOrigin() + t0 * dir - center).Normalized();//�洢���㴦�ķ���
		h.set (t0, this->material, normalNew,r);
		return true;
	}
	else if (t1 >= tmin && t1 <= h.getT ()) {
		Vec3f normalNew = (r.getOrigin() + t1 * dir - center).Normalized();
		h.set (t1, this->material, normalNew, r);
		return true;
	}
}



