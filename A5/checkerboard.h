#pragma once
#include "Material.h"

class Checkerboard :public Material
{
public:
	Checkerboard (Matrix *m, Material *mat1, Material *mat2);
	virtual Vec3f Shade (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;
	static void glSetMaterial (void);
	~Checkerboard ();
	bool getPosMat (Vec3f pos)const;
private:
	Matrix * m;
	Material *mat1;
	Material *mat2;
};

