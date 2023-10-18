#pragma once
#include "Framework/World.h"
#include "Core/Math/Vector3.h"
#include "Renderer/Renderer.h"
#include "Core/Math/Transform.h"
#include <vector>

namespace nc
{
	class World04 : public World
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

		glm::vec3 m_ambientLight{ 0.2 };
		glm::vec3 m_diffuseLight{ 0.8 };
		glm::vec3 m_lightPosition{ 0,8,0 };

		res_t<Program> m_program;
		res_t<Texture> m_texture;
		res_t<VertexBuffer> m_vertexBuffer;
		res_t<Material> m_material;
		res_t<Model> m_model;
	};
}