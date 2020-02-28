#pragma once
#include "Curve.h"
class BezierCurve :
	public Curve
{
public:
	BezierCurve (int num_v);
	void Paint (ArgParser *args);
	void OutputBezier (FILE *file);
	void OutputBSpline (FILE *file);
	void addControlPoint (int selectedPoint, float x, float y);
	void deleteControlPoint (int selectedPoint);
	~BezierCurve ();
};

