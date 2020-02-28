#include "stdafx.h"
#include "RadialForceField.h"


RadialForceField::RadialForceField (float magnitude)
{
	this->magnitude = magnitude;
}

Vec3f RadialForceField::getAcceleration (const Vec3f &position, float mass, float t)
{
	//�������� ���ٶ�a=position*magnitude/m*��-1��
	//����������ԭ�㣬���Ĵ�С�͵�ԭ��ľ��������
	Vec3f force = position * magnitude;
	force /= mass;
	Vec3f a = force * -1.0f;

	return a;
}