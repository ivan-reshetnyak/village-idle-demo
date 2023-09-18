#version 420

layout (location = 0) in vec2 TopLeft;
layout (location = 1) in vec2 BottomRight;
layout (location = 2) in vec4 BackgroundColor;

out VS_OUT {
  vec2 TopLeft;
  vec2 BottomRight;
  vec4 BackgroundColor;
} vs_out;

void main() {
  vs_out.TopLeft = TopLeft;
  vs_out.BottomRight = BottomRight;
  vs_out.BackgroundColor = BackgroundColor;
}
