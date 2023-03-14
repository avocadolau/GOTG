#pragma once

#include <Wiwa/core/Core.h>
#include <vector>
#include <string>
#include <Wiwa/utilities/math/Math.h>
#include <glm/glm.hpp>
#include <Wiwa/utilities/filesystem/FileSystem.h>
struct aiNodeAnim;

namespace Wiwa {
    struct VectorKey
    {
        //time of this key when the transformation should happen
        double time;
        glm::vec3 value;
        VectorKey()
        {
            time = 0; value = { 0,0,0 };
        }
        VectorKey(double time, const glm::vec3 value)
        {
            this->time = time;
            this->value = value;
        }
    };
    struct QuatKey {
        //time of this key when the transformation should happen
        double time;
        glm::quat value;
        QuatKey()
        {
            time = 0; value = { 0,0,0,0 };
        }
        QuatKey(double time, const glm::quat& value)
        {
            this->time = time;
            this->value = value;
        }
    };
    //change to AnimNode
    class WI_API Bone
    {
    public:

        std::vector<VectorKey> m_Positions;
        std::vector<QuatKey> m_Rotations;
        std::vector<VectorKey> m_Scales;
        int m_NumPositionKeys;
        int m_NumRotationKeys;
        int m_NumScalingKeys;

        glm::mat4 m_LocalTransform;
        std::string m_Name;
        int m_ID;

    public:

        /*reads keyframes from aiNodeAnim*/
        Bone(const std::string& name, int ID, const aiNodeAnim* channel);
        Bone() {};


        /*interpolates  b/w positions,rotations & scaling keys based on the curren time of
        the animation and prepares the local transformation matrix by combining all keys
        tranformations*/
        void Update(float animationTime);

        glm::mat4 GetLocalTransform() { return m_LocalTransform; }
        std::string GetBoneName() const { return m_Name; }
        int GetBoneID() { return m_ID; }


        /* Gets the current index on mKeyPositions to interpolate to based on
        the current animation time*/
        int GetPositionIndex(float animationTime);

        /* Gets the current index on mKeyRotations to interpolate to based on the
        current animation time*/
        int GetRotationIndex(float animationTime);

        /* Gets the current index on mKeyScalings to interpolate to based on the
        current animation time */
        int GetScaleIndex(float animationTime);

    public:
        static void SaveWiAnimNode(File& file, Bone* node);
        void LoadAnimationNode(const aiNodeAnim* aiAnimNode);
        static Bone* LoadWiAnimNode(File& file);
        /* Gets normalized value for Lerp & Slerp*/
        float GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime);

        /*figures out which position keys to interpolate b/w and performs the interpolation
        and returns the translation matrix*/
        glm::mat4 InterpolatePosition(float animationTime);

        /*figures out which rotations keys to interpolate b/w and performs the interpolation
        and returns the rotation matrix*/
        glm::mat4 InterpolateRotation(float animationTime);

        /*figures out which scaling keys to interpolate b/w and performs the interpolation
        and returns the scale matrix*/
        glm::mat4 InterpolateScaling(float animationTime);

    };
}

//class AssimpGLMHelpers
//{
//public:
//
//    static inline glm::mat4 ConvertMatrixToGLMFormat(const aiMatrix4x4& from)
//    {
//        glm::mat4 to;
//        //the a,b,c,d in assimp is the row ; the 1,2,3,4 is the column
//        to[0][0] = from.a1; to[1][0] = from.a2; to[2][0] = from.a3; to[3][0] = from.a4;
//        to[0][1] = from.b1; to[1][1] = from.b2; to[2][1] = from.b3; to[3][1] = from.b4;
//        to[0][2] = from.c1; to[1][2] = from.c2; to[2][2] = from.c3; to[3][2] = from.c4;
//        to[0][3] = from.d1; to[1][3] = from.d2; to[2][3] = from.d3; to[3][3] = from.d4;
//        return to;
//    }
//
//    static inline glm::vec3 GetGLMVec(const aiVector3D& vec)
//    {
//        return glm::vec3(vec.x, vec.y, vec.z);
//    }
//
//    static inline glm::quat GetGLMQuat(const aiQuaternion& pOrientation)
//    {
//        return glm::quat(pOrientation.w, pOrientation.x, pOrientation.y, pOrientation.z);
//    }
//};