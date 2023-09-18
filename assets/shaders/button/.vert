#version 420

layout (location = 0) in vec2 V;

out VS_OUT {
  vec2 V;
} vs_out;

void main() {
  vs_out.V = V;
}
