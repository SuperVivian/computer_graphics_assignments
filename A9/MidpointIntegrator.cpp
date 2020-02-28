#include "stdafx.h"
#include "MidpointIntegrator.h"


void MidpointIntegrator::Update (Particle *p, ForceField *forcefield, float current_time, float dt)
{
	color = Vec3f (0, 1, 0);
	float mass = p->getMass ();
	Vec3f a = forcefield->getAcceleration (p->getPosition (), mass, current_time);
	Vec3f v = p->getVelocity ();
	Vec3f pm = p->getPosition () + v * (dt / 2);
	Vec3f vm = v + a * (dt / 2);
	Vec3f position = p->getPosition () + vm * dt;
	p->setPosition (position);
	Vec3f velocity = v + forcefield->getAcceleration (pm, mass, current_time + dt / 2)*dt;
	p->setVelocity (velocity);
	p->increaseAge (dt);
}
