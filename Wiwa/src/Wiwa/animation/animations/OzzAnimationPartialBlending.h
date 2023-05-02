#pragma once

#include "../OzzAnimation.h"

namespace Wiwa {
	class WI_API OzzAnimationPartialBlending : public OzzAnimation {
	private:
		// Sampler struct that helps to control an animation
		struct Sampler {
			// Constructor, default initialization.
			Sampler() : weight_setting(1.f), joint_weight_setting(1.f) {}

			// Playback animation controller. This is a utility class that helps with
			// controlling animation playback time.
			ozz::sample::PlaybackController controller;

			// Blending weight_setting for the layer.
			float weight_setting;

			// Blending weight_setting setting of the joints of this layer that are
			// affected
			// by the masking.
			float joint_weight_setting;

			// Runtime animation.
			ozz::animation::Animation animation;

			// Sampling context.
			ozz::animation::SamplingJob::Context context;

			// Buffer of local transforms as sampled from animation_.
			ozz::vector<ozz::math::SoaTransform> locals;

			// Per-joint weights used to define the partial animation mask. Allows to
			// select which joints are considered during blending, and their individual
			// weight_setting.
			ozz::vector<ozz::math::SimdFloat4> joint_weights;
		};
	private:
		// Index of the joint at the base of the upper body hierarchy.
		int upper_body_root_;

		// Blending job rest pose threshold.
		float threshold_;

		enum {
			kLowerBody = 0,
			kUpperBody = 1,
			kNumLayers = 2,
		};

		// Samplers list
		Sampler samplers_[kNumLayers];

		// Sampling context.
		ozz::animation::SamplingJob::Context context_;

		// Buffer of local transforms as sampled from animation_.
		ozz::vector<ozz::math::SoaTransform> locals_;

		// The mesh used by the sample.
		ozz::vector<ozz::sample::Mesh> meshes_;

		// Buffer of skinning matrices, result of the joint multiplication of the
		// inverse bind pose with the model space matrix.
		ozz::vector<ozz::math::Float4x4> skinning_matrices_;

		// Runtime skeleton.
		ozz::animation::Skeleton skeleton_;

		// Buffer of local transforms which stores the blending result.
		ozz::vector<ozz::math::SoaTransform> blended_locals_;

		// Buffer of model space matrices. These are computed by the local-to-model
		// job after the blending stage.
		ozz::vector<ozz::math::Float4x4> models_;

		void SetupPerJointWeights();

		// Helper functor used to set weights while traversing joints hierarchy.
		struct WeightSetupIterator {
			WeightSetupIterator(ozz::vector<ozz::math::SimdFloat4>* _weights,
				float _weight_setting)
				: weights(_weights), weight_setting(_weight_setting) {}
			void operator()(int _joint, int) {
				ozz::math::SimdFloat4& soa_weight = weights->at(_joint / 4);
				soa_weight = ozz::math::SetI(
					soa_weight, ozz::math::simd_float4::Load1(weight_setting),
					_joint % 4);
			}
			ozz::vector<ozz::math::SimdFloat4>* weights;
			float weight_setting;
		};

		bool m_Loaded;
	public:
		OzzAnimationPartialBlending();
		~OzzAnimationPartialBlending();

		bool LoadInfo(const char* mesh, const char* skeleton, const char* lower, const char* upper);
		void SetUpperBodyRoot(int ubr);

		bool Init() override;
		bool Update(float _dt) override;
	};
}