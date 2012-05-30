#version 130
in vec3 vertex;
in vec2 texCoord;

uniform mat4 matrixp; // projection
uniform mat4 matrixv; // view (camera)

out vec2 t;

void main(void)
{
    gl_Position = matrixp * vec4(mat3(matrixv) * vertex, 1.0);
    t = texCoord;
}
