#pragma once
#include "Filter.h"
class GaussianFilter :
	public Filter
{
public:
	GaussianFilter (float sigma);
	~GaussianFilter ();
	virtual float getWeight (float x, float y)override;
	virtual int getSupportRadius ();
private:
	float sigma;
};

