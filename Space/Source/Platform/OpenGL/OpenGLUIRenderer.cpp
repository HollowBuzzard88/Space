#include "SP_pch.h"
#include "Space/Rendering/UI/UIRenderer.h"

#include <glad/glad.h>

namespace Space
{
    void UIRenderer::DrawIndexed(VertexArray *vertexArray)
    {
        vertexArray->Bind();
        uint32_t count = vertexArray->GetIndexBuffer()->GetCount();
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);
    }
}
