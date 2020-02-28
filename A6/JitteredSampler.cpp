#include "stdafx.h"
#include "JitteredSampler.h"




JitteredSampler::~JitteredSampler ()
{
}
Vec2f JitteredSampler::getSamplePosition (int n) {
	Vec2f uniformPos = UniformSampler::getSamplePosition (n);
	Vec2f turbulence = Vec2f ( (rand()% 10000 / 10000.0) - 0.5f,   ((rand()%10000 / 10000.0) - 0.5f) / float (_alignedStep));
	Vec2f jitteredPos;	Vec2f::Add (jitteredPos, uniformPos, turbulence);	
	jitteredPos.Clamp ();
	return jitteredPos;
}
