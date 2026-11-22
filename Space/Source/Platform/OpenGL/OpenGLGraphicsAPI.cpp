#include "SP_pch.h"
#include "Platform/OpenGL/OpenGLGraphicsAPI.h"

#include <glad/glad.h>

namespace Space
{
    GraphicsAPI* GraphicsAPI::Create() { return new OpenGLGraphicsAPI(); }


    void OpenGLGraphicsAPI::SetClearColor(const glm::vec4& color) { m_ClearColor = color; }
    void OpenGLGraphicsAPI::Clear()
    {
        glClearColor(m_ClearColor.r, m_ClearColor.g, m_ClearColor.b, m_ClearColor.a);
        glClear(GL_COLOR_BUFFER_BIT);
    }

    void OpenGLGraphicsAPI::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) { glViewport(x, y, width, height); }

    void OpenGLGraphicsAPI::Init()
    {
    }

    void OpenGLGraphicsAPI::DrawIndexed(VertexArray* vertexArray)
    {
        vertexArray->Bind();
        uint32_t count = vertexArray->GetIndexBuffer()->GetCount();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }
}
