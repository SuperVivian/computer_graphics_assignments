#include "stdafx.h"
#include "BezierCurve.h"


BezierCurve::BezierCurve (int num_v)
{
	num_vertices = num_v;
	vertices = new Vec3f[num_vertices];
	type_id = Bezier;
}

void BezierCurve::Paint (ArgParser *arg)
{
	Curve::Paint (arg);	
}

void BezierCurve::OutputBezier (FILE *file)
{
	fprintf (file, "bezier  \n");
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

void BezierCurve::OutputBSpline (FILE *file)
{
	//已知beizer_G, beizer_B, bs_B, 求bspline_G
	//bspline_G = beizer_G * beizer_B * bs_B的逆矩阵;
	Matrix beizer_G;
	for (int i = 0; i < num_vertices; i++)
	{
		beizer_G.Set (i, 0, vertices[i].x ());
		beizer_G.Set (i, 1, vertices[i].y ());
		beizer_G.Set (i, 2, vertices[i].z ());
	}
	float beizer_b[16] = { -1, 3, -3, 1, 3, -6, 3, 0, -3, 3, 0, 0, 1, 0, 0, 0 };
	Matrix beizer_B (beizer_b);
	float bs_b[16] = { -1, 3, -3, 1, 3, -6, 0, 4, -3, 3, 3, 1, 1, 0, 0, 0 };
	Matrix bs_B (bs_b);
	bs_B = bs_B * (1.0f / 6.0f);

	bs_B.Inverse ();
	Matrix bspline_G = beizer_G * beizer_B * bs_B;

	fprintf (file, "bspline  \n");
	fprintf (file, "num_vertices  %d\n", num_vertices);
	for (int i = 0; i < num_vertices; i++)
	{
		float x = bspline_G.Get (i, 0);
		float y = bspline_G.Get (i, 1);
		float z = bspline_G.Get (i, 2);
		fprintf (file, "%f ", x);
		fprintf (file, "%f ", y);
		fprintf (file, "%f ", z);
		fprintf (file, "\n");
	}
}

void BezierCurve::addControlPoint (int selectedPoint, float x, float y)//同一个位置添加3个点
{
	num_vertices += 3;
	Vec3f *temp = new Vec3f[num_vertices];
	for (int i = 0; i < num_vertices - 3; i++)
	{
		temp[i] = vertices[i];
	}
	temp[num_vertices - 1] = Vec3f (x, y, 0);
	temp[num_vertices - 2] = Vec3f (x, y, 0);
	temp[num_vertices - 3] = Vec3f (x, y, 0);

	delete[]vertices;
	vertices = new Vec3f[num_vertices];

	for (int i = 0; i < num_vertices; i++)
	{
		vertices[i] = temp[i];
	}

	delete[]temp;
}

void BezierCurve::deleteControlPoint (int selectedPoint)//一次删除3个点 删除选择的点以及下标离它最近的两个点
{
	if (num_vertices < 5) {
		return;
	}
	num_vertices -= 3;
	Vec3f *temp = new Vec3f[num_vertices];
	int pre = selectedPoint;
	int last = selectedPoint;
	int count = 1;
	while (count != 3) {
		count++;
		if (count % 2 == 0) {
			pre--;
			if (pre < 0) {
				pre++;
				last++;
			}
		}
		else {
			last++;
			if (last == (num_vertices + 3)) {
				last--;
				pre--;
			}
		}
	}
	for (int i = 0; i < pre; i++)
	{
		temp[i] = vertices[i];
	}
	for (int i = last + 1; i < num_vertices; i++)
	{
		temp[i - 3] = vertices[i];
	}
	delete[]vertices;

	vertices = new Vec3f[num_vertices];
	for (int i = 0; i < num_vertices; i++)
	{
		vertices[i] = temp[i];
	}

	delete[]temp;
}

BezierCurve::~BezierCurve ()
{
}
