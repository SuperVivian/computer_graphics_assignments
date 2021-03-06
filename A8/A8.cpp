// A8.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <stdio.h>
#include <assert.h>
#include<iostream>
using namespace std;

#include "arg_parser.h"
#include "glCanvas.h"
#include "spline_parser.h"

int main(int argc, char *argv[])
{
	//解析命令行和读入txt
	ArgParser *args = new ArgParser (argc, argv);
	SplineParser* splines = new SplineParser (args->input_file);//Spline数组

	//启动曲线编辑器
	if (args->gui) {
		GLCanvas glcanvas;
		glcanvas.initialize (args, splines);		
		//1.交互：spline中的 pick / move / add /delete /(save->)output
		//2.绘制：spline中的paint
	}

	//对应命令自动保存
	splines->SaveBezier (args);//贝塞尔
	splines->SaveBSpline (args);//B样条
	splines->SaveTriangles (args);//三角面模型

	//清理
	delete args;
	delete splines;
    return 0;
}

