#pragma once
#include "Object3D.h"
#include "object3dvector.h"
#include "marchinginfo.h"
#include "phongmaterial.h"
#include "rayTree.h"
#include <vector>
class Grid :	public Object3D
{
public:
	Grid (BoundingBox *bb, int nx, int ny, int nz);
	~Grid ();
	BoundingBox * getBoundingBox ();
	Vec3f getGird ();
	void insertIntoThis (int i, bool v, Object3D *obj);
	virtual bool intersect (const Ray &r, Hit &h, float tmin);
	virtual void paint ();
	void initializeRayMarch (MarchingInfo &mi,
		const Ray &r, float tmin) const;
	void setMaterial (Material *m) { this->material = m; };
	void changeMaterial (int);
private:
	int nx;
	int ny;
	int nz;
	vector<bool> m_is_voxel_opaque;
	vector<int> objNum;
	vector<Object3D*>object3ds;
	bool visualize;
};

