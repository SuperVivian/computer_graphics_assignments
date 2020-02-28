#include "stdafx.h"
#include "BSplineCurve.h"

BSplineCurve::BSplineCurve (int num_v)
{
	num_vertices = num_v;
	vertices = new Vec3f[num_vertices];
	type_id = BSpline;
}

void BSplineCurve::Paint (ArgParser *arg)
{
	Curve::Paint (arg);
}

void BSplineCurve::OutputBezier (FILE *file)
{	//已知bspline_G, bs_B, beizer_B, 求beizer_G
	//beizer_G = bspline_G * bs_B * beizer_B的逆矩阵;
	Matrix bspline_G;
	for (int i = 0; i < num_vertices; i++)
	{
		bspline_G.Set (i, 0, vertices[i].x ());
		bspline_G.Set (i, 1, vertices[i].y ());
		bspline_G.Set (i, 2, vertices[i].z ());
	}

	float beizer_b[16] = { -1, 3, -3, 1, 3, -6, 3, 0, -3, 3, 0, 0, 1, 0, 0, 0 };
	Matrix beizer_B (beizer_b);
	float bs_b[16] = { -1, 3, -3, 1, 3, -6, 0, 4, -3, 3, 3, 1, 1, 0, 0, 0 };
	Matrix bs_B (bs_b);
	bs_B = bs_B * (1.0f / 6.0f);

	beizer_B.Inverse ();
	Matrix beizer_G = bspline_G * bs_B*beizer_B;

	fprintf (file, "bezier  \n");
	fprintf (file, "num_vertices  %d\n", num_vertices);
	for (int i = 0; i < num_vertices; i++)
	{
		float x = beizer_G.Get (i, 0);
		float y = beizer_G.Get (i, 1);
		float z = beizer_G.Get (i, 2);
		fprintf (file, "%f ", x);
		fprintf (file, "%f ", y);
		fprintf (file, "%f ", z);
		fprintf (file, "\n");
	}
}

void BSplineCurve::OutputBSpline (FILE *file)
{
	fprintf (file, "bspline  \n");
	fprintf (file, "num_vertices  %d\n", num_vertices);
	for (int i = 0; i < num_vertices; i++)
	{
		float x = vertices[i].x ();
		float y = vertices[i].y ();
		float z = vertices[i].z ();
		fprintf (file, "%f ", x);
		fprintf (file, "%f ", y);
		fprintf (file, "%f ", z);
		fprintf (file, "\n");
	}
}

BSplineCurve::~BSplineCurve ()
{

}