//
//  quadTree.cpp
//  layout++
//
//  Created by Andrei Kashcha on 5/21/15.
//  Copyright (c) 2015 Andrei Kashcha. All rights reserved.
//
//  NOTE: This code was originally implemented in https://github.com/anvaka/ngraph.quadtreebh3d/blob/master/index.js
//  Some of the coments are no longer relvant to C++.
//

#include "quadtree.cc/quadtree.h"

NotEnoughQuadSpaceException  _NotEnoughQuadSpaceException;

QuadTreeNode *QuadTree::createRootNode(const std::vector<Body *> &bodies) {
  QuadTreeNode *root = treeNodes.get();
  Vector3 &min = root->minBounds;
  Vector3 &max = root->maxBounds;
  min.set(INT32_MAX);
  max.set(INT32_MIN);

  const int size = 3;
  for (auto body : bodies) {
    for(int i = 0; i < size; ++i) {
      double v = body->pos.coord[i];
      if (v < min.coord[i]) min.coord[i] = v;
      if (v > max.coord[i]) max.coord[i] = v;
    }
  }

  // squarify bounds:
  double maxSide = 0;
  for (int i = 0; i < size; ++i) {
    double side = max.coord[i] - min.coord[i];
    if (side > maxSide) maxSide = side;
  }

  if (maxSide == 0) {
    maxSide = bodies.size() * 500;
    for (int i = 0; i < size; ++i) {
      min.coord[i] -= maxSide;
      max.coord[i] += maxSide;
    }
  } else {
    for (int i = 0; i < size; ++i) max.coord[i] = min.coord[i] + maxSide;
  }

  return root;
}

void QuadTree::insert(Body *body, QuadTreeNode *node) {
  if (node->isLeaf()) {
    // We are trying to add to the leaf node.
    // We have to convert current leaf into "internal node"
    // and continue adding two nodes.
    Body *oldBody = node->body;

    // Node is not considered a leaf it has no body:
    node->body = NULL;

    if (oldBody->pos.sameAs(body->pos)) {
      // Ugh, both bodies are at the same position. Let's try to
      // bump them within the quadrant:
      int retriesCount = 3;
      do {
        double offset = random.nextDouble();
        Vector3 diff = node->maxBounds - node->minBounds;
        diff.multiplyScalar(offset)->add(node->minBounds);

        oldBody->pos.set(diff);
        retriesCount -= 1;
        // Make sure we don't bump it out of the box. If we do, next iteration should fix it
      } while (retriesCount > 0 && oldBody->pos.sameAs(body->pos));

      if (retriesCount == 0 && oldBody->pos.sameAs(body->pos)) {
        // This is very bad, we ran out of precision.
        // We cannot proceed under current root's constraints, so let's
        // throw - this will cause parent to give bigger space for the root
        // node, and hopefully we can fit on the subsequent iteration.
        throw _NotEnoughQuadSpaceException;
      }
    }
    // Insert both bodies into a node that is no longer a leaf:
    insert(oldBody, node);
    insert(body, node);
  } else {
    // This is internal node. Update the total mass of the node and center-of-mass.
    Vector3& pos = body->pos;
    node->mass += body->mass;
    node->massVector.addScaledVector(pos, body->mass);

    // Recursively insert the body in the appropriate quadrant.
    // But first find the appropriate quadrant.
    int quadIdx = 0; // Assume we are in the 0's quad.
    Vector3 tempMin(node->minBounds), tempMax;
    tempMax.setMedian(node->minBounds, node->maxBounds);

    for (int i = 0; i < Vector3::size; ++i) {
      if (pos.coord[i] > tempMax.coord[i]) {
        quadIdx += (1 << i);
        auto oldLeft = tempMin.coord[i];
        tempMin.coord[i] = tempMax.coord[i];
        tempMax.coord[i] = tempMax.coord[i] + (tempMax.coord[i] - oldLeft);
      }
    }

    QuadTreeNode *child = node->quads[quadIdx];
    if (child) {
      // continue searching in this quadrant.
      insert(body, child);
    } else {
      // The node is internal but this quadrant is not taken. Add subnode to it.
      child = treeNodes.get();
      child->minBounds.set(tempMin);
      child->maxBounds.set(tempMax);
      child->body = body;
      node->quads[quadIdx] = child;
    }
  }
}

void QuadTree::insertBodies(const std::vector<Body *> &bodies) {
  for (int attempt = 0; attempt < 3; ++attempt) {
    try {
      treeNodes.reset();
      root = createRootNode(bodies);
      if (bodies.size() > 0) {
        root->body = bodies[0];
      }

      for (size_t i = 1; i < bodies.size(); ++i) {
        insert(bodies[i], root);
      }
      return; // no need to retry - everything inserted properly.
    } catch(NotEnoughQuadSpaceException &e) {
      // well we tried, but some bodies ended up on the same
      // spot, cannot do anything, but hope that next iteration will fix it
    }
  }
  std::cerr << "Could not insert bodies: Not enought tree precision" << std::endl;
};

void QuadTree::updateBodyForce(Body *sourceBody) {
  Vector3 force;

  auto visitNode = [&](const QuadTreeNode *node) -> bool {
    Body *body = node->body;
    if (node->body == sourceBody) return false; // no need to traverse: This is current body

    if (node->isLeaf()) {
      Vector3 dt = body->pos - sourceBody->pos;
      auto dist = dt.length();
      if (dist == 0) {
        dist = 0.1;
      }
      auto v = _gravity * body->mass * sourceBody->mass / (dist * dist * dist);
      force.addScaledVector(dt, v);

      return false; // no need to traverse this route;
    }

    // This is not a leaf then.
    // Calculate the ratio s / r,  where s is the width of the region
    // represented by the internal node, and r is the distance between the body
    // and the node's center-of-mass

    Vector3 centerOfMass(node->massVector);
    centerOfMass.multiplyScalar(1./node->mass);

    Vector3 dt = centerOfMass - sourceBody->pos;
    auto distanceToCenterOfMass = dt.length();

    if (distanceToCenterOfMass == 0) {
      distanceToCenterOfMass = 0.1;
    }

    auto regionWidth = node->maxBounds.coord[0] - node->minBounds.coord[0];
    // If s / r < θ, treat this entire node as a single body, and calculate the
    // force it exerts on sourceBody. Add this amount to sourceBody's net force.
    if (regionWidth / distanceToCenterOfMass < _theta) {
      // in the if statement above we consider node's width only
      // because the region was squarified during tree creation.
      // Thus there is no difference between using width or height.
      auto v = _gravity * node->mass * sourceBody->mass / (distanceToCenterOfMass * distanceToCenterOfMass * distanceToCenterOfMass);
      force.addScaledVector(dt, v);
      return false;
    }

    // Otherwise, run the procedure recursively on each of the current node's children.
    return true;
  };

  traverse(root, visitNode);

  sourceBody->force.add(force);
}

void traverse(const QuadTreeNode *node, const QuadTreeVisitor &visitor) {
  auto shouldDescent = visitor(node);
  if (shouldDescent) {
    int size = node->quads.size();
    for (int i = 0; i < size; ++i) {
      if (node->quads[i]) traverse(node->quads[i], visitor);
    }
  }
}