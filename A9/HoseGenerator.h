#pragma once
#include "Generator.h"
class HoseGenerator :
	public Generator
{
public:
	HoseGenerator (Vec3f, float, Vec3f, float);
	int numNewParticles (float current_time, float dt) const;
	void Paint ()const;
	Particle* Generate (float current_time, int i);
protected:
	Vec3f position;
	float position_randomness;//λ���������
	Vec3f velocity;
	float velocity_randomness;
};

