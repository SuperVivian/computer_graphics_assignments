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
	Random * random;//������飬����������ͬsize

	Vec3f color;//��ʼ��ɫ
	Vec3f dead_color;//��ʧ��ɫ
	float color_randomness;//��ɫ�������

	float mass;//����
	float mass_randomness;//�������

	float lifespan;//��������
	float lifespan_randomness;//�����������

	int desired_num_particles;//��������
};

