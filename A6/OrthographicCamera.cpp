
#include "stdafx.h"
#include "OrthographicCamera.h";

OrthographicCamera::OrthographicCamera(Vec3f center, Vec3f direction, Vec3f up, float size)
{
	this->center = center;
	this->direction = direction;
	this->up = up;
	this->size = size;
	this->direction.Normalize();
	this->up.Normalize();
	Vec3f::Cross3(this->horizontal, this->direction, this->up);
	Vec3f::Cross3(this->up, this->horizontal, this->direction);
	this->horizontal.Normalize();
	this->up.Normalize();
}

Ray OrthographicCamera::generateRay(Vec2f point)
{
	Vec3f origin = center + (-0.5f + point.x ())*size*horizontal + (-0.5f + point.y ())*size*up;
	Ray ray (origin, direction);
	return  ray;
}

float OrthographicCamera::getTMin() const
{
	return -1.0f * INT_MAX;
}

void OrthographicCamera::glPlaceCamera()
{
	gluLookAt(center.x(), center.y(), center.z(), center.x() + direction.x(), center.y() + direction.y(), center.z() + direction.z(), up.x(), up.y(), up.z());
}

void OrthographicCamera::dollyCamera(float dist)
{
	center = center + direction * dist;
}

void OrthographicCamera::rotateCamera(float rx, float ry)
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

}

void OrthographicCamera::glInit(int w, int h)
{
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	if (w > h)
	{
		glOrtho(-size / 2.0f, size / 2.0f, -size*(float)h / (float)w / 2.0, size*(float)h / (float)w / 2.0, 0.5, 40.0);
	}
	else
	{
		glOrtho(-size*(float)w / (float)h / 2.0, size*(float)w / (float)h / 2.0, -size / 2.0, size / 2.0, 0.5, 40.0);
	}
}

void OrthographicCamera::truckCamera(float dx, float dy)
{
	Vec3f screenUp;
	Vec3f::Cross3(screenUp, horizontal, direction);
	center += horizontal * dx + screenUp * dy;
}

