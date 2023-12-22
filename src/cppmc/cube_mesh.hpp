#pragma once

#include "cppmc/texture_atlas.hpp"
#include "cppmc/world_common.hpp"
#include "cppmc/world.hpp"
#include <vector>
#include <GL/glew.h>
#include <glm/glm.hpp>

class CubeMeshWriter {
private:
  const TextureAtlas &m_Atlas;
  std::vector<GLfloat> &m_Positions;
  std::vector<GLfloat> &m_TexCoords;
  std::vector<GLuint> &m_Indices;
  GLuint &m_Index;

public:
  CubeMeshWriter(
    const TextureAtlas &atlas,
    std::vector<GLfloat> &positions,
    std::vector<GLfloat> &texcoords,
    std::vector<GLuint> &indices,
    GLuint &index
  );

  void MakeFace(
    Direction dir,
    const glm::ivec3 &pos,
    const glm::ivec2 &atlas_coord
  );
};

class ChunkMeshBuilder {
private:
  CubeMeshWriter &m_Writer;
  BlockTextures m_BlockTextures;
  World &m_World;
  glm::ivec3 m_ChunkCoord;

  ChunkView m_ChunkView;

  std::vector<Direction> FacesToDraw(const glm::ivec3 &chunkblockcoord);
public:
  ChunkMeshBuilder(CubeMeshWriter &writer, BlockTextures &blocktextures, World &world, const glm::ivec3 &chunkcoord);

  void Run();
};