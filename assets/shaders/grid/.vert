#version 420

out VS_OUT {
  vec2 V;
} vs_out;

uniform vec2 Position;

void main() {
  vs_out.V = Position;
}
