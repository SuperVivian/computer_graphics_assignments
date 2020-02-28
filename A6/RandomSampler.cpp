#include "stdafx.h"
#include "RandomSampler.h"


RandomSampler::RandomSampler ()
{

}


RandomSampler::~RandomSampler ()
{
}
Vec2f RandomSampler::getSamplePosition (int n) {
	//随机取一个0-1之内的浮点数
	return Vec2f (static_cast<float>(rand () % 10000 / 10000.0), static_cast<float>(rand () % 10000 / 10000.0));
}
