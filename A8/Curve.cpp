#include "stdafx.h"
#include "Curve.h"


void Curve::set (int i, Vec3f v)
{
	vertices[i] = v;
}

void Curve::Paint (ArgParser *arg)
{
	drawPoints ();//红点
	drawLine ();//蓝线
	drawCurve (arg);//緑曲线
}

void Curve::drawPoints ()
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

void Curve::drawLine ()
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

void Curve::drawCurve (ArgParser *arg)
{
	curve_tessellation = arg->curve_tessellation;
	GLfloat range = 1.0f / curve_tessellation;
	glColor3f (0, 1.0f, 0);
	glPointSize (1.0f);
	glBegin (GL_LINES);
	if (type_id == Bezier) {
		//  Q(t)=     p1*(1-t)^3     +    3*p2*t(1-t)^2       +     3*p3*t^2(1-t)     +     p4*t^3
		for (int i = 0; i < num_vertices - 3; i += 3)//最后四个点是一条曲线
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
	else if (type_id == BSpline) {
		//  Q(t)=     p[i-3]*(1/6)*(1-t)^3     +    p[i-2]*(1/6)*(3t^3-6t^2+4)       +     p[i-1]*(1/6)*(-3t^3+3t^2+3t+1)     +     p[i]*(1/6)*t^3
		for (int i = 0; i < num_vertices - 3; i++)//最后四个点是一条曲线
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
//旋转曲面的生成方法：
TriangleMesh* Curve::OutputTriangles (ArgParser *args)
{
	curve_tessellation = args->curve_tessellation;
	int _v_tess = ((num_vertices / 4) + 1) * curve_tessellation;//旋转面分割的个数
	TriangleNet *tri = new TriangleNet (args->revolution_tessellation, _v_tess);
	double degree = 2 * M_PI / args->revolution_tessellation;//旋转次数
	if (type_id == Bezier) {
		int count = 0;
		for (int i = 0; i < num_vertices - 3; i += 3)
		{
			for (int t2 = 0; t2 <= curve_tessellation; t2++)
			{
				//先画出曲线上的每个点
				GLfloat t = t2 * 1.0f / curve_tessellation;
				GLfloat x = vertices[i].x ()*pow (1.0f - t, 3) + 3 * vertices[1 + i].x ()*t*pow (1.0f - t, 2) + 3 * vertices[2 + i].x ()*t*t*(1.0f - t) + vertices[3 + i].x ()*pow (t, 3);
				GLfloat y = vertices[i].y ()*pow (1.0f - t, 3) + 3 * vertices[1 + i].y ()*t*pow (1.0f - t, 2) + 3 * vertices[2 + i].y ()*t*t*(1.0f - t) + vertices[3 + i].y ()*pow (t, 3);
				GLfloat z = vertices[i].z ()*pow (1.0f - t, 3) + 3 * vertices[1 + i].z ()*t*pow (1.0f - t, 2) + 3 * vertices[2 + i].z ()*t*t*(1.0f - t) + vertices[3 + i].z ()*pow (t, 3);
				//指定网格点的顶点位置: 0-10   1-11
				for (int j = 0; j <= args->revolution_tessellation; j++)
				{
					GLfloat x2 =  x * cos (j*degree);
					GLfloat y2 = y;
					GLfloat z2 = - x * sin (j*degree);
					//u_tess   v_tess
					tri->SetVertex (j, t2 + count * curve_tessellation, Vec3f (x2, y2, z2));
				}
			}
			count++;
		}
	}
	else if (type_id == BSpline) {
		_v_tess = (num_vertices - 3) * curve_tessellation;
		int count = 0;
		for (int i = 0; i < num_vertices - 3; i++)
		{
			for (int t2 = 0; t2 <= curve_tessellation; t2++)
			{//先画出曲线上的每个点
				GLfloat t = t2 * 1.0f / curve_tessellation;
				GLfloat x = vertices[i].x ()*pow (1.0f - t, 3) / 6 + vertices[1 + i].x ()*(3 * pow (t, 3) - 6 * pow (t, 2) + 4) / 6 + vertices[2 + i].x ()*(-3 * pow (t, 3) + 3 * pow (t, 2) + 3 * t + 1) / 6 + vertices[3 + i].x ()*pow (t, 3) / 6;
				GLfloat y = vertices[i].y ()*pow (1.0f - t, 3) / 6 + vertices[1 + i].y ()*(3 * pow (t, 3) - 6 * pow (t, 2) + 4) / 6 + vertices[2 + i].y ()*(-3 * pow (t, 3) + 3 * pow (t, 2) + 3 * t + 1) / 6 + vertices[3 + i].y ()*pow (t, 3) / 6;
				GLfloat z = vertices[i].z ()*pow (1.0f - t, 3) / 6 + vertices[1 + i].z ()*(3 * pow (t, 3) - 6 * pow (t, 2) + 4) / 6 + vertices[2 + i].z ()*(-3 * pow (t, 3) + 3 * pow (t, 2) + 3 * t + 1) / 6 + vertices[3 + i].z ()*pow (t, 3) / 6;
				//指定网格点的顶点位置: 0-10   1-11
				for (int j = 0; j <= args->revolution_tessellation; j++)
				{
					GLfloat x2 =  x * cos (j*degree);
					GLfloat y2 = y;
					GLfloat z2 = - x * sin (j*degree);
					//u_tess   v_tess
					tri->SetVertex (j, t2 + count, Vec3f (x2, y2, z2));
				}
			}
			count++;
		}
	}
	return tri;
}

Curve::~Curve ()
{
	delete[]vertices;
}
