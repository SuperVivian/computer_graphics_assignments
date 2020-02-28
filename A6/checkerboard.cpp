#include "stdafx.h"
#include "checkerboard.h"


Checkerboard::Checkerboard (Matrix *m, Material *mat1, Material *mat2)
{
	this->m = m;
	this->mat1 = mat1;
	this->mat2 = mat2;
}


Checkerboard::~Checkerboard ()
{
}
bool Checkerboard::getPosMat (Vec3f pos)const {
	Vec3f hitpos = pos;
	this->m->Transform (hitpos);
	int x = floor (hitpos.x ());	int y = floor (hitpos.y ());	int z = floor (hitpos.z ());
	return !((x % 2 + y % 2 + z % 2) % 2);
}
 Vec3f Checkerboard::shade (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor, bool shade_back) const {
	 Vec3f point = hit.getIntersectionPoint ();
	 bool isMat1 = getPosMat (point);//���ﲻ��������ֻ��ѡ�������Ӧ�����ĸ�����	
	 Hit tempHit;
	 if (isMat1) {
		 tempHit.set (hit.getT(),mat1,hit.getNormal(),ray);
	 }
	 else {
		 tempHit.set (hit.getT (), mat2, hit.getNormal (), ray);
	 }
	 return  hit.getMaterial ()->shade (ray, tempHit, dirToLight, lightColor,shade_back);
}
 void Checkerboard::glSetMaterial ()const {
	 mat1->glSetMaterial ();
 }

