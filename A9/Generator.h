#pragma once
#include"particle.h"
#include"vectors.h"
#include"random.h"
class Generator
{
public:
	Generator ();
	virtual void Restart ();
	virtual int numNewParticles (float time, float dt)const = 0;
	virtual Particle* Generate (float current_time, int i) = 0;
	virtual void Paint ()const = 0;
	virtual void SetColors (Vec3f color, Vec3f dead_color, float color_randomness);
	virtual void SetMass (float mass, float mass_randomness);
	virtual void SetLifespan (float lifespan, float lifespan_randomness, int desired_num_particles);
protected:
	Random * random;//随机数组，和粒子数组同size

	Vec3f color;//初始颜色
	Vec3f dead_color;//消失颜色
	float color_randomness;//颜色随机参数

	float mass;//质量
	float mass_randomness;//质量随机

	float lifespan;//生命周期
	float lifespan_randomness;//生命周期随机

	int desired_num_particles;//粒子数量
};

