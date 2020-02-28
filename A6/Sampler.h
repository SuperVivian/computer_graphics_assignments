#pragma once

class Sampler
{
public:
	Sampler(){}
	Sampler (int nSamples);
	~Sampler ();
	virtual Vec2f getSamplePosition (int n) = 0;//returns the 2D offset for the specified sample
protected:
	int nSamples;
};

