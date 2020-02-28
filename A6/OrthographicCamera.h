#ifndef ORTHOGRAPHICCAMERA_H
#define ORTHOGRAPHICCAMERA_H

#include "camera.h"

class OrthographicCamera :public Camera
{
public:
	OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size);
	virtual Ray generateRay(Vec2f point);
	virtual float getTMin() const;
	virtual void glPlaceCamera();
	virtual void dollyCamera(float dist);
	virtual void rotateCamera(float x_theta, float y_theta);
	virtual void glInit(int w, int h);
	virtual void truckCamera(float dx, float dy);
private:
	Vec3f center;
	Vec3f direction;
	Vec3f up;
	Vec3f horizontal;
	float size;
};

#endif