#pragma once
#include"vectors.h"
#include <fstream>
#include <GL/glut.h>
#include <cmath>
#include "triangle_mesh.h"
#include "arg_parser.h"
#include "matrix.h"
class Spline
{
public:
	// ��ͼ
	virtual void Paint (ArgParser *args);

	// ��������ת��
	virtual void OutputBezier (FILE *file);
	virtual void OutputBSpline (FILE *file);

	//�õ����Ƶ�
	virtual int getNumVertices ();
	virtual Vec3f getVertex (int i);

	// �༭����
	virtual void moveControlPoint (int selectedPoint, float x, float y);
	virtual void addControlPoint (int selectedPoint, float x, float y);
	virtual void deleteControlPoint (int selectedPoint);

	// ����������
	virtual TriangleMesh* OutputTriangles (ArgParser *args);
public:
	int num_vertices;
	Vec3f* vertices;//��������
	enum {
		Bezier=1,
		BSpline=2
	}type_id;

};
