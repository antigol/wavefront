#version 130
in vec4 vertex;

uniform mat4 matrixp; // projection
uniform mat4 matrixv; // view (camera)
uniform mat4 matrixm; // model (position of object in scene)

//out vec3 p;
out float c;

void main(void)
{
//    vec4 mp = matrixm * vec4(vertex.xyz, 1.0);

    gl_Position = matrixp * matrixv * matrixm * vec4(vertex.xyz, 1.0);
//    p = vec3(mp);
    c = vertex.w;
}
