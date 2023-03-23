#pragma once
#pragma warning(disable : 4251)

#include <Wiwa/core/Core.h>

// 3D maths lib
// #include <MathGeoLib.h>
// 2D maths lib
#include <Wiwa/utilities/math/Math.h>

#include <Wiwa/utilities/Log.h>
#include <Wiwa/core/Resources.h>

// #include "utilities/gui/Texture.h"

#include <Wiwa/utilities/render/VertexBuffer.h>

// #include "Empathy/headers/utilities/renderer/Batch.h"
#include <Wiwa/utilities/render/FrameBuffer.h>
#include <Wiwa/utilities/render/Camera.h>

#define MAXQUADS 40500
#define MAXVERTEX MAXQUADS * 4
#define MAXINDICES MAXQUADS * 6

namespace Wiwa
{
	class InstanceRenderer;
	class Scene;

	class WI_API Renderer2D
	{
	public:
		enum class Pivot
		{
			UPLEFT,
			DOWNLEFT,
			CENTER
		};

	private:
		// Default FrameBuffer

		uint32_t m_RenderCallsInstancedCount;

		Camera m_ActiveCamera;

	public:
		Renderer2D();
		~Renderer2D();

		static Vector2f CalculateScreenGlPos(const Rect2i &rect, Pivot pivot);
		static Vector2f CalculateScreenGlPos(const Vector2i &position, const Size2i &size, Pivot pivot);
		static TextureClip CalculateTextureClip(const Rect2i &rect, const Size2i &texSize);

		bool Init();
		void Update();

		void Close();

		// Instance rendering functions
		uint32_t CreateInstancedQuadTex(Scene *scene, uint32_t textureId, const Size2i &srcSize, const Vector2i &position, const Size2i &size, const Color4f &color, const Rect2i &clip, Pivot pivot = Pivot::CENTER);
		uint32_t CreateInstancedQuadTex(Scene *scene, uint32_t textureId, const Size2i &srcSize, const Vector2i &position, const Size2i &size, Pivot pivot = Pivot::CENTER);
		uint32_t CreateInstancedQuadTex(Scene *scene, uint32_t textureId, const Size2i &srcSize, const Vector2i &position, const Size2i &size, const Rect2i &clip, Pivot pivot = Pivot::CENTER);

		void RemoveInstance(Scene *scene, uint32_t instance);
		void EnableInstance(Scene *scene, uint32_t instance);
		void DisableInstance(Scene *scene, uint32_t instance);
		void SetInstanceEnabled(Scene *scene, uint32_t instance, bool enabled);

		void UpdateInstancedQuadTexPosition(Scene *scene, uint32_t id, const Vector2i &position, Pivot pivot = Pivot::CENTER);
		void UpdateInstancedQuadTexSize(Scene* scene, uint32_t id, const Vector2i& pos, const Size2i& size, Renderer2D::Pivot pivot = Renderer2D::Pivot::CENTER);
		void UpdateInstancedQuadTexClip(Scene* scene, uint32_t id, const Size2i& srcSize, const Rect2i& clip);
		void UpdateInstancedQuadTexTexture(Scene* scene, uint32_t id, uint32_t tex_id);
		void UpdateInstancedQuadTexColor(Scene* scene, uint32_t id, const Color4f& color);

		void UpdateInstancedQuad(Scene* scene,uint32_t id, const Vector2i &position, const Size2i &size, const Color4f &color);

		void UpdateInstanced(Scene *scene);

		// Getters
		uint32_t getRenderInstancedCalls() { return m_RenderCallsInstancedCount; }
		uint32_t getRenderInstanceCount() { return MAXQUADS; }
		uint32_t getColorBufferTexture() { return m_ActiveCamera.frameBuffer->getColorBufferTexture(); }

		glm::mat4 GetOrthoProjection() { return m_ActiveCamera.getProjection(); }
		glm::mat4 GetView() { return m_ActiveCamera.getView(); }
	};
}