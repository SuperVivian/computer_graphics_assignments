#pragma once
#include "Generator.h"
class RingGenerator :
	public Generator
{
public:
	RingGenerator (float, Vec3f, float);
	int numNewParticles (float time, float dt)const;
	Particle* Generate (float current_time, int i);
	void Paint ()const;
private:
	Vec3f velocity;
	float position_randomness;
	float velocity_randomness;
};

