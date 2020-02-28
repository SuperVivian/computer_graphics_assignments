#pragma once
#include "Surface.h"
#include"Curve.h"
class SurfaceOfRevolution :
	public Surface
{
public:
public:
	SurfaceOfRevolution (Curve *c);
	void Paint (ArgParser *arg);
	TriangleMesh* OutputTriangles (ArgParser *args);
private:
	Curve * curve;
};

