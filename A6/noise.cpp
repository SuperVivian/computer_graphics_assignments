#include "stdafx.h"
#include "noise.h"

Noise::Noise (Matrix *m, Material *mat1, Material *mat2, int _octaves)
{
	this->m = m;
	octaves = _octaves;
	this->mat1 = mat1;
	this->mat2 = mat2;
}

Noise::~Noise ()
{
}
 Vec3f Noise::shade (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor,bool shade_back)const {

	 Vec3f hitpos = hit.getIntersectionPoint ();
	 this->m->Transform (hitpos);

	 double answer = 0;
	 for (int i = 0; i < octaves; i++)
	 {
		 float tmp = pow (2.0f, i);
		 answer += PerlinNoise::noise (tmp*hitpos[0], tmp*hitpos[1], tmp*hitpos[2]) / float (tmp);
	 }

	 Hit tempHit;
     tempHit.set (hit.getT (), mat1, hit.getNormal (), ray);
	 Vec3f color0 = hit.getMaterial ()->shade (ray, tempHit, dirToLight, lightColor,shade_back);

	 tempHit.set (hit.getT (), mat2, hit.getNormal (), ray);
	 Vec3f color1 = hit.getMaterial ()->shade (ray, tempHit, dirToLight, lightColor,shade_back);

	 Vec3f color2 = answer * color0 + (1 - answer) * color1;
	 return  color2;
}
 void Noise::glSetMaterial (void) const {



 };
 
