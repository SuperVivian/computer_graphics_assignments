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

bool Triangle::intersect (const Ray &r, Hit &h, float tmin) {
	//bool intersect = false;
	////Ro + t * Rd = a + B(b-a) + G(c-a) 
	////if    B+G < 1 &  B > 0   &   G > 0  intersect
	////用三阶行列式求解三元一次方程组
	//Vec3f origin = r.getOrigin ();
	//Vec3f dir = r.getDirection ();
	//Vec3f ao = a - origin;
	//float A = Matrix::Determinant (ab,ac,dir);
	//float B = Matrix::Determinant (ao,ac,dir) / A;
	//float G = Matrix::Determinant (ab,ao,dir) / A;
	//float  t = Matrix::Determinant (ab,ac,ao) / A;
	//if (t > tmin && B>0 && G>0 && (B + G)>0 && t < h.getT () ) {
	//	h.set (t,mat,normal,r);
	//	intersect = true;
	//}
	//return intersect;
	Vec3f R_o = r.getOrigin ();
	Vec3f R_d = r.getDirection ();

	Matrix3f A (this->a.x () - this->b.x (), this->a.x () - this->c.x (), R_d.x (),
		this->a.y () - this->b.y (), this->a.y () - this->c.y (), R_d.y (),
		this->a.z () - this->b.z (), this->a.z () - this->c.z (), R_d.z ());

	Matrix3f BetaM (this->a.x () - R_o.x (), this->a.x () - this->c.x (), R_d.x (),
		this->a.y () - R_o.y (), this->a.y () - this->c.y (), R_d.y (),
		this->a.z () - R_o.z (), this->a.z () - this->c.z (), R_d.z ());

	float beta = BetaM.determinant () / A.determinant ();

	Matrix3f GammaM (this->a.x () - this->b.x (), this->a.x () - R_o.x (), R_d.x (),
		this->a.y () - this->b.y (), this->a.y () - R_o.y (), R_d.y (),
		this->a.z () - this->b.z (), this->a.z () - R_o.z (), R_d.z ());

	float gamma = GammaM.determinant () / A.determinant ();

	float alpha = 1.0f - beta - gamma;

	Matrix3f tM (this->a.x () - this->b.x (), this->a.x () - this->c.x (), this->a.x () - R_o.x (),
		this->a.y () - this->b.y (), this->a.y () - this->c.y (), this->a.y () - R_o.y (),
		this->a.z () - this->b.z (), this->a.z () - this->c.z (), this->a.z () - R_o.z ());

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

