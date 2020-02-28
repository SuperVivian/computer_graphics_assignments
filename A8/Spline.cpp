#include "stdafx.h"
#include "Spline.h"


void Spline::Paint (ArgParser *args)
{

}

void Spline::OutputBezier (FILE *file)
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

void Spline::OutputBSpline (FILE *file)
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

int Spline::getNumVertices ()
{
	return num_vertices;
}

Vec3f Spline::getVertex (int i)
{
	return vertices[i];
}

void Spline::moveControlPoint (int selectedPoint, float x, float y)
{
	vertices[selectedPoint].Set (x, y, vertices[selectedPoint].z ());
}

void Spline::addControlPoint (int selectedPoint, float x, float y)
{
	num_vertices++;
	Vec3f *temp = new Vec3f[num_vertices];

	for (int i = 0; i < selectedPoint; i++)
	{
		temp[i] = vertices[i];
	}
	temp[selectedPoint] = Vec3f (x, y, 0);
	for (int i = selectedPoint + 1; i < num_vertices; i++)
	{
		temp[i] = vertices[i - 1];
	}
	delete[]vertices;
	vertices = new Vec3f[num_vertices];
	for (int i = 0; i < num_vertices; i++)
	{
		vertices[i] = temp[i];
	}

	delete[]temp;
}

void Spline::deleteControlPoint (int selectedPoint)//如果少于5个点就不删除
{
	if (num_vertices < 5) {
		return;
	}
	num_vertices--;
	Vec3f *temp = new Vec3f[num_vertices];
	for (int i = 0; i < selectedPoint; i++)
	{
		temp[i] = vertices[i];
	}
	for (int i = selectedPoint + 1; i < num_vertices; i++)
	{
		temp[i - 1] = vertices[i];
	}
	delete[]vertices;

	vertices = new Vec3f[num_vertices];
	for (int i = 0; i < num_vertices; i++)
	{
		vertices[i] = temp[i];
	}

	delete[]temp;
}

TriangleMesh* Spline::OutputTriangles (ArgParser *args)
{
	return NULL;
}
