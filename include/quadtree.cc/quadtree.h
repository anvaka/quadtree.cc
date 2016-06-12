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
#include <functional>
#include <iostream>

#include "primitives.h"
#include "random.cc/random.h"

/**
 * A single physical body in the tree node
 */
struct Body {
  Vector3 pos;
  Vector3 force;
  Vector3 velocity;
  double mass = 1.0;

  // TODO: Should this be a reference?
  std::vector<Body *> springs;  // these are outgoing connections.

  Body() {}

  Body(Vector3 _pos): pos(_pos) { }

  void setPos(const Vector3 &_pos) {
    pos = _pos;
  }

  bool positionInitialized() {
    return !pos.isZero();
  }
};

// TODO: Make it into a template by vector type?
struct QuadTreeNode {
  std::vector<QuadTreeNode *> quads;
  Body *body;

  double mass;        // This is total mass of the current node;
  Vector3 massVector; // This is a center of the mass-vector for the current node;

  Vector3 minBounds;    // "left" bounds of the node.
  Vector3 maxBounds;    // "right" bounds of the node.


  QuadTreeNode(int dimension) : quads(1 << dimension) {}

  void reset() {
    quads[0] = quads[1] = quads[2] = quads[3] = quads[4] = quads[5] = quads[6] = quads[7] = NULL;
    body = NULL;
    massVector.reset();
    mass = 0;
    minBounds.set(0);
    maxBounds.set(0);
  }

  bool isLeaf() const {
    return body != NULL;
  }
};

/**
 * Visitor for quadtree node iteration. It takes current node, and returns 
 * whether iterator should continue descent into current node.
 */
typedef std::function<bool(const QuadTreeNode *node)> QuadTreeVisitor;

/**
 * Iterates over each node of the quadtree. `visitor()` takes current node
 * and returns true or false. If true is returned, then iterator should
 * continue descent into this node. Otherwise iteration should not descent.
 */
void traverse(const QuadTreeNode *node, const QuadTreeVisitor &visitor);

/**
 * This class manages creation of a QuadTree nodes between iterations.
 * So that we are not creating to much memor pressure.
 */
class NodePool {
  size_t currentAvailable = 0;
  std::vector<QuadTreeNode *> pool;

public:
  ~NodePool() {
    for(auto node : pool) {
      delete node;
    }
  }

  void reset() {
    currentAvailable = 0;
  }

  /**
   * Gets a new node from the pool.
   */
  QuadTreeNode* get() {
    QuadTreeNode *result;
    if (currentAvailable < pool.size()) {
      result = pool[currentAvailable];
      result->reset();
    } else {
      result = new QuadTreeNode(3);
      pool.push_back(result);
    }
    currentAvailable += 1;
    return result;
  }
};

class QuadTree {
  Random random;
  double _theta;
  double _gravity;

  NodePool treeNodes;
  QuadTreeNode *root;

  QuadTreeNode *createRootNode(const std::vector<Body *> &bodies);
  void insert(Body *body, QuadTreeNode *node);

public:
  QuadTree() : QuadTree(-1.2, 0.8) {}
  QuadTree(const double &gravity, const double &theta) : random(1984), _theta(theta), _gravity(gravity) {}

  void insertBodies(const std::vector<Body *> &bodies);
  void updateBodyForce(Body *sourceBody);

  const QuadTreeNode* getRoot() {
    return root;
  }
};

#endif /* defined(__layout____quadTree__) */
