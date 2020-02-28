#pragma once
#include "Material.h"
class Noise :
	public Material
{
public:
	Noise (Matrix *m, Material *mat1, Material *mat2, int _octaves);
	~Noise ();
	virtual Vec3f Shade (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;
	virtual void glSetMaterial (void) const;

private:
	Matrix * m;
	Material * mat1;
	Material *mat2;
	int octaves;
};

