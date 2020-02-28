
#include "stdafx.h"
#include "marchinginfo.h"
#include <assert.h>

MarchingInfo::MarchingInfo()
{
	tmin = INFINITY;
	tnext_x = INFINITY;
	tnext_y = INFINITY;
	tnext_z = INFINITY;
}

void MarchingInfo::nextCell()
{
	assert(tnext_x >= 0 && tnext_y >= 0 && tnext_z >= 0);
	assert(dt_x >= 0 && dt_y >= 0 && dt_z >= 0);

	if (tnext_x < tnext_y) {
		if (tnext_x < tnext_z) {
			i += sign_x;
			tmin = tnext_x;
			tnext_x += dt_x;
			normal = Vec3f(-1.0f, 0.0f, 0.0f) * sign_x;
		}
		else {
			k += sign_z;
			tmin = tnext_z;
			tnext_z += dt_z;
			normal = Vec3f(0.0f, 0.0f, -1.0f) * sign_z;
		}
	}
	else {
		if (tnext_y < tnext_z) {
			j += sign_y;
			tmin = tnext_y;
			tnext_y += dt_y;
			normal = Vec3f(0.0f, -1.0f, 0.0f) * sign_y;
		}
		else {
			k += sign_z;
			tmin = tnext_z;
			tnext_z += dt_z;
			normal = Vec3f(0.0f, 0.0f, -1.0f) * sign_z;
		}
	}
}