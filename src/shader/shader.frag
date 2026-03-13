#version 460 core
out vec4 FragColor;

in vec2 TexCoord;

uniform sampler2D saunaTexture;

void main()
{
    FragColor = texture(saunaTexture, TexCoord);
}
