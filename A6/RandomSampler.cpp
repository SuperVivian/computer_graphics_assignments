#include "stdafx.h"
#include "RandomSampler.h"


RandomSampler::RandomSampler ()
{

}


RandomSampler::~RandomSampler ()
{
}
Vec2f RandomSampler::getSamplePosition (int n) {
	//���ȡһ��0-1֮�ڵĸ�����
	return Vec2f (static_cast<float>(rand () % 10000 / 10000.0), static_cast<float>(rand () % 10000 / 10000.0));
}
