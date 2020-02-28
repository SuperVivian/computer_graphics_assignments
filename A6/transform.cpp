
#include "stdafx.h"
#include "transform.h"

Transform::Transform(Matrix &m, Object3D *o)
{
	this->transform_matrix = m;
	this->object3d = o;
	this->material = o->material;
	boundingBox = TransformBoundingBox ();
}

bool Transform::intersect(const Ray &r, Hit &h, float tmin)
{
	bool result = false;
	Matrix tmp1_matrix = transform_matrix;
	if (tmp1_matrix.Inverse()){
		Vec3f direction = r.getDirection();
		Vec3f origin = r.getOrigin();
		tmp1_matrix.Transform(origin);
		tmp1_matrix.TransformDirection(direction);
		Ray ray = Ray(origin, direction);
		result = object3d->intersect(ray, h, tmin);
		if (result)
		{
			Vec3f n = h.getNormal();
			Matrix tmp2_matrix = tmp1_matrix;
			tmp2_matrix.Transpose();
			tmp2_matrix.TransformDirection(n);
			n.Normalize();
			h.set(h.getT(), h.getMaterial(), n, r);
		}
	}
	return result;
}

void Transform::paint()
{
	glPushMatrix();
	Matrix tmp = Matrix(transform_matrix);
	GLfloat *glMatrix = tmp.glGet();
	glMultMatrixf(glMatrix);
	delete[] glMatrix;
	object3d->paint();
	glPopMatrix();
}

BoundingBox* Transform::TransformBoundingBox () {
	Matrix m_matrix = transform_matrix;
	if (object3d->triangle) {
		Vec3f a = object3d->a;
		Vec3f b = object3d->b;
		Vec3f c = object3d->c;
		m_matrix.Transform (a);
		m_matrix.Transform (b);
		m_matrix.Transform (c);
		boundingBox = new BoundingBox (Vec3f (MIN (a.x (), b.x (), c.x ()), MIN (a.y (), b.y (), c.y ()), MIN (a.z (), b.z (), c.z ())), Vec3f (MAX (a.x (), b.x (), c.x ()), MAX (a.y (), b.y (), c.y ()), MAX (a.z (), b.z (), c.z ())));
		return boundingBox;
	}
	BoundingBox *bb = object3d->getBoundingBox ();
	Vec3f _v1 = bb->getMax ();
	Vec3f _v2 = bb->getMin ();
	float _x1 = _v1.x (), _y1 = _v1.y (), _z1 = _v1.z ();
	float _x2 = _v2.x (), _y2 = _v2.y (), _z2 = _v2.z ();
	Vec3f _v3 (_x2, _y1, _z1);	Vec3f _v4 (_x2, _y2, _z1);
	Vec3f _v5 (_x1, _y2, _z1);	Vec3f _v6 (_x1, _y2, _z2);
	Vec3f _v7 (_x1, _y1, _z2);	Vec3f _v8 (_x2, _y1, _z2);

	m_matrix.Transform (_v1);	m_matrix.Transform (_v2);	m_matrix.Transform (_v3);	m_matrix.Transform (_v4);
	m_matrix.Transform (_v5);	m_matrix.Transform (_v6);	m_matrix.Transform (_v7);	m_matrix.Transform (_v8);

	_x1 = _v1.x (), _y1 = _v1.y (), _z1 = _v1.z ();	_x2 = _v2.x (), _y2 = _v2.y (), _z2 = _v2.z ();
	float _x3 = _v3.x (), _y3 = _v3.y (), _z3 = _v3.z ();	float _x4 = _v4.x (), _y4 = _v4.y (), _z4 = _v4.z ();
	float _x5 = _v5.x (), _y5 = _v5.y (), _z5 = _v5.z ();	float _x6 = _v6.x (), _y6 = _v6.y (), _z6 = _v6.z ();
	float _x7 = _v7.x (), _y7 = _v7.y (), _z7 = _v7.z ();	float _x8 = _v8.x (), _y8 = _v8.y (), _z8 = _v8.z ();

	float _xmin = _x1 < _x2 ? _x1 : _x2;	_xmin = _xmin < _x3 ? _xmin : _x3;
	_xmin = _xmin < _x4 ? _xmin : _x4;	_xmin = _xmin < _x5 ? _xmin : _x5;
	_xmin = _xmin < _x6 ? _xmin : _x6;	_xmin = _xmin < _x7 ? _xmin : _x7;
	_xmin = _xmin < _x8 ? _xmin : _x8;

	float _ymin = _y1 < _y2 ? _y1 : _y2;	_ymin = _ymin < _y3 ? _ymin : _y3;
	_ymin = _ymin < _y4 ? _ymin : _y4;	_ymin = _ymin < _y5 ? _ymin : _y5;
	_ymin = _ymin < _y6 ? _ymin : _y6;	_ymin = _ymin < _y7 ? _ymin : _y7;
	_ymin = _ymin < _y8 ? _ymin : _y8;

	float _zmin = _z1 < _z2 ? _z1 : _z2;	_zmin = _zmin < _z3 ? _zmin : _z3;
	_zmin = _zmin < _z4 ? _zmin : _z4;	_zmin = _zmin < _z5 ? _zmin : _z5;
	_zmin = _zmin < _z6 ? _zmin : _z6;	_zmin = _zmin < _z7 ? _zmin : _z7;
	_zmin = _zmin < _z8 ? _zmin : _z8;

	float _xmax = _x1 > _x2 ? _x1 : _x2;	_xmax = _xmax > _x3 ? _xmax : _x3;
	_xmax = _xmax > _x4 ? _xmax : _x4;	_xmax = _xmax > _x5 ? _xmax : _x5;
	_xmax = _xmax > _x6 ? _xmax : _x6;	_xmax = _xmax > _x7 ? _xmax : _x7;
	_xmax = _xmax > _x8 ? _xmax : _x8;

	float _ymax = _y1 > _y2 ? _y1 : _y2;	_ymax = _ymax > _y3 ? _ymax : _y3;
	_ymax = _ymax > _y4 ? _ymax : _y4;	_ymax = _ymax > _y5 ? _ymax : _y5;
	_ymax = _ymax > _y6 ? _ymax : _y6;	_ymax = _ymax > _y7 ? _ymax : _y7;
	_ymax = _ymax > _y8 ? _ymax : _y8;

	float _zmax = _z1 > _z2 ? _z1 : _z2;	_zmax = _zmax > _z3 ? _zmax : _z3;
	_zmax = _zmax > _z4 ? _zmax : _z4;	_zmax = _zmax > _z5 ? _zmax : _z5;
	_zmax = _zmax > _z6 ? _zmax : _z6;	_zmax = _zmax > _z7 ? _zmax : _z7;
	_zmax = _zmax > _z8 ? _zmax : _z8;
	boundingBox = new BoundingBox (Vec3f (_xmax, _ymax, _zmax), Vec3f (_xmin, _ymin, _zmin));

	return boundingBox;
}

bool inBound (Vec3f m_min, Vec3f m_max, Vec3f othermin, Vec3f othermax) {
	bool ret = false;
	ret = (m_min.x () <= othermax.x () + FLT_EPSILON) &&
		(m_min.y () <= othermax.y () + FLT_EPSILON) &&
		(m_min.z () <= othermax.z () + FLT_EPSILON) &&
		(m_max.x () >= othermin.x () - FLT_EPSILON) &&
		(m_max.y () >= othermin.y () - FLT_EPSILON) &&
		(m_max.z () >= othermin.z () - FLT_EPSILON);
	return ret;
}
void Transform::insertIntoGrid(Grid *g, Matrix *m)
{	
	Vec3f m_min = boundingBox->getMin ();	Vec3f m_max = boundingBox->getMax ();
	Vec3f v = g->getGird ();
	BoundingBox *bb = g->getBoundingBox ();
	Vec3f min = bb->getMin ();
	Vec3f max = Vec3f (bb->getMax ().x () + FLT_EPSILON, bb->getMax ().y () + FLT_EPSILON, bb->getMax ().z () + FLT_EPSILON);
	int x = v.x ();	int y = v.y ();	int z = v.z ();
	Vec3f size = max - min;
	float grid_x = size.x () / x;	float grid_y = size.y () / y;	float grid_z = size.z () / z;

	int _start_i = (fabs (m_min.x () - min.x ())) * (1 / grid_x);	int _start_j = (fabs (m_min.y () - min.y ())) * (1 / grid_y);
	int _start_k = (fabs (m_min.z () - min.z ())) * (1 / grid_z);	int _end_i = (fabs (m_max.x () - min.x ()))*(1 / grid_x);
	int _end_j = (fabs (m_max.y () - min.y ()))*(1 / grid_y);	int _end_k = (fabs (m_max.z () - min.z ()))*(1 / grid_z);

	if (_start_i == _end_i) _start_i--;
	if (_start_j == _end_j) _start_j--;
	if (_start_k == _end_k) _start_k--;
	if (_start_i > _end_i) swap (_start_i, _end_i);
	if (_start_j > _end_j) swap (_start_j, _end_j);
	if (_start_k > _end_k) swap (_start_k, _end_k);

	if (_end_i > x)_end_i--;	if (_end_j > y)_end_j--;	if (_end_k > z)_end_k--;

	for (int _i = _start_i; _i < _end_i; _i++) {
		for (int _j = _start_j; _j < _end_j; _j++) {
			for (int _k = _start_k; _k < _end_k; _k++) {
				g->insertIntoThis ((_i * y + _j) * z + _k, true, this);
			}
		}
	}
}