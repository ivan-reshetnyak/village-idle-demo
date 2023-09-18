#version 420

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
  vec2 TopLeft;
  vec2 BottomRight;
} gs_in[];


out GS_OUT {
  vec2 Tex;
} gs_out;


void main() {
  float width = gs_in[0].BottomRight.x - gs_in[0].TopLeft.x;
  float height = gs_in[0].TopLeft.y - gs_in[0].BottomRight.y;

  gl_Position = vec4(gs_in[0].TopLeft + vec2(0, 0), 0, 1);
  gs_out.Tex = vec2(0, 0);
  EmitVertex();

  gl_Position = vec4(gs_in[0].TopLeft + vec2(0, -height), 0, 1);
  gs_out.Tex = vec2(0, 1);
  EmitVertex();

  gl_Position = vec4(gs_in[0].TopLeft + vec2(width, 0), 0, 1);
  gs_out.Tex = vec2(1, 0);
  EmitVertex();

  gl_Position = vec4(gs_in[0].TopLeft + vec2(width, -height), 0, 1);
  gs_out.Tex = vec2(1, 1);
  EmitVertex();

  EndPrimitive();
}
