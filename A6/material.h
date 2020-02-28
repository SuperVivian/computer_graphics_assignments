#ifndef _MATERIAL_H_
#define _MATERIAL_H_

#include "vectors.h"
#include "hit.h"
#include "ray.h"
#include"perlin_noise.h"

#include "matrix.h"
// ====================================================================
// ====================================================================

// You will extend this class in later assignments

class Material {

public:

	// CONSTRUCTORS & DESTRUCTOR
	Material () {};
	Material(const Vec3f &d_color) { diffuseColor = d_color; };
	virtual ~Material() {};

	// ACCESSORS
	virtual Vec3f getDiffuseColor() const { return diffuseColor; }
	Vec3f getSpecularColor() const { return specularColor; };
	Vec3f getReflectiveColor() const { return reflectiveColor; };
	Vec3f getTransparentColor() const { return transparentColor; };
	float getIndexOfRefraction() const{ return indexOfRefraction; };
	float getExponent() const { return exponent; };
	virtual void glSetMaterial() const = 0;
	virtual void setDiffuseColor (Vec3f color)
	{
		diffuseColor = color;
	}
	virtual Vec3f shade (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor, bool shade_back) const = 0;

protected:

	// REPRESENTATION
	Vec3f diffuseColor;
	Vec3f specularColor;
	Vec3f reflectiveColor;
	Vec3f transparentColor;

	float indexOfRefraction;
	float exponent;
};

// ====================================================================
// ====================================================================

#endif
