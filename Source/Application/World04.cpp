#include "World04.h"
#include "Framework/Framework.h"

#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/color_space.hpp>


//#define INTERLEAVE
#define INDEX

namespace nc
{
    bool World04::Initialize()
    {
        //m_material = GET_RESOURCE(Material, "materials/quad.mtrl");

        //shaders
        //m_program = GET_RESOURCE(Program, "Shaders/unlit_texture.prog");
        //m_program->Use();

        //m_texture = GET_RESOURCE(Texture, "Textures/llama.jpg");
        //m_texture->Bind();
        //m_texture->SetActive(GL_TEXTURE0);

        //auto material = GET_RESOURCE(Material, "materials/multi.mtrl");
        //m_model = std::make_shared<Model>();
        //m_model->SetMaterial(material);
        //m_model->Load("models/plane.obj");

        m_material = GET_RESOURCE(Material, "materials/squirrel.mtrl");
        m_model = std::make_shared<Model>();
        //m_model->SetMaterial(material);
        m_model->Load("models/squirrel.glb", glm::vec3{ 0, -0.7f, 0 }, glm::vec3{ 0 }, glm::vec3{ 0.4f });

        m_transform.position.y = -1;

        for (int i = 0; i < m_numLights; i++)
        {
            m_lights[i].type = light_t::Point;
            m_lights[i].color = glm::rgbColor(glm::vec3{ randomf() * 360, 1, 1 });            m_lights[i].intensity = 1;
            m_lights[i].range = 6;
            m_lights[i].direction = glm::vec3{ 0, -1, 0 };
            m_lights[i].position = glm::vec3{ randomf(-5.0f, 5.0f),randomf(1.0f, 8.0f),randomf(-5.0f, 5.0f)};
            m_lights[i].innerAngle = 10.0f;
            m_lights[i].outerAngle = 30.0f;
        }
        


        //m_position.z = -10.0f;
        return true;
    }

    void World04::Shutdown()
    {
    }

    void World04::Update(float dt)
    {
        

        ENGINE.GetSystem<Gui>()->BeginFrame();


        ImGui::Begin("Scene");
        ImGui::ColorEdit3("Ambient Color", glm::value_ptr(m_ambientLight));
        ImGui::Separator();

        for (int i = 0; i < 3; i++)
        {
            std::string name = "light" + std::to_string(i);
            if (ImGui::Selectable(name.c_str(), m_selected == i)) m_selected = i;
        }
        ImGui::End();

        ImGui::Begin("Lighting");
        const char* types[] = { "Point", "Directional", "Spot" };
        ImGui::Combo("Type", (int*)(&m_lights[m_selected].type), types, 3);
        ImGui::ColorEdit3("Ambient Light", &m_ambientLight[0]);
        ImGui::ColorEdit3("Diffuse Light", glm::value_ptr(m_lights[m_selected].color));
        ImGui::DragFloat("Intensity", &m_lights[m_selected].intensity, 0.01f, 0.0f, 10.0f);
        if (m_lights[m_selected].type != light_t::Directional) {
            ImGui::DragFloat3("Light Position", glm::value_ptr(m_lights[m_selected].position), 0.1f);
            ImGui::DragFloat("Range", &m_lights[m_selected].range, 0.1f, 0.1f, 50.0f);
        }
        if(m_lights[m_selected].type != light_t::Point) ImGui::DragFloat3("Direction", glm::value_ptr(m_lights[m_selected].direction), 0.1f);
        if (m_lights[m_selected].type == light_t::Spot) {
            ImGui::DragFloat("Inner Angle", &m_lights[m_selected].innerAngle, 1.0f, 0.0f, m_lights[m_selected].outerAngle);
            ImGui::DragFloat("Outer Angle", &m_lights[m_selected].outerAngle, 1.0f, m_lights[m_selected].innerAngle, 90.0f);
        }
        ImGui::End();


        m_time += dt;

        //auto material = m_model->GetMaterial();
        m_material->ProcessGui();
        m_material->Bind();

        //m_transform.rotation.z += 180 * dt;

        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt * speed : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? +dt * speed : 0;

        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? -dt * speed : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? +dt * speed : 0;

        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_SPACE) && !ENGINE.GetSystem<InputSystem>()->GetPreviousKeyDown(SDL_SCANCODE_SPACE)) {
            //Magic debug :)
            std::cout << "debug";
        }

        
        //glm::mat4 position = glm::translate(glm::mat4{ 1 }, m_position);
        //glm::mat4 rotation = glm::rotate(glm::mat4{ 1 }, m_time, glm::vec3{ 0,0,1 });
        //glm::mat4 model = position * rotation;

        //m_program->SetUniform("offset", m_offset);
        //m_program->SetUniform("tiling", m_tiling);

        //m_material->GetProgram()->SetUniform("model", m_transform.GetMatrix());
        m_material->GetProgram()->SetUniform("model", m_transform.GetMatrix());

        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 3 }, glm::vec3{ 0,0,0 }, glm::vec3{ 0,1,0 });
        m_material->GetProgram()->SetUniform("view", view);

        //projection
        float aspectRatio = (float)ENGINE.GetSystem<Renderer>()->GetWidth() / ENGINE.GetSystem<Renderer>()->GetHeight();
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), aspectRatio, 0.01f, 100.0f);
        m_material->GetProgram()->SetUniform("projection", projection);
        m_material->GetProgram()->SetUniform("IWanttoDie", 4);

        for (int i = 0; i < 3; i++)
        {
            std::string name = "lights[" + std::to_string(i) + "]";
            m_material->GetProgram()->SetUniform(name + ".type", m_lights[i].type);
            m_material->GetProgram()->SetUniform(name + ".position", m_lights[i].position);
            m_material->GetProgram()->SetUniform(name + ".direction", normalize(m_lights[i].direction));
            m_material->GetProgram()->SetUniform("ambientLight", m_ambientLight);
            m_material->GetProgram()->SetUniform(name + ".color", m_lights[i].color);
            m_material->GetProgram()->SetUniform(name + ".intensity", m_lights[i].intensity);
            m_material->GetProgram()->SetUniform(name + ".range", m_lights[i].range);
            m_material->GetProgram()->SetUniform(name + ".innerAngle", glm::radians(m_lights[i].innerAngle));
            m_material->GetProgram()->SetUniform(name + ".outerAngle", glm::radians(m_lights[i].outerAngle));
        }


        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World04::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();
        // render
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_material->Bind();
        m_model->Draw();
        ENGINE.GetSystem<Gui>()->Draw();
        // post-render
        renderer.EndFrame();
    }
}
