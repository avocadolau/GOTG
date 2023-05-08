#include <wipch.h>
#include "OzzAnimationPartialBlending.h"
#include <Wiwa/core/Renderer3D.h>
#include <Wiwa/core/Application.h>

namespace Wiwa {
	void OzzAnimationPartialBlending::SetupPerJointWeights()
	{
        // Setup partial animation mask. This mask is defined by a weight_setting
        // assigned to each joint of the hierarchy. Joint to disable are set to a
        // weight_setting of 0.f, and enabled joints are set to 1.f.
        // Per-joint weights of lower and upper body layers have opposed values
        // (weight_setting and 1 - weight_setting) in order for a layer to select
        // joints that are rejected by the other layer.
        Sampler& lower_body_sampler = samplers_[kLowerBody];
        Sampler& upper_body_sampler = samplers_[kUpperBody];

        // Disables all joints: set all weights to 0.
        for (int i = 0; i < skeleton_.num_soa_joints(); ++i) {
            lower_body_sampler.joint_weights[i] = ozz::math::simd_float4::one();
            upper_body_sampler.joint_weights[i] = ozz::math::simd_float4::zero();
        }

        // Sets the weight_setting of all the joints children of the lower and upper
        // body weights. Note that they are stored in SoA format.
        WeightSetupIterator lower_it(&lower_body_sampler.joint_weights,
            lower_body_sampler.joint_weight_setting);
        ozz::animation::IterateJointsDF(skeleton_, lower_it, upper_body_root_);

        WeightSetupIterator upper_it(&upper_body_sampler.joint_weights,
            upper_body_sampler.joint_weight_setting);
        ozz::animation::IterateJointsDF(skeleton_, upper_it, upper_body_root_);
	}

    bool OzzAnimationPartialBlending::UpdateAnimation(float _dt)
    {
        // Updates and samples both animations to their respective local space
        // transform buffers.
        for (int i = 0; i < kNumLayers; ++i) {
            Sampler& sampler = samplers_[i];

            // Updates animations time.
            sampler.controller.Update(sampler.animation, _dt);

            // Setup sampling job.
            ozz::animation::SamplingJob sampling_job;
            sampling_job.animation = &sampler.animation;
            sampling_job.context = &sampler.context;
            sampling_job.ratio = sampler.controller.time_ratio();
            sampling_job.output = make_span(sampler.locals);

            // Samples animation.
            if (!sampling_job.Run()) {
                return false;
            }
        }

        // Blends animations.
        // Blends the local spaces transforms computed by sampling all animations
        // (1st stage just above), and outputs the result to the local space
        // transform buffer blended_locals_

        // Prepares blending layers.
        ozz::animation::BlendingJob::Layer layers[kNumLayers];
        for (int i = 0; i < kNumLayers; ++i) {
            layers[i].transform = make_span(samplers_[i].locals);
            layers[i].weight = samplers_[i].weight_setting;

            // Set per-joint weights for the partially blended layer.
            layers[i].joint_weights = make_span(samplers_[i].joint_weights);
        }

        // Setups blending job.
        ozz::animation::BlendingJob blend_job;
        blend_job.threshold = threshold_;
        blend_job.layers = layers;
        blend_job.rest_pose = skeleton_.joint_rest_poses();
        blend_job.output = make_span(blended_locals_);

        // Blends.
        if (!blend_job.Run()) {
            return false;
        }

        // Converts from local space to model space matrices.
        // Gets the output of the blending stage, and converts it to model space.

        // Setup local-to-model conversion job.
        ozz::animation::LocalToModelJob ltm_job;
        ltm_job.skeleton = &skeleton_;
        ltm_job.input = make_span(blended_locals_);
        ltm_job.output = make_span(models_);

        // Run ltm job.
        if (!ltm_job.Run()) {
            return false;
        }

        ozz::sample::Mesh& mesh = meshes_[0];

        size_t s = mesh.joint_remaps.size();

        for (size_t i = 0; i < s; i++) {
            skinning_matrices_[i] = models_[mesh.joint_remaps[i]] * mesh.inverse_bind_poses[i];
        }

        // Test
        /*static bool f = true;

        if (f) {
            ozz::span<const char* const> names = skeleton_.joint_names();
            size_t names_size = names.size();

            for (size_t i = 0; i < names_size; i++) {
                std::cout << "Ozz bone [" << i << "] " << names[i] << std::endl;
            }

            f = false;
        }*/

        return true;
    }

    bool OzzAnimationPartialBlending::RenderAnimationSkinned()
    {
        bool success = true;

        Wiwa::Renderer3D& r3d = Wiwa::Application::Get().GetRenderer3D();

        CameraManager& man = Wiwa::SceneManager::getActiveScene()->GetCameraManager();
        size_t cam_count = man.getCameraSize();

        const ozz::math::Float4x4 transform = ozz::math::Float4x4::identity();

        for (const ozz::sample::Mesh& mesh : meshes_) {
            for (size_t i = 0; i < mesh.joint_remaps.size(); ++i) {
                skinning_matrices_[i] =
                    models_[mesh.joint_remaps[i]] * mesh.inverse_bind_poses[i];
            }

            // Render for every camera
            for (size_t i = 0; i < cam_count; i++) {
                Camera* cam = man.getCamera(i);

                // Renders skin
                success &= r3d.RenderOzzSkinnedMesh(cam,
                    mesh, make_span(skinning_matrices_), transform);
            }

            success &= r3d.RenderOzzSkinnedMesh(man.editorCamera,
                mesh, make_span(skinning_matrices_), transform);
        }

        return success;
    }

    OzzAnimationPartialBlending::OzzAnimationPartialBlending() :
        upper_body_root_(0),
        threshold_(ozz::animation::BlendingJob().threshold),
        m_Loaded(false)
    {
        m_AnimType = AT_PARTIAL_BLEND;
    }

    OzzAnimationPartialBlending::~OzzAnimationPartialBlending()
    {}

    std::vector<glm::mat4> OzzAnimationPartialBlending::GetOrderedFinalBoneMatrices(Model* model)
    {
        size_t bone_count = model->GetBoneCount();
        std::vector<glm::mat4> ordered_list(bone_count, glm::mat4(0.0f));

        std::map<std::string, BoneInfo>& assimp_bones_map = model->GetBoneInfoMap();

        ozz::span<const char* const> names = skeleton_.joint_names();
        size_t names_size = names.size();

        for (size_t i = 0; i < names_size; i++) {
            const char* const name = names[i];

            BoneInfo& binfo = assimp_bones_map[name];

            glm::mat4& mat = ordered_list[binfo.id];

            for (int y = 0; y < 4; y++) {
                for (int x = 0; x < 4; x++) {
                    mat[y][x] = skinning_matrices_[i].cols[y].m128_f32[x];
                }
            }
        }

        return ordered_list;
    }

    bool OzzAnimationPartialBlending::LoadInfo(const char* mesh, const char* skeleton, const char* lower, const char* upper)
    {
        m_Loaded = false;

        if (!ozz::sample::LoadSkeleton(skeleton, &skeleton_)) {
            return false;
        }

        const char* filenames[] = { lower, upper };

        for (int i = 0; i < kNumLayers; ++i) {
            Sampler& sampler = samplers_[i];

            if (!ozz::sample::LoadAnimation(filenames[i], &sampler.animation)) {
                return false;
            }

            // Animation match skeleton joints
            if (skeleton_.num_joints() != sampler.animation.num_tracks()) {
                WI_ERROR("The provided animation doesn't match skeleton (joint count mismatch).");
                return false;
            }
        }

        // Allocates runtime buffers.
        const int num_soa_joints = skeleton_.num_soa_joints();
        locals_.resize(num_soa_joints);
        const int num_joints = skeleton_.num_joints();
        models_.resize(num_joints);

        // Allocates a context that matches animation requirements.
        context_.Resize(num_joints);

        if (!ozz::sample::LoadMeshes(mesh, &meshes_)) {
            return false;
        }

        size_t num_skinning_matrices = 0;
        for (const ozz::sample::Mesh& mesh : meshes_) {
            num_skinning_matrices =
                ozz::math::Max(num_skinning_matrices, mesh.joint_remaps.size());
        }

        // Allocates skinning matrices.
        skinning_matrices_.resize(num_skinning_matrices);
        //skinning_matrices_.resize(num_joints);

        // Check the skeleton matches with the mesh, especially that the mesh
        // doesn't expect more joints than the skeleton has.
        for (const ozz::sample::Mesh& mesh : meshes_) {
            if (num_joints < mesh.highest_joint_index()) {
                WI_ERROR("The provided mesh doesn't match skeleton (joint count mismatch).");
                return false;
            }
        }

        m_Loaded = true;

        return true;
    }

    void OzzAnimationPartialBlending::SetUpperBodyRoot(int ubr)
    {
        upper_body_root_ = ubr;

        SetupPerJointWeights();
    }

    bool OzzAnimationPartialBlending::Init()
    {
        // Return if not loaded
        if (!m_Loaded) return false;   

        // Initialize skeleton joint data
        const int num_joints = skeleton_.num_joints();
        const int num_soa_joints = skeleton_.num_soa_joints();

        // Initialize animations.
        
        for (int i = 0; i < kNumLayers; ++i) {
            Sampler& sampler = samplers_[i];

            // Allocates sampler runtime buffers.
            sampler.locals.resize(num_soa_joints);

            // Allocates per-joint weights used for the partial animation. Note that
            // this is a Soa structure.
            sampler.joint_weights.resize(num_soa_joints);

            // Allocates a context that matches animation requirements.
            sampler.context.Resize(num_joints);
        }

        // Default weight settings.
        Sampler& lower_body_sampler = samplers_[kLowerBody];
        lower_body_sampler.weight_setting = 1.f;
        lower_body_sampler.joint_weight_setting = 0.f;

        Sampler& upper_body_sampler = samplers_[kUpperBody];
        upper_body_sampler.weight_setting = 1.f;
        upper_body_sampler.joint_weight_setting = 1.f;
        

        // Allocates local space runtime buffers of blended data.
        blended_locals_.resize(num_soa_joints);

        // Allocates model space runtime buffers of blended data.
        models_.resize(num_joints);

        // Set body root joint
        upper_body_root_ = 10;
        
        // Init joint weights
        SetupPerJointWeights();

        return true;
    }

    bool OzzAnimationPartialBlending::Update(float _dt)
	{
        if (!m_Loaded) return false;
        
        bool update = true;

        update &= UpdateAnimation(_dt);
        update &= RenderAnimationSkinned();

        return update;
	}
}