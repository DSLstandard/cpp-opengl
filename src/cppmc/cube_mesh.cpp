#include "cube_mesh.hpp"
#include "cppmc/texture_atlas.hpp"
#include "cppmc/world.hpp"
#include "cppmc/world_common.hpp"

#include <iostream>
#include <glm/ext/quaternion_trigonometric.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/quaternion.hpp>

static void PushVec3(std::vector<GLfloat> &vec, const glm::vec3 &v) {
  vec.push_back(v.x);
  vec.push_back(v.y);
  vec.push_back(v.z);
}

static void MakeQuadPositions(
  std::vector<GLfloat> &positions,
  const glm::vec3 &center,
  const glm::vec3 &down,
  const glm::vec3 &right
) {
  PushVec3(positions, center - down - right);
  PushVec3(positions, center + down - right);
  PushVec3(positions, center + down + right);
  PushVec3(positions, center - down + right);
}

CubeMeshWriter::CubeMeshWriter(
  const TextureAtlas &atlas,
  std::vector<GLfloat> &positions,
  std::vector<GLfloat> &texcoords,
  std::vector<GLuint> &indices,
  GLuint &index
)
  : m_Atlas(atlas)
  , m_Positions(positions)
  , m_TexCoords(texcoords)
  , m_Indices(indices)
  , m_Index(index)
{
}

static void MakeQuadIndices(std::vector<GLuint> &indices, GLuint &index) {
  const GLuint &i = index; // Alias
  indices.insert(indices.end(), {
    i, i + 1, i + 3,
    i + 3, i + 1, i + 2
  });
  index += 4;
}

void CubeMeshWriter::MakeFace(
  Direction dir,
  const glm::ivec3 &blockcoord,
  const glm::ivec2 &atlas_coord
)
{
  // Base face is North
  glm::vec3 startpos(blockcoord);
  glm::vec3 center = startpos + glm::vec3(0.5f, 0.5f, 0.5f);
  glm::quat q;
  switch (dir) {
    case Direction::North:
      q = glm::quat();
      break;
    case Direction::East:
      q = glm::angleAxis(-M_PI_2f, WORLD_UP);
      break;
    case Direction::South:
      q = glm::angleAxis(M_PIf, WORLD_UP);
      break;
    case Direction::West:
      q = glm::angleAxis(M_PI_2f, WORLD_UP);
      break;
    case Direction::Up:
      q = glm::angleAxis(M_PI_2f, WORLD_EAST);
      break;
    case Direction::Down:
      q = glm::angleAxis(-M_PI_2f, WORLD_EAST);
      break;
  }

  glm::vec3 start = center + DirectionToVec3(dir) / 2.0f;
  glm::vec3 down = glm::rotate(q, WORLD_DOWN / 2.0f);
  glm::vec3 right = glm::rotate(q, WORLD_WEST / 2.0f);

  MakeQuadPositions(m_Positions, start, down, right);

  auto uvs = m_Atlas.GetTexCoordsAt(atlas_coord);
  m_TexCoords.insert(m_TexCoords.end(), uvs.begin(), uvs.end());

  MakeQuadIndices(m_Indices, m_Index);
}

ChunkMeshBuilder::ChunkMeshBuilder(
  CubeMeshWriter &writer,
  BlockTextures &blocktextures,
  World &world,
  const glm::ivec3 &chunkcoord
)
  : m_Writer(writer)
  , m_BlockTextures(blocktextures)
  , m_World(world)
  , m_ChunkCoord(chunkcoord)
  , m_ChunkView(world, chunkcoord)
{
}

std::vector<Direction> ChunkMeshBuilder::FacesToDraw(const glm::ivec3 &chunkblockcoord) {
  std::vector<Direction> directions;
  for (auto dir : ALL_DIRECTIONS) {
    glm::ivec3 neighborcoord = chunkblockcoord; // Copy
    DirectionMoveIVec3(dir, neighborcoord);
    Block *neighborblock = m_ChunkView.GetBlockOrNullRelative(neighborcoord);

    bool draw;
    if (neighborblock == nullptr) {
      draw = true;
    } else if (neighborblock->IsAir()) {
      draw = true;
    } else {
      draw = false;
    }

    if (draw)
      directions.push_back(dir);
  }
  return directions;
}

void ChunkMeshBuilder::Run() {
  assert(m_World.GetChunkOrNull(m_ChunkCoord) != nullptr);

  ChunkView view(m_World, m_ChunkCoord);
  for (int i = 0; i < CHUNK_SIZE; i++) {
    for (int j = 0; j < CHUNK_SIZE; j++) {
      for (int k = 0; k < CHUNK_SIZE; k++) {
        const glm::ivec3 chunkblockcoord(i, j, k);
        const glm::ivec3 worldblockcoord = ChunkToWorldBlockCoord(m_ChunkCoord, chunkblockcoord);

        Block *block = view.GetBlockOrNullRelative(chunkblockcoord);
        assert(block != nullptr);
        if (block->IsAir())
          continue;

        for (auto dir : FacesToDraw(chunkblockcoord)) {
          // TODO: UV
          auto atlas_coord = m_BlockTextures.GetAtlasCoord(block->m_ID, dir);
          m_Writer.MakeFace(dir, worldblockcoord, atlas_coord);
        }
      }
    }
  }
}