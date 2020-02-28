#ifndef OBJECT3D_H_
#define OBJECT3D_H_

#include"Ray.h"
#include"Hit.h"
#include"Material.h"

class Object3D
{
public:
	Object3D () {};
	~Object3D () {};
	virtual bool intersect (const Ray &r, Hit &h, float tmin) = 0;
protected:
	Material * material;
};
#endif
