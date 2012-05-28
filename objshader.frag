#version 130
in vec3 n; // in model coordinates
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
    float dfactor = clamp(dot(light, normalize(n)), 0.0, 1.0);

    vec3 e = normalize(p);
    vec3 i = e - 2.0 * n * dot(e, n); // reflection
    float sfactor = pow(clamp(dot(mat3(matrixv) * light, i), 0.0, 1.0), hardness);

    color = ambiant + dfactor * diffuse + sfactor * specular;
}
