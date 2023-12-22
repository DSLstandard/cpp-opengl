#pragma once

#include "cppmc/world_common.hpp"
#include <GL/glew.h>
#include <glm/glm.hpp>
#include <map>
#include <glm/gtx/hash.hpp>
#include <vector>

class TextureAtlas {
private:
  GLuint m_Texture;
  int m_NumRows;
  int m_NumCols;

public:
  TextureAtlas(GLuint texture, int num_rows, int num_cols);

  std::vector<GLfloat> GetTexCoordsAt(const glm::ivec2 &atlascoord) const;

  GLuint GetTextureID();
};

class BlockTextures {
private:
  // std::unordered_map is better but I dont want to deal with making pairs hashable in this god awful language
  std::map<std::pair<int, Direction>, glm::ivec2> m_Mapping;
  glm::ivec2 m_ErrorAtlasCoord;

  void Initialize();

public:
  BlockTextures();

  void Register(int block_id, Direction dir, const glm::ivec2 &atlas_coord);

  glm::ivec2 GetAtlasCoord(int block_id, Direction dir);
};