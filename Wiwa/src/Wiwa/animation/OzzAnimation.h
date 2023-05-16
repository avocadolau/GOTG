#pragma once

// Include core
#include <Wiwa/core/Core.h>

// Include ozz
#include "ozz/animation/runtime/animation.h"
#include "ozz/animation/runtime/blending_job.h"
#include "ozz/animation/runtime/local_to_model_job.h"
#include "ozz/animation/runtime/sampling_job.h"
#include "ozz/animation/runtime/skeleton.h"
#include "ozz/animation/runtime/skeleton_utils.h"
#include "ozz/base/containers/vector.h"
#include "ozz/base/log.h"
#include "ozz/base/maths/simd_math.h"
#include "ozz/base/maths/soa_transform.h"
#include "ozz/base/maths/vec_float.h"
#include "ozz/options/options.h"
#include "samples/utils.h"
#include "samples/mesh.h"

namespace Wiwa {
    enum AnimationType {
        AT_PARTIAL_BLEND,
        AT_SIMPLE,
        AT_LAST
    };

	class WI_API OzzAnimation {
    public:
        enum Status {
            VALID,
            INVALID,
            LAST
        };
	protected:
        AnimationType m_AnimType;

        Status m_Status;

        // Runtime skeleton.
        ozz::animation::Skeleton* skeleton_;

        // Sampling context.
        ozz::animation::SamplingJob::Context context_;

        float m_PlaybackSpeed;
    public:
        OzzAnimation() : m_AnimType(AT_LAST), skeleton_(nullptr), m_Status(INVALID), m_PlaybackSpeed(1.0f) {}

        AnimationType getAnimationType() { return m_AnimType; }

        virtual ozz::vector<ozz::math::SoaTransform>& getLocals() = 0;

        virtual Status Validate() = 0;

        virtual bool Update(float _dt) = 0;

        virtual void OnSkeletonSet() = 0;

        virtual void setTimeRatio(float time_ratio) = 0;
        virtual float getTimeRatio() = 0;

        virtual void OnPlaybackSpeedChange() = 0;

        void setPlaybackSpeed(float speed) { m_PlaybackSpeed = speed; OnPlaybackSpeedChange(); }
        float getPlaybackSpeed() { return m_PlaybackSpeed; }

        void SetSkeleton(ozz::animation::Skeleton* skeleton);

        Status getStatus() { return m_Status; }
	};
}