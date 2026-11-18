#include "SP_pch.h"
#include "Space/Rendering/Renderer.h"

#include "glad/glad.h"

#include "Space/Scene/Planet.h"

#include "Space/Core/AssetManager.h"

namespace Space
{
    Renderer::API Renderer::s_API = Renderer::API::OpenGL;

    unsigned int Renderer::VBO;
    unsigned int Renderer::VAO;
    Shader Renderer::ShaderProgram;
    std::vector<float> Renderer::verts;

    void Renderer::Init()
    {
        ShaderProgram = Shader(
            AssetManager::GetAbsolutePath("Shaders/vertex.vert"),
            AssetManager::GetAbsolutePath("Shaders/fragment.frag")
        );

        verts = Planet::GeneratePlanet();

        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        
        glBindVertexArray(VAO);

        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, verts.size() * sizeof(float), verts.data(), GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
        glEnableVertexAttribArray(0);

        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }

    void Renderer::Shutdown()
    {

    }

    void Renderer::OnWindowResize(uint32_t width, uint32_t height)
    {

    }

    void Renderer::Draw(Mesh element)
    {

    }

    void Renderer::Render()
    {
        ShaderProgram.Activate();
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0.0f, verts.size() / 3.0f);
    }
}
