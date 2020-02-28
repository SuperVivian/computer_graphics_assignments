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

