#pragma once
#include"vectors.h"
#include"ForceField.h"
#include"particle.h"
class Integrator
{
public:
	virtual void Update (Particle *p, ForceField *forcefield, float current_time, float dt) = 0;
	virtual Vec3f getColor ();
protected:
	Vec3f color;
};

