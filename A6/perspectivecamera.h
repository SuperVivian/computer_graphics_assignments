#ifndef PERSPECTIVECAMERA_H
#define PERSPECTIVECAMERA_H

#include "camera.h"

class PerspectiveCamera :public Camera
{
public:
	PerspectiveCamera(Vec3f &center, Vec3f &direction, Vec3f &up, float angle);
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
	Vec3f horizontal;
	Vec3f up;
	float angle;
	float distance;
};

#endif