#pragma once
#include "Material.h"
#include"perlin_noise.h"

class Marble :	public Material
{
public:
	Marble (Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude);
	~Marble ();
	virtual Vec3f shade (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor, bool shade_back)const;
	virtual void glSetMaterial (void) const;
private:
	Matrix * m;
	Material * mat1;
	Material *mat2;
	int octaves;
	float frequency;
	float amplitude;
};

