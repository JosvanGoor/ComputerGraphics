#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 1) in vec3 vertColor_in;
layout (location = 2) in vec3 vertNormal_in;

// Specify the Uniforms of the vertex shader
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

// Specify the outputs of the vertex shader
// These will be the input for the fragment shader

//color to be passed to the fragment shader
out vec3 color;

void main()
{
    gl_Position = projection * view * model * vec4(vertCoordinates_in, 1.0);
    color = vertColor_in;
}
