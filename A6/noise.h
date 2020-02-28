#pragma once
#include "Material.h"

class Noise :	public Material
{
public:
	Noise (Matrix *m, Material *mat1, Material *mat2, int _octaves);
	~Noise ();
	virtual Vec3f shade (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor, bool shade_back) const;
	virtual void glSetMaterial (void) const;

private:
	Matrix * m;
	Material * mat1;
	Material *mat2;
	int octaves;
};

