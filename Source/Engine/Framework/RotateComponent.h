#pragma once
#include "Components/Component.h"
#include <glm/glm/glm.hpp>
namespace nc {
	class RotateComponent : public Component{
	public:
		CLASS_DECLARATION(RotateComponent);
		void Update(float dt) override;
		void ProcessGui();
	private:
		glm::vec3 euler;
	};
}