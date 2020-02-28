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
	// 画图
	virtual void Paint (ArgParser *args);

	// 样条类型转换
	virtual void OutputBezier (FILE *file);
	virtual void OutputBSpline (FILE *file);

	//得到控制点
	virtual int getNumVertices ();
	virtual Vec3f getVertex (int i);

	// 编辑操作
	virtual void moveControlPoint (int selectedPoint, float x, float y);
	virtual void addControlPoint (int selectedPoint, float x, float y);
	virtual void deleteControlPoint (int selectedPoint);

	// 产生三角形
	virtual TriangleMesh* OutputTriangles (ArgParser *args);
public:
	int num_vertices;
	Vec3f* vertices;//顶点数组
	enum {
		Bezier=1,
		BSpline=2
	}type_id;

};
