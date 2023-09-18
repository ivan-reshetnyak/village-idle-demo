#version 420

in GS_OUT {
  vec4 Color;
} fs_in;

out vec4 OutCol;

void main() {
  OutCol = fs_in.Color;
}
