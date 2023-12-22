#pragma once

#include <GL/glew.h>
#include <vector>

class Mesh {
private:
  GLuint m_VertexArray;
  GLuint m_BufferPositions;
  GLuint m_BufferTexCoords;
  GLuint m_BufferIndices;
  int m_NumIndices;

  bool HasData();
public:
  explicit Mesh();

  void Initialize();
  void DeleteData();

  void SetData(
    const std::vector<GLfloat> &positions,
    const std::vector<GLfloat> &texcoords,
    const std::vector<GLuint> &indices
  );

  void Draw();
};