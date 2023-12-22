#include "world.hpp"
#include "cppmc/util.hpp"

#include <numeric>

glm::ivec3 ChunkToWorldBlockCoord(const glm::ivec3 &chunkcoord, const glm::ivec3 &chunkblockcoord) {
  return chunkcoord * 16 + chunkblockcoord;
}

bool Block::IsAir() {
  return m_ID == 0;
}

void Block::SetToAir() {
  m_ID = 0;
}

Chunk::Chunk(const glm::ivec3 &coord)
  : m_ChunkCoord(coord)
{
  for (int i = 0; i < CHUNK_SIZE; i++) {
    for (int j = 0; j < CHUNK_SIZE; j++) {
      for (int k = 0; k < CHUNK_SIZE; k++) {
        m_Blocks[i][j][k].SetToAir();
      }
    }
  }
}

Block &Chunk::GetBlockRelative(const glm::ivec3 &relcoord) {
  return m_Blocks[relcoord.x][relcoord.y][relcoord.z];
}

const glm::ivec3 &Chunk::GetChunkCoord() {
  return m_ChunkCoord;
}

glm::ivec3 Chunk::GetStartingBlockCoord() {
  return m_ChunkCoord * glm::ivec3(CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE);
}

void World::GenerateChunk(const glm::ivec3 &chunkcoord) {
  Chunk *chunk = new Chunk(chunkcoord);
  m_ChunkMap[chunkcoord] = chunk;

  for (int i = 0; i < CHUNK_SIZE; i++) {
    for (int j = 0; j < CHUNK_SIZE; j++) {
      for (int k = 0; k < CHUNK_SIZE; k++) {
        if (((i + j + k) / 2) % 2 == 0) {
          chunk->m_Blocks[i][j][k].m_ID = 1;
        }
      }
    }
  }
}

Chunk *World::GetChunkOrNull(const glm::ivec3 &chunkcoord) {
  auto iter = m_ChunkMap.find(chunkcoord);
  if (iter == m_ChunkMap.end()) {
    return nullptr;
  } else {
    return iter->second;
  }
}

bool World::HasChunk(const glm::ivec3 &chunkcoord) {
  return m_ChunkMap.count(chunkcoord) > 0;
}

std::vector<Chunk *> World::ListChunks() {
  std::vector<Chunk *> chunks;
  for (auto iter = m_ChunkMap.begin(); iter != m_ChunkMap.end(); iter++) {
    chunks.push_back(iter->second);
  }
  return chunks;
}

Chunk *& ChunkView::OfChunkRelative(const glm::ivec3 &relchunkcoord) {
  return m_Chunks[relchunkcoord.x + 1][relchunkcoord.y + 1][relchunkcoord.z + 1];
}

ChunkView::ChunkView(World &world, const glm::ivec3 &chunkcoord)
  : m_World(world)
  , m_CenterChunkCoord(chunkcoord)
{
  for (int dx = -1; dx <= 1; dx++) {
    for (int dy = -1; dy <= 1; dy++) {
      for (int dz = -1; dz <= 1; dz++) {
        glm::ivec3 relchunkcoord(dx, dy, dz);
        OfChunkRelative(relchunkcoord) = m_World.GetChunkOrNull(chunkcoord + relchunkcoord);
      }
    }
  }
}

Block *ChunkView::GetBlockOrNullRelative(const glm::ivec3 &in) {
  glm::ivec3 relchunkcoord, relblockcoord;
  Util::Partition(in.x, CHUNK_SIZE, relchunkcoord.x, relblockcoord.x);
  Util::Partition(in.y, CHUNK_SIZE, relchunkcoord.y, relblockcoord.y);
  Util::Partition(in.z, CHUNK_SIZE, relchunkcoord.z, relblockcoord.z);
  Chunk *chunk = OfChunkRelative(relchunkcoord);
  if (chunk == nullptr)
    return nullptr;
  else
    return &chunk->GetBlockRelative(relblockcoord);
}