#include "shader.hpp"

#include <glm/gtc/type_ptr.hpp>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

Shader::Shader(GLuint program) { m_Program = program; }

GLuint Shader::Location(const char *name) {
  return glGetUniformLocation(m_Program, name);
}

void Shader::Set(const char *name, GLint x) {
  glUniform1i(Location(name), x);
}

void Shader::Set(const char *name, GLfloat x) {
  glUniform1f(Location(name), x);
}

void Shader::Set(const char *name, const glm::mat4 &x, bool transpose) {
  glUniformMatrix4fv(Location(name), 1, transpose, glm::value_ptr(x));
}

void Shader::Start() {
  glUseProgram(m_Program);
}

void Shader::Stop() {
  glUseProgram(0);
}

bool LoadFileShader(GLuint &shader, const std::string &path, GLenum type) {
  std::ifstream file(path);
  if (file.fail()) {
    std::cerr << "[!] Cannot open file " << path << std::endl;
    return false;
  }

  std::ostringstream ss;
  ss << file.rdbuf();

  std::string code = ss.str(); // Controls the lifetime of code_handle
  const char *code_handle = code.c_str();

  shader = glCreateShader(type);
  glShaderSource(shader, 1, &code_handle, nullptr);
  glCompileShader(shader);

  GLint success;
  glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
  if (!success) {
    GLint log_len;
    glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &log_len);
    char *log = new char[log_len + 1]; // + null terminator
    glGetShaderInfoLog(shader, log_len, nullptr, log);
    std::cerr << "--- SHADER COMPILATION ERROR (" << path << ") ---"
              << std::endl;
    std::cerr << log << std::endl;
    delete[] log; // free up log from memory
    return false;
  }

  return true;
}

bool LoadProgram(GLuint &program, GLuint vshader, GLuint fshader) {
  program = glCreateProgram();
  glAttachShader(program, vshader);
  glAttachShader(program, fshader);
  glLinkProgram(program);

  GLint success;
  glGetProgramiv(program, GL_LINK_STATUS, &success);
  if (!success) {
    GLint log_len;
    glGetProgramiv(program, GL_INFO_LOG_LENGTH, &log_len);
    char *log = new char[log_len + 1]; // + null terminator
    glGetProgramInfoLog(program, log_len, nullptr, log);
    std::cerr << "--- PROGRAM LINKING ERROR ---" << std::endl;
    std::cerr << log << std::endl;
    delete[] log; // free up log from memory
    return false;
  }

  glDetachShader(program, vshader);
  glDetachShader(program, fshader);
  glDeleteShader(vshader);
  glDeleteShader(fshader);

  return true;
}