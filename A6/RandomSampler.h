#pragma once
#include "Sampler.h"
class RandomSampler :
	public Sampler
{
public:
	RandomSampler ();
	~RandomSampler ();
	virtual Vec2f getSamplePosition (int n);//returns the 2D offset for the specified sample
};

