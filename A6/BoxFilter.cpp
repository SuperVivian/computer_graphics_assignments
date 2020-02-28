#include "stdafx.h"
#include "BoxFilter.h"


BoxFilter::BoxFilter (float radius)
{
	this->radius = radius;
}


BoxFilter::~BoxFilter ()
{
}
float BoxFilter::getWeight (float x, float y) {
	return fabsf (x) < radius && fabsf (y) < radius;
}
int BoxFilter::getSupportRadius () {
	return int (ceilf (radius - 0.5f));
}