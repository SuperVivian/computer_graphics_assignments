#pragma once
#include"Ray.h"
#include"Hit.h"
#include <GL/gl.h>
#include <GL/glu.h>
#include"matrix.h"
class Camera
{
public:
	Camera () {};
	~Camera () {};
	virtual Ray generateRay (Vec2f point) = 0;
	virtual float getTMin () const = 0;
	virtual void glInit (int w, int h) = 0;
	virtual void glPlaceCamera (void) = 0;
	virtual void dollyCamera (float dist) = 0;
	virtual void truckCamera (float dx, float dy) = 0;
	virtual void rotateCamera (float rx, float ry) = 0;
};



