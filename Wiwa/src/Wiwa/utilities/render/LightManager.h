#pragma once

#include <Wiwa/core/Core.h>

namespace Wiwa {
	class WI_API LightManager
	{
	public:

		LightManager();
		~LightManager();
		

		void AddPointLight(size_t light);
		void RemovePointLight(size_t light);

		void AddSpotLight(size_t light);
		void RemoveSpotLight(size_t light);

		inline size_t GetPointLightsSize() { return m_PointLights.size(); }
		inline std::vector<size_t>& GetPointLights() { return m_PointLights; }

		inline size_t GetSpotLightsSize() { return m_SpotLights.size(); }
		inline std::vector<size_t>& GetSpotLights() { return m_SpotLights; }


		// Set to -1 if there's no directional lights
		inline void SetDirectionalLight(size_t light) { m_DirectionalLight = light; }
		inline size_t GetDirectionalLight() { return m_DirectionalLight; }
	private:

		std::vector<size_t> m_PointLights;
		std::vector<size_t> m_SpotLights;
		size_t m_DirectionalLight = -1;
	};
}