#include "World03.h"
#include "Framework/Framework.h"

#include "Input/InputSystem.h"

//#define INTERLEAVE
#define INDEX

namespace nc
{
    bool World03::Initialize()
    {
        //shaders
        m_program = GET_RESOURCE(Program, "Shaders/unlit_color.prog");
        m_program->Use();

#ifdef INTERLEAVE
        //vertex data
        float vertexData[] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.843f, 0.0f,
            -0.5f, 0.5f, 0.0f, 1.0f, 0.843f, 0.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 0.647f, 0.0f,
             0.5f,  -0.5f, 0.0f,1.0f, 0.647f, 0.0f
        };

        GLuint vbo;
        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);


        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glBindVertexBuffer(0, vbo, 0, 6 * sizeof(GLfloat));

        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);

        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
        glVertexAttribBinding(1, 0);
#elif defined(INDEX)
        //vertex data
        const float vertexData[] = {
            -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, // top-left
             1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, // top-right
             1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, // bottom-right
            -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f  // bottom-left
        };

        GLuint indexes[] = {
            0,1,2,
            2,3,0
        };

        //vertex buffer object
        GLuint vbo;
        glGenBuffers(1, &vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertexData), vertexData, GL_STATIC_DRAW);

        //index buffer object
        GLuint ibo;
        glGenBuffers(1, &ibo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indexes), indexes, GL_STATIC_DRAW);

        //vertex array object
        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glBindVertexBuffer(0, vbo, 0, 6 * sizeof(GLfloat));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);

        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
        glVertexAttribBinding(1, 0);
#else
        //vertex data
        float positionData[] = {
            -0.5f, -0.5f, 0.0f,
             -0.5f, 0.5f, 0.0f,
             0.5f,  0.5f, 0.0f,
             0.5f,  -0.5f, 0.0f
        };

        float colorData[] =
        {
            1.0f, 0.843f, 0.0f,
            1.0f, 0.843f, 0.0f,
            1.0f, 0.647f, 0.0f,
            1.0f, 0.647f, 0.0f
        };

        GLuint vbo[2];
        glGenBuffers(2, vbo);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[0]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(positionData), positionData, GL_STATIC_DRAW);

        glBindBuffer(GL_ARRAY_BUFFER, vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(colorData), colorData, GL_STATIC_DRAW);


        glGenVertexArrays(1, &m_vao);
        glBindVertexArray(m_vao);

        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexBuffer(0, vbo[0], 0, 3 * sizeof(GLfloat));

        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 0, 0);
        glBindVertexBuffer(1, vbo[1], 0, 3 * sizeof(GLfloat));
#endif // INTERLEAVE


        return true;
    }

    void World03::Shutdown()
    {
    }

    void World03::Update(float dt)
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

        GLint uniform = glGetUniformLocation(m_program->m_program, "time");
        glUniform1f(uniform, m_time);
    }

    void World03::Draw(Renderer& renderer)
    {
        // pre-render
        renderer.BeginFrame();

        // render
        glBindVertexArray(m_vao);
#ifdef INDEX
        glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
#else
        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
#endif

        // post-render
        renderer.EndFrame();
    }
}
