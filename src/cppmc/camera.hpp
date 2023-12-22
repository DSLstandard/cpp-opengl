#pragma once

#include "cppmc/world_common.hpp"
#include <glm/glm.hpp>
#include <GLFW/glfw3.h>

class Camera {
private:
  glm::vec3 m_Position;
  float m_Yaw, m_Pitch;
  float m_Fov;
  int m_Width, m_Height;

public:
  Camera();

  void UpdateWindowSize(int width, int height);
  glm::vec3 &Position();

  float GetPitch();
  void SetPitch(float pitch);

  float GetYaw();
  void SetYaw(float yaw);

  Direction GetClosestLookDirection();

  glm::vec3 GetLookDir();
  glm::mat4 GetPerspective();
  glm::mat4 GetWorldTransform();
  glm::vec3 GetMoveForwardDir();
  glm::vec3 GetMoveLeftDir();
};

class CameraControl {
private:
  GLFWwindow *m_Window;
  Camera *m_Camera;
  bool m_HasLastMousePos;
  glm::vec2 m_LastMousePos;
  glm::vec2 m_DeltaMousePos;

  void UpdateDeltaMousePos();
public:
  CameraControl(GLFWwindow *window, Camera &camera);

  void HandleRotation();
  void HandleMovement();
  void Update();
};