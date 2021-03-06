#include "stdafx.h"
#include "scene_parser.h"
#include "image.h"
#include "group.h"
#include "hit.h"
#include "light.h"
#include "OrthographicCamera.h"
#include "PerspectiveCamera.h"
#include "raytracer.h"
#include "glCanvas.h"
#include"film.h"
#include"boxfilter.h"
#include"tentfilter.h"
#include"gaussianfilter.h"
#include"sampler.h"
#include"RandomSampler.h"
#include"UniformSampler.h"
#include"JitteredSampler.h"

static float _err = 1e-5f;
int width = 100; int height = 100;
float depth_min = 0; float depth_max = 1;
int bounce = 0;
float weight = 0.0f;
int tessellationX = 0; int tessellationY = 0;
int nx = 0; int ny = 0; int nz = 0;
int zoomFactor = 0;//zoom系数
int numSamples = 0;//采样点数
float filter_radius = 0;
float filter_sigma = 0;

bool shade_back = false;
bool cast_shadow = false;
bool gui = false;
bool visualize_grid = false;
bool grid = false;
bool gouraud = false;
bool stats = false;
bool render_samples = false;//是否渲染采样图
enum {
	random_samples,
	uniform_samples,
	jittered_samples
}sampler;
bool render_filter = false;//是否渲染过滤图
enum {
	box_filter,
	tent_filter,
	gaussian_filter
}filter;

char *input_file = new char[100];
char *output_file = NULL;
char *depth_file = NULL;
char *normal_file = NULL;
char*sample_file = new char[100];
char*filter_file =NULL;
SceneParser* scene=NULL;
Grid *g = NULL;
RayTracer* raytracer=NULL;


void traceRayFunction (float x, float y)
{
	x = x * 2.0f - 1.0f;
	y = y * 2.0f - 1.0f;
	Ray ray = scene->getCamera ()->generateRay (Vec2f (x, y));
	Hit h (INT_MAX, NULL, Vec3f (0.0f, 0.0f, 1.0f));
	float tmin = 0.1f;
	RayTracer rt (scene, bounce, weight, cast_shadow, Vec3f (nx, ny, nz), grid,visualize_grid);
	Vec3f pixel = rt.traceRay (ray, tmin, 0, 1.0, 1.0, h, shade_back, true);
}

void RenderScene ()
{
	printf ("Rendering....");
	Image* image = new Image (width, height);
	image->SetAllPixels (Vec3f (0.f, 0.f, 0.f));
	Image* depth_image = new Image (width, height);
	depth_image->SetAllPixels (Vec3f (0.0, 0.0, 0.0));
	float zone = depth_max - depth_min;
	if (zone == 0)	{	zone = 1.0f;}
	Image* normal_image = new Image (width, height);
	normal_image->SetAllPixels (Vec3f (0.0, 0.0, 0.0));

	Film* film = new Film(width, height, numSamples);
	Filter*m_filter=NULL;
	switch (filter) {
	case box_filter: m_filter = new BoxFilter(filter_radius); break;
	case tent_filter: m_filter = new TentFilter(filter_radius); break;
	case gaussian_filter: m_filter = new GaussianFilter(filter_sigma); break;
	}
	if (m_filter == NULL){ m_filter = new BoxFilter(_err); }//默认过滤

	Sampler*m_sampler = NULL;
	switch (sampler) {
	case random_samples: m_sampler = new RandomSampler (); break;
	case uniform_samples: m_sampler = new UniformSampler (numSamples); break;
	case jittered_samples: m_sampler = new JitteredSampler (numSamples); break;
	}

	if(m_sampler==NULL)
	{
		numSamples = 1;
		m_sampler = new UniformSampler (numSamples);
	}//默认采样方式

	for (int x = 0; x < width; x++) 
	{
		for (int y = 0; y < height; y++)
		{
			float maxImageSize = max (width, height);
			float xMargin = (maxImageSize - width) / 2.f;
			float yMargin = (maxImageSize - height) / 2.f;
			Vec2f pixelPos ((float (x) + xMargin) / maxImageSize, (float (y) + yMargin) / maxImageSize);//像素坐标
			Vec2f pixelSize (1.f / width, 1.f / height);//像素大小

			Hit hit (INT_MAX, NULL, Vec3f (0.0f, 0.0f, 0.0f));

			//对sampler指定的采样点采样，并把颜色存到film中
			if (numSamples > 0) {
				for (int i = 0; i < film->getNumSamples (); ++i) {//每个像素存储的采样数量
					if (x == 2 && y == 2&& i==0)
					{
						printf ("  ssss");
					}
					Hit hit (INT_MAX, NULL, Vec3f (0.0f, 0.0f, 0.0f));
					Vec2f samplePos = m_sampler->getSamplePosition (i);//得到采样点在像素中的位置
					Ray ray = scene->getCamera ()->generateRay (pixelPos + samplePos * pixelSize);//产生射线
					Vec3f pixel = raytracer->traceRay (ray, scene->getCamera ()->getTMin (), 1, 1.0f, 0, hit, shade_back, true);//ray tracer
					film->setSample (x, y, i, samplePos, pixel);//存到film中
				}
			}
			else {
				Ray ray = scene->getCamera ()->generateRay (pixelPos);//产生射线
				Vec3f pixel = raytracer->traceRay (ray, scene->getCamera ()->getTMin (), 1, 1.0f, 0, hit, shade_back, true);//ray tracer
					image->SetPixel (x, y, pixel);
			}
				
		
			float t = hit.getT ();
			if (t < depth_min)
			{
				t = depth_min;
			}
			if (t > depth_max)
			{
				t = depth_max;
			}
			float volumn = (depth_max - t) / zone;
			depth_image->SetPixel (x, y, Vec3f (volumn, volumn, volumn));
			Vec3f normal = hit.getNormal ();
			Vec3f normal_fab = Vec3f (fabs (normal.x ()), fabs (normal.y ()), fabs (normal.z ()));
			normal_image->SetPixel (x, y, normal_fab);
		}
	}

	//得到每个像素的最终颜色
	if (output_file != NULL) {
		if (numSamples > 0) {
			for (int x = 0; x < width; ++x) {
				for (int y = 0; y < height; ++y) {
					image->SetPixel (x, y, m_filter->getColor (x, y,film));
				}
			}
			image->SaveTGA (output_file);
		}
		else {
			image->SaveTGA (output_file);
		}		
	}

	if (depth_file != NULL) {
		depth_image->SaveTGA (depth_file);
	}
	if (normal_file != NULL) {
		normal_image->SaveTGA (normal_file);
	}
	if (sample_file != NULL && render_samples) {
		film->renderSamples (sample_file,zoomFactor);
	}
	if (filter_file != NULL && render_filter && m_filter!=NULL) {
		film->renderFilter (filter_file, zoomFactor, m_filter);
	}

	printf ("done.\n");	
	if (stats) {
		RayTracingStats::PrintStatistics ();
	}

}




int main (int argc, char*argv[])
{
	//解析输入命令
	bool in = true;
	if (in) {
		for (int i = 1; i < argc; i++) {
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
			}
			else if (!strcmp (argv[i], "-shade_back")) {
				shade_back = true;
			}
			else if (!strcmp (argv[i], "-bounces")) {
				i++;
				assert (i < argc);
				bounce = atoi (argv[i]);
			}
			else if (!strcmp (argv[i], "-weight")) {
				i++;
				assert (i < argc);
				weight = (float)(atof (argv[i]));
			}
			else if (!strcmp (argv[i], "-shadows")) {
				cast_shadow = true;
			}
			else if (!strcmp (argv[i], "-gui")) {
				gui = true;
			}
			else if (!strcmp (argv[i], "-tessellation")) {
				i++;
				assert (i < argc);
				tessellationX = atoi (argv[i]);
				i++;
				assert (i < argc);
				tessellationY = atoi (argv[i]);
			}
			else if (!strcmp (argv[i], "-gouraud")) {
				gouraud = true;
			}
			else if (!strcmp (argv[i], "-visualize_grid")) {
				visualize_grid = true;
			}
			else if (!strcmp (argv[i], "-stats")) {
				stats = true;
			}
			else if (!strcmp (argv[i], "-render_samples")) {
				render_samples = true;
				i++; assert (i < argc);
				sample_file = argv[i];
				i++; assert (i < argc);
				zoomFactor = atoi (argv[i]);//int类型
			}
			else if (!strcmp (argv[i], "-random_samples")) {
				sampler=random_samples;
				i++;
				assert (i < argc);
				numSamples = atoi (argv[i]);
			}
			else if (!strcmp (argv[i], "-uniform_samples")) {
				sampler=uniform_samples;
				i++;
				assert (i < argc);
				numSamples = atoi (argv[i]);
			}
			else if (!strcmp (argv[i], "-jittered_samples")) {
				sampler=jittered_samples;
				i++;
				assert (i < argc);
				numSamples = atoi (argv[i]);
			}
			else if (!strcmp (argv[i], "-render_filter")) {
				render_filter = true;
				i++; assert (i < argc);
				filter_file = argv[i];
				i++;	assert (i < argc);
				zoomFactor = atoi (argv[i]);
			}
			else if (!strcmp (argv[i], "-box_filter")) {
				filter = box_filter;
				i++;	assert (i < argc);
				filter_radius = atof (argv[i]);//float类型
			}
			else if (!strcmp (argv[i], "-gaussian_filter")) {
				filter = gaussian_filter;
				i++;	assert (i < argc);
				filter_sigma = atof (argv[i]);//float类型
			}
			else if (!strcmp (argv[i], "-tent_filter")) {
				filter = tent_filter;
				i++;	assert (i < argc);
				filter_radius = atof (argv[i]);//float类型
			}
			else if (!strcmp (argv[i], "-grid")) {
				grid = true;
				i++;
				assert (i < argc);
				nx = atoi (argv[i]);
				i++;
				assert (i < argc);
				ny = atoi (argv[i]);
				i++;
				assert (i < argc);
				nz = atoi (argv[i]);
			}
			else {
				printf ("whoops error with command line argument %d: '%s'\n", i, argv[i]);
				assert (0);
			}
		}
	}
	else {
		//-input scene7_01_scenehere_triangle.txt -size 9 9 -render_samples samples7_01a.tga 20 -random_samples 4
		string tempStr;
		cin >> tempStr >> input_file;
		cin >> tempStr >> width >> height;
		cin >> tempStr;
		if (tempStr == "-render_samples")render_samples = true;
		cin >>sample_file;
		cin >> zoomFactor;
		cin >> tempStr;
		if (tempStr == "-random_samples") {
			sampler = random_samples;
			cin >> numSamples;
		}
		else if (tempStr == "-uniform_samples") {
			sampler = uniform_samples;
			cin >> numSamples;
		}
		else if (tempStr == "-jittered_samples") {
			sampler = jittered_samples;
			cin >> numSamples;
		}

	
	  

		//if (tempStr == "-grid")grid = true;
		//cin >> nx >> ny >> nz;
		/*cin >> tempStr;
		if (tempStr == "-stats")stats = true;
		cin >> tempStr;
		if (tempStr == "-shadows")cast_shadow = true;		*/
	}
	

	scene = new SceneParser (input_file);


	RayTracingStats::Initialize (width, height, NULL, 0, 0, 0);

	raytracer =new RayTracer (scene, bounce, weight, cast_shadow, Vec3f (nx, ny, nz), grid, visualize_grid);
	g = raytracer->getGrid ();

	if (gui) {
		GLCanvas canvas;
		canvas.initialize (scene, RenderScene, traceRayFunction, g, visualize_grid);
	}
	
	RenderScene ();

	system ("pause");
	return 0;
}