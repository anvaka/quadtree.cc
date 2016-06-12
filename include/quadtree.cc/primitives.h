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

struct Vector3 {
  static const int size = 3;
  double coord[size];

  Vector3() {
    coord[0] = coord[1] = coord[2] = 0;
  }

  Vector3(const Vector3 &other) {
//    for(int i = 0; i < size; ++i) {
//      coord[i] = other.coord[i];
//    }
    coord[0] = other.coord[0];
    coord[1] = other.coord[1];
    coord[2] = other.coord[2];
  }

  friend Vector3 operator-(Vector3 left, const Vector3& right) {
    Vector3 result(left);
    result.sub(right);
    return result;
  }

  bool isZero() {
//    for (int i = 0; i < size; ++i) {
//      if (coord[i] != 0) return false;
//    }
    // return true;
    return coord[0] == 0 && coord[1] == 0 && coord[2] == 0;

  }
  
  void reset () {
    // for (int i = 0; i < size; ++i) coord[i] = 0;
    coord[0] = coord[1] = coord[2] = 0;
  }
  
  bool sameAs(const Vector3 &other) {
    return std::abs(coord[0] - other.coord[0]) < 1e-8 &&
    std::abs(coord[1] - other.coord[1]) < 1e-8 &&
    std::abs(coord[2] - other.coord[2]) < 1e-8;
//    for (int i = 0; i < size; ++i) {
//      if (std::abs(coord[i] - other.coord[i]) >= 1e-8) return false;
//    }
//    return true;
  }
  
  bool operator==(const Vector3 &other) {
    return coord[0] == other.coord[0] &&
    coord[1] == other.coord[1] &&
    coord[2] == other.coord[2];
//    for (int i = 0; i < size; ++i) {
//      if (coord[i] != other.coord[i]) return false;
//    }
//    return true;
  }

  void operator=(const Vector3 &other) {
    // for (int i = 0; i < size; ++i) coord[i] = other.coord[i];
    coord[0] = other.coord[0];
    coord[1] = other.coord[1];
    coord[2] = other.coord[2];
  }

  double length() {
    double sum = coord[0] * coord[0] + coord[1] * coord[1] + coord[2] * coord[2];
    return sqrt(sum);
//    double sum = 0;
//    for (int i = 0; i < size; ++i) sum += coord[i] * coord[i];
//    return sqrt(sum);
  }

  Vector3* multiplyScalar(const double &scalar) {
    //for (int i = 0; i < size; ++i) coord[i] *= scalar;
    coord[0] *= scalar;
    coord[1] *= scalar;
    coord[2] *= scalar;
    return this;
  }

  Vector3* setMedian(const Vector3 min, const Vector3 max) {
    coord[0] = (min.coord[0] + max.coord[0])/2;
    coord[1] = (min.coord[1] + max.coord[1])/2;
    coord[2] = (min.coord[2] + max.coord[2])/2;
    return this;
  }

  Vector3* set(const Vector3 &other) {
    //for (int i = 0; i < size; ++i) coord[i] = other.coord[i];
    coord[0] = other.coord[0];
    coord[1] = other.coord[1];
    coord[2] = other.coord[2];
    return this;
  }

  Vector3* set(double c) {
    return set(c, c, c);
  }

  Vector3* set(double _x, double _y, double _z) {
    coord[0] = _x;
    coord[1] = _y;
    coord[2] = _z;
    return this;
  }

  Vector3* normalize() {
    auto length = this->length();
    //for (int i = 0; i < size; ++i) coord[i] /= length;
    coord[0] /= length;
    coord[1] /= length;
    coord[2] /= length;
    return this;
  }

  Vector3* addScaledVector(Vector3 &v, double s) {
    //for (int i = 0; i < size; ++i) coord[i] += v.coord[i] * s;
    coord[0] += v.coord[0] * s;
    coord[1] += v.coord[1] * s;
    coord[2] += v.coord[2] * s;
    return this;
  }

  Vector3* sub(const Vector3 &other) {
    //for (int i = 0; i < size; ++i) coord[i] -= other.coord[i];
    coord[0] -= other.coord[0];
    coord[1] -= other.coord[1];
    coord[2] -= other.coord[2];
    return this;
  }
  Vector3* add(const Vector3 &other) {
    //for (int i = 0; i < size; ++i) coord[i] += other.coord[i];
    coord[0] += other.coord[0];
    coord[1] += other.coord[1];
    coord[2] += other.coord[2];
    return this;
  }
};

class NotEnoughQuadSpaceException: public exception {};

#endif
