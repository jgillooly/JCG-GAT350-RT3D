#pragma once
#include "Framework/World.h"
#include "Core/Math/Vector3.h"
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"
#include <vector>

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
		float speed = 5;
		bool automatic = false;
		Transform m_transform;
		glm::vec2 m_offset{ 0 };
		glm::vec2 m_tiling{ 1 };

		res_t<Program> m_program;
		res_t<Texture> m_texture;
		res_t<VertexBuffer> m_vertexBuffer;
		res_t<Material> m_material;
	};
}