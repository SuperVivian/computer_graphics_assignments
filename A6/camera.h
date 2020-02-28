#ifndef CAMERA_H
#define CAMERA_H

#include <windows.h>
#include <GL/GL.h>
#include <GL/glut.h>

#include "vectors.h"
#include "ray.h"
#include "hit.h"
#include "matrix.h"

class Camera
{
public:
	virtual Ray generateRay(Vec2f point) = 0;
	virtual float getTMin() const = 0;
	virtual void glPlaceCamera() = 0;
	virtual void dollyCamera(float dist) = 0;
	virtual void rotateCamera(float x_theta, float y_theta) = 0;
	virtual void glInit(int w, int h) = 0;
	virtual void truckCamera(float dx, float dy) = 0;
};

#endif