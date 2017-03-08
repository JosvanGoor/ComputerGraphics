#version 330 core

// Define constants
#define M_PI 3.141593

// Specify the inputs to the fragment shader
// These must have the same type and name!

// in vec3 vertPos; Using the output from the vertex shader example
in vec3 coorVert;
in vec3 normVert;

// Specify the Uniforms of the vertex shaders
// uniform vec3 lightPosition; for example
uniform vec3 colorFrag;   //color of object
uniform vec4 materialFrag;  //material of object
uniform vec3 lightPosition;
uniform vec3 eyeFrag;

// Specify the output of the fragment shader
// Usually a vec4 describing a color (Red, Green, Blue, Alpha/Transparency)
out vec4 fColor;

void main()
{

    // Without normal matrix
    vec3 lightColor = vec3(1.0, 1.0, 1.0);
    vec3 L = normalize(lightPosition - coorVert);
    vec3 N = normalize(normVert);
    vec3 R = normalize(L - 2 * dot(N, L) * N);
    vec3 V = normalize(-eyeFrag);

    vec3 finalColor = vec3(0.0, 0.0, 0.0);

    //ambient
    finalColor = colorFrag * materialFrag.x;

    //diffuse
    finalColor +=  max(0.0, dot(L, N)) * colorFrag * lightColor * materialFrag.y;

    //specular
    if (dot(L, N) >= 0.0) finalColor += pow(max(0.0, dot(R, V)), materialFrag.w) * lightColor * materialFrag.z;


    fColor = vec4(finalColor, 1.0);
    //fColor = vec4(colorFrag, 1.0);
}
