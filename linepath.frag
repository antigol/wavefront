#version 130
//in vec3 p; // in model coordinates
in float c;

uniform vec4 color1;
uniform vec4 color2;
uniform float step;

out vec4 color;

void main(void)
{
//    vec3 v = mod(p, 2.0 * step);

//    bool x = v.x < step;
//    bool y = v.y < step;
//    bool z = v.z < step;

//    if (x ^^ y ^^ z)
//        color = color1;
//    else
//        color = color2;
    float k = clamp(c, 0.0, 1.0);
    color = (1.0-k) * color1 + k * color2;
}
