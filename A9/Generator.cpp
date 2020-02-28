#include "stdafx.h"
#include "generator.h"

Generator::Generator ()
{
	color_randomness = 0;
	mass = 0;
	mass_randomness = 0;
	lifespan = 0;
	lifespan_randomness = 0;
	desired_num_particles = 0;
	this->random = new Random (100);
}

void Generator::Restart ()
{
	delete this->random;
	this->random = new Random (100);
}

void Generator::SetColors (Vec3f color, Vec3f dead_color, float color_randomness)
{
	this->color = color;
	this->dead_color = dead_color;
	this->color_randomness = color_randomness;
}

void Generator::SetMass (float mass, float mass_randomness)
{
	this->mass = mass;
	this->mass_randomness = mass_randomness;
}

void Generator::SetLifespan (float lifespan, float lifespan_randomness, int desired_num_particles)
{
	this->lifespan = lifespan;
	this->lifespan_randomness = lifespan_randomness;
	this->desired_num_particles = desired_num_particles;
}
