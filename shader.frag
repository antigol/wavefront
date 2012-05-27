#version 130
in vec3 n;
in vec3 p;

uniform vec4 ambiant;
uniform vec4 diffuse;
uniform vec4 specular;
uniform float specularpower;
uniform vec3 light;

void main(void)
{
    float dfactor = clamp(dot(light, normalize(n)), 0.0, 1.0);

    vec3 np = normalize(p);
    vec3 i = np - 2.0 * n * dot(np, n);
    float sfactor = pow(clamp(dot(light, i), 0.0, 1.0), specularpower);

    gl_FragColor = ambiant + dfactor * diffuse + sfactor * specular;
}
