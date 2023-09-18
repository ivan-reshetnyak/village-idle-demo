#version 420

in GS_OUT {
  vec2 Tex;
} fs_in;

out vec4 OutCol;

uniform sampler2D Texture;
uniform vec4 Color;

void main() {
  OutCol = Color * texture2D(Texture, fs_in.Tex);
}
