#pragma once

#include <glm/glm.hpp>
#include <unordered_map>
#include <memory>
#include <vector>
#include <glm/gtx/hash.hpp>

#define CHUNK_SIZE 16

struct Block {
  unsigned short m_ID;

  bool IsAir();
  void SetToAir();
};

glm::ivec3 ChunkToWorldBlockCoord(const glm::ivec3 &chunkcoord, const glm::ivec3 &chunkblockcoord);

class Chunk {
private:
  glm::ivec3 m_ChunkCoord;
public:
  Chunk(const glm::ivec3 &coord);
  Block m_Blocks[CHUNK_SIZE][CHUNK_SIZE][CHUNK_SIZE]; // [x][y][z]

  glm::ivec3 ToAbsoluteCoord(const glm::ivec3 &chunkblockcoord);
  Block &GetBlockRelative(const glm::ivec3 &chunkblockcoord);
  const glm::ivec3 &GetChunkCoord();
  glm::ivec3 GetStartingBlockCoord();
};

class World {
private:
  std::unordered_map<glm::ivec3, Chunk*> m_ChunkMap;

public:
  void GenerateChunk(const glm::ivec3 &chunkcoord);

  Chunk *GetChunkOrNull(const glm::ivec3 &chunkcoord);
  bool HasChunk(const glm::ivec3 &chunkcoord);

  std::vector<Chunk *> ListChunks();
};

class ChunkView {
private:
  World &m_World;
  glm::ivec3 m_CenterChunkCoord;

  Chunk *m_Chunks[3][3][3];
  
  Chunk *&OfChunkRelative(const glm::ivec3 &relchunkcoord);
public:
  ChunkView(World &world, const glm::ivec3 &chunkcoord);

  Block *GetBlockOrNullRelative(const glm::ivec3 &chunkblockcoord);
};