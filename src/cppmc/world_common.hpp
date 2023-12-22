#pragma once

#include <glm/glm.hpp>
#include <vector>

enum Direction {
  Up, Down, North, South, East, West
};

extern const std::vector<Direction> ALL_DIRECTIONS;
extern const glm::vec3 WORLD_UP;
extern const glm::vec3 WORLD_DOWN;
extern const glm::vec3 WORLD_NORTH;
extern const glm::vec3 WORLD_SOUTH;
extern const glm::vec3 WORLD_EAST;
extern const glm::vec3 WORLD_WEST;

Direction GetClosestDirection(const glm::vec3 &vec);
const glm::vec3& DirectionToVec3(Direction dir);
const char *DirectionToString(Direction dir);
void DirectionMoveIVec3(Direction dir, glm::ivec3 &vec);