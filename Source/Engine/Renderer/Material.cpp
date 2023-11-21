#include "Material.h"
#include "Program.h"
#include "Texture.h"
#include "Cubemap.h"
#include "Core/Core.h"
#include <imgui/ImFileDialog.h>

namespace nc
{
	bool Material::Create(std::string filename, ...)
	{
		// load program json document
		rapidjson::Document document;
		bool success = Json::Load(filename, document);
		if (!success)
		{
			INFO_LOG("Could not load program file (%s)." << filename);
			return false;
		}

		// read the program name
		std::string program;
		READ_DATA(document, program);
		// get program resource
		m_program = GET_RESOURCE(Program, program);

		// read the textures
		std::string albedoTextureName;
		READ_NAME_DATA(document, "albedoTexture", albedoTextureName);
		if (!albedoTextureName.empty()) {
			albedoTexture = GET_RESOURCE(Texture, albedoTextureName);
			
		}

		std::string specularTextureName;
		READ_NAME_DATA(document, "specularTexture", specularTextureName);
		if (!specularTextureName.empty()) {
			specularTexture = GET_RESOURCE(Texture, specularTextureName);
			
		}

		std::string normalTextureName;
		READ_NAME_DATA(document, "normalTexture", normalTextureName);
		if (!normalTextureName.empty()) {
			specularTexture = GET_RESOURCE(Texture, normalTextureName);

		}

		std::string emissiveTextureName;
		READ_NAME_DATA(document, "emissiveTexture", emissiveTextureName);
		if (!emissiveTextureName.empty()) {
			emissiveTexture = GET_RESOURCE(Texture, emissiveTextureName);
			
		}

		std::string cubemapName;
		if (READ_NAME_DATA(document, "cubemap", cubemapName)) {
			params |= CUBEMAP_TEXTURE_MASK;
			std::vector<std::string> cubemaps;
			READ_DATA(document, cubemaps);
			cubemapTexture = GET_RESOURCE(Cubemap, cubemapName, cubemaps);
		}

		READ_DATA(document, albedo);
		READ_DATA(document, specular);
		READ_DATA(document, emissive);
		READ_DATA(document, shininess);
		READ_DATA(document, tiling);
		READ_DATA(document, offset);

		return true;
	}

	void Material::Bind()
	{
		m_program->Use();
		m_program->SetUniform("material.params", params);
		m_program->SetUniform("material.albedo", albedo);
		m_program->SetUniform("material.specular", specular);
		m_program->SetUniform("material.emissive", emissive);
		m_program->SetUniform("material.shininess", shininess);
		m_program->SetUniform("material.tiling", tiling);
		m_program->SetUniform("material.offset", offset);

		if (albedoTexture) {
			albedoTexture->SetActive(GL_TEXTURE0);
			albedoTexture->Bind();
			params |= ALBEDO_TEXTURE_MASK;
		}

		if (specularTexture) {
			specularTexture->SetActive(GL_TEXTURE1);
			specularTexture->Bind();
			params |= SPECULAR_TEXTURE_MASK;
		}

		if (normalTexture) {
			normalTexture->SetActive(GL_TEXTURE2);
			normalTexture->Bind();
			params |= NORMAL_TEXTURE_MASK;
		}

		if (emissiveTexture) {
			emissiveTexture->SetActive(GL_TEXTURE3);
			emissiveTexture->Bind();
			params |= EMISSIVE_TEXTURE_MASK;
		}

		if (cubemapTexture)
		{
			cubemapTexture->SetActive(GL_TEXTURE4);
			cubemapTexture->Bind();
			params |= CUBEMAP_TEXTURE_MASK;
		}

		if (depthTexture)
		{
			depthTexture->SetActive(GL_TEXTURE5);
			depthTexture->Bind();
		}

	}

	void Material::ProcessGui()
	{
		// shader
		ImGui::Text("Shader:");
		ImGui::SameLine();
		ImGui::Text("%s", m_program->name.c_str());
		Gui::GetDialogueResource<Program>(m_program, "ShaderTextureDialog", "Open Shader", "Shader file (*.prog){.prog},.*");

		ImGui::TextColored(ImVec4{ 0, 1, 0, 1 }, "Name: %s", name.c_str());
		//albedo
		ImGui::Text("Albedo  ");
		ImGui::SameLine();
		ImGui::ColorEdit3("Albedo", glm::value_ptr(albedo), ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine();
		(albedoTexture) ? ImGui::Text("%s", albedoTexture->name.c_str()) : ImGui::Text("None");
		Gui::GetDialogueResource<Texture>(albedoTexture, "AlbedoTextureDialog", "Open texture", "Image file (*.png;*.jpg;*.jpeg;*.bmp;*.tga){.png,.jpg,.jpeg,.bmp,.tga},.*");
		//ImGui::ColorEdit3("Albedo", glm::value_ptr(albedo));
		

		//specular
		ImGui::Text("Specular  ");
		ImGui::SameLine();
		ImGui::ColorEdit3("Specular", glm::value_ptr(specular), ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine();
		(specularTexture) ? ImGui::Text("%s", specularTexture->name.c_str()) : ImGui::Text("None");
		Material mat;
		Gui::GetDialogueResource<Texture>(specularTexture, "ModelTextureDialog", "Open Model", "Model file (*.obj;*.fbx){.obj,.fbx},.*");
		//ImGui::ColorEdit3("Specular", glm::value_ptr(specular));
		ImGui::DragFloat("Shininess", &shininess, 0.1, 2.0f, 200.0f);
		//emissive
		ImGui::Text("Emissive  ");
		ImGui::SameLine();
		ImGui::ColorEdit3("Emissive", glm::value_ptr(emissive), ImGuiColorEditFlags_NoLabel | ImGuiColorEditFlags_NoInputs);
		ImGui::SameLine();
		(emissiveTexture) ? ImGui::Text("%s", emissiveTexture->name.c_str()) : ImGui::Text("None");
		ImGui::Text("Normal  ");
		ImGui::SameLine();
		(normalTexture) ? ImGui::Text("%s", normalTexture->name.c_str()) : ImGui::Text("None");
		//ImGui::ColorEdit3("Emissive", glm::value_ptr(emissive));
		ImGui::DragFloat2("Tiling", glm::value_ptr(tiling));
		ImGui::DragFloat2("Offset", glm::value_ptr(offset), 0.1f);

	}
}