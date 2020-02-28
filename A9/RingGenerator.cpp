#include "stdafx.h"
#include "RingGenerator.h"

RingGenerator::RingGenerator (float position_randomness, Vec3f velocity, float velocity_randomness)
{
	this->velocity = Vec3f (velocity.x (), velocity.y (), velocity.z ());
	this->position_randomness = position_randomness;
	this->velocity_randomness = velocity_randomness;
}

int RingGenerator::numNewParticles (float time, float dt)const
{
	float num = dt * desired_num_particles / lifespan + time;
	return num;
}

Particle* RingGenerator::Generate (float current_time, int i)
{
	float radius = 3.5f + (1.0f * random->next () - 0.5f);
	float theta = random->next () * 2.0f * 3.14159f;
	Vec3f position_random = Vec3f (radius * sinf (theta), 0.0f, radius * cosf (theta));
	Vec3f velocity_random = velocity + Vec3f (0.707f * (2 * random->next () - 1.0f) *  velocity_randomness, 0.707f * (2 * random->next () - 1.0f) *  velocity_randomness, 0.0f);
	float mass_random = mass + (2 * random->next () - 1.0f) * mass_randomness;
	float lifespan_random = lifespan + (2 * random->next () - 1.0f) * lifespan_randomness;
	Particle* particles = new Particle (position_random, velocity_random, color, dead_color, mass_random, lifespan_random);
	return particles;
}

void RingGenerator::Paint ()const
{
	glBegin (GL_QUADS);
	glVertex3f (-5.0, 0.0, 5.0);
	glVertex3f (5.0, 0.0, 5.0);
	glVertex3f (5.0, 0.0, -5.0);
	glVertex3f (-5.0, 0.0, -5.0);
	glEnd ();
}
