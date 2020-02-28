#include "stdafx.h"
#include "Triangle.h"


Triangle::Triangle (Vec3f &a, Vec3f &b, Vec3f &c, Material *m)
{	
		this->a = a;
		this->b = b;
		this->c = c;
		this->mat = m;
		ab = a - b; ac = a - c; bc = b - c;
		Vec3f::Cross3 (normals[0],ab,ac);
		normals[1] = normals[2] = normals[0];
}


Triangle::~Triangle ()
{
}

void Triangle::paint () {
	mat->glSetMaterial ();
	glBegin (GL_TRIANGLES);
	glNormal3f (normals[0].x (), normals[0].y (), normals[0].z ());
	glVertex3f (a.x (),a.y(),a.z());
	glVertex3f (b.x(),b.y(),b.z());
	glVertex3f (c.x(),c.y(),c.z());
	glEnd ();
}

bool Triangle::intersect (const Ray &r, Hit &h, float tmin) {
	//Ro + t * Rd = a + B(b-a) + G(c-a) 
	//if    B+G < 1 &  B > 0   &   G > 0  intersect
	//用三阶行列式求解三元一次方程组
	RayTracingStats::IncrementNumIntersections ();
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

		Vec3f newNormal =Vec3f(alpha*normals[0] + beta *normals[1] + gamma *normals[2]).Normalized();
		h.set (t, this->mat, newNormal,r);
		return true;
	}
	else {
		return false;
	}
}

BoundingBox* Triangle::getBoundingBox ()
{
	boundingBox = new BoundingBox (
		Vec3f (MIN (a.x (), b.x (), c.x ()), MIN (a.y (), b.y (), c.y ()), MIN (a.z (), b.z (), c.z ())), 
		Vec3f (MAX (a.x (), b.x (), c.x ()), MAX (a.y (), b.y (), c.y ()), MAX (a.z (), b.z (), c.z ()))
	);
	return boundingBox;
}

void Triangle::insertIntoGrid (Grid *g, Matrix *m)
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
	//取得开始和结束的索引
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
