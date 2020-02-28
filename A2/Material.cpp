#include"stdafx.h"
#include"Material.h"

 Vec3f Material::shade (const Ray& ray, const Hit& hit, const Vec3f& lightDir,const Vec3f& lightColor) {

	 Vec3f shadeColor;
	 Vec3f normal = hit.getNormal ();
	 // you'll need to flip the normal when the eye is on the "wrong" side of the surface (when the dot product of the ray direction & the normal is positive)
	 float wrong = ray.getDirection ().Dot3 (normal);
	 if (wrong > 0)
	 {
			 normal *= -1.0f;
	}

	 //diffuse
	 float d = normal.Dot3 (lightDir);
	 if (d < 0)d = 0;

	 shadeColor = lightColor * diffuseColor*d;

	 return shadeColor;
}