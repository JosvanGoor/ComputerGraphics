#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the input locations of attributes
layout (location = 0) in vec3 vertCoordinates_in;
layout (location = 1) in vec3 vertNormal_in;
layout (location = 2) in vec2 texCoordinates_in;

// Specify the Uniforms of the vertex shader

uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;
uniform mat3 normal;

// Specify the outputs of the vertex shader
// These will be the input for the fragment shader
out vec3 coorVert;
out vec3 normVert;
out vec2 texVert;
out vec4 eyeCord;

void main()
{
    // gl_Position is the output (a vec4) of the vertex shader
    // Currently without any transformation

    //send data for fragment shader
    gl_Position = projection * view * model * vec4(vertCoordinates_in, 1.0);
    vec3 newNormal = normal * vertNormal_in;

    texVert = texCoordinates_in;
    normVert = newNormal;
    coorVert = vec3(model * vec4(vertCoordinates_in, 1.0));
}
