
#include "stdafx.h"
#include "perspectivecamera.h"

PerspectiveCamera::PerspectiveCamera(Vec3f &center, Vec3f &direction, Vec3f &up, float angle)
{
	this->center = center;
	this->direction = direction;
	this->up = up;
	this->angle = angle;
	Vec3f::Cross3(this->horizontal, this->direction, this->up);
	Vec3f::Cross3(this->up, this->horizontal, this->direction);
	this->direction.Normalize();
	this->up.Normalize();
	this->horizontal.Normalize();
	this->distance = 1.0f / tanf(angle / 2);
}

Ray PerspectiveCamera::generateRay(Vec2f point)
{
	Vec3f origin = center;
	//映射到虚拟image上，然后确定dir
	float  k_size = tan (angle / 2.0f); /* if (k_size < 0)k_size = 0;*/
	Vec3f end = center + (-0.5 + point.x ())*horizontal* k_size
		+ (-0.5 + point.y ())*up* k_size + direction * 0.5f;
	Vec3f dir = end - center;  dir.Normalize ();
	Ray ray (origin, dir);
	return  ray;
}

float PerspectiveCamera::getTMin() const
{
	return 0.0;
}

void PerspectiveCamera::glPlaceCamera()
{
	gluLookAt(center.x(), center.y(), center.z(), center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(), up.x(), up.y(), up.z());
}

void PerspectiveCamera::dollyCamera(float dist)
{
	center = center + direction * dist;
}

void PerspectiveCamera::rotateCamera(float rx, float ry)
{
	Vec3f horizontal;
	Vec3f::Cross3 (horizontal, direction, up);
	horizontal.Normalize ();

	// Don't let the model flip upside-down (There is a singularity
	// at the poles when 'up' and 'direction' are aligned)
	float tiltAngle = acos (up.Dot3 (direction));
	if (tiltAngle - ry > 3.13)
		ry = tiltAngle - 3.13;
	else if (tiltAngle - ry < 0.01)
		ry = tiltAngle - 0.01;

	Matrix rotMat = Matrix::MakeAxisRotation (up, rx);
	rotMat *= Matrix::MakeAxisRotation (horizontal, ry);

	rotMat.Transform (center);
	rotMat.TransformDirection (direction);
	direction.Normalize ();

}

void PerspectiveCamera::glInit(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(angle *180.0 / 3.14159, (float)w / float(h), 0.5, 40.0);
}

void PerspectiveCamera::truckCamera(float dx, float dy)
{
	Vec3f screenUp;
	Vec3f::Cross3(screenUp, horizontal, direction);
	center += horizontal * dx + screenUp * dy;
}

