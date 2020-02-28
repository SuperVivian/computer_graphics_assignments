#pragma once
#include "Material.h"
#include "glCanvas.h"  

#ifdef SPECULAR_FIX
// OPTIONAL:  global variable allows (hacky) communication 
// with glCanvas::display
extern int SPECULAR_FIX_WHICH_PASS;
#endif

class PhongMaterial :
	public Material
{
public:
	PhongMaterial (const Vec3f &diffuseColor, const Vec3f &specularColor, float exponent);
	~PhongMaterial ();
	virtual Vec3f Shade (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor) const;
	virtual void glSetMaterial (void) const;
	Vec3f getSpecularColor ()const { return specularColor; }
private:
	Vec3f specularColor;
	float exponent;
};

