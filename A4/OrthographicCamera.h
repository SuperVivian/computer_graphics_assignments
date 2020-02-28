#pragma once
#include "Camera.h"
#include"Group.h"
class OrthographicCamera :	public Camera
{
public:
	OrthographicCamera (Vec3f center, Vec3f direction, Vec3f up, float size);
	~OrthographicCamera ();
	virtual Ray generateRay (Vec2f point);
	virtual float getTMin () const;
	virtual void glInit (int w, int h);
	virtual void glPlaceCamera (void);
	virtual void dollyCamera (float dist);
	virtual void truckCamera (float dx, float dy);
	virtual void rotateCamera (float rx, float ry);
private:
	Vec3f center;
	Vec3f direction;
	Vec3f up;
	Vec3f horizontal;
	float imgSize;
};

