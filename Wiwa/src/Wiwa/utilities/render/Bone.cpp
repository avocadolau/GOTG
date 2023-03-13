#include <wipch.h>

#include "Bone.h"
#include "glm/glm.hpp"
#include "glm/gtc/quaternion.hpp"
#include "glm/common.hpp"
#include <glm/gtx/quaternion.hpp>

#include <assimp/cimport.h>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

namespace Wiwa {
    Bone::Bone(const std::string& name, int ID, const aiNodeAnim* aiNode) :m_Name(name), m_ID(ID), m_LocalTransform(1.0f)
    {
        m_NumPositionKeys = aiNode->mNumPositionKeys;
        m_NumRotationKeys = aiNode->mNumRotationKeys;
        m_NumScalingKeys = aiNode->mNumScalingKeys;

        for (unsigned int i = 0; i < aiNode->mNumPositionKeys; i++)
        {
            VectorKey posKey;

            posKey.time = aiNode->mPositionKeys[i].mTime;
            posKey.value.x = aiNode->mPositionKeys[i].mValue.x;
            posKey.value.y = aiNode->mPositionKeys[i].mValue.y;
            posKey.value.z = aiNode->mPositionKeys[i].mValue.z;
            m_Positions.push_back(posKey);
        }
        for (unsigned int i = 0; i < aiNode->mNumRotationKeys; i++)
        {
            QuatKey quatKey;

            quatKey.time = aiNode->mRotationKeys[i].mTime;
            quatKey.value.x = aiNode->mRotationKeys[i].mValue.x;
            quatKey.value.y = aiNode->mRotationKeys[i].mValue.y;
            quatKey.value.z = aiNode->mRotationKeys[i].mValue.z;
            quatKey.value.w = aiNode->mRotationKeys[i].mValue.w;
            m_Rotations.push_back(quatKey);
        }
        for (unsigned int i = 0; i < aiNode->mNumScalingKeys; i++)
        {
            VectorKey scaleKey;

            scaleKey.time = aiNode->mScalingKeys[i].mTime;
            scaleKey.value.x = aiNode->mScalingKeys[i].mValue.x;
            scaleKey.value.y = aiNode->mScalingKeys[i].mValue.y;
            scaleKey.value.z = aiNode->mScalingKeys[i].mValue.z;
            m_Scales.push_back(scaleKey);
        }
    }

    void Bone::Update(float animationTime)
    {
        glm::mat4 translation = InterpolatePosition(animationTime);
        glm::mat4 rotation = InterpolateRotation(animationTime);
        glm::mat4 scale = InterpolateScaling(animationTime);
        m_LocalTransform = translation * rotation * scale;
    }

    int Bone::GetPositionIndex(float animationTime)
    {
        for (int index = 0; index < m_NumPositionKeys - 1; ++index)
        {
            if (animationTime < m_Positions[index + 1].time)
                return index;
        }
        assert(0);
    }

    int Bone::GetRotationIndex(float animationTime)
    {
        for (int index = 0; index < m_NumRotationKeys - 1; ++index)
        {
            if (animationTime < m_Rotations[index + 1].time)
                return index;
        }
        assert(0);
    }

    int Bone::GetScaleIndex(float animationTime)
    {
        for (int index = 0; index < m_NumScalingKeys - 1; ++index)
        {
            if (animationTime < m_Scales[index + 1].time)
                return index;
        }
        assert(0);
    }

    float Bone::GetScaleFactor(float lastTimeStamp, float nextTimeStamp, float animationTime)
    {
        float scaleFactor = 0.0f;
        float midWayLength = animationTime - lastTimeStamp;
        float framesDiff = nextTimeStamp - lastTimeStamp;
        scaleFactor = midWayLength / framesDiff;
        return scaleFactor;
    }

    glm::mat4 Bone::InterpolatePosition(float animationTime)
    {
        if (1 == m_NumPositionKeys)
            return glm::translate(glm::mat4(1.0f), m_Positions[0].value);

        int p0Index = GetPositionIndex(animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor = GetScaleFactor(m_Positions[p0Index].time,
            m_Positions[p1Index].time, animationTime);
        glm::vec3 finalPosition = glm::mix(m_Positions[p0Index].value, m_Positions[p1Index].value, scaleFactor);
        return glm::translate(glm::mat4(1.0f), finalPosition);
    }

    glm::mat4 Bone::InterpolateRotation(float animationTime)
    {
        if (1 == m_NumRotationKeys)
        {
            auto rotation = glm::normalize(m_Rotations[0].value);
            return glm::toMat4(rotation);
        }

        int p0Index = GetRotationIndex(animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor = GetScaleFactor(m_Rotations[p0Index].time,
            m_Rotations[p1Index].time, animationTime);
        glm::quat finalRotation = glm::slerp(m_Rotations[p0Index].value, m_Rotations[p1Index].value, scaleFactor);
        finalRotation = glm::normalize(finalRotation);
        return glm::toMat4(finalRotation);


    }

    glm::mat4 Bone::InterpolateScaling(float animationTime)
    {
        if (1 == m_NumScalingKeys)
            return glm::scale(glm::mat4(1.0f), m_Scales[0].value);

        int p0Index = GetScaleIndex(animationTime);
        int p1Index = p0Index + 1;
        float scaleFactor = GetScaleFactor(m_Scales[p0Index].time,
            m_Scales[p1Index].time, animationTime);
        glm::vec3 finalScale = glm::mix(m_Scales[p0Index].value, m_Scales[p1Index].value, scaleFactor);
        return glm::scale(glm::mat4(1.0f), finalScale);
    }
}