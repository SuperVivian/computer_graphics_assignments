#pragma once
#include "Filter.h"
class TentFilter :
	public Filter
{
public:
	TentFilter (float radius);
	~TentFilter ();
	virtual float getWeight (float x, float y)override;
	virtual int getSupportRadius ();
private:
	float radius;
};

