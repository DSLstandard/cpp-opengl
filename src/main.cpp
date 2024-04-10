#include <GL/glew.h>

#include "cppmc/camera.hpp"
#include "cppmc/shader.hpp"
#include "cppmc/texture_atlas.hpp"
#include "cppmc/cube_mesh.hpp"
#include "cppmc/mesh.hpp"
#include "cppmc/world_common.hpp"
#include <GLFW/glfw3.h>
#include <cstddef>
#include <cstdlib>
#include <fstream>
#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/trigonometric.hpp>
#include <iostream>
#include <memory>
#include <sstream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

static void TestGLError() {
  GLenum err = glGetError();
  if (err != GL_NO_ERROR) {
    fprintf(stderr, "OpenGL Error: 0x%04x\n", err);
  }
}

static Shader CreateProgram() {
  GLuint vs, fs, program;
  LoadFileShader(vs, "res/vs.glsl", GL_VERTEX_SHADER);
  LoadFileShader(fs, "res/fs.glsl", GL_FRAGMENT_SHADER);
  LoadProgram(program, vs, fs);
  return Shader(program);
}

static TextureAtlas LoadAtlas() {
  std::string path("res/images/DefaultPack2.png");
  int width, height, n_channels;
  unsigned char *image =
      stbi_load(path.c_str(), &width, &height, &n_channels, 4);
  if (image == nullptr) {
    std::cerr << "Cannot load image from path " << path << std::endl;
  }

  GLuint texture;
  glGenTextures(1, &texture);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, width, height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image);
  glBindTexture(GL_TEXTURE_2D, 0);

  stbi_image_free(image);
  return TextureAtlas(texture, 16, 16);
}

// static Mesh CreateMesh(const TextureAtlas &atlas) {
//   std::vector<GLfloat> positions = {
//     -0.5f, +0.5f, 0.0f,
//     -0.5f, -0.5f, 0.0f,
//     +0.5f, -0.5f, 0.0f,
//     +0.5f, +0.5f, 0.0f,
//   };

//   std::vector<GLfloat> texcoords = atlas.GetTexCoordsAt({0, 0});
//   std::vector<GLuint> indices = {0, 1, 3, 3, 1, 2};

//   Mesh mesh;
//   mesh.SetData(positions, texcoords, indices);
//   return mesh;
// }

template<typename T>
static void PrintVector(std::vector<T> vec) {
  bool first = true;

  std::cout << "[";
  for (T item : vec) {
    if (first) {
      first = false;
    } else {
      std::cout << " ";
    }
    std::cout << item;
  }
  std::cout << "]" << std::endl;
}

static Mesh CreateMesh(BlockTextures &block_textures, World &world, const TextureAtlas &atlas) {
  std::vector<GLfloat> positions;
  std::vector<GLfloat> texcoords;
  std::vector<GLuint> indices;
  GLuint index = 0;

  CubeMeshWriter writer(atlas, positions, texcoords, indices, index);
  ChunkMeshBuilder builder(writer, block_textures, world, {0, 0, 0});
  builder.Run();

  // CubeMeshWriter writer(atlas, positions, texcoords, indices, index);
  // for (int i = 0; i < 10; i++) {
  //   for (int j = 0; j < 10; j++) {
  //     for (int k = 0; k < 10; k++) {
  //       if (i % 2 == 0 || j % 2 == 0 || k % 2 == 0)
  //         continue;
  //       glm::vec3 pos(i, j, k);
  //       writer.MakeFace(Direction::South, pos, {1.0f, 0.0f});
  //       writer.MakeFace(Direction::North, pos, {1.0f, 0.0f});
  //       writer.MakeFace(Direction::East, pos, {1.0f, 0.0f});
  //       writer.MakeFace(Direction::West, pos, {1.0f, 0.0f});
  //       writer.MakeFace(Direction::Down, pos, {2.0f, 0.0f});
  //       writer.MakeFace(Direction::Up, pos, {0.0f, 0.0f});
  //     }
  //   }
  // }

  // PrintVector(positions);
  // PrintVector(texcoords);
  // PrintVector(indices);

  Mesh mesh;
  mesh.SetData(positions, texcoords, indices);
  return mesh;
}

int main() {
  glfwInit();

  GLFWwindow *win = glfwCreateWindow(800, 600, "cppmc", nullptr, nullptr);
  glfwMakeContextCurrent(win);
  if (glewInit() != GLEW_OK) {
    std::cerr << "Cannot initialize GLEW" << std::endl;
    return EXIT_FAILURE;
  }

  glfwSetInputMode(win, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

  World world;
  world.GenerateChunk({0, 0, 0});

  BlockTextures block_textures;
  TextureAtlas atlas = LoadAtlas();
  Mesh mesh = CreateMesh(block_textures, world, atlas);
  Shader shader = CreateProgram();

  Camera camera;
  camera.Position().z = 1;

  CameraControl camera_control(win, camera);

  glEnable(GL_DEPTH_TEST);

  glEnable(GL_CULL_FACE);
  glCullFace(GL_FRONT_FACE);
  glFrontFace(GL_CCW);

  while (true) {
    if (glfwWindowShouldClose(win))
      break;

    int win_width, win_height;
    glfwGetWindowSize(win, &win_width, &win_height);
    glViewport(0, 0, win_width, win_height);

    camera_control.Update();

    if (true) {
      std::cout << "--------" << std::endl;
      std::cout << "pos=" << glm::to_string(camera.Position()) << std::endl;
      std::cout << "direction=" << DirectionToString(camera.GetClosestLookDirection()) << std::endl;
      std::cout << "pitch=" << camera.GetPitch() << " yaw=" << camera.GetYaw() << std::endl;
    }

    glClearColor(0.2, 0.2, 0.2, 1.0);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    shader.Start();

    glActiveTexture(GL_TEXTURE0);
    glBindTexture(GL_TEXTURE_2D, atlas.GetTextureID());
    shader.Set("u_Texture", 0);

    glm::mat4 transform = camera.GetPerspective() * camera.GetWorldTransform();

    shader.Set("u_Transform", transform);
    mesh.Draw();
    shader.Stop();

    glfwSwapBuffers(win);
    glfwPollEvents();
  }

  glfwTerminate();
  return EXIT_SUCCESS;
}
