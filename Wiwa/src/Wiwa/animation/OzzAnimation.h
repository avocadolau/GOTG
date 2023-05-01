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

namespace Wiwa {
    enum AnimationType {
        AT_PARTIAL_BLEND,
        AT_LAST
    };

	class WI_API OzzAnimation {
	protected:
        AnimationType m_AnimType;

        virtual bool Init() = 0;
        virtual bool Update(float _dt) = 0;
    public:
        OzzAnimation() : m_AnimType(AT_LAST){}

        AnimationType getAnimationType() { return m_AnimType; }

	};
}