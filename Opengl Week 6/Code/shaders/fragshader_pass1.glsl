#version 330 core

#define M_PI 3.141593

in vec4 normVert;
in vec2 texVert;
in vec4 fragPos;

uniform sampler2D colorData;

layout (location = 0) out vec3 fColor;
layout (location = 1) out vec3 fNorm;
//layout (location = 2) out vec3 fPosition;

void main()
{
    fColor = texture(colorData, texVert).xyz;
    fNorm = ((normVert + 1.0) / 2.0).xyz;
    //fPosition = fragPos.xyz;
}
