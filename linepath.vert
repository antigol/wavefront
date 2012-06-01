#version 130
in vec4 vertex;

uniform mat4 matrixp; // projection
uniform mat4 matrixv; // view (camera)
uniform mat4 matrixm; // model (position of object in scene)

out float c;

void main(void)
{
    gl_Position = matrixp * matrixv * matrixm * vec4(vertex.xyz, 1.0);
    c = vertex.w;
}
