#include "World04.h"
#include "Framework/Framework.h"

#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/glm.hpp>


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

        auto material = GET_RESOURCE(Material, "materials/grid.mtrl");
        m_model = std::make_shared<Model>();
        m_model->SetMaterial(material);
        m_model->Load("models/plane.obj");

        m_transform.position.y = -1;

        m_light.type = light_t::Point;
        m_light.color = glm::vec3(1);
        m_light.direction = glm::vec3{ 0, -1, 0 };
        m_light.position = glm::vec3{ 0,5,0 };
        m_light.cutoff = 30.0f;


        //m_position.z = -10.0f;
        return true;
    }

    void World04::Shutdown()
    {
    }

    void World04::Update(float dt)
    {
        

        ENGINE.GetSystem<Gui>()->BeginFrame();
        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0], 0.1f);
        ImGui::DragFloat3("Scale", &m_transform.scale[0], 0.1f);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0]);
        ImGui::End();


        ImGui::Begin("Lighting");
        const char* types[] = { "Point", "Directional", "Spot" };
        ImGui::Combo("Type", (int*)(& m_light.type), types, 3);
        ImGui::ColorEdit3("Ambient Light", &m_ambientLight[0]);
        ImGui::ColorEdit3("Diffuse Light", glm::value_ptr(m_light.color));
        if(m_light.type != light_t::Directional) ImGui::DragFloat3("Light Position", glm::value_ptr(m_light.position), 0.1f);
        if(m_light.type != light_t::Point) ImGui::DragFloat3("Direction", glm::value_ptr(m_light.direction), 0.1f);
        if (m_light.type == light_t::Spot) ImGui::DragFloat("Cutoff", &m_light.cutoff, 1.0f, 0.0f, 90.0f);
        ImGui::End();


        m_time += dt;

        auto material = m_model->GetMaterial();
        material->ProcessGui();
        material->Bind();

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
        material->GetProgram()->SetUniform("model", m_transform.GetMatrix());

        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 3 }, glm::vec3{ 0,0,0 }, glm::vec3{ 0,1,0 });
        material->GetProgram()->SetUniform("view", view);

        //projection
        float aspectRatio = (float)ENGINE.GetSystem<Renderer>()->GetWidth() / ENGINE.GetSystem<Renderer>()->GetHeight();
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), aspectRatio, 0.01f, 100.0f);
        material->GetProgram()->SetUniform("projection", projection);


        material->GetProgram()->SetUniform("light.type", m_light.type);
        material->GetProgram()->SetUniform("light.position", m_light.position);
        material->GetProgram()->SetUniform("light.direction", m_light.direction);
        material->GetProgram()->SetUniform("ambientLight", m_ambientLight);
        material->GetProgram()->SetUniform("light.color", m_light.color);
        material->GetProgram()->SetUniform("light.cutoff", glm::radians(m_light.cutoff));

        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World04::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();
        // render
        //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
        m_model->Draw();
        ENGINE.GetSystem<Gui>()->Draw();
        // post-render
        renderer.EndFrame();
    }
}
