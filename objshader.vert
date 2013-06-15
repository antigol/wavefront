#version 130
in vec3 vertex;
in vec3 normal;

uniform mat4 matrixp; // projection
uniform mat4 matrixv; // view (camera)
uniform mat4 matrixm; // model (position of object in scene)
uniform mat3 matrixvn; // normal view (camera)
uniform mat3 matrixmn; // normal model (position of object in scene)

out vec3 n;
out vec3 p;

void main(void)
{
    n = matrixvn * matrixmn * normal;

    vec4 vmp = matrixv * matrixm * vec4(vertex, 1.0);
    p = vec3(vmp);

    gl_Position = matrixp * vmp;
}
