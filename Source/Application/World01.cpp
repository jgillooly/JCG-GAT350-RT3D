#include "World01.h"
#include "Framework/Framework.h"
#include "Renderer/Renderer.h"
#include "Input/InputSystem.h"

namespace nc
{
    bool World01::Initialize()
    {
        return true;
    }

    void World01::Shutdown()
    {
    }

    void World01::Update(float dt)
    {
        m_time += dt;

        if (automatic) {
            m_angle += 45 * dt;
        }
        else {
            m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt : 0;
            m_position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? dt : 0;
            m_position.x = Clamp(m_position.x, -1.0f, 1.0f);

            m_position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? -dt : 0;
            m_position.y += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? dt : 0;
            m_position.y = Clamp(m_position.y, -1.0f, 1.0f);

            m_angle += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_Q) ? dt * 45 : 0;
            m_angle += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_E) ? -dt * 45 : 0;

            

        }
        if (ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_SPACE) && !ENGINE.GetSystem<InputSystem>()->GetPreviousKeyDown(SDL_SCANCODE_SPACE)) {
            automatic = !automatic;
        }
    }

    void World01::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render



        glPushMatrix();
        glRotatef(m_angle, 0, 0, 1);
        glTranslatef(m_position.x, m_position.y, 0);

        //glScalef(sin(m_time), 1, 1);

        glBegin(GL_TRIANGLES);

        glColor3f(1, 0, 0);
        glVertex2f(-0.5f, -0.5f);

        glColor3f(0, 0, 1);
        glVertex2f(0, 0.5f);

        glColor3f(0, 1, 0);
        glVertex2f(0.5f, -0.5f);


        glEnd();

        glPopMatrix();

        // post-render
        renderer.EndFrame();
    }
}
