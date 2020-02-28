#pragma once

#include"UniformSampler.h"
class JitteredSampler :
	public UniformSampler
{
public:
	JitteredSampler (int nSamples) : UniformSampler (nSamples) {};
	~JitteredSampler ();
	virtual Vec2f getSamplePosition (int n);
};

