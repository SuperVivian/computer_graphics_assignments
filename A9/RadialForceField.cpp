#include "stdafx.h"
#include "RadialForceField.h"


RadialForceField::RadialForceField (float magnitude)
{
	this->magnitude = magnitude;
}

Vec3f RadialForceField::getAcceleration (const Vec3f &position, float mass, float t)
{
	//径向力场 加速度a=position*magnitude/m*（-1）
	//将粒子拉向原点，力的大小和到原点的距离成正比
	Vec3f force = position * magnitude;
	force /= mass;
	Vec3f a = force * -1.0f;

	return a;
}