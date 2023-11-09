#include "World06.h"
#include "Framework/Framework.h"

#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/glm.hpp>
#include <glm/glm/gtx/color_space.hpp>


//#define INTERLEAVE
#define INDEX

namespace nc
{
    bool World06::Initialize()
    {

        m_scene = std::make_unique<Scene>();
        m_scene->Load("Scenes/scene.json");
        m_scene->Initialize();

        auto texture = std::make_shared<Texture>();
        texture->CreateTexture(512, 512);
        ADD_RESOURCE("fb_texture", texture);

        auto framebuffer = std::make_shared<Framebuffer>();
        framebuffer->CreateFramebuffer(texture);
        ADD_RESOURCE("fb", framebuffer);
        
        auto material = GET_RESOURCE(Material, "materials/framebuffer.mtrl");
        if (material) {
            material->albedoTexture = texture;
        }



        return true;
    }

    void World06::Shutdown()
    {
    }

    void World06::Update(float dt)
    {
        m_time += dt;

        ENGINE.GetSystem<Gui>()->BeginFrame();
        m_scene->Update(dt);
        m_scene->ProcessGui();
       

        //m_transform.rotation.z += 180 * dt;

        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_SPACE) && !ENGINE.GetSystem<InputSystem>()->GetPreviousKeyDown(SDL_SCANCODE_SPACE)) {
            //Magic debug :)
            std::cout << "debug";
        }


        ENGINE.GetSystem<Gui>()->EndFrame();
    }

    void World06::Draw(Renderer& renderer)
    {
        //PASS 1
        //m_scene->GetActorByName("cube")->active = false;
        //auto framebuffer = GET_RESOURCE(Framebuffer, "fb");
        //renderer.setViewport(framebuffer->GetSize().x, framebuffer->GetSize().y);
        //framebuffer->Bind();

        //renderer.BeginFrame({1,0,0});
        //m_scene->Draw(renderer);

        //framebuffer->Unbind();

        //PASS 2
        //m_scene->GetActorByName("cube")->active = true;
        renderer.resetViewport();
        renderer.BeginFrame();
        m_scene->Draw(renderer);



        ENGINE.GetSystem<Gui>()->Draw();
        // post-render
        renderer.EndFrame();
    }
}
