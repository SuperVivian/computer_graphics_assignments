#include "stdafx.h"
#include "TriangleMesh.h"


TriangleMesh::TriangleMesh (const char * filename, Material* m)
{
	this->mat = m;
}


TriangleMesh::~TriangleMesh ()
{
}
void TriangleMesh::paint () {

}
bool TriangleMesh::intersect (const Ray &r, Hit &h, float tmin) {
	return false;
}