#version 330 core

#define M_PI 3.141593

layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 1) in vec2 texCoordinates_in;

out vec2 texVert;

void main()
{
    gl_Position = vec4(vertCoordinates_in, 1.0);
    texVert = texCoordinates_in;
}
