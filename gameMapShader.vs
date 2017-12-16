#version 130

uniform mat4 worldMatrix, viewMatrix, projectionMatrix;
 
out vec4 color;
/// flat 
out vec3 normal;

void main()
{
    color = gl_Color;
    color.a = 1.0f;
    normal = gl_Normal;
    gl_Position = projectionMatrix * viewMatrix * worldMatrix * gl_Vertex;
}