#version 420

layout (points) in;
layout (triangle_strip, max_vertices = 8) out;

in VS_OUT {
  vec2 TopLeft;
  vec2 BottomRight;
  vec4 BackgroundColor;
} gs_in[];


out GS_OUT {
  vec4 Color;
} gs_out;

uniform vec4 FillColor;
uniform float Progress;

void main() {
  float width = gs_in[0].BottomRight.x - gs_in[0].TopLeft.x;
  float height = gs_in[0].TopLeft.y - gs_in[0].BottomRight.y;

  gl_Position = vec4(gs_in[0].TopLeft + vec2(0, 0), 0, 1);
  gs_out.Color = FillColor;
  EmitVertex();

  gl_Position = vec4(gs_in[0].TopLeft + vec2(0, -height), 0, 1);
  gs_out.Color = FillColor;
  EmitVertex();

  gl_Position = vec4(gs_in[0].TopLeft + vec2(width * Progress, 0), 0, 1);
  gs_out.Color = FillColor;
  EmitVertex();

  gl_Position = vec4(gs_in[0].TopLeft + vec2(width * Progress, -height), 0, 1);
  gs_out.Color = FillColor;
  EmitVertex();

  EndPrimitive();

  gl_Position = vec4(gs_in[0].TopLeft + vec2(width * Progress, 0), 0, 1);
  gs_out.Color = gs_in[0].BackgroundColor;
  EmitVertex();

  gl_Position = vec4(gs_in[0].TopLeft + vec2(width * Progress, -height), 0, 1);
  gs_out.Color = gs_in[0].BackgroundColor;
  EmitVertex();

  gl_Position = vec4(gs_in[0].TopLeft + vec2(width, 0), 0, 1);
  gs_out.Color = gs_in[0].BackgroundColor;
  EmitVertex();

  gl_Position = vec4(gs_in[0].TopLeft + vec2(width, -height), 0, 1);
  gs_out.Color = gs_in[0].BackgroundColor;
  EmitVertex();

  EndPrimitive();
}
