#pragma once
#include "Surface.h"
class BezierPatch :
	public Surface
{
public:
	BezierPatch ();
	void Paint (ArgParser *arg);
	TriangleMesh* OutputTriangles (ArgParser *args);
};

