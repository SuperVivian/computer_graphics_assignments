#pragma once
#include"film.h"
class Filter
{
public:
	Filter ();
	~Filter ();
	Vec3f getColor (int i, int j, Film *film);
	virtual float getWeight (float x, float y) = 0;
	virtual int getSupportRadius () = 0;
};

