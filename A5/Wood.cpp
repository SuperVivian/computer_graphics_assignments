#include "stdafx.h"
#include "Wood.h"


Wood::Wood (Matrix *m, Material *mat1, Material *mat2, int _octaves, float freq, float amp)
{
	octaves = _octaves;//倍频
	frequency = freq;//频率：两个点之间的距离
	amplitude = amp;//振幅：最大值和最小值的差值
	this->m = m;
	this->mat1 = mat1;
	this->mat2 = mat2;
}


Wood::~Wood ()
{
}
Vec3f Wood::Shade (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor)const {
	Vec3f hitpos = hit.getIntersectionPoint ();
	double answer = 0;
	for (int i = 0; i < octaves; i++)
	{
		float tmp = pow (2.0f, i);
		answer += PerlinNoise::noise (tmp*hitpos[0], tmp*hitpos[1], tmp*hitpos[2]) / float (tmp);
	}

	double m = 0.5 + 0.5 * sin (frequency * hitpos[0] + amplitude * answer);

	Hit tempHit;
	tempHit.set (hit.getT (), mat1, hit.getNormal (), ray);
	Vec3f color0 = hit.getMaterial ()->Shade (ray, tempHit, dirToLight, lightColor);

	tempHit.set (hit.getT (), mat2, hit.getNormal (), ray);
	Vec3f color1 = hit.getMaterial ()->Shade (ray, tempHit, dirToLight, lightColor);

	Vec3f color2 = m * color0 + (1 - m) * color1;
	return  color2;
}
void Wood::glSetMaterial (void) const {
};