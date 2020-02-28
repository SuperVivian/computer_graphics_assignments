#ifndef _MATERIAL_H_
#define _MATERIAL_H_ 
#include "vectors.h"
#include"Hit.h"
#include"Ray.h"
#include"matrix.h"
#include "perlin_noise.h"
// ====================================================================
// ====================================================================
// You will extend this class in later assignments 

class Material
{
public:
	// CONSTRUCTORS & DESTRUCTOR 
	Material () { diffuseColor = Vec3f (0, 0, 0); };
	Material (const Vec3f &d_color)
	{
		diffuseColor = d_color;
	}

	virtual ~Material () {}
	// ACCESSORS 
	virtual Vec3f getDiffuseColor () const
	{
		return diffuseColor;
	} 
	virtual Vec3f getReflectiveColor ()const { return reflectiveColor; }
	virtual Vec3f getTransparentColor ()const { return transparentColor; }
	virtual float getIndexOfRefraction ()const { return indexOfRefraction; }
	virtual Vec3f Shade	(const Ray &ray, const Hit &hit, const Vec3f &dirToLight,const Vec3f &lightColor) const = 0;
	virtual void glSetMaterial (void) const {};
	virtual void setDiffuseColor (Vec3f color) 
	{
		diffuseColor=color;
	}

protected:
		// REPRESENTATION 
		Vec3f diffuseColor;
		Vec3f reflectiveColor;
		Vec3f transparentColor;
		float indexOfRefraction;
};

// ====================================================================
// ==================================================================== 

#endif 


