#include "ModelComponent.h"
#include "Framework/Actor.h"
#include "Framework/Resource/ResourceManager.h"
#include "Core/StringUtils.h"

namespace nc
{
	CLASS_DEFINITION(ModelComponent)

	bool ModelComponent::Initialize()
	{
		if (!modelName.empty())
		{
			model = std::make_shared<Model>();
			model = GET_RESOURCE(Model, modelName);
			//ADD_RESOURCE(modelName, model);
		}
		if (model && !materialName.empty())
		{
			material = (GET_RESOURCE(Material, materialName));
		}

		return true;
	}

	void ModelComponent::Update(float dt)
	{
	}

	void ModelComponent::Draw(Renderer& renderer)
	{
		material->Bind();
		material->GetProgram()->SetUniform("model", m_owner->transform.GetMatrix());

		glDepthMask(enableDepth);
		glCullFace(cullface);
		model->Draw();
		//m_model->Draw(renderer, m_owner->transform);
	}

	void ModelComponent::Read(const json_t& value)
	{
		READ_DATA(value, modelName);
		READ_DATA(value, materialName);
		READ_DATA(value, enableDepth);
		std::string cullfacename;
		READ_NAME_DATA(value, "cullface", cullfacename);
		if (StringUtils::IsEqualIgnoreCase(cullfacename, "front")) cullface = GL_FRONT;
	}
}
