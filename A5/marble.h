#pragma once
#include "Material.h"
class Marble :	public Material
{
public:
	Marble (Matrix *m, Material *mat1, Material *mat2, int octaves, float frequency, float amplitude);
	~Marble ();
	virtual Vec3f Shade (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor)const;
	virtual void glSetMaterial (void) const;
private:
	Matrix * m;
	Material * mat1;
	Material *mat2;
	int octaves;
	float frequency;
	float amplitude;
};

