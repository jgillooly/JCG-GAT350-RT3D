#include "World03.h"
#include "Framework/Framework.h"

#include "Input/InputSystem.h"
#include <glm/glm/gtc/type_ptr.hpp>
#include <glm/glm/glm.hpp>


//#define INTERLEAVE
#define INDEX

namespace nc
{
    bool World03::Initialize()
    {
        //shaders
        m_program = GET_RESOURCE(Program, "Shaders/unlit_texture.prog");
        m_program->Use();

        m_texture = GET_RESOURCE(Texture, "Textures/llama.jpg");
        m_texture->Bind();
        m_texture->SetActive(GL_TEXTURE0);

#ifdef INTERLEAVE
        //vertex data
        float vertexData[] = {
            -0.5f, -0.5f, 0.0f, 1.0f, 0.843f, 0.0f, 0.0f, 0.0f,
            -0.5f, 0.5f, 0.0f, 1.0f, 0.843f, 0.0f, 0.0f, 1.0f,
             0.5f,  0.5f, 0.0f, 1.0f, 0.647f, 0.0f, 1.0f, 1.0f,
             0.5f,  -0.5f, 0.0f,1.0f, 0.647f, 0.0f, 1.0f, 0.0f
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
            -1.0f,  1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 0.0f, 1.0f,// top-left
             1.0f,  1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f,// top-right
             1.0f, -1.0f, 0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.0f,// bottom-right
            -1.0f, -1.0f, 0.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f // bottom-left
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

        glBindVertexBuffer(0, vbo, 0, 8 * sizeof(GLfloat));

        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ibo);

        glEnableVertexAttribArray(0);
        glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(0, 0);

        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat));
        glVertexAttribBinding(1, 0);

        glEnableVertexAttribArray(2);
        glVertexAttribFormat(2, 2, GL_FLOAT, GL_FALSE, 6 * sizeof(GLfloat));
        glVertexAttribBinding(2, 0);
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

        //m_position.z = -10.0f;
        return true;
    }

    void World03::Shutdown()
    {
    }

    void World03::Update(float dt)
    {
        ENGINE.GetSystem<Gui>()->BeginFrame();
        ImGui::Begin("Transform");
        ImGui::DragFloat3("Position", &m_transform.position[0]);
        ImGui::DragFloat3("Scale", &m_transform.scale[0]);
        ImGui::DragFloat3("Rotation", &m_transform.rotation[0]);
        ImGui::End();

        m_time += dt;

        //m_transform.rotation.z += 180 * dt;

        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_A) ? -dt * speed : 0;
        m_transform.position.x += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_D) ? +dt * speed : 0;

        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_W) ? -dt * speed : 0;
        m_transform.position.z += ENGINE.GetSystem<InputSystem>()->GetKeyDown(SDL_SCANCODE_S) ? +dt * speed : 0;

        
        //glm::mat4 position = glm::translate(glm::mat4{ 1 }, m_position);
        //glm::mat4 rotation = glm::rotate(glm::mat4{ 1 }, m_time, glm::vec3{ 0,0,1 });
        //glm::mat4 model = position * rotation;

        m_program->SetUniform("model", m_transform.GetMatrix());

        glm::mat4 view = glm::lookAt(glm::vec3{ 0, 0, 3 }, glm::vec3{ 0,0,0 }, glm::vec3{ 0,1,0 });
        m_program->SetUniform("view", view);

        //projection
        glm::mat4 projection = glm::perspective(glm::radians(70.0f), 800.0f / 600.0f, 0.01f, 100.0f);
        m_program->SetUniform("projection", projection);

        ENGINE.GetSystem<Gui>()->EndFrame();
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
        ENGINE.GetSystem<Gui>()->Draw();
        // post-render
        renderer.EndFrame();
    }
}
