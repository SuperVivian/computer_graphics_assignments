#pragma once
#include "Curve.h"
class BSplineCurve :
	public Curve
{
public:
	BSplineCurve (int nvm_v);
	void Paint (ArgParser *arg);
	void OutputBezier (FILE *file);
	void OutputBSpline (FILE *file);
	~BSplineCurve ();
};

