#version 420

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in VS_OUT {
  vec2 V;
} gs_in[];


out GS_OUT {
  vec2 Tex;
} gs_out;

uniform vec2 Sizes;

void main() {
  gl_Position = vec4(gs_in[0].V.x - Sizes.x, gs_in[0].V.y - Sizes.y, 0, 1);
  gs_out.Tex = vec2(0, 1);
  EmitVertex();

  gl_Position = vec4(gs_in[0].V.x + Sizes.x, gs_in[0].V.y - Sizes.y, 0, 1);
  gs_out.Tex = vec2(1, 1);
  EmitVertex();

  gl_Position = vec4(gs_in[0].V.x - Sizes.x, gs_in[0].V.y + Sizes.y, 0, 1);
  gs_out.Tex = vec2(0, 0);
  EmitVertex();

  gl_Position = vec4(gs_in[0].V.x + Sizes.x, gs_in[0].V.y + Sizes.y, 0, 1);
  gs_out.Tex = vec2(1, 0);
  EmitVertex();

  EndPrimitive();
}
