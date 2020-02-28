#pragma once
#include "Sampler.h"
class UniformSampler :
	public Sampler
{
public:
	UniformSampler (int nSamples);
	~UniformSampler ();
	virtual Vec2f getSamplePosition (int n);
protected:
	int _alignedStep;
};

