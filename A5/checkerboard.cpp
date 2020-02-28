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
	int x = floor (hitpos.x ());
	int y = floor (hitpos.y ());
	int z = floor (hitpos.z ());
	return !((x % 2 + y % 2 + z % 2) % 2);
}
 Vec3f Checkerboard::Shade (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const {
	 Vec3f point = hit.getIntersectionPoint ();
	 bool isMat1 = getPosMat (point);//这里不做工作，只是选择这个点应该用哪个材质	
	 Hit tempHit;
	 if (isMat1) {
		 tempHit.set (hit.getT(),mat1,hit.getNormal(),ray);
	 }
	 else {
		 tempHit.set (hit.getT (), mat2, hit.getNormal (), ray);
	 }
	 return  hit.getMaterial ()->Shade (ray, tempHit, dirToLight, lightColor);
}
 void Checkerboard::glSetMaterial (void) {

 }

