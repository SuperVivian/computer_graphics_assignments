#include "stdafx.h"
#include "SurfaceOfRevolution.h"


SurfaceOfRevolution::SurfaceOfRevolution (Curve *c)
{
	num_vertices = c->getNumVertices ();
	vertices = new Vec3f[num_vertices];
	for (int i = 0; i < num_vertices; i++)
	{
		vertices[i] = c->getVertex (i);
	}
	if (c->type_id == Bezier) {
		type = Bezier;
	}
	else if (c->type_id == BSpline) {
		type = BSpline;
	}
	curve = c;
}

void SurfaceOfRevolution::Paint (ArgParser *arg)
{
	Surface::Paint (arg);
}

TriangleMesh* SurfaceOfRevolution::OutputTriangles (ArgParser *arg)
{
	return curve->OutputTriangles (arg);
}
