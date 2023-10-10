#pragma once
#include "Framework/World.h"
#include "Core/Math/Vector2.h"
#include "Renderer/Renderer.h"

namespace nc
{
	class World03 : public World
	{
	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_angle = 0;
		float m_time;
		glm::vec3 m_position;
		float m_speed = 5;

		GLuint m_vao;
		res_t<Program> m_program;
	};
}