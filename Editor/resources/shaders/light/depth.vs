#version 330 core
layout (location = 0) in vec3 aPos;

uniform mat4 u_View;
uniform mat4 u_Proj;
uniform mat4 u_Model;

void main()
{
    gl_Position = u_Model * u_View * u_Proj * vec4(aPos, 1.0);
}  