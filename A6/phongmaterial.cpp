
#include "stdafx.h"
#include "phongmaterial.h"

PhongMaterial::PhongMaterial(const Vec3f &diffuseColor,const Vec3f &specularColor,float exponent,const Vec3f &reflectiveColor,const Vec3f &transparentColor,float indexOfRefraction)
	: Material(diffuseColor)
{
	this->specularColor = specularColor;
	this->reflectiveColor = reflectiveColor;
	this->transparentColor = transparentColor;
	this->indexOfRefraction = indexOfRefraction;
	this->exponent = exponent;
}

void PhongMaterial::glSetMaterial() const
{
	GLfloat one[4] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat zero[4] = { 0.0, 0.0, 0.0, 0.0 };
	GLfloat specular[4] = {
		getSpecularColor().r(),
		getSpecularColor().g(),
		getSpecularColor().b(),
		1.0 };
	GLfloat diffuse[4] = {
		getDiffuseColor().r(),
		getDiffuseColor().g(),
		getDiffuseColor().b(),
		1.0 };

	float glexponent = exponent;
	if (glexponent < 0) 
		glexponent = 0;
	if (glexponent > 128)
		glexponent = 128;

	glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, diffuse);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, specular);
	glMaterialfv(GL_FRONT_AND_BACK, GL_SHININESS, &glexponent);
}

 Vec3f PhongMaterial::shade (const Ray &ray, const Hit &hit, const Vec3f &dirToLight, const Vec3f &lightColor, bool shade_back) const
{
	Vec3f direction = ray.getDirection();
	Vec3f normal = hit.getNormal();	
	if (shade_back && direction.Dot3(normal) > 0){
		normal.Negate();
	}

	float diffuse = dirToLight.Dot3(normal);
	Vec3f reflection_out = normal * 2.0f * normal.Dot3(dirToLight) - dirToLight;
	float specular = -1.0f * direction.Dot3(reflection_out);

	if (diffuse < 0.0f){
		diffuse = 0.0f;
	}
	if (specular < 0.0f){
		specular = 0.0f;
	}

	Vec3f color = getDiffuseColor();
	color = color * diffuse + specularColor * pow(specular, exponent);
	color = color * lightColor;

	return color;
}