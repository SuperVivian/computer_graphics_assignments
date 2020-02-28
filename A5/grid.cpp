#include "stdafx.h"
#include "grid.h"


Grid::Grid (BoundingBox *bb, int nx, int ny, int nz)
{
	this->boundingBox = bb;
	this->nx = nx;
	this->ny = ny;
	this->nz = nz;
	m_is_voxel_opaque.assign (nx * ny * nz, false);
	object3ds.resize (nx*ny*nz);
	objNum.assign (nx * ny * nz, 0);
}

Grid::~Grid () {

}

Vec3f Grid::getGird () {
	return Vec3f (nx, ny, nz);
}

BoundingBox * Grid::getBoundingBox ()
{
	return boundingBox;
}

void Grid::insertIntoThis (int i, bool v, Object3D *obj)
{
	m_is_voxel_opaque[i] = v;
	object3ds[i] = obj;
	objNum[i]++;
}

void Grid::changeMaterial (int i) {
	switch (objNum[i]) {
	case 1: material->setDiffuseColor (Vec3f (1, 1, 1)); break;//白
	case 2: material->setDiffuseColor (Vec3f (1, 0, 1)); break;//紫
	case 3: material->setDiffuseColor (Vec3f (0, 0, 1)); break;//蓝
	case 4: material->setDiffuseColor (Vec3f (0, 1, 0)); break;//緑
	case 5: material->setDiffuseColor (Vec3f (1, 1, 0)); break;//黄
	case 6: material->setDiffuseColor (Vec3f (0, 1, 1)); break;
	case 7: material->setDiffuseColor (Vec3f (0, 0, 0)); break;
	}
	if (objNum[i] >= 8) {
		material->setDiffuseColor (Vec3f (1, 0, 0));//红
	}
}
bool Grid::intersect (const Ray &r, Hit &h, float tmin)
{
	bool result = false;
	MarchingInfo mi;
	initializeRayMarch (mi, r, tmin);//初始化mi中的变量
	if (mi.tmin < h.getT ()) {
		while (mi.i < nx && mi.j < ny && mi.k < nz &&
			mi.i >= 0 && mi.j >= 0 && mi.k >= 0) {//在grid 的range内
			if (m_is_voxel_opaque[(mi.i * ny + mi.j) * nz + mi.k]) {
				if (object3ds[(mi.i * ny + mi.j) * nz + mi.k]->material == NULL)
					object3ds[(mi.i * ny + mi.j) * nz + mi.k]->material = material;
				h.set (mi.tmin, object3ds[(mi.i * ny + mi.j) * nz + mi.k]->material, mi.normal, r);
				result = true;
				break;
			}
			mi.nextCell ();
		}
	}
	return result;
}

//OpenGL 显示
void Grid::paint ()
{	
	Vec3f min = Vec3f (boundingBox->getMin ().x () - FLT_EPSILON, boundingBox->getMin ().y () - FLT_EPSILON, boundingBox->getMin ().z () - FLT_EPSILON);
	Vec3f max = Vec3f (boundingBox->getMax ().x () + FLT_EPSILON, boundingBox->getMax ().y () + FLT_EPSILON, boundingBox->getMax ().z () + FLT_EPSILON);
	Vec3f size = max - min;
	float grid_x = size.x () / nx;
	float grid_y = size.y () / ny;
	float grid_z = size.z () / nz;//根据group boundingbox 的大小来确定cell的大小
	Vec3f center = boundingBox->getMin ();
	glPushMatrix ();
	glTranslatef (center.x (), center.y (), center.z ());//以最小点作为原点
	for (int i = 0; i < nx*ny*nz; i++)
	{
		if (m_is_voxel_opaque[i]) {
			int kk = i % nz;
			int jj = ((i - kk) / nz) % ny;
			int ii = (((i - kk) / nz) - jj) / ny;
			changeMaterial (i);
			material->glSetMaterial ();
			float x0 = ii * grid_x, x1 = (ii + 1)*grid_x;
			float y0 = jj * grid_y, y1 = (jj + 1)*grid_y;
			float z0 = kk * grid_z, z1 = (kk + 1)*grid_z;

			glBegin (GL_QUADS);
			//底
			glNormal3f (0,-1,0);
			glVertex3f (x0,y0,z1);// 0 0 1
			glVertex3f (x1,y0,z1);// 1 0 1
			glVertex3f (x1,y0,z0);// 1 0 0 
			glVertex3f (x0,y0,z0);//0 0 0
			//左
			glNormal3f (-1, 0, 0);
			glVertex3f (ii*grid_x, jj*grid_y, (kk + 1)*grid_z);// 0 0 1
			glVertex3f (ii*grid_x, (jj + 1)*grid_y, (kk + 1)*grid_z);// 0 1 1
			glVertex3f (ii*grid_x, (jj + 1)*grid_y, kk*grid_z);// 0 1 0
			glVertex3f (ii*grid_x, jj*grid_y, kk*grid_z);// 0 0 0
			//顶
			glNormal3f (0, 1, 0);
			glVertex3f (ii*grid_x, (jj + 1)*grid_y, (kk + 1)*grid_z);// 0 1 1
			glVertex3f ((ii + 1)*grid_x, (jj + 1)*grid_y, (kk + 1)*grid_z);// 1 1 1
			glVertex3f ((ii + 1)*grid_x, (jj + 1)*grid_y, kk*grid_z);// 1 1 0
			glVertex3f (ii*grid_x, (jj + 1)*grid_y, kk*grid_z);// 0 1 0
			//右
			glNormal3f (1, 0, 0);
			glVertex3f ((ii + 1)*grid_x, (jj + 1)*grid_y, kk*grid_z);//1 1 0
			glVertex3f ((ii + 1)*grid_x, (jj + 1)*grid_y, (kk + 1)*grid_z);//1 1 1
			glVertex3f ((ii + 1)*grid_x, jj*grid_y, (kk + 1)*grid_z);// 1 0 1
			glVertex3f ((ii + 1)*grid_x, jj*grid_y, kk*grid_z);// 1 0 0
			//后
			glNormal3f (0, 0, -1);
			glVertex3f (ii*grid_x, jj*grid_y, kk*grid_z);//0 0 0
			glVertex3f (ii*grid_x, (jj + 1)*grid_y, kk*grid_z);//0 1 0
			glVertex3f ((ii + 1)*grid_x, (jj + 1)*grid_y, kk*grid_z);//1 1 0
			glVertex3f ((ii + 1)*grid_x, jj*grid_y, kk*grid_z);//1 0 0
			//前
			glNormal3f (0, 0, 1);
			glVertex3f (ii*grid_x, jj*grid_y, (kk + 1)*grid_z);// 0 0 1
			glVertex3f ((ii + 1)*grid_x, jj*grid_y, (kk + 1)*grid_z);//1 0 1
			glVertex3f ((ii + 1)*grid_x, (jj + 1)*grid_y, (kk + 1)*grid_z);//1 1 1
			glVertex3f (ii*grid_x, (jj + 1)*grid_y, (kk + 1)*grid_z);//0 1 1
			glEnd ();
		}
	}
	glPopMatrix ();
}

void Grid::initializeRayMarch (MarchingInfo &mi,	const Ray &r, float tmin) const
{
	const Vec3f D = r.getDirection ();
	const Vec3f O = r.getOrigin ();
	float _tnear = tmin;	float _tfar = INFINITY;
	Vec3f _n (0.0f, 0.0f, 0.0f);
	//对每一维，计算交点距离t1,t2
	// x 轴
	float _dirx = D.x ();
	float _t1x = (boundingBox->getMin ().x () - O.x ()) / _dirx;//假设包围盒的min点更近
	float _t2x = (boundingBox->getMax ().x () - O.x ()) / _dirx;//假设包围盒的max点更远
	if (_t1x > _t2x) { float _tmp = _t1x; _t1x = _t2x; _t2x = _tmp; }//若大小不对，则交换
	// y 轴
	float _diry = D.y ();
	float _t1y = (boundingBox->getMin ().y () - O.y ()) / _diry;
	float _t2y = (boundingBox->getMax ().y () - O.y ()) / _diry;
	if (_t1y > _t2y) { float _tmp = _t1y; _t1y = _t2y; _t2y = _tmp; }
	// z 轴
	float _dirz = D.z ();
	float _t1z = (boundingBox->getMin ().z () - O.z ()) / _dirz;
	float _t2z = (boundingBox->getMax ().z () - O.z ()) / _dirz;
	if (_t1z > _t2z) { float _tmp = _t1z; _t1z = _t2z; _t2z = _tmp; }
	//更新每一维的tnear和tfar
	_tnear = (_t1x > _tnear) ? _t1x : _tnear;
	_tnear = (_t1y > _tnear) ? _t1y : _tnear;
	_tnear = (_t1z > _tnear) ? _t1z : _tnear;
	_tfar = (_t2x < _tfar) ? _t2x : _tfar;
	_tfar = (_t2y < _tfar) ? _t2y : _tfar;
	_tfar = (_t2z < _tfar) ? _t2z : _tfar;
	if (_tfar < _tnear) return; // missed
	mi.tmin = _tnear;//初始化tmin（是射线和包围盒最近的交点，也是起点）
	//标志位
	mi.sign_x = _dirx>0 ? 1 : -1;
	mi.sign_y = _diry>0 ? 1 : -1;
	mi.sign_z = _dirz>0 ? 1 : -1;
	//计算法线
	if (_tnear == _t1x) _n += Vec3f (-1.0f, 0.0f, 0.0f) * mi.sign_x;
	if (_tnear == _t1y) _n += Vec3f (0.0f, -1.0f, 0.0f) * mi.sign_y;
	if (_tnear == _t1z) _n += Vec3f (0.0f, 0.0f, -1.0f) * mi.sign_z;
	_n.Normalize ();
	//获得单元格大小
	Vec3f min = boundingBox->getMin ();
	Vec3f max = Vec3f (boundingBox->getMax ().x () + FLT_EPSILON, boundingBox->getMax ().y () + FLT_EPSILON, boundingBox->getMax ().z () + FLT_EPSILON);
	Vec3f size = max - min;
	float grid_x = size.x () / nx;
	float grid_y = size.y () / ny;
	float grid_z = size.z () / nz;
	//计算行进增量dt_x,y,z，必须取绝对值
	mi.dt_x = fabs (grid_x / _dirx);
	mi.dt_y = fabs (grid_y / _diry);
	mi.dt_z = fabs (grid_z / _dirz);
	//计算tnext_x,y,z
	if (_t1x > -INFINITY) {
		while (_t1x <= _tnear) { _t1x += mi.dt_x; }
		mi.tnext_x = _t1x;
	}
	if (_t1y > -INFINITY) {
		while (_t1y <= _tnear) { _t1y += mi.dt_y; }
		mi.tnext_y = _t1y;
	}
	if (_t1z> -INFINITY) {
		while (_t1z <= _tnear) { _t1z += mi.dt_z; }
		mi.tnext_z = _t1z;
	}
	Vec3f _rp = O + D * _tnear - boundingBox->getMin ();//把min作为原点
	//通过划分坐标，强制取整，得到起始cell的索引
	_rp.Divide (grid_x, grid_y, grid_z);
	mi.i = static_cast<int>(_rp.x ());
	if (mi.sign_x < 0 && mi.i == nx) mi.i--;//超出边界，-1
	mi.j = static_cast<int>(_rp.y ()); 
	if (mi.sign_y < 0 && mi.j == ny) mi.j--;
	mi.k = static_cast<int>(_rp.z ());
	if (mi.sign_z < 0 && mi.k == nz) mi.k--;
	mi.normal = _n;
	/*const Vec3f D = r.getDirection ();
	const Vec3f O = r.getOrigin ();*/

	//float _tnear = tmin;
	//float _tfar = INFINITY;
	//Vec3f _n (0.0f, 0.0f, 0.0f);

	//assert (_tnear >= 0);

	//// x axy
	//float _dx = D.x ();
	//float _t1x = (boundingBox->getMin ().x () - O.x ()) / _dx;
	//float _t2x = (boundingBox->getMax ().x () - O.x ()) / _dx;
	//if (_t1x > _t2x) { float _tmp = _t1x; _t1x = _t2x; _t2x = _tmp; }

	//// y axy
	//float _dy = D.y ();
	//float _t1y = (boundingBox->getMin ().y () - O.y ()) / _dy;
	//float _t2y = (boundingBox->getMax ().y () - O.y ()) / _dy;
	//if (_t1y > _t2y) { float _tmp = _t1y; _t1y = _t2y; _t2y = _tmp; }

	//// z axy
	//float _dz = D.z ();
	//float _t1z = (boundingBox->getMin ().z () - O.z ()) / _dz;
	//float _t2z = (boundingBox->getMax ().z () - O.z ()) / _dz;
	//if (_t1z > _t2z) { float _tmp = _t1z; _t1z = _t2z; _t2z = _tmp; }

	//_tnear = (_t1x > _tnear) ? _t1x : _tnear;
	//_tnear = (_t1y > _tnear) ? _t1y : _tnear;
	//_tnear = (_t1z > _tnear) ? _t1z : _tnear;

	//_tfar = (_t2x < _tfar) ? _t2x : _tfar;
	//_tfar = (_t2y < _tfar) ? _t2y : _tfar;
	//_tfar = (_t2z < _tfar) ? _t2z : _tfar;

	//if (_tfar < _tnear) return; // missed

	//mi.sign_x = _dx>0 ? 1 : -1;
	//mi.sign_y = _dy>0 ? 1 : -1;
	//mi.sign_z = _dz>0 ? 1 : -1;

	//if (_tnear == _t1x) _n += Vec3f (-1.0f, 0.0f, 0.0f) * mi.sign_x;
	//if (_tnear == _t1y) _n += Vec3f (0.0f, -1.0f, 0.0f) * mi.sign_y;
	//if (_tnear == _t1z) _n += Vec3f (0.0f, 0.0f, -1.0f) * mi.sign_z;
	//_n.Normalize ();

	//Vec3f min = boundingBox->getMin ();
	//Vec3f max = Vec3f (boundingBox->getMax ().x () + FLT_EPSILON, boundingBox->getMax ().y () + FLT_EPSILON, boundingBox->getMax ().z () + FLT_EPSILON);
	//Vec3f size = max - min;
	//float grid_x = size.x () / nx;
	//float grid_y = size.y () / ny;
	//float grid_z = size.z () / nz;

	//mi.tmin = _tnear;
	//mi.dt_x = fabs (grid_x / _dx);
	//mi.dt_y = fabs (grid_y / _dy);
	//mi.dt_z = fabs (grid_z / _dz);
	//if (_t1x > -INFINITY) {
	//	while (_t1x <= _tnear) { _t1x += mi.dt_x; }
	//	mi.tnext_x = _t1x;
	//}
	//if (_t1y > -INFINITY) {
	//	while (_t1y <= _tnear) { _t1y += mi.dt_y; }
	//	mi.tnext_y = _t1y;
	//}
	//if (_t1z> -INFINITY) {
	//	while (_t1z <= _tnear) { _t1z += mi.dt_z; }
	//	mi.tnext_z = _t1z;
	//}
	//Vec3f _rp = O + D * _tnear - boundingBox->getMin ();
	//_rp.Divide (grid_x, grid_y, grid_z);
	//mi.i = static_cast<int>(_rp.x ()); if (mi.sign_x < 0 && mi.i == nx) mi.i--;
	//mi.j = static_cast<int>(_rp.y ()); if (mi.sign_y < 0 && mi.j == ny) mi.j--;
	//mi.k = static_cast<int>(_rp.z ()); if (mi.sign_z < 0 && mi.k == nz) mi.k--;
	//mi.normal = _n;
}
