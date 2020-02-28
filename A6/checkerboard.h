#pragma once
#include "Material.h"

class Checkerboard :public Material
{
public:
	Checkerboard (Matrix *m, Material *mat1, Material *mat2);
	virtual Vec3f shade (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor,bool shade_back) const;
	virtual void glSetMaterial () const;
	~Checkerboard ();
	bool getPosMat (Vec3f pos)const;
	Hit m_hit;
private:
	Matrix * m;
	Material *mat1;
	Material *mat2;

};

