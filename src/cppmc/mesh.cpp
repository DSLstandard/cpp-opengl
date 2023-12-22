#include "mesh.hpp"

bool Mesh::HasData() {
  return m_VertexArray != 0;
}

Mesh::Mesh() {
  Initialize();
}

void Mesh::Initialize() {
  glGenVertexArrays(1, &m_VertexArray);
  glGenBuffers(1, &m_VertexArray);
  glGenBuffers(1, &m_BufferPositions);
  glGenBuffers(1, &m_BufferTexCoords);
  glGenBuffers(1, &m_BufferIndices);

  glBindVertexArray(m_VertexArray);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferIndices);

  glBindBuffer(GL_ARRAY_BUFFER, m_BufferPositions);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 3, (void *) 0);
  glEnableVertexArrayAttrib(m_VertexArray, 0);

  glBindBuffer(GL_ARRAY_BUFFER, m_BufferTexCoords);
  glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, sizeof(GLfloat) * 2, (void *) 0);
  glEnableVertexArrayAttrib(m_VertexArray, 1);

  glBindVertexArray(0);
}

void Mesh::DeleteData() {
  glDeleteVertexArrays(1, &m_VertexArray);
  glDeleteBuffers(1, &m_BufferPositions);
  glDeleteBuffers(1, &m_BufferTexCoords);
  glDeleteBuffers(1, &m_BufferIndices);
}

void Mesh::SetData(
  const std::vector<GLfloat> &positions,
  const std::vector<GLfloat> &texcoords,
  const std::vector<GLuint> &indices
) {
  glBindBuffer(GL_ARRAY_BUFFER, m_BufferPositions);
  glBufferData(GL_ARRAY_BUFFER, positions.size() * sizeof(GLfloat), positions.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ARRAY_BUFFER, m_BufferTexCoords);
  glBufferData(GL_ARRAY_BUFFER, texcoords.size() * sizeof(GLfloat), texcoords.data(), GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_BufferIndices);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, indices.size() * sizeof(GLuint), indices.data(), GL_STATIC_DRAW);
  m_NumIndices = indices.size();
}

void Mesh::Draw() {
  glBindVertexArray(m_VertexArray);
  glDrawElements(GL_TRIANGLES, m_NumIndices, GL_UNSIGNED_INT, (void *)0);
  glBindVertexArray(0);
}