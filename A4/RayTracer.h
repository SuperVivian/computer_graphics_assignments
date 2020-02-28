#pragma once
#include"SceneParser.h"
#include"Light.h"
#include"rayTree.h"

class RayTracer
{
public:
	RayTracer (SceneParser *s, int max_bounces, float cutoff_weight, bool shadows);
	~RayTracer ();
	Vec3f traceRay (Ray &ray, float tmin, int bounces, float weight,	float indexOfRefraction, Hit &hit);
	Vec3f mirrorDirection (const Vec3f &normal, const Vec3f &incoming);
	bool transmittedDirection (const Vec3f &normal, const Vec3f &incoming,
		float index_i, float index_t, Vec3f &transmitted);
	bool InShadow (Ray &ray, Hit& hit, float dis) const;
private:
	SceneParser * scene;
	int max_bounces;
	float cutoff_weight;
	bool shadows;
};

