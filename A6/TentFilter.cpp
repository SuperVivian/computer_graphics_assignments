#include "stdafx.h"
#include "TentFilter.h"


TentFilter::TentFilter (float radius)
{
	this->radius = radius;
}


TentFilter::~TentFilter ()
{
}
float TentFilter::getWeight (float x, float y) {
	float dist = sqrtf (x * x + y * y);
	return max (0.f, 1.f - dist / radius);
}
int TentFilter::getSupportRadius () {
	return int (ceilf (radius - 0.5f));
}