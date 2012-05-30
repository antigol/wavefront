#version 130
in vec3 vertex;

uniform mat4 matrixp; // projection
uniform mat4 matrixv; // view (camera)
uniform mat4 matrixm; // model (position of object in scene)

out vec3 p;

void main(void)
{
    vec4 mp = matrixm * vec4(vertex, 1.0);

    gl_Position = matrixp * matrixv * mp;
    p = vec3(mp);
}
