#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.hpp"
#include "quadtree.cc/quadtree.h"

TEST_CASE( "insert and update update forces", "[insert]" ) {
  QuadTree tree;
  Body body, other;
  std::vector<Body> bodies;
  bodies.push_back(body);

  tree.insertBodies(bodies);
  tree.updateBodyForce(&body);

  REQUIRE(body.pos == other.pos);
}

TEST_CASE( "It can get root", "[insert]" ) {
  QuadTree tree;
  Body body;
  std::vector<Body> bodies;
  bodies.push_back(body);
  
  tree.insertBodies(bodies);
  
  auto root = tree.getRoot();
  auto rootIsPresent = root != nullptr;
  REQUIRE(rootIsPresent);
  auto rootHasTheSameBody = root->body == &bodies[0];
  REQUIRE(rootHasTheSameBody);
}

TEST_CASE("Two bodies repel each other", "[insert]") {
  QuadTree tree;
  std::vector<Body> bodies;
  bodies.push_back(Body());
  bodies.push_back(Body());
  Body *bodyA = &bodies[0];
  Body *bodyB = &bodies[1];

  bodyA->pos.x = 1; bodyA->pos.y = 0;
  bodyB->pos.x = 2; bodyB->pos.y = 0;

  
  tree.insertBodies(bodies);
  tree.updateBodyForce(bodyA);
  tree.updateBodyForce(bodyB);

  // 'Forces should be equivalent, with opposite sign'
  REQUIRE(bodyA->force.x + bodyB->force.x == 0);

  // 'X-force for body A should not be zero'
  REQUIRE(bodyA->force.x != 0);
  // 'X-force for body B should not be zero'
  REQUIRE(bodyB->force.x != 0);
  // On the other hand, our bodies should not move by Y axis.
  // 'Y-force for body A should be zero'
  REQUIRE(bodyA->force.y == 0);
  // 'Y-force for body B should be zero'
  REQUIRE(bodyB->force.y == 0);
};

TEST_CASE("Can handle two bodies at the same location", "[insert]") {
  QuadTree tree;
  std::vector<Body> bodies;
  bodies.push_back(Body());
  bodies.push_back(Body());
  Body *bodyA = &bodies[0];
  Body *bodyB = &bodies[1];

  tree.insertBodies(bodies);
  tree.updateBodyForce(bodyA);
  tree.updateBodyForce(bodyB);
  
  // Both bodies got some force
  REQUIRE(bodyB->force.x != 0);
  REQUIRE(bodyA->force.x != 0);
};