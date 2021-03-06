// A5.cpp: 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include"image.h"
#include"Ray.h"
#include"Hit.h"
#include"SceneParser.h"
#include"light.h"
#include"glCanvas.h"
#include"RayTracer.h"
#include"rayTree.h"
#include"grid.h"


//变量定义
char *input_file = NULL;
char *output_file = NULL;
char *depth_file = NULL;
char *normal_file = NULL;

int width = 100;  int height = 100;
float depth_min = 0;  float depth_max = 1;
int n_theta = 0;  int n_phi = 0;
int bounces;  float weight;
int nx = 0;int ny = 0;int nz = 0;

bool depth_on = false;
bool normal_on = false;
bool shadeBack = false;
bool gouraud = false;
bool shadow = false;
bool rayTrace = true;
bool gui = true;
bool visualize_grid = false;
bool isGrid = false;
bool stats = false;

SceneParser* scene = NULL;
Image* image = NULL;
RayTracer* rayTracer = NULL;
Grid *grid = NULL;


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
				pixelColor = rayTracer->traceRay (ray, scene->getCamera ()->getTMin (), bounces, weight, 1, hit);
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
		Ray toCamera = Ray (mainOri, ray.getDirection ()*-1);
		RayTree::SetMainSegment (toCamera, 0, 10);

		//阴影射线
		if (shadow)
		{
			for (int i = 0; i < scene->getNumLights (); i++)
			{
				Vec3f lightColor, lightDir;	float distanceToLight;
				scene->getLight (i)->getIllumination (point, lightDir, lightColor, distanceToLight);
				lightDir.Normalize ();
				Vec3f origin = point + n * 0.1f;
				Ray toLight = Ray (origin, lightDir);//再从交点处，向光源发射一条射线
				Hit m_hit = Hit (FLT_MAX, NULL, Vec3f (0, 0, 0));
				scene->getGroup ()->intersect (toLight, m_hit, EPSILON);
				RayTree::AddShadowSegment (toLight, 0, m_hit.getT());
			}
		}

		//rayTracer->traceRay (ray, scene->getCamera ()->getTMin (), bounces, 1, weight, hit);
	}
}

int main (int argc, char*argv[])
{
	//---输入测试---
	for (int i = 1; i < argc; i++) {
		if (!strcmp(argv[i], "-input")) {
			i++; assert(i < argc);
			input_file = argv[i];
		}
		else if (!strcmp(argv[i], "-size")) {
			i++; assert(i < argc);
			width = atoi(argv[i]);
			i++; assert(i < argc);
			height = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-output")) {
			i++; assert(i < argc);
			output_file = argv[i];
		}
		else if (!strcmp(argv[i], "-depth")) {
			i++; assert(i < argc);
			depth_min = atof(argv[i]);
			i++; assert(i < argc);
			depth_max = atof(argv[i]);
			i++; assert(i < argc);
			depth_file = argv[i];
		}
		else if (!strcmp(argv[i], "-normals")){
			i++; assert(i < argc);
			normal_file = argv[i];
		}
		else if (!strcmp(argv[i], "-shade_back")){
			shadeBack = true;
		}
		else if (!strcmp(argv[i], "-bounces")) {
			i++;
			assert(i < argc);
			bounces = atoi(argv[i]);
		}
		else if (!strcmp(argv[i], "-weight")) {
			i++;
			assert(i < argc);
			weight = (float)(atof(argv[i]));
		}
		else if (!strcmp(argv[i], "-shadows")) {
			shadow = true;
		}
		else if (!strcmp(argv[i], "-gui")) {
			gui = true;
		}
		else if (!strcmp(argv[i], "-tessellation")){
			i++;
			assert(i < argc);
			n_theta = atoi(argv[i]);//x方向上的细分
			i++;
			assert(i < argc);
			n_phi = atoi(argv[i]);//y方向上的细分
		}
		else if (!strcmp(argv[i], "-gouraud")){
			gouraud = true;
		}
		else if (!strcmp(argv[i], "-visualize_grid")){
			visualize_grid = true;
		}
		else if (!strcmp(argv[i], "-grid")){
			isGrid = true;
			i++;
			assert(i < argc);
			nx = atoi(argv[i]);
			i++;
			assert(i < argc);
			ny = atoi(argv[i]);
			i++;
			assert(i < argc);
			nz = atoi(argv[i]);
		}
		else {
			printf("whoops error with command line argument %d: '%s'\n", i, argv[i]);
			assert(0);
		}
	}

	//---渲染---
	scene = new SceneParser (input_file);//初始化场景（Camera/Lights/BackGround/Materials/Group）
	image = new Image (width, height);//初始化保存的图片
	rayTracer = new RayTracer (scene, bounces, weight, shadow, Vec3f (nx, ny, nz), isGrid);//初始化光线追踪器
	grid = rayTracer->getGrid ();

	if (gui) {
		GLCanvas glCanvas;
		glCanvas.initialize (scene, Render, TraceRay, grid, visualize_grid);//初始化OpenGL交互画布
	}
	if (stats) {
		RayTracingStats::PrintStatistics ();
	}


	//---清理---
	delete[] input_file;	input_file = NULL;
	delete[] output_file;	output_file = NULL;
	delete[] depth_file; depth_file = NULL;
	delete[] normal_file; normal_file = NULL;
	delete scene;	scene = NULL;
	delete image;	image = NULL;
	delete rayTracer;	rayTracer = NULL;
	delete grid;	grid = NULL;

	return 0;
}

