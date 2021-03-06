// A2.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <string>
#include<assert.h>
#include<string>
#include<iostream>
#include"image.h"
#include"Ray.h"
#include"Hit.h"
#include"SceneParser.h"
#include"light.h"
#include<cmath>


using namespace std;

//变量定义
char *input_file = new char[100];
int width = 100;
int height = 100;
char *output_file = new char[100];
bool depth_on = false;
float depth_min = 0;
float depth_max = 1;
char *depth_file = new char[100];
bool normal_on = false;
char *normal_file = new char[100];
bool shadeBack = false;

int main(int argc, char*argv[])
{	
	//-input scene2_01_diffuse.txt -size 200 200 -output output2_01.tga
	//解析输入
	string tempStr;
	cin >> tempStr >> input_file;
	cin >> tempStr >> width >> height;
	cin >> tempStr >> output_file;
	//cin >> tempStr >> depth_min>>depth_max>>depth_file;
	//cin >> tempStr >> normal_file;
	//cin >> tempStr;
	//if (tempStr == "-shade_back")shadeBack = true;

	/*for (int i = 1; i < argc; i++) {
		if (!strcmp (argv[i], "-input")) {
			i++; assert (i < argc);
			input_file = argv[i];
		}
		else if (!strcmp (argv[i], "-size")) {
			i++; assert (i < argc);
			width = atoi (argv[i]);
			i++; assert (i < argc);
			height = atoi (argv[i]);
		}
		else if (!strcmp (argv[i], "-output")) {
			i++; assert (i < argc);
			output_file = argv[i];
		}
		else if (!strcmp (argv[i], "-depth")) {
			i++; assert (i < argc);
			depth_min = atof (argv[i]);
			i++; assert (i < argc);
			depth_max = atof (argv[i]);
			i++; assert (i < argc);
			depth_file = argv[i];
		}
		else if (!strcmp (argv[i], "-normals")) {
			i++; assert (i < argc);
			normal_file = argv[i];
		}else if (!strcmp (argv[i], "-shade_back")) {
			i++; assert (i < argc);
			shadeBack = true;
		}
		else {
			printf ("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert (0);
		}
	}*/

	SceneParser* scene = new SceneParser (input_file);//初始化场景
	Image* image = new Image (width, height);
	image->SetAllPixels (scene->getBackgroundColor ());
	Vec3f  ambient = scene->getAmbientLight ();
	int numLights = scene->getNumLights ();
	bool hasAmbient = true;
 //**********************************************Render Color Image************************************************//

	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			Vec2f coord ((x + 0.0) / (width - 1), (y + 0.0) / (height - 1)); //逐像素：范围0-1
			Ray ray = scene->getCamera ()->generateRay (coord);	
			Hit hit (FLT_MAX, NULL, Vec3f (0, 0, 0));
			if (scene->getGroup ()->intersect (ray, hit, scene->getCamera ()->getTMin ()))
			{	
				//相交后，根据hit存储的值着色
				Vec3f pixelColor(0.0,0.0,0.0);			
				for (int i = 0; i < numLights; i++) {
					Vec3f lightColor, lightDir;
					scene->getLight (i)->getIllumination (ray.pointAtParameter (hit.getT ()), lightDir, lightColor);
					//Vec3f shadeColor = hit.getMaterial ()->shade (shadeBack,ray,hit,lightDir,lightColor);		
					Vec3f shadeColor;
					Vec3f normal = hit.getNormal ();
					float wrong = ray.getDirection ().Dot3 (normal);
					if (wrong > 0)
					{
						if (shadeBack) {//渲染背面，就反向法线
							normal *= -1.0f;   hasAmbient = true;
						}
						else {
							hasAmbient = false;
						}
					}else   hasAmbient = true;
					//diffuse
					float d = normal.Dot3 (lightDir);
					if (d < 0)d = 0;
					shadeColor = lightColor * hit.getMaterial ()->getDiffuseColor ()*d;
					pixelColor += shadeColor;
				}
				if(hasAmbient) 	pixelColor += (ambient*hit.getMaterial()->getDiffuseColor());	
				image->SetPixel (x, y, pixelColor);
			}
		}
	}
	image->SaveTGA (output_file);

	//**********************************************RenderDepthImage************************************************//
	if (depth_on) {
		image->SetAllPixels (Vec3f (0, 0, 0));//背景色
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++)
			{
				Vec2f coord ((x + 0.0) / (width - 1), (y + 0.0) / (height - 1)); //逐像素：范围0-1
				Ray ray = scene->getCamera ()->generateRay (coord);
				Hit hit (FLT_MAX, NULL, Vec3f (0, 0, 0)); //初始化材质为空，距离为无穷大
				if (scene->getGroup ()->intersect (ray, hit, scene->getCamera ()->getTMin ()))
				{
					if (depth_file != NULL)
					{
						if (hit.getT () < depth_min)
						{
							image->SetPixel (x, y, Vec3f (1.0f, 1.0f, 1.0f));
						}
						else if (hit.getT () > depth_max)
						{
							image->SetPixel (x, y, Vec3f (0.0f, 0.0f, 0.0f));
						}
						else
						{
							float grayScale = (depth_max - hit.getT ()) / (depth_max - depth_min);
							image->SetPixel (x, y, Vec3f (grayScale, grayScale, grayScale));
						}
					}
				}
			}
		}
		image->SaveTGA (depth_file);
	}

	//**********************************************Render Normal mage************************************************//
	if (normal_on) {
		image->SetAllPixels (Vec3f (0, 0, 0));//背景色
		for (int x = 0; x < width; x++) {
			for (int y = 0; y < height; y++) {
				Vec2f coord ((x + 0.0) / (width - 1), (y + 0.0) / (height - 1)); //逐像素：范围0-1
				Ray ray = scene->getCamera ()->generateRay (coord);
				Hit hit (FLT_MAX, NULL, Vec3f (0, 0, 0)); //初始化材质为空，距离为无穷大
				if (scene->getGroup ()->intersect (ray, hit, scene->getCamera ()->getTMin ()))
				{
					Vec3f pixcol = hit.getNormal ().Abs ();
					image->SetPixel (x, y, pixcol);
				}
			}
		}
		image->SaveTGA (normal_file);
	}



	//清理
	delete[] input_file;	input_file = NULL;
	delete[] output_file;	output_file = NULL;
	delete[] depth_file; depth_file = NULL;
	delete[] normal_file; normal_file = NULL;
	delete scene;	scene = NULL;
	delete image;	image = NULL;
	system ("pause");
    return 0;
}

