#pragma once
#include "Framework/World.h"
#include "Core/Math/Vector3.h"
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"
#include <vector>



namespace nc
{

	class World05 : public World
	{

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

		float m_refraction = 1;
	};
}