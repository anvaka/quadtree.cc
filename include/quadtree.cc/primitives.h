//
//  primitives.h
//  layout++
//
//  Created by Andrei Kashcha on 5/21/15.
//  Copyright (c) 2015 Andrei Kashcha. All rights reserved.
//

#ifndef __primitives_h
#define __primitives_h
#include <cmath>        // std::abs
#include <vector>

using namespace std;

// TODO: Support standard operations from quadtree logic
struct Vector3 {
  double x = 0.0;
  double y = 0.0;
  double z = 0.0;
  
  Vector3(double _x, double _y, double _z) :
  x(_x), y(_y), z(_z) {};
  
  Vector3() {}

  Vector3(const Vector3 &other) {
    x = other.x;
    y = other.y;
    z = other.z;
  }

  friend Vector3 operator-(Vector3 left, const Vector3& right) {
    Vector3 result(left);
    result.sub(right);
    return result;
  }
  
  void reset () {
    x = y = z = 0;
  }
  
  bool sameAs(const Vector3 &other) {
    
    double dx = std::abs(x - other.x);
    double dy = std::abs(y - other.y);
    double dz = std::abs(z - other.z);
    
    return (dx < 1e-8 && dy < 1e-8 && dz < 1e-8);
  }
  
  bool operator==(const Vector3 &other) {
    return x == other.x && y == other.y && z == other.z;
  }

  void operator=(const Vector3 &other) {
    x = other.x; y = other.y; z = other.z;
  }

  double length() {
    return sqrt(x * x + y * y + z * z);
  }

  Vector3* multiplyScalar(const double &scalar) {
    x *= scalar;
    y *= scalar;
    z *= scalar;
    return this;
  }

  Vector3* set(const Vector3 &other) {
    x = other.x; y = other.y; z = other.z;
    return this;
  }

  Vector3* set(double c) {
    return set(c, c, c);
  }

  Vector3* set(double _x, double _y, double _z) {
    x = _x; y = _y; z = _z;
    return this;
  }

  Vector3* normalize() {
    auto length = this->length();
    x /= length;
    y /= length;
    z /= length;
    return this;
  }

  Vector3* addScaledVector(Vector3 &v, double s) {
    x += v.x * s; y += v.y * s; z += v.z * s;
    return this;
  }

  Vector3* sub(const Vector3 &other) {
    x -= other.x;
    y -= other.y;
    z -= other.z;
    return this;
  }
  Vector3* add(const Vector3 &other) {
    x += other.x;
    y += other.y;
    z += other.z;
    return this;
  }
};

class NotEnoughQuadSpaceException: public exception {};

#endif
