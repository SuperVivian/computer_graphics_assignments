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
	Checkerboard::glSetMaterial ();
	material->glSetMaterial ();
	int M = n_phi; int N = n_theta;
	float step_y = M_PI / M;  //y方向每次步进的角度
	float step_xz = 2 * M_PI / N;//x,z平面每次步进的角度
	float x[4], y[4], z[4];//四个顶点坐标

	float angle_y = 0.0;//起始角度
	float angle_xz = 0.0;
	int i = 0, j = 0;
	glBegin (GL_QUADS);
	for (i = 0; i < M; i++)
	{
		angle_y = i * step_y; //每次步进step_y
		for (j = 0; j < N; j++)
		{
			angle_xz = j * step_xz; //每次步进step_xz

			x[0] = radius * sin (angle_y) * cos (angle_xz);//      :  :   左上角
			z[0] = radius * sin (angle_y) * sin (angle_xz);
			y[0] = radius * cos (angle_y);

			x[1] = radius * sin (angle_y + step_y) * cos (angle_xz); //      :  :   左下角
			z[1] = radius * sin (angle_y + step_y) * sin (angle_xz);
			y[1] = radius * cos (angle_y + step_y);

			x[2] = radius * sin (angle_y + step_y)*cos (angle_xz + step_xz);//      :  :   右下角
			z[2] = radius * sin (angle_y + step_y)*sin (angle_xz + step_xz);
			y[2] = radius * cos (angle_y + step_y);

			x[3] = radius * sin (angle_y) * cos (angle_xz + step_xz);//      :  :   右上角
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

			//画出这个平面
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
		}//循环画出这一层的平面，组成一个环
	}//y轴++，画出剩余层
	glEnd ();
}

bool Sphere::intersect (const Ray &r, Hit &h, float tmin)
{
	RayTracingStats::IncrementNumIntersections ();
	Vec3f OC = r.getOrigin () - center;//OC距离
	Vec3f dir = r.getDirection ();

	/**************************************代数法*******************************************************/
	//float a = dir.absLength ();
	//float b = 2.0*dir.Dot3(OC);
	//float c = OC.absLength () - pow (this->radius, 2);
	//float discrim = pow (b, 2) - (4 * a*c);
	//float t0=0, t1=0;

	//if (discrim < 0) return false;//无根
	//else if (discrim < FLT_EPSILON) { //一个根
	//	t0=t1 = (-1.0*b - sqrt (discrim)) / (2.0*a);
	//}else {//两个根
	//	t0 = (-1.0*b - sqrt (discrim)) / (2.0*a);
	//	t1 = (-1.0*b + sqrt (discrim)) / (2.0*a);
	//}

	/**************************************几何法*******************************************************/
	Vec3f oc = center - r.getOrigin ();
	float tp = dir.Dot3 (oc);//oc在dir上的投影
	if (tp < 0)	return false; //无交点，投影小于0

	float oc2 = oc.Dot3 (oc);
	float distance2 = oc2 - tp * tp;//求出d平方

	if (distance2 > pow (radius, 2))	return false;//无交点

	float discriminant = pow (radius, 2) - distance2;//t'的平方
	float t0 = 0, t1 = 0;
	if (discriminant < FLT_EPSILON)	t0 = t1 = tp;
	else {
		discriminant = sqrt (discriminant);
		t0 = tp - discriminant;
		t1 = tp + discriminant;
	}

	//**************************************判断根是否有效*******************************************************/
	if (t0 >= tmin && t0 <= h.getT ()) {
		Vec3f normalNew = (r.pointAtParameter(t0)-center).Normalized();//存储交点处的法线
		h.set (t0, this->material, normalNew,r);
		return true;
	}
	else if (t1 >= tmin && t1 <= h.getT ()) {
		Vec3f normalNew = (r.pointAtParameter (t1) - center).Normalized();
		h.set (t1, this->material, normalNew, r);
		return true;
	}
}

BoundingBox* Sphere::getBoundingBox () {
	boundingBox = new BoundingBox (Vec3f (center.x () - radius, center.y () - radius, center.z () - radius), Vec3f (center.x () + radius, center.y () + radius, center.z () + radius));

	return boundingBox;
}

void Sphere::insertIntoGrid (Grid *g, Matrix *m)
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
	float grid_x = size.x () / (nx+0.0);
	float grid_y = size.y () / (ny+0.0);
	float grid_z = size.z () / (nz+0.0);//获得每个体素的长宽高
	Vec3f cen = center - min;//以min点处为原点，计算体素坐标的时候比较方便
	Vec3f voxel;
	for (int i = 0; i < nx; i++)	{		
		for (int j = 0; j < ny; j++)	{		
			for (int k = 0; k < nz; k++){
				Vec3f p1 ((i + 0)*grid_x, (j + 0)*grid_y, (k + 0)*grid_z); 
				Vec3f p2 ((i + 0)*grid_x, (j + 0)*grid_y, (k + 1)*grid_z);
				Vec3f p3 ((i + 0)*grid_x, (j + 1)*grid_y, (k + 0)*grid_z); 
				Vec3f p4 ((i + 0)*grid_x, (j + 1)*grid_y, (k + 1)*grid_z); 
				Vec3f p5 ((i + 1)*grid_x, (j + 0)*grid_y, (k + 0)*grid_z);
				Vec3f p6 ((i + 1)*grid_x, (j + 0)*grid_y, (k + 1)*grid_z); 
				Vec3f p7 ((i + 1)*grid_x, (j + 1)*grid_y, (k + 0)*grid_z); 
				Vec3f p8 ((i + 1)*grid_x, (j + 1)*grid_y, (k + 1)*grid_z); 
				float minx = min (p8.x (), min (p7.x (), min (p6.x (), min (p5.x (), min (p4.x (), min (p3.x (), min (p2.x(), p1.x ())))))));
				float miny = min (p8.y (), min (p7.y (), min (p6.y (), min (p5.y (), min (p4.y (), min (p3.y (), min (p2.y (), p1.y ())))))));
				float minz = min (p8.z (), min (p7.z (), min (p6.z (), min (p5.z (), min (p4.z (), min (p3.z (), min (p2.z (), p1.z ())))))));
				float maxx = max (p8.x (), max (p7.x (), max (p6.x (), max (p5.x (), max (p4.x (), max (p3.x (), max (p2.x (), p1.x ())))))));
				float maxy = max (p8.y (), max (p7.y (), max (p6.y (), max (p5.y (), max (p4.y (), max (p3.y (), max (p2.y (), p1.y ())))))));
				float maxz = max (p8.z (), max (p7.z (), max (p6.z (), max (p5.z (), max (p4.z (), max (p3.z (), max (p2.z (), p1.z ())))))));
				float	xx = cen.x(), yy = cen.y(), zz = cen.z();
				if (xx>maxx)xx = maxx;	if (xx<minx)xx = minx;
				if (yy>maxy)yy = maxy;	if (yy<miny)yy = miny;
				if (zz>maxz)zz = maxz; if (zz<minz)zz = minz;				
////计算哪个点离cen最近
				if (((xx - cen.x ())*(xx - cen.x ()) + (yy - cen.y ())*(yy - cen.y ()) + (zz - cen.z ())*(zz - cen.z ())) <= radius*radius)
				{
					g->insertIntoThis (i * ny*nz+j* nz + k, true, this);
				}
			}
		}
	}
	
}




