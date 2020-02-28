#pragma once
#include "Integrator.h"
class MidpointIntegrator :
	public Integrator
{
public:
	void Update (Particle *p, ForceField *forcefield, float current_time, float dt);
};

