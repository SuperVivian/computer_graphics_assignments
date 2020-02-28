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
	
	fscanf (input, "%d", &num_transforms);    //����������
	//�½����顾ע��������
	matrixArray = new Matrix[num_transforms];
	matrixRandom = new float[num_transforms];

	//����ÿ������ĸ��ʺ�ֵ
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

	for (int i = 1; i <= num_points; i++)//���Ƶ�����
	{
		Vec2f V0 = Vec2f (rand () / (RAND_MAX + 0.0), rand () / (RAND_MAX + 0.0));//����̶���
		for (int k = 0; k < num_iters; k++)//��������
		{
			float p = rand () / (RAND_MAX + 0.0);//0~1֮��������
			float a = 0.0; int j=0;
			for (j = 0; j < num_transforms; j++)//�����������飬�ҵ�p��Ӧ������
			{
				float b = a+matrixRandom[j];
				if (p >= a && p < b)
				{
					break;					
				}
				a = b;				
			}
			matrixArray[j].Transform (V0);//���ݶ�Ӧ��������ľ�����б任�����ص�V0�Ѿ��仯��
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