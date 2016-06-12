#define CATCH_CONFIG_MAIN  // This tells Catch to provide a main() - only do this in one cpp file

#include "catch.hpp"
#include "quadtree.cc/quadtree.h"

TEST_CASE( "insert and update update forces", "[insert]" ) {
  QuadTree<3> tree;
  Body<3> body, other;
  std::vector<Body<3> *> bodies;
  bodies.push_back(&body);

  tree.insertBodies(bodies);
  tree.updateBodyForce(&body);

  auto same = (body.pos == other.pos);
  REQUIRE(same);
}

TEST_CASE( "It can get root", "[insert]" ) {
  QuadTree<3> tree;
  Body<3> body;
  std::vector<Body<3> *> bodies;
  bodies.push_back(&body);
  
  tree.insertBodies(bodies);
  
  auto root = tree.getRoot();
  auto rootIsPresent = root != nullptr;
  REQUIRE(rootIsPresent);
  auto rootHasTheSameBody = root->body == &body;
  REQUIRE(rootHasTheSameBody);
}

TEST_CASE("Two bodies repel each other", "[insert]") {
  QuadTree<3> tree;
  std::vector<Body<3> *> bodies;
  bodies.push_back(new Body<3>());
  bodies.push_back(new Body<3>());
  auto bodyA = bodies[0];
  auto bodyB = bodies[1];

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
  QuadTree<3> tree;
  std::vector<Body<3> *> bodies;
  bodies.push_back(new Body<3>());
  bodies.push_back(new Body<3>());
  auto bodyA = bodies[0];
  auto bodyB = bodies[1];

  tree.insertBodies(bodies);
  tree.updateBodyForce(bodyA);
  tree.updateBodyForce(bodyB);
  
  // Both bodies got some force
  REQUIRE(bodyB->force.coord[0] != 0);
  REQUIRE(bodyA->force.coord[0] != 0);
};

TEST_CASE("It Can handle large bodies number", "[huge]") {
  QuadTree<3> tree;
  const int count = 60000;
  std::vector<Body<3> *> bodies;
  
  for (int i = 0; i < count; ++i) {
    bodies.push_back(new Body<3>());
  }
  
  tree.insertBodies(bodies);
  
  for (int i = 0; i < count; ++i) {
    tree.updateBodyForce(bodies[i]);
  }
  
  REQUIRE(true);
}

TEST_CASE("It can be two dimensional", "[insert]") {
  QuadTree<2> tree;
  std::vector<Body<2> *> bodies;
  bodies.push_back(new Body<2>());
  bodies.push_back(new Body<2>());
  auto bodyA = bodies[0];
  auto bodyB = bodies[1];

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
}