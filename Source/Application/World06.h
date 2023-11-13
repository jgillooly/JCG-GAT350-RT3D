#pragma once
#include "Framework/World.h"
#include "Core/Math/Vector3.h"
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"
#include <vector>



namespace nc
{

	class World06 : public World
	{
	public:
		const uint32_t INVERT_MASK = (1 << 0);
		const uint32_t GRAYSCALE_MASK = (1 << 1);
		const uint32_t COLORTINT_MASK = (1 << 2);

	public:
		bool Initialize() override;
		void Shutdown() override;
		void Update(float dt) override;
		void Draw(Renderer& renderer) override;

	private:
		float m_angle = 0;
		float m_time;
		float speed = 5;
		bool automatic = false;
		glm::vec3 m_ambientLight;

		uint32_t m_params{ 0 };

		float m_blend = 1;
		float m_refraction = 1;
	};
}