#include "stdafx.h"
#include "GaussianFilter.h"


GaussianFilter::GaussianFilter (float sigma)
{
	this->sigma = sigma;
}


GaussianFilter::~GaussianFilter ()
{
}
float GaussianFilter::getWeight (float x, float y) {
	float dist = sqrtf (x * x + y * y);
	return expf ((-dist * dist) / (2.f * sigma * sigma));
}
int GaussianFilter::getSupportRadius () {
	int radius = (ceil (2.0f * sigma - 0.5f));
	return radius;
}