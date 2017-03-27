# version 330 core

uniform sampler2D diffuse;
uniform sampler2D normals;
uniform sampler2D depth;

in vec2 texVert;

out vec4 fColor;

void main ()
{
    fColor = vec4(texture2D(diffuse, texVert).rgb , 1);
    //fColor = vec4(1.0, 0.0, 0.0, 1.0);
}