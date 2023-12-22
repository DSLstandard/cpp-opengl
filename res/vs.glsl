#version 330 core

layout (location = 0) in vec3 in_Position;
layout (location = 1) in vec2 in_TexCoord;

out vec2 v_TexCoord;

uniform mat4 u_Transform;

void main() {
  gl_Position = u_Transform * vec4(in_Position, 1.0);
  v_TexCoord = in_TexCoord;
}