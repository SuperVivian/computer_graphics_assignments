#include "stdafx.h"
#include "marble.h"


Marble::Marble (Matrix *m, Material *mat1, Material *mat2, int _octaves, float freq, float amp)
{
	octaves = _octaves;//��Ƶ
	frequency = freq;//Ƶ�ʣ�������֮��ľ���
	amplitude = amp;//��������ֵ����Сֵ�Ĳ�ֵ
	this->m = m;
	this->mat1 = mat1;
	this->mat2 = mat2;
}


Marble::~Marble ()
{
}
Vec3f Marble::Shade (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor)const {
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
void Marble::glSetMaterial (void) const {
};