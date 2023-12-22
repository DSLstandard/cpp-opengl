#include "camera.hpp"

#include <glm/ext/matrix_transform.hpp>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtx/rotate_vector.hpp>
#include <glm/gtx/string_cast.hpp>
#include <glm/gtx/vector_angle.hpp>
#include <glm/trigonometric.hpp>

#include "world_common.hpp"

Camera::Camera() {
  m_Position = {0.0f, 0.0f, 0.0f};
  m_Yaw = 0.0f;
  m_Pitch = 0.0f;
  m_Fov = glm::radians(90.0f);
}

void Camera::UpdateWindowSize(int width, int height) {
  m_Width = width;
  m_Height = height;
}

glm::vec3 &Camera::Position() { return m_Position; }

float Camera::GetPitch() { return m_Pitch; }

void Camera::SetPitch(float pitch) {
  float bound = M_PI_2 - 0.0001f;
  if (pitch >= bound)
    this->m_Pitch = bound;
  else if (pitch <= -bound)
    this->m_Pitch = -bound;
  else
    this->m_Pitch = pitch;
}

float Camera::GetYaw() { return m_Yaw; }

void Camera::SetYaw(float yaw) { m_Yaw = std::fmod(yaw, M_PI * 2); }

Direction Camera::GetClosestLookDirection() {
  return GetClosestDirection(GetLookDir());
}

glm::vec3 Camera::GetLookDir() {
  glm::quat q(glm::vec3(m_Pitch, m_Yaw, 0.0f));
  return glm::rotate(q, WORLD_NORTH);
}

glm::mat4 Camera::GetPerspective() {
  float aspect_ratio = (float)m_Width / m_Height;
  return glm::perspective(m_Fov, aspect_ratio, 0.001f, 1000.0f);
}

glm::mat4 Camera::GetWorldTransform() {
  return glm::lookAt(m_Position, m_Position + GetLookDir(), WORLD_UP);
}

glm::vec3 Camera::GetMoveForwardDir() {
  return glm::rotate(WORLD_NORTH, m_Yaw, WORLD_UP);
}

glm::vec3 Camera::GetMoveLeftDir() {
  return glm::cross(WORLD_UP, GetMoveForwardDir());
}

void CameraControl::UpdateDeltaMousePos() {
  double x, y;
  glfwGetCursorPos(m_Window, &x, &y);
  glm::vec2 curr_mouse_pos((float)x, (float)y);

  if (!m_HasLastMousePos) {
    m_HasLastMousePos = true;
    m_DeltaMousePos = glm::vec2(0.0f, 0.0f);
  } else {
    m_DeltaMousePos = curr_mouse_pos - m_LastMousePos;
  }
  m_LastMousePos = curr_mouse_pos;
}

CameraControl::CameraControl(GLFWwindow *window, Camera &camera) {
  m_Window = window;
  m_Camera = &camera;
  m_HasLastMousePos = false;
}

void CameraControl::HandleRotation() {
  float sensitivity = 0.003f;

  m_Camera->SetYaw(m_Camera->GetYaw() - sensitivity * m_DeltaMousePos.x);
  m_Camera->SetPitch(m_Camera->GetPitch() - sensitivity * m_DeltaMousePos.y);
}

void CameraControl::HandleMovement() {
  float speed = 0.05f;

  // Movement
  glm::vec3 move_forward = m_Camera->GetMoveForwardDir();
  glm::vec3 move_left = m_Camera->GetMoveLeftDir();
  glm::vec3 movement(0.0f);
  if (glfwGetKey(m_Window, GLFW_KEY_W) == GLFW_PRESS)
    movement += move_forward;
  if (glfwGetKey(m_Window, GLFW_KEY_S) == GLFW_PRESS)
    movement -= move_forward;
  if (glfwGetKey(m_Window, GLFW_KEY_A) == GLFW_PRESS)
    movement += move_left;
  if (glfwGetKey(m_Window, GLFW_KEY_D) == GLFW_PRESS)
    movement -= move_left;
  if (glfwGetKey(m_Window, GLFW_KEY_SPACE) == GLFW_PRESS)
    movement += WORLD_UP;
  if (glfwGetKey(m_Window, GLFW_KEY_Z) == GLFW_PRESS)
    movement -= WORLD_UP;
  m_Camera->Position() += movement * speed;
}

void CameraControl::Update() {
  // Update camera's window size
  int win_width, win_height;
  glfwGetWindowSize(m_Window, &win_width, &win_height);
  m_Camera->UpdateWindowSize(win_width, win_height);

  UpdateDeltaMousePos();
  HandleRotation();
  HandleMovement();
}