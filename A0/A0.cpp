// A0.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"IFS.h"
#include<fstream>
#include<assert.h>
#include"Matrix.h"

int main(int argc, char *argv[])
{
	//变量定义
	char *input_file = new char[100];
	int num_points = 10000;
	int num_iters = 10;
	int m_size = 100;
	char *output_file = new char[100];

	string tempStr;
	cin >> tempStr >> input_file;
	cin >> tempStr >> num_points;
	cin >> tempStr >> num_iters;
	cin >> tempStr >> m_size;
	cin >> tempStr >> output_file;


	//着色
	Image* image = new Image (m_size, m_size);//新建一张图片
	IFS ifs;
	ifs.Input (input_file);//读入分形描述txt
	ifs.Render (image, num_points,num_iters);//进行绘制
	image->SaveTGA (output_file);//输出tga文件

	//清理
	ifs.CleanUp ();
	delete[] input_file;	input_file = NULL;
	delete[] output_file;	output_file = NULL;

    return 0;
}

