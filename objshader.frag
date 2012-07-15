#version 130
in vec3 n; // in view coordinates
in vec3 p; // in view coordinates

uniform mat4 matrixv; // view (camera)

uniform vec4 ambiant;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float hardness;
uniform vec3 light; // is given in model coordinates

out vec4 color;

void main(void)
{
    vec3 nn = normalize(n);
    vec3 pp = normalize(p);
    vec3 l = mat3(matrixv) * light;

    float dfactor = clamp(dot(l, nn), 0.0, 1.0);

    vec3 i = reflect(pp, nn);
    float sfactor = pow(clamp(dot(l, i), 0.0, 1.0), hardness);

    color = ambiant + dfactor * diffuse + sfactor * specular;
}
