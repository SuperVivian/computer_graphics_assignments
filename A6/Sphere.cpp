#include "stdafx.h"
#include "sphere.h"
Sphere::Sphere(Vec3f center, float radius, Material* material)
{
	_type = ObjectType::SphereObject;
	this->center = center;
	this->radius = radius;
	this->material = material;
	boundingBox = new BoundingBox (Vec3f (center.x () - radius, center.y () - radius, center.z () - radius), Vec3f (center.x () + radius, center.y () + radius, center.z () + radius));
}

bool Sphere::intersect(const Ray &r, Hit &h, float tmin)
{
	RayTracingStats::IncrementNumIntersections ();
	bool result = false;

	Vec3f direction = r.getDirection();
	Vec3f origin = r.getOrigin();

	float a = direction.Dot3(direction);
	float b = 2.0f * direction.Dot3(origin - (center));
	float c = (origin - (center)).Length() * (origin - (center)).Length() - radius * radius;
	float delta = b * b - 4 * a * c;
	float t = INT_MAX * 1.0f;
	if (delta >= 0)
	{
		float d = sqrt(delta);
		float t1 = (-b - d) / (a * 2.0f);
		float t2 = (-b + d) / (a * 2.0f);
		if (t1 >= tmin)
		{
			t = t1;
			result = true;
		}
		else if (t2 >= tmin)
		{
			t = t2;
			result = true;
		}
		if (t < h.getT())
		{
			Vec3f normal = origin + direction * t - center;
			normal.Normalize();
			h.set(t, material, normal, r);
		}
	}
	return result;
}

void Sphere::paint()
{
	material->glSetMaterial ();
	int M = tessellationX; int N = tessellationY;
	float step_y = M_PI / M;  //y����ÿ�β����ĽǶ�
	float step_xz = 2 * M_PI / N;//x,zƽ��ÿ�β����ĽǶ�
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

			Vec3f a (center.x () + x[0], center.y () + y[0], center.z () + z[0]);
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

void Sphere::insertIntoGrid(Grid *g, Matrix *m)
{
	if (m != NULL) {
		Object3D::insertIntoGrid (g, m);
		return;
	}
	Vec3f v = g->getGird ();
	int nx = v.x ();	int ny = v.y ();	int nz = v.z ();
	BoundingBox *bb = g->getBoundingBox ();
	Vec3f min = Vec3f (bb->getMin ().x () - FLT_EPSILON, bb->getMin ().y () - FLT_EPSILON, bb->getMin ().z () - FLT_EPSILON);
	Vec3f max = Vec3f (bb->getMax ().x () + FLT_EPSILON, bb->getMax ().y () + FLT_EPSILON, bb->getMax ().z () + FLT_EPSILON);
	Vec3f size = max - min;
	float grid_x = size.x () / (nx + 0.0);
	float grid_y = size.y () / (ny + 0.0);
	float grid_z = size.z () / (nz + 0.0);//���ÿ�����صĳ����
	Vec3f cen = center - min;//��min�㴦Ϊԭ�㣬�������������ʱ��ȽϷ���
	Vec3f voxel;
	for (int i = 0; i < nx; i++) {
		for (int j = 0; j < ny; j++) {
			for (int k = 0; k < nz; k++) {
				Vec3f p1 ((i + 0)*grid_x, (j + 0)*grid_y, (k + 0)*grid_z);
				Vec3f p2 ((i + 0)*grid_x, (j + 0)*grid_y, (k + 1)*grid_z);
				Vec3f p3 ((i + 0)*grid_x, (j + 1)*grid_y, (k + 0)*grid_z);
				Vec3f p4 ((i + 0)*grid_x, (j + 1)*grid_y, (k + 1)*grid_z);
				Vec3f p5 ((i + 1)*grid_x, (j + 0)*grid_y, (k + 0)*grid_z);
				Vec3f p6 ((i + 1)*grid_x, (j + 0)*grid_y, (k + 1)*grid_z);
				Vec3f p7 ((i + 1)*grid_x, (j + 1)*grid_y, (k + 0)*grid_z);
				Vec3f p8 ((i + 1)*grid_x, (j + 1)*grid_y, (k + 1)*grid_z);
				float minx = min (p8.x (), min (p7.x (), min (p6.x (), min (p5.x (), min (p4.x (), min (p3.x (), min (p2.x (), p1.x ())))))));
				float miny = min (p8.y (), min (p7.y (), min (p6.y (), min (p5.y (), min (p4.y (), min (p3.y (), min (p2.y (), p1.y ())))))));
				float minz = min (p8.z (), min (p7.z (), min (p6.z (), min (p5.z (), min (p4.z (), min (p3.z (), min (p2.z (), p1.z ())))))));
				float maxx = max (p8.x (), max (p7.x (), max (p6.x (), max (p5.x (), max (p4.x (), max (p3.x (), max (p2.x (), p1.x ())))))));
				float maxy = max (p8.y (), max (p7.y (), max (p6.y (), max (p5.y (), max (p4.y (), max (p3.y (), max (p2.y (), p1.y ())))))));
				float maxz = max (p8.z (), max (p7.z (), max (p6.z (), max (p5.z (), max (p4.z (), max (p3.z (), max (p2.z (), p1.z ())))))));
				float	xx = cen.x (), yy = cen.y (), zz = cen.z ();
				if (xx>maxx)xx = maxx;	if (xx<minx)xx = minx;
				if (yy>maxy)yy = maxy;	if (yy<miny)yy = miny;
				if (zz>maxz)zz = maxz; if (zz<minz)zz = minz;
				////�����ĸ�����cen���
				if (((xx - cen.x ())*(xx - cen.x ()) + (yy - cen.y ())*(yy - cen.y ()) + (zz - cen.z ())*(zz - cen.z ())) <= radius * radius)
				{
					g->insertIntoThis (i * ny*nz + j * nz + k, true, this);
				}
			}
		}
	}
}

Sphere::~Sphere()
{
	
}