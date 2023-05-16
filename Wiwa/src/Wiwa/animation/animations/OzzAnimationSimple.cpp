#include <wipch.h>
#include "OzzAnimationSimple.h"

namespace Wiwa {
	OzzAnimationSimple::OzzAnimationSimple() :
		m_Loaded(false)
	{
		m_AnimType = AT_SIMPLE;
	}

	OzzAnimationSimple::~OzzAnimationSimple()
	{
	}

	bool OzzAnimationSimple::LoadAnimation(const char* file)
	{
		if (!skeleton_) return false;

		m_Loaded = ozz::sample::LoadAnimation(file, &animation_);
		
		if (m_Loaded) {
			m_AnimationPath = file;

			if (skeleton_->num_joints() != animation_.num_tracks()) {
				WI_ERROR("The provided animation doesn't match skeleton (joint count mismatch).");
				m_Loaded = false;
			}
		}

		Validate();

		return m_Loaded;
	}
	
	OzzAnimation::Status OzzAnimationSimple::Validate()
	{
		m_Status = INVALID;

		if (!skeleton_) return INVALID;
		if (!m_Loaded) return INVALID;

		m_Status = VALID;

		return VALID;
	}

	void OzzAnimationSimple::OnSkeletonSet()
	{
		// Initialize skeleton joint data
		const int num_joints = skeleton_->num_joints();
		const int num_soa_joints = skeleton_->num_soa_joints();

		context_.Resize(num_joints);

		// Allocates local space runtime buffers of blended data.
		locals_.resize(num_soa_joints);
	}

	bool OzzAnimationSimple::Update(float _dt)
	{
		// Updates current animation time.
		controller_.Update(animation_, _dt);

		// Samples optimized animation at t = animation_time_.
		ozz::animation::SamplingJob sampling_job;
		sampling_job.animation = &animation_;
		sampling_job.context = &context_;
		sampling_job.ratio = controller_.time_ratio();
		sampling_job.output = make_span(locals_);
		if (!sampling_job.Run()) {
			return false;
		}

		return true;
	}
}