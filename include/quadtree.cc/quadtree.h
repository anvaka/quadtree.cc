//
//  quadTree.h
//  layout++
//
//  Created by Andrei Kashcha on 5/21/15.
//  Copyright (c) 2015 Andrei Kashcha. All rights reserved.
//

#ifndef __quadTree__
#define __quadTree__

#include <vector>
#include <cmath>
#include "primitives.h"
#include "random.cc/random.h"

/**
 * A single physical body in the tree node
 */
struct Body {
  Vector3 pos;
  Vector3 prevPos;
  Vector3 force;
  Vector3 velocity;
  double mass = 1.0;

  vector<int> springs; // these are outgoing connections.
  // This is just a number of incoming connections for this body,
  // so we can count its mass appropriately.
  int incomingCount = 0;

  Body() { }
  Body(Vector3 _pos): pos(_pos), prevPos(_pos) {}

  void setPos(const Vector3 &_pos) {
    pos = _pos;
    prevPos = _pos;
  }

  bool positionInitialized() {
    return pos.x != 0 || pos.y != 0 || pos.z != 0;
  }
};

// TODO: Make it into a template by vector type?
struct QuadTreeNode {
  QuadTreeNode *quads[8];
  Body *body;
  double mass;
  Vector3 massVector;

  double left;
  double right;
  double top;
  double bottom;
  double front;
  double back;

  void reset() {
    quads[0] = quads[1] = quads[2] = quads[3] = quads[4] = quads[5] = quads[6] = quads[7] = NULL;
    body = NULL;
    massVector.reset();
    mass = 0;
    left = right = top = bottom = front = back = 0;
  }
};

class NodePool {
  size_t currentAvailable = 0;
  std::vector<QuadTreeNode *> pool;
public:
  void reset() {
    currentAvailable = 0;
  }

  QuadTreeNode* get() {
    QuadTreeNode *result;
    if (currentAvailable < pool.size()) {
      result = pool[currentAvailable];
      result->reset();
    } else {
      result = new QuadTreeNode();
      pool.push_back(result);
    }
    currentAvailable += 1;
    return result;
  }
};

class QuadTree {
  const LayoutSettings layoutSettings;
  Random random;
  
  NodePool treeNodes;
  QuadTreeNode *root;
  
  QuadTreeNode *createRootNode(std::vector<Body> &bodies);
  void insert(Body *body, QuadTreeNode *node);
  
public:
  QuadTree(const LayoutSettings& _settings) : layoutSettings(_settings), random(1984) {
  }
  
  QuadTree() : layoutSettings(), random(1984) {
  }
  
  void insertBodies(std::vector<Body> &bodies);
  void updateBodyForce(Body *sourceBody);

  const QuadTreeNode* getRoot() {
    return root;
  }
};

#endif /* defined(__layout____quadTree__) */
