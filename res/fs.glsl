#version 330 core

in vec2 v_TexCoord;
out vec4 out_Color;

uniform sampler2D u_Texture;

void main() {
  out_Color = texture(u_Texture, v_TexCoord);
}