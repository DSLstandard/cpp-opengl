#include "texture_atlas.hpp"
#include "cppmc/world_common.hpp"

TextureAtlas::TextureAtlas(GLuint texture, int num_rows, int num_cols) {
  m_Texture = texture;
  m_NumRows = num_rows;
  m_NumCols = num_cols;
}

std::vector<GLfloat> TextureAtlas::GetTexCoordsAt(const glm::ivec2 &coord) const {
  // Cast them for convenience
  float row = coord.y;
  float col = coord.x;
  return std::vector<GLfloat>
    { col / m_NumCols, row / m_NumRows
    , col / m_NumCols, (row + 1) / m_NumRows
    , (col + 1) / m_NumCols, (row + 1) / m_NumRows
    , (col + 1) / m_NumCols, row / m_NumRows
    };
}

GLuint TextureAtlas::GetTextureID() {
  return m_Texture;
}

BlockTextures::BlockTextures()
{
  Initialize();
}

void BlockTextures::Initialize() {
  m_ErrorAtlasCoord = {15, 15};
  Register(1, Direction::Down, {2, 0});
  Register(1, Direction::Up, {0, 0});
  Register(1, Direction::North, {1, 0});
  Register(1, Direction::East, {1, 0});
  Register(1, Direction::South, {1, 0});
  Register(1, Direction::West, {1, 0});
}

void BlockTextures::Register(int block_id, Direction dir, const glm::ivec2 &atlas_coord)
{
  m_Mapping[{block_id, dir}] = atlas_coord;
}

glm::ivec2 BlockTextures::GetAtlasCoord(int block_id, Direction dir) {
  auto key = std::make_pair(block_id, dir);
  auto iter = m_Mapping.find(key);
  if (iter == m_Mapping.end()) {
    return m_ErrorAtlasCoord;
  } else {
    return iter->second;
  }
}