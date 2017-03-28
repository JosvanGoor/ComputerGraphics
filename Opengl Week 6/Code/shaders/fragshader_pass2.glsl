# version 330 core

uniform sampler2D diffuse;
uniform sampler2D normals;
uniform sampler2D depth;

uniform mat4 inverseMatrix;
uniform vec3 lightPos;
uniform vec3 lightColor;
uniform vec3 viewPos;
uniform vec4 material;

in vec2 texVert;

out vec4 fColor;

void main ()
{
    vec3 N = texture(normals, texVert).rgb;
    vec3 Diffuse = texture(diffuse, texVert).rgb;

    float z = texture(depth, texVert).z;
    vec4 fragPos = vec4(texVert.x, texVert.y, z * 2 - 1, 1.0f);
    vec4 vertex = inverseMatrix * fragPos;

    vec3 V = normalize(viewPos - vertex.xyz);
    vec3 L = normalize(lightPos - vertex.xyz);
    vec3 R = normalize(2 * dot(N, L) * N - L);
    N = normalize(N);

    vec3 Color = vec3(0.0, 0.0, 0.0);

    //Ambient
    Color += Diffuse * material.x;

    // Diffuse
    Color += max(dot(N, L), 0.0) * Diffuse * lightColor * material.y;

    //Specular
    vec3 specularColor;
    Color += pow(max(0.0, dot(R, V)), material.w) * lightColor * material.z;

    fColor = vec4(Color, 1.0);
}
