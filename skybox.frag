#version 130
in vec2 t;

uniform sampler2D texture;

out vec4 color;

void main(void)
{
    color = texture2D(texture, t);
}
