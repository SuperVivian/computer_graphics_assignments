
#include "stdafx.h"
#include "triangle.h"
#include"Matrix3f.h"
static float _err = 1e-5f;

Triangle::Triangle(Vec3f &a, Vec3f &b, Vec3f &c, Material *m)
{
	_type = ObjectType::TriangleObject;
	this->a = a;
	this->b = b;
	this->c = c;
	this->material = m;
	Vec3f::Cross3(normal, b - a, c - a);
	this->ca = a - c;
	this->cb = b - c;
	this->m = NULL;
	this->triangle = true;
	boundingBox = new BoundingBox (
		Vec3f (MIN (a.x (), b.x (), c.x ()), MIN (a.y (), b.y (), c.y ()), MIN (a.z (), b.z (), c.z ())),
		Vec3f (MAX (a.x (), b.x (), c.x ()), MAX (a.y (), b.y (), c.y ()), MAX (a.z (), b.z (), c.z ()))
	);
}

bool Triangle::intersect(const Ray &r, Hit &h, float tmin)
{
	RayTracingStats::IncrementNumIntersections ();
	//Ro + t * Rd = a + B(b-a) + G(c-a) 
	//if    B+G < 1 &  B > 0   &   G > 0  intersect
	//用三阶行列式求解三元一次方程组

	Vec3f R_o = r.getOrigin ();
	Vec3f R_d = r.getDirection ();

	Matrix3f A (this->a.x () - this->b.x (), this->a.x () - this->c.x (), R_d.x (),
		this->a.y () - this->b.y (), this->a.y () - this->c.y (), R_d.y (),
		this->a.z () - this->b.z (), this->a.z () - this->c.z (), R_d.z ());

	Matrix3f BetaM (this->a.x () - R_o.x (), this->a.x () - this->c.x (), R_d.x (),
		this->a.y () - R_o.y (), this->a.y () - this->c.y (), R_d.y (),
		this->a.z () - R_o.z (), this->a.z () - this->c.z (), R_d.z ());

	Matrix3f GammaM (this->a.x () - this->b.x (), this->a.x () - R_o.x (), R_d.x (),
		this->a.y () - this->b.y (), this->a.y () - R_o.y (), R_d.y (),
		this->a.z () - this->b.z (), this->a.z () - R_o.z (), R_d.z ());

	Matrix3f tM (this->a.x () - this->b.x (), this->a.x () - this->c.x (), this->a.x () - R_o.x (),
		this->a.y () - this->b.y (), this->a.y () - this->c.y (), this->a.y () - R_o.y (),
		this->a.z () - this->b.z (), this->a.z () - this->c.z (), this->a.z () - R_o.z ());

	float beta = BetaM.determinant () / A.determinant ();
	float gamma = GammaM.determinant () / A.determinant ();
	float alpha = 1.0f - beta - gamma;
	float t = tM.determinant () / A.determinant ();

	if (beta + gamma > 1) {
		return false;
	}
	if (beta < 0) {
		return false;
	}
	if (gamma < 0) {
		return false;
	}
	if (t > tmin && t < h.getT ()) {
		
		Vec3f newNormal = Vec3f (alpha*normal + beta * normal + gamma * normal.Normalized ());
		h.set (t, this->material, newNormal, r);
		return true;
	}
	else {
		return false;
	}
}

void Triangle::paint()
{
	material->glSetMaterial();
	glBegin(GL_TRIANGLES);
	glNormal3f(normal.x(), normal.y(), normal.z());
	glVertex3f(a.x(), a.y(), a.z());
	glVertex3f(b.x(), b.y(), b.z());
	glVertex3f(c.x(), c.y(), c.z());
	glEnd();
}

bool Triangle::inBound (Vec3f othermin, Vec3f othermax) {
	bool ret = false;
	ret = (m_min.x () <= othermax.x () + FLT_EPSILON) &&
		(m_min.y () <= othermax.y () + FLT_EPSILON) &&
		(m_min.z () <= othermax.z () + FLT_EPSILON) &&
		(m_max.x () >= othermin.x () - FLT_EPSILON) &&
		(m_max.y () >= othermin.y () - FLT_EPSILON) &&
		(m_max.z () >= othermin.z () - FLT_EPSILON);
	return ret;
}

void Triangle::insertIntoGrid(Grid *g, Matrix *m)
{
	//此三角形的包围盒
	m_min = boundingBox->getMin ();
	m_max = boundingBox->getMax ();

	//grid的包围盒
	Vec3f gird = g->getGird();
	BoundingBox *bb = g->getBoundingBox();
	Vec3f g_min =bb->getMin();
	Vec3f g_max = Vec3f(bb->getMax().x() + FLT_EPSILON, bb->getMax().y() + FLT_EPSILON, bb->getMax().z() + FLT_EPSILON);
	int nx = gird.x();	int ny = gird.y();	int nz = gird.z();
	Vec3f size = g_max - g_min;
	float grid_x = size.x() / nx;	float grid_y = size.y() / ny;	float grid_z = size.z() / nz;	

	//遍历grid的每个cell，如果三角形包围盒最小值大于它的最小值且最大值大于它的最大值，就置为真
	for (int _i = 0; _i < nx; _i++)
	{
		for (int _j = 0; _j < ny; _j++)
		{
			for (int _k = 0; _k < nz; _k++)
			{
				Vec3f cellMin (_i*grid_x, _j*grid_y, _k*grid_z); cellMin += g_min;
				Vec3f cellMax ((_i + 1)*grid_x, (_j + 1)*grid_y, (_k + 1)*grid_z); cellMax += g_min;
				if (inBound (cellMin, cellMax)) {
					g->insertIntoThis ((_i * ny + _j) * nz + _k, true, this);
				}
			}
		}
	}	
}