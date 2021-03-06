// A1.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include<assert.h>
#include<string>
#include<iostream>
#include "SceneParser.h"
#include"image.h"
using namespace std;

//变量定义
char *input_file = new char[100];
int width = 100;
int height = 100;
char *output_file = new char[100];
float depth_min = 0;
float depth_max = 1;
char *depth_file = new char[100];


int main(int argc, char *argv[])
{
	//解析输入
	string tempStr;
	cin >> tempStr >> input_file;
	cin >> tempStr >> width>>height;
	cin >> tempStr >> output_file;
	cin >> tempStr >> depth_min>>depth_max>>depth_file;

	SceneParser* scene =new SceneParser(input_file);//初始化场景：camera、背景颜色、场景物体、材质
	Image* image = new Image (width,height);//新建图片
	image->SetAllPixels (scene->getBackgroundColor());//背景色

	//**********************************************RenderColorImage************************************************//
	for (int x = 0; x < width; x++)	{
		for (int y = 0; y < height; y++){
			Vec2f coord(  (x+0.0) / (width-1),  (y+0.0) / (height-1) ); //逐像素：范围0-1
			Ray ray = scene->getCamera ()->generateRay (coord);
			Hit hit (FLT_MAX, NULL); //初始化材质为空，距离为无穷大
			if (scene->getGroup ()->intersect (ray, hit, scene->getCamera ()->getTMin ()))
			{		
					image->SetPixel (x, y, hit.getMaterial ()->getDiffuseColor ());//相交后，根据hit存储的值着色
			}
		}
	}
	image->SaveTGA (output_file);
	//**********************************************RenderDepthImage************************************************//
	image->SetAllPixels (Vec3f(0,0,0));//背景色
	float zone = depth_max - depth_min;
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			Vec2f coord ((x + 0.0) / (width - 1), (y + 0.0) / (height - 1)); //逐像素：范围0-1
			Ray ray = scene->getCamera ()->generateRay (coord);
			Hit hit (FLT_MAX, NULL); //初始化材质为空，距离为无穷大
			if (scene->getGroup ()->intersect (ray, hit, scene->getCamera ()->getTMin ()))
			{
				//相交后，根据hit存储的值着色
				float t = hit.getT ();
				if (t >depth_min && t< depth_max) {
					//把 t 值 映射成灰度
					float c =(depth_max- t )/ zone;
					image->SetPixel (x,y,Vec3f(c,c,c));
				}
			}
		}
	}
	image->SaveTGA (depth_file);


	//清理
	delete[] input_file;	input_file = NULL;
	delete[] output_file;	output_file = NULL;
	delete[] depth_file;	depth_file = NULL;
	delete scene;	scene = NULL;
	delete image;	image = NULL;

	system ("pause");
    return 0;
}

