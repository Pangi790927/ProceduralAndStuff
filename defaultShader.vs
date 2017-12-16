#version 130

uniform mat4 worldMatrix, viewMatrix, projectionMatrix;
 
out vec4 color;
out vec3 varNormal;

void main()
{
    color = gl_Color;
    color.a = 1.0f;
    varNormal = gl_Normal;
    gl_Position = projectionMatrix * viewMatrix * worldMatrix * gl_Vertex;
}