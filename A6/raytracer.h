#ifndef RAYTARCER
#define RAYTARCER

#include "vectors.h"
#include "material.h"
#include "matrix.h"
#include "scene_parser.h"
#include "group.h"
#include "rayTree.h"
#include "light.h"
#include "ray.h"


class RayTracer
{
public:
	RayTracer(SceneParser *s, int max_bounces, float cutoff_weight, bool shadows, Vec3f g, bool isG, bool isVisual);
	Vec3f traceRay(Ray &ray, float tmin, int bounces, float weight, float indexOfRefraction, Hit &hit, bool shade_back, bool first) const;
	Vec3f mirrorDirection(const Vec3f &normal, const Vec3f &incoming) const;
	bool transmittedDirection(const Vec3f &normal, const Vec3f &incoming, float index_i, float index_t, Vec3f &transmitted) const;
	Grid *getGrid(){ return grid; }
private:
	float cutoff_weight;
	int max_bounces;
	bool shadows;
	SceneParser* scene_parser;
	Grid *grid;
};

#endif