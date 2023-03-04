#version 330
in vec3 LocalPos;
in vec3 Normal;
in vec2 TexCoord;
flat in ivec4 BoneIDs;
in vec4 Weights;

out vec4 FragColor;

uniform int u_DisplayBoneIndex = 0;


void main()
{
        FragColor =  vec4(0.0,0.0,1.0,1.0);
}