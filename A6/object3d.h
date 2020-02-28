#ifndef OBJECT3D_H
#define OBJECT3D_H

#define MIN(a,b,c) (a<b?(a<c?a:c):(b<c?b:c))
#define MAX(a,b,c) (a>b?(a>c?a:c):(b>c?b:c))

#include <windows.h>
#include <GL/GL.h>
#include <GL/glut.h>

#include "hit.h"
#include "material.h"
#include "ray.h"
#include "boundingbox.h"
#include "object3dvector.h"

class Grid;
class Transform;

enum ObjectType {
	SphereObject, PlaneObject, TriangleObject, GridObject, GroupObject
};
class Object3D
{
public:
	Object3D(){ material = NULL; };
	virtual bool intersect(const Ray &r, Hit &h, float tmin) = 0;
	virtual void paint() = 0;
	virtual BoundingBox * getBoundingBox () { return boundingBox; }

	virtual void insertIntoGrid(Grid *g, Matrix *m);
	ObjectType getObjectType () const { return _type; }
	~Object3D(){};
public:
	bool triangle = false;
	Vec3f a;
	Vec3f b;
	Vec3f c;
	ObjectType _type;
	Material *material;
	BoundingBox *boundingBox;
};

#endif