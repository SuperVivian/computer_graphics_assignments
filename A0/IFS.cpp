#include "stdafx.h"
#include "IFS.h"

IFS::IFS ()
{
}


IFS::~IFS ()
{	
}

void IFS::Input (const char *file)
{
	FILE *input = fopen (file, "r");
	assert (input != NULL);
	
	fscanf (input, "%d", &num_transforms);    //读入矩阵个数
	//新建数组【注意析构】
	matrixArray = new Matrix[num_transforms];
	matrixRandom = new float[num_transforms];

	//读入每个矩阵的概率和值
	for (int i = 0; i < num_transforms; i++) 
	{
		fscanf (input, "%f", &matrixRandom[i]);
		matrixArray[i].Read3x3 (input);
	}
	fclose (input);
}

void IFS::Render (Image *image, const int &num_points, const int &num_iters)
{
	int width = image->Width ();
	int height = image->Height ();

	for (int i = 1; i <= num_points; i++)//绘制点总数
	{
		Vec2f V0 = Vec2f (rand () / (RAND_MAX + 0.0), rand () / (RAND_MAX + 0.0));//定义固定点
		for (int k = 0; k < num_iters; k++)//迭代次数
		{
			float p = rand () / (RAND_MAX + 0.0);//0~1之间的随机数
			float a = 0.0; int j=0;
			for (j = 0; j < num_transforms; j++)//遍历概率数组，找到p对应的区间
			{
				float b = a+matrixRandom[j];
				if (p >= a && p < b)
				{
					break;					
				}
				a = b;				
			}
			matrixArray[j].Transform (V0);//根据对应概率区间的矩阵进行变换，返回的V0已经变化了
		}
		if (V0.x () >= 0 && V0.x () <= 1 && V0.y () >= 0 && V0.y () <= 1)
		{
			image->SetPixel (V0.x ()*width, V0.y ()*height, Vec3f (1, 1, 1));
		}
	}
}

void  IFS::CleanUp ()
{
	delete[] matrixArray;
	matrixArray = NULL;
	delete[] matrixRandom;
	matrixRandom = NULL;
}