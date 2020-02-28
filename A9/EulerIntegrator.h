#pragma once
#include "Integrator.h"
class EulerIntegrator :
	public Integrator
{
public:
	void Update (Particle *p, ForceField *forcefield, float current_time, float dt);
};

