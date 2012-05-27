#version 130
attribute vec3 vertex;
attribute vec3 normal;

uniform mat4 matrixp;
uniform mat4 matrixmv;

out vec3 n;
out vec3 p;

void main(void)
{
    n = vec3(matrixmv * vec4(normal, 0.0));
    vec4 mvp = matrixmv * vec4(vertex, 1.0);
    p = vec3(mvp);

    gl_Position = matrixp * mvp;
}
