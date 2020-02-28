#pragma once
#include "Spline.h"
class Surface :
	public Spline
{
public:
public:
	virtual void set (int i, Vec3f v);
	virtual void Paint (ArgParser *arg);
	~Surface ();
public:
	int curve_tessellation;
protected:
	void drawLine ();
	void drawPoints ();
	void drawCurve (ArgParser *arg);
protected:
	enum {
		Bezier = 1,
		BSpline = 2
	}type;
};

