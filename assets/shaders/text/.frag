#version 420

in VS_OUT {
  vec2 Tex;
} fs_in;

out vec4 OutCol;

uniform sampler2D Texture;
uniform vec4 TextColor;
uniform vec4 BackgroundColor;

void main() {
  vec4 LetterColor = texture2D(Texture, fs_in.Tex) * TextColor;
  OutCol = mix(BackgroundColor, LetterColor, LetterColor.a);
}
