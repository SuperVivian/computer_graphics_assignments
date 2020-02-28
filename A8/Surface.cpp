#include "stdafx.h"
#include "Surface.h"

void Surface::set (int i, Vec3f v)
{
	vertices[i] = v;
}

void Surface::Paint (ArgParser *arg)
{
	drawPoints ();
	drawLine ();
	drawCurve (arg);
}
void Surface::drawPoints ()
{
	glColor3f (1.0f, 0, 0);
	glPointSize (5.0f);
	glBegin (GL_POINTS);
	for (int i = 0; i < num_vertices; i++)
	{
		glVertex3f (vertices[i].x (), vertices[i].y (), vertices[i].z ());
	}
	glEnd ();
}
void Surface::drawLine ()
{
	glColor3f (0, 0, 1.0f);
	glPointSize (1.0f);
	glBegin (GL_LINES);
	for (int i = 0; i < num_vertices - 1; i++)
	{
		glVertex3f (vertices[i].x (), vertices[i].y (), vertices[i].z ());
		glVertex3f (vertices[i + 1].x (), vertices[i + 1].y (), vertices[i + 1].z ());
	}
	glEnd ();
}



void Surface::drawCurve (ArgParser *arg)
{
	curve_tessellation = arg->curve_tessellation;
	GLfloat range = 1.0f / curve_tessellation;
	glColor3f (0, 1.0f, 0);
	glPointSize (1.0f);
	glBegin (GL_LINES);
	if (type == Bezier) {
		for (int i = 0; i < num_vertices - 3; i += 3)
		{
			for (GLfloat t = 0; t < 1.0f; t += range)
			{
				GLfloat x1 = vertices[i].x ()*pow (1.0f - t, 3) + 3 * vertices[1 + i].x ()*t*pow (1.0f - t, 2) + 3 * vertices[2 + i].x ()*t*t*(1.0f - t) + vertices[3 + i].x ()*pow (t, 3);
				GLfloat y1 = vertices[i].y ()*pow (1.0f - t, 3) + 3 * vertices[1 + i].y ()*t*pow (1.0f - t, 2) + 3 * vertices[2 + i].y ()*t*t*(1.0f - t) + vertices[3 + i].y ()*pow (t, 3);
				GLfloat z1 = vertices[i].z ()*pow (1.0f - t, 3) + 3 * vertices[1 + i].z ()*t*pow (1.0f - t, 2) + 3 * vertices[2 + i].z ()*t*t*(1.0f - t) + vertices[3 + i].z ()*pow (t, 3);

				GLfloat t2 = t + range;
				GLfloat x2 = vertices[i].x ()*pow (1.0f - t2, 3) + 3 * vertices[1 + i].x ()*t2*pow (1.0f - t2, 2) + 3 * vertices[2 + i].x ()*t2*t2*(1.0f - t2) + vertices[3 + i].x ()*pow (t2, 3);
				GLfloat y2 = vertices[i].y ()*pow (1.0f - t2, 3) + 3 * vertices[1 + i].y ()*t2*pow (1.0f - t2, 2) + 3 * vertices[2 + i].y ()*t2*t2*(1.0f - t2) + vertices[3 + i].y ()*pow (t2, 3);
				GLfloat z2 = vertices[i].z ()*pow (1.0f - t2, 3) + 3 * vertices[1 + i].z ()*t2*pow (1.0f - t2, 2) + 3 * vertices[2 + i].z ()*t2*t2*(1.0f - t2) + vertices[3 + i].z ()*pow (t2, 3);

				glVertex3f (x1, y1, z1);
				glVertex3f (x2, y2, z2);
			}
		}
	}
	else if (type == BSpline) {
		for (int i = 0; i < num_vertices - 3; i++)
		{
			for (GLfloat t = 0; t < 1.0f; t += range)
			{
				GLfloat x = vertices[i].x ()*pow (1.0f - t, 3) / 6 + vertices[1 + i].x ()*(3 * pow (t, 3) - 6 * pow (t, 2) + 4) / 6 + vertices[2 + i].x ()*(-3 * pow (t, 3) + 3 * pow (t, 2) + 3 * t + 1) / 6 + vertices[3 + i].x ()*pow (t, 3) / 6;
				GLfloat y = vertices[i].y ()*pow (1.0f - t, 3) / 6 + vertices[1 + i].y ()*(3 * pow (t, 3) - 6 * pow (t, 2) + 4) / 6 + vertices[2 + i].y ()*(-3 * pow (t, 3) + 3 * pow (t, 2) + 3 * t + 1) / 6 + vertices[3 + i].y ()*pow (t, 3) / 6;
				GLfloat z = vertices[i].z ()*pow (1.0f - t, 3) / 6 + vertices[1 + i].z ()*(3 * pow (t, 3) - 6 * pow (t, 2) + 4) / 6 + vertices[2 + i].z ()*(-3 * pow (t, 3) + 3 * pow (t, 2) + 3 * t + 1) / 6 + vertices[3 + i].z ()*pow (t, 3) / 6;

				GLfloat t2 = t + range;
				GLfloat x2 = vertices[i].x ()*pow (1.0f - t2, 3) / 6 + vertices[1 + i].x ()*(3 * pow (t2, 3) - 6 * pow (t2, 2) + 4) / 6 + vertices[2 + i].x ()*(-3 * pow (t2, 3) + 3 * pow (t2, 2) + 3 * t2 + 1) / 6 + vertices[3 + i].x ()*pow (t2, 3) / 6;
				GLfloat y2 = vertices[i].y ()*pow (1.0f - t2, 3) / 6 + vertices[1 + i].y ()*(3 * pow (t2, 3) - 6 * pow (t2, 2) + 4) / 6 + vertices[2 + i].y ()*(-3 * pow (t2, 3) + 3 * pow (t2, 2) + 3 * t2 + 1) / 6 + vertices[3 + i].y ()*pow (t2, 3) / 6;
				GLfloat z2 = vertices[i].z ()*pow (1.0f - t2, 3) / 6 + vertices[1 + i].z ()*(3 * pow (t2, 3) - 6 * pow (t2, 2) + 4) / 6 + vertices[2 + i].z ()*(-3 * pow (t2, 3) + 3 * pow (t2, 2) + 3 * t2 + 1) / 6 + vertices[3 + i].z ()*pow (t2, 3) / 6;

				glVertex3f (x, y, z);
				glVertex3f (x2, y2, z2);
			}
		}
	}
	glEnd ();
}

Surface::~Surface ()
{
	delete[]vertices;
}
