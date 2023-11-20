#include "World08.h"
#include "Framework/Framework.h"

#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/color_space.hpp>


//#define INTERLEAVE
#define INDEX

namespace nc
{
    bool World08::Initialize()
    {

        m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/scene_cell_shaded.json");
        m_scene->Initialize();

        m_editor = std::make_unique<Editor>();

        //depth texture
        auto texture = std::make_shared<Texture>();
        texture->CreateDepthTexture(1024, 1024);
        ADD_RESOURCE("depth_texture", texture);

        //depth buffer
        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateDepthBuffer(texture);
        ADD_RESOURCE("depth_buffer", framebuffer);
        
        //set depth texture to debug sprite
        auto material = GET_RESOURCE(Material, "materials/sprite.mtrl");
        if (material) {
            material->albedoTexture = texture;
        }

        auto materials = GET_RESOURCES(Material);
        for (auto& material : materials) {
            material->depthTexture = texture;
        }



        return true;
    }

    void World08::Shutdown()
    {
    }

    void World08::Update(float dt)
    {
        m_time += dt;

        ENGINE.GetSystem<Gui>()->BeginFrame();
        m_scene->Update(dt);


        m_editor->ProcessGui(m_scene.get());

        //m_transform.rotation.z += 180 * dt;

        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_SPACE) && !ENGINE.GetSystem<InputSystem>()->GetPreviousKeyDown(SDL_SCANCODE_SPACE)) {
            //Magic debug :)
            std::cout << "debug";
        }

        auto material = GET_RESOURCE(Material, "fantasy/fantasy_cell.mtrl");
        material->ProcessGui();
        ImGui::DragInt("Cel Levels", &celLevels, 1.0f, 4, 50);
        ImGui::DragFloat("Cel Outline", &celOutline, 0.1f, 0, 1);

        ENGINE.GetSystem<Gui>()->EndFrame();
        auto program = GET_RESOURCE(Program, "shaders/cell_shaded.prog");
        if (program) {
            program->Use();
            program->SetUniform("celLevels", celLevels);
            program->SetUniform("celOutline", celOutline);
        }
    }

    void World08::Draw(Renderer& renderer)
    {
        //PASS 1

        auto framebuffer = GET_RESOURCE(Framebuffer, "depth_buffer");
        renderer.setViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        framebuffer->Bind();

        renderer.ClearDepth();
        auto program = GET_RESOURCE(Program, "shaders/shadow_depth.prog");
        program->Use();

        auto lights = m_scene->GetComponents<LightComponent>();
        for (auto& light : lights) {
            if (light->castShadow) {
                glm::mat4 shadowMatrix = light->GetShadowMatrix();
                program->SetUniform("shadowVP", shadowMatrix);
            }
        }

        auto models = m_scene->GetComponents<ModelComponent>();
        for (auto& model : models) {
            if (model->castShadow) {
                program->SetUniform("model", model->m_owner->transform.GetMatrix());
                model->model->Draw();
            }
        }

        auto material = GET_RESOURCE(Material, "fantasy/fantasy.mtrl");

        //m_scene->Draw(renderer);

        framebuffer->Unbind();

        //PASS 2
        renderer.resetViewport();
        renderer.BeginFrame();
        m_scene->Draw(renderer);



        ENGINE.GetSystem<Gui>()->Draw();
        // post-render
        renderer.EndFrame();
    }
}
