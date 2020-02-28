#include "stdafx.h"
#include "HoseGenerator.h"

HoseGenerator::HoseGenerator (Vec3f position, float position_randomness, Vec3f velocity, float velocity_randomness)
{
	this->position = position;
	this->position_randomness = position_randomness;
	this->velocity = velocity;
	this->velocity_randomness = velocity_randomness;
}

Particle* HoseGenerator::Generate (float current_time, int i)
{
	//软管生成器：

	//随机位置
	Vec3f position_random = position + Vec3f (MID_SQRT2 * (2 * random->next () - 1.0f) *  position_randomness, MID_SQRT2 * (2 * random->next () - 1.0f) *  position_randomness, 0);
	//随机速度
	Vec3f velocity_random = velocity + Vec3f (MID_SQRT2 * (2 * random->next () - 1.0f) *  velocity_randomness, MID_SQRT2 * (2 * random->next () - 1.0f) *  velocity_randomness, 0);
	//随机质量
	float mass_random = mass + (2 * random->next () - 1.0f) * mass_randomness;
	//随机生命值
	float lifespan_random = lifespan + (2 * random->next () - 1.0f) * lifespan_randomness;

	Particle* particles = new Particle (position_random, velocity_random, color, dead_color, mass_random, lifespan_random);
	return particles;
}

int HoseGenerator::numNewParticles (float current_time, float dt)const
{
	int num = dt * desired_num_particles / lifespan;
	return num;
}

void HoseGenerator::Paint ()const
{

}
