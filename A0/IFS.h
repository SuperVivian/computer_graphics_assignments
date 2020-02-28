#pragma once
#include"image.h"
#include"matrix.h"
#include<assert.h>
#include<iostream>
#include<string>
#include<fstream>
#include<stdlib.h>
#include<time.h>
using namespace std;

class IFS
{
public:
	IFS ();
	~IFS ();

	void Input (const char *file);
	void Render (Image *image, const int &num_points, const int &num_iters);
	void CleanUp ();
private:
	int num_transforms;   //the number of transformations
	Matrix *matrixArray;
	float *matrixRandom;
};

