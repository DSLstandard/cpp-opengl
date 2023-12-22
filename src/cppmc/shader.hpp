#pragma once

#include <GL/glew.h>
#include <string>
#include <glm/glm.hpp>

class Shader {
private:
  GLuint m_Program;

public:
  explicit Shader(GLuint program);

  GLuint Location(const char *name);

  void Set(const char *name, GLint x);
  void Set(const char *name, GLfloat x);
  void Set(const char *name, const glm::mat4 &x, bool transpose = false);

  void Start();
  void Stop();

  void Delete();
};

bool LoadFileShader(GLuint &shader, const std::string &path, GLenum type);
bool LoadProgram(GLuint &program, GLuint vshader, GLuint fshader);