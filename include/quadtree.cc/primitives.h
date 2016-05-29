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

struct LayoutSettings {
  double stableThreshold = 0.009;
  double gravity = -1.2;
  double theta = 1.2;
  double dragCoeff = 0.02;
  double springCoeff = 0.0008;
  double springLength = 30;
  double timeStep = 20;
  
  LayoutSettings& operator =(const LayoutSettings &other) {
    stableThreshold = other.stableThreshold;
    gravity = other.gravity;
    theta = other.theta;
    dragCoeff = other.dragCoeff;
    springCoeff = other.springCoeff;
    springLength = other.springLength;
    timeStep = other.timeStep;
    return *this;
  }
};

// TODO: Support standard operations from quadtree logic
struct Vector3 {
  double x = 0.0;
  double y = 0.0;
  double z = 0.0;

  Vector3(double _x, double _y, double _z) :
  x(_x), y(_y), z(_z) {};

  Vector3() {}

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
};

class NotEnoughQuadSpaceException: public exception {};

#endif
