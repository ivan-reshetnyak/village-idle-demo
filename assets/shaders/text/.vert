#version 420

layout (location = 0) in vec2 Pos;
layout (location = 1) in vec2 Tex;

out VS_OUT {
  vec2 Tex;
} vs_out;

void main() {
  gl_Position = vec4(Pos, 0, 1);
//  gl_Position = vec4(0, 0, 0, 1);
  vs_out.Tex = Tex;
}
