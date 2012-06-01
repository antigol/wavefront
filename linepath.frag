#version 130
in float c;

uniform vec4 color1;
uniform vec4 color2;

out vec4 color;

void main(void)
{
    float k = clamp(c, 0.0, 1.0);
    color = (1.0-k) * color1 + k * color2;
}
