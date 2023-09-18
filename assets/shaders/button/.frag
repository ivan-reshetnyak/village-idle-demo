#version 420

in GS_OUT {
  vec2 Tex;
} fs_in;

out vec4 OutCol;

uniform sampler2D ButtonTexture;

void main() {
  OutCol = texture2D(ButtonTexture, fs_in.Tex);
}
