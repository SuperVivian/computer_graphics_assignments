// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: 在此处引用程序需要的其他头文件
#include<istream>
using namespace std;
#include <windows.h>	// Windows的头文件
#include <gl/GL.h>		// OpenGL32库的头文件
#include <gl/glu.h>		// GLu32库的头文件
#include<gl/glut.h>
#include<vector>
#include<assert.h>
#include<iostream>
using namespace std;
#include<cmath>
#include<time.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include<string>
#include"raytracing_stats.h"
#pragma comment( lib, "winmm.lib")
#pragma comment( lib, "opengl32.lib")	// OpenGL32连接库
#pragma comment( lib, "glu32.lib")		// GLu32连接库
#pragma comment( lib, "glut32.lib")		// GLu32连接库

#define M_PI 3.1415926f
#define EPSILON  0.0001f