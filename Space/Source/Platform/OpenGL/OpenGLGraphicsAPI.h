#pragma once

#include "Space/Rendering/GraphicsAPI.h"

namespace Space
{
    class OpenGLGraphicsAPI : public GraphicsAPI
    {
    public:
        OpenGLGraphicsAPI() = default;

        virtual void Init() override;

        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) override;

        virtual void SetClearColor(const glm::vec4& color) override;
        virtual void Clear() override;

        virtual void DrawIndexed(VertexArray* vertexArray) override;
    private:
        glm::vec4 m_ClearColor;
    };
}
