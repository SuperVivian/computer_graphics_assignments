#pragma once
#include "Filter.h"
class BoxFilter :
	public Filter
{
public:
	BoxFilter (float radius);
	~BoxFilter ();
	virtual float getWeight (float x, float y)override;
	virtual int getSupportRadius ();
private:
	float radius;
};

