#version 420

layout (location = 0) in vec2 TopLeft;
layout (location = 1) in vec2 BottomRight;
layout (location = 2) in vec4 Color;

out VS_OUT {
  vec2 TopLeft;
  vec2 BottomRight;
} vs_out;

uniform vec2 Offset;
uniform float Scale;

void main() {
  vs_out.TopLeft = TopLeft * Scale + Offset;
  vs_out.BottomRight = BottomRight * Scale + Offset;
}
