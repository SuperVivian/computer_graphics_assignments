#include "stdafx.h"
#include "UniformSampler.h"


UniformSampler::UniformSampler (int nSamples)
{
	assert (int (sqrtf (nSamples)) * int (sqrtf (nSamples)) == int (roundf (nSamples)));
	this->nSamples = nSamples;
	_alignedStep = int (sqrtf (this->nSamples));
}


UniformSampler::~UniformSampler ()
{
}
Vec2f UniformSampler::getSamplePosition (int n) {
	int x = n % _alignedStep, y = n / _alignedStep;
	float xPos = 1.f / _alignedStep * (x + 0.5f), yPos = 1.f / _alignedStep * (y + 0.5f);
	return Vec2f (xPos, yPos);
}
