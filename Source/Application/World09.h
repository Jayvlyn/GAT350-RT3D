#pragma once
#include "Framework/World.h"
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"
#include <vector>

namespace nc
{
	class World09 : public World
	{
	public:
		const uint32_t INVERT_MASK	  = (1 << 0);
		const uint32_t GRAYSCALE_MASK = (1 << 1);
		const uint32_t COLORTINT_MASK = (1 << 2);
		const uint32_t GRAIN_MASK	  = (1 << 3);
		const uint32_t SCANLINE_MASK  = (1 << 4);
		const uint32_t KERNEL_MASK	  = (1 << 5);
	
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_time;
		float m_speed = 5;
		float m_refraction = 1;

		uint32_t m_params{ 0 };
		glm::vec3 m_colorTint{ 1, 0, 0 };
		float scanlineSpacing{ 1 };
		float scanlineIntensity{ 1 };
		float kernelOffset = 300;

		int celSteps = 6;

		float m_blend = 1;

		glm::vec3 m_ambientColor{ 0.2f };
	};
}