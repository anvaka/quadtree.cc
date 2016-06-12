#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.hpp"
#include "quadtree.cc/quadtree.h"

TEST_CASE( "insert and update update forces", "[insert]" ) {
  QuadTree tree;
  Body body, other;
  std::vector<Body *> bodies;
  bodies.push_back(&body);

  tree.insertBodies(bodies);
  tree.updateBodyForce(&body);

  auto same = (body.pos == other.pos);
  REQUIRE(same);
}

TEST_CASE( "It can get root", "[insert]" ) {
  QuadTree tree;
  Body body;
  std::vector<Body *> bodies;
  bodies.push_back(&body);
  
  tree.insertBodies(bodies);
  
  auto root = tree.getRoot();
  auto rootIsPresent = root != nullptr;
  REQUIRE(rootIsPresent);
  auto rootHasTheSameBody = root->body == &body;
  REQUIRE(rootHasTheSameBody);
}

TEST_CASE("Two bodies repel each other", "[insert]") {
  QuadTree tree;
  std::vector<Body *> bodies;
  bodies.push_back(new Body());
  bodies.push_back(new Body());
  Body *bodyA = bodies[0];
  Body *bodyB = bodies[1];

  bodyA->pos.coord[0] = 1; bodyA->pos.coord[1] = 0;
  bodyB->pos.coord[0] = 2; bodyB->pos.coord[1] = 0;

  tree.insertBodies(bodies);
  tree.updateBodyForce(bodyA);
  tree.updateBodyForce(bodyB);

  // 'Forces should be equivalent, with opposite sign'
  REQUIRE(bodyA->force.coord[0] + bodyB->force.coord[0] == 0);

  // 'X-force for body A should not be zero'
  REQUIRE(bodyA->force.coord[0] != 0);
  // 'X-force for body B should not be zero'
  REQUIRE(bodyB->force.coord[0] != 0);
  // On the other hand, our bodies should not move by Y axis.
  // 'Y-force for body A should be zero'
  REQUIRE(bodyA->force.coord[1] == 0);
  // 'Y-force for body B should be zero'
  REQUIRE(bodyB->force.coord[1] == 0);
};

TEST_CASE("Can handle two bodies at the same location", "[insert]") {
  QuadTree tree;
  std::vector<Body *> bodies;
  bodies.push_back(new Body());
  bodies.push_back(new Body());
  Body *bodyA = bodies[0];
  Body *bodyB = bodies[1];

  tree.insertBodies(bodies);
  tree.updateBodyForce(bodyA);
  tree.updateBodyForce(bodyB);
  
  // Both bodies got some force
  REQUIRE(bodyB->force.coord[0] != 0);
  REQUIRE(bodyA->force.coord[0] != 0);
};

TEST_CASE("It Can handle large bodies number", "[huge]") {
  QuadTree tree;
  const int count = 60000;
  std::vector<Body *> bodies;
  
  for (int i = 0; i < count; ++i) {
    bodies.push_back(new Body());
  }
  
  tree.insertBodies(bodies);
  
  for (int i = 0; i < count; ++i) {
    tree.updateBodyForce(bodies[i]);
  }
  
  REQUIRE(true);
}