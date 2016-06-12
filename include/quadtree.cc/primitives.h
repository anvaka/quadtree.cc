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

template <size_t DIMENSION>
struct Vector3 {
  static const int size = DIMENSION;
  double coord[size];

  Vector3() {
    for(int i = 0; i < size; ++i) coord[i] = 0;
  }

  Vector3(const Vector3 &other) {
    for(int i = 0; i < size; ++i) {
      coord[i] = other.coord[i];
    }
  }

  friend Vector3 operator-(Vector3 left, const Vector3& right) {
    Vector3 result(left);
    result.sub(right);
    return result;
  }

  bool isZero() {
    for (int i = 0; i < size; ++i) {
      if (coord[i] != 0) return false;
    }
    return true;
  }
  
  void reset () {
    for (int i = 0; i < size; ++i) coord[i] = 0;
  }
  
  bool sameAs(const Vector3 &other) {
    for (int i = 0; i < size; ++i) {
      if (std::abs(coord[i] - other.coord[i]) >= 1e-8) return false;
    }
    return true;
  }
  
  bool operator==(const Vector3 &other) {
    for (int i = 0; i < size; ++i) {
      if (coord[i] != other.coord[i]) return false;
    }
    return true;
  }

  void operator=(const Vector3 &other) {
    for (int i = 0; i < size; ++i) coord[i] = other.coord[i];
  }

  double length() {
    double sum = 0;
    for (int i = 0; i < size; ++i) sum += coord[i] * coord[i];
    return sqrt(sum);
  }

  Vector3* multiplyScalar(const double &scalar) {
    for (int i = 0; i < size; ++i) coord[i] *= scalar;
    return this;
  }

  Vector3* setMedian(const Vector3 min, const Vector3 max) {
    for(int i = 0; i < size; ++i) coord[i] = (min.coord[i] + max.coord[i]) / 2.0;
    return this;
  }

  Vector3* set(const Vector3 &other) {
    for (int i = 0; i < size; ++i) coord[i] = other.coord[i];
    return this;
  }

  Vector3* set(double c) {
    for (int i = 0; i < size; ++i) coord[i] = c;
    return this;
  }

  Vector3* normalize() {
    auto length = this->length();
    for (int i = 0; i < size; ++i) coord[i] /= length;
    return this;
  }

  Vector3* addScaledVector(Vector3 &v, double s) {
    for (int i = 0; i < size; ++i) coord[i] += v.coord[i] * s;
    return this;
  }

  Vector3* sub(const Vector3 &other) {
    for (int i = 0; i < size; ++i) coord[i] -= other.coord[i];
    return this;
  }
  Vector3* add(const Vector3 &other) {
    for (int i = 0; i < size; ++i) coord[i] += other.coord[i];
    return this;
  }
};

template <>
struct Vector3<3> {
  static const int size = 3;
  double coord[size];

  Vector3() {
    coord[0] = coord[1] = coord[2] = 0;
  }

  Vector3(const Vector3 &other) {
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
    return coord[0] == 0 && coord[1] == 0 && coord[2] == 0;

  }

  void reset () {
    coord[0] = coord[1] = coord[2] = 0;
  }

  bool sameAs(const Vector3 &other) {
    return std::abs(coord[0] - other.coord[0]) < 1e-8 &&
    std::abs(coord[1] - other.coord[1]) < 1e-8 &&
    std::abs(coord[2] - other.coord[2]) < 1e-8;
  }

  bool operator==(const Vector3 &other) {
    return coord[0] == other.coord[0] &&
    coord[1] == other.coord[1] &&
    coord[2] == other.coord[2];
  }

  void operator=(const Vector3 &other) {
    coord[0] = other.coord[0];
    coord[1] = other.coord[1];
    coord[2] = other.coord[2];
  }

  double length() {
    double sum = coord[0] * coord[0] + coord[1] * coord[1] + coord[2] * coord[2];
    return sqrt(sum);
  }

  Vector3* multiplyScalar(const double &scalar) {
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
    coord[0] = other.coord[0];
    coord[1] = other.coord[1];
    coord[2] = other.coord[2];
    return this;
  }

  Vector3* set(double c) {
    coord[0] = c;
    coord[1] = c;
    coord[2] = c;
    return this;
  }

  Vector3* normalize() {
    auto length = this->length();
    coord[0] /= length;
    coord[1] /= length;
    coord[2] /= length;
    return this;
  }

  Vector3* addScaledVector(Vector3 &v, double s) {
    coord[0] += v.coord[0] * s;
    coord[1] += v.coord[1] * s;
    coord[2] += v.coord[2] * s;
    return this;
  }

  Vector3* sub(const Vector3 &other) {
    coord[0] -= other.coord[0];
    coord[1] -= other.coord[1];
    coord[2] -= other.coord[2];
    return this;
  }
  Vector3* add(const Vector3 &other) {
    coord[0] += other.coord[0];
    coord[1] += other.coord[1];
    coord[2] += other.coord[2];
    return this;
  }
};

class NotEnoughQuadSpaceException: public exception {};

#endif
