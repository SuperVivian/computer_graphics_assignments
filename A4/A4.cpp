// A4.cpp: 定义控制台应用程序的入口点。
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
#include"glCanvas.h"
#include"RayTracer.h"
#include"rayTree.h"
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
SceneParser* scene = NULL;
Image* image = NULL;
int n_theta = 0;
int n_phi = 0;
bool gouraud = false;
int bounces;//递归深度
float weight;//射线权重
bool shadow = false;
bool rayTrace = true;
RayTracer* rayTracer=NULL;

void Render () {
	if (!rayTrace)image->SetAllPixels (scene->getBackgroundColor ());
	Vec3f  ambient = scene->getAmbientLight ();
	int numLights = scene->getNumLights ();
	bool hasAmbient = true;

	//**********************************************Render Color Image************************************************//
	float distanceToLight = INFINITY;
	Hit hit (FLT_MAX, NULL, Vec3f (0, 0, 0));
	Vec3f pixelColor (0.0, 0.0, 0.0);
	for (int x = 0; x < width; x++) {
		for (int y = 0; y < height; y++) {
			Vec2f coord ((x + 0.0) / (width - 1), (y + 0.0) / (height - 1)); //逐像素：范围0-1
			Ray ray = scene->getCamera ()->generateRay (coord);
			if (!rayTrace) {				
				if (scene->getGroup ()->intersect (ray, hit, scene->getCamera ()->getTMin ()))
				{
					//相交后，根据hit存储的值着色					
					for (int i = 0; i < numLights; i++) {
						Vec3f lightColor, lightDir;
						scene->getLight (i)->getIllumination (ray.pointAtParameter (hit.getT ()), lightDir, lightColor, distanceToLight);
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
						}
						else   hasAmbient = true;
						hit.set (hit.getT (), hit.getMaterial (), normal, ray);//对法线进行更改
						Vec3f shadeColor = hit.getMaterial ()->Shade (ray, hit, lightDir, lightColor);
						pixelColor += shadeColor;
					}
					if (hasAmbient) pixelColor += (ambient*hit.getMaterial ()->getDiffuseColor ());					
				}
			}			
			else {			
				pixelColor = rayTracer->traceRay (ray, scene->getCamera ()->getTMin (), bounces,weight, 1,hit);
			}
			image->SetPixel (x, y, pixelColor);
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
}
void TraceRay (float x, float y) {
	//屏幕坐标（0,0）->(1,1)
	Vec2f coord (x, y);
	Ray ray = scene->getCamera ()->generateRay (coord);
	Hit hit (FLT_MAX, NULL, Vec3f (0, 0, 0));
	if (scene->getGroup ()->intersect (ray, hit, scene->getCamera ()->getTMin ())) {
		Vec3f point = hit.getIntersectionPoint ();
		Vec3f n = hit.getNormal ();

		//画出摄像机射线
		Vec3f mainOri = point + n * 0.1f;
		Ray toCamera = Ray (mainOri, ray.getDirection()*-1);
		RayTree::SetMainSegment (toCamera, 0, 10);

		//阴影射线
		//if (shadow)
		//{
		//	for (int i = 0; i < scene->getNumLights (); i++)
		//	{
		//		Vec3f lightColor, lightDir;	float distanceToLight;
		//		scene->getLight (i)->getIllumination (point, lightDir, lightColor, distanceToLight);
		//		lightDir.Normalize ();
		//		Vec3f origin = point + n * 0.1f;
		//		Ray toLight = Ray (origin, lightDir);//再从交点处，向光源发射一条射线
		//		Hit m_hit = Hit (FLT_MAX, NULL, Vec3f (0, 0, 0));
		//		scene->getGroup ()->intersect (toLight, m_hit, EPSILON);
		//		RayTree::AddShadowSegment (toLight, 0, m_hit.getT());
		//		cout << "画出了阴影射线" << endl;
		//	}
		//}

		rayTracer->traceRay (ray, scene->getCamera ()->getTMin (), bounces, 1,weight,hit);
	}
}

int main (int&argc, char*argv[])
{
	//解析输入
	//glutInit (argc,argv);

	string tempStr;
	cin >> tempStr >> input_file;
	cin >> tempStr >> width >> height;
	cin >> tempStr >> output_file;
	cin >> tempStr;
	if (tempStr == "-shadows")shadow = true;
	//cin >> tempStr >> bounces >> tempStr >> weight;
	//cin  >> tempStr >> tempStr >> tempStr >> n_theta >> n_phi;
	cin >> tempStr >>tempStr>> n_theta >> n_phi;
	cin >> tempStr;
	if (tempStr == "-gouraud")gouraud = true;

	//cin >> tempStr >> depth_min>>depth_max>>depth_file;
	//cin >> tempStr >> normal_file;  normal_on = true;
	//cin >> tempStr;
	//if (tempStr == "-shade_back")shadeBack = true;


	scene = new SceneParser (input_file);//初始化场景
	image = new Image (width, height);
	rayTracer = new RayTracer (scene, 0, 0, shadow);
	GLCanvas glCanvas;
	glCanvas.initialize (scene, Render, TraceRay);



	//清理
	delete[] input_file;	input_file = NULL;
	delete[] output_file;	output_file = NULL;
	delete[] depth_file; depth_file = NULL;
	delete[] normal_file; normal_file = NULL;
	delete scene;	scene = NULL;
	delete image;	image = NULL;
	delete rayTracer;	rayTracer = NULL;

	system ("pause");
	return 0;
}
