#version 330
const int MAX_BONES = 200;
const int MAX_BONE_INFLUENCE = 4;
// Positions/Coordinates
layout (location = 0) in vec3 aPos;
// Normals
layout (location = 1) in vec3 aNormal;
// Texture Coordinates
layout (location = 2) in vec2 aTex;
//bones ids
layout (location = 3) in ivec4 aBoneIDs;
//bones weights
layout (location = 4) in vec4 aWeights;

out vec3 Normal;
out vec3 LocalPos;
out vec2 TexCoord;
flat out ivec4 BoneIDs;
out vec4 Weights;

uniform mat4 u_Model;
uniform mat4 u_Proj;
uniform mat4 u_View;
uniform mat4 u_Bones[MAX_BONES];

void main()
{
    mat4 BoneTransform = u_Bones[aBoneIDs[0]] * aWeights[0];
    BoneTransform     += u_Bones[aBoneIDs[1]] * aWeights[1];
    BoneTransform     += u_Bones[aBoneIDs[2]] * aWeights[2];
    BoneTransform     += u_Bones[aBoneIDs[3]] * aWeights[3];

   
    vec4 PosL = BoneTransform * vec4(aPos, 1.0);
	gl_Position = u_Proj * u_View * u_Model * vec4(aPos, 1.0);
	TexCoord = aTex;
	Normal = aNormal;
	LocalPos = aPos;
	BoneIDs = aBoneIDs;
    Weights = aWeights;
}
