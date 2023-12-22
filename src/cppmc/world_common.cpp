#include "world_common.hpp"

const std::vector<Direction> ALL_DIRECTIONS = {Up, Down, North, East, South, West};

const glm::vec3 WORLD_UP = glm::vec3(0.0f, 1.0f, 0.0f);
const glm::vec3 WORLD_DOWN = glm::vec3(0.0f, -1.0f, 0.0f);
const glm::vec3 WORLD_NORTH = glm::vec3(0.0f, 0.0f, -1.0f);
const glm::vec3 WORLD_SOUTH = glm::vec3(0.0f, 0.0f, 1.0f);
const glm::vec3 WORLD_EAST = glm::vec3(1.0f, 0.0f, 0.0f);
const glm::vec3 WORLD_WEST = glm::vec3(-1.0f, 0.0f, 0.0f);
const glm::vec3 WORLD_FORWARD = WORLD_NORTH;

Direction GetClosestDirection(const glm::vec3 &vec) {
  Direction best_dir;
  float best_score;

  bool first = true;
  for (auto dir : ALL_DIRECTIONS) {
    float score = glm::dot(vec, DirectionToVec3(dir));
    if (first || score > best_score) {
      best_score = score;
      best_dir = dir;
      first = false;
    }
  }
  return best_dir;
}

const glm::vec3& DirectionToVec3(Direction dir) {
  switch (dir) {
    case Direction::Up: 
      return WORLD_UP;
    case Direction::Down: 
      return WORLD_DOWN;
    case Direction::North: 
      return WORLD_NORTH;
    case Direction::South: 
      return WORLD_SOUTH;
    case Direction::East: 
      return WORLD_EAST;
    case Direction::West: 
      return WORLD_WEST;
  }
}

void DirectionMoveIVec3(Direction dir, glm::ivec3 &vec) {
  switch (dir) {
    case Direction::Up: 
      vec.y++;
      break;
    case Direction::Down: 
      vec.y--;
      break;
    case Direction::North: 
      vec.z--;
      break;
    case Direction::South: 
      vec.z++;
      break;
    case Direction::East: 
      vec.x++;
      break;
    case Direction::West: 
      vec.x--;
      break;
  }
}

const char *DirectionToString(Direction dir) {
  switch (dir) {
    case Up: return "Up";
    case Down: return "Down";
    case North: return "North";
    case South: return "South";
    case East: return "East";
    case West: return "West";
  }
}