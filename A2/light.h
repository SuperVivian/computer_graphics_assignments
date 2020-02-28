#ifndef _LIGHT_H_
#define _LIGHT_H_

#include "vectors.h"
#include "object3d.h"

// ====================================================================
// ====================================================================

class Light {

public:

  // CONSTRUCTOR & DESTRUCTOR
  Light() {}
  virtual ~Light() {}

  // VIRTUAL METHODS
  virtual void getIllumination (const Vec3f &p, Vec3f &dir, Vec3f &col) const = 0;//返回某点处的光源方向和光源颜色

};

// ====================================================================
// ====================================================================

class DirectionalLight : public Light {

public:

  // CONSTRUCTOR & DESTRUCTOR
  DirectionalLight(const Vec3f &d, const Vec3f &c) {
    direction = d; direction.Normalize();
    color = c; }
  ~DirectionalLight() {}

  // VIRTUAL METHOD
  void getIllumination (const Vec3f &p, Vec3f &dir, Vec3f &col) const {
    // the direction to the light is the opposite of the
    // direction of the directional light source
    dir = direction * (double)(-1.0);
    col = color;
  }

private:

  DirectionalLight(); // don't use

  // REPRESENTATION
  Vec3f direction;
  Vec3f color;

};

// ====================================================================
// ====================================================================

#endif
