#pragma once

#include "Space/Rendering/VertexArray.h"
#include <glm/glm.hpp>

namespace Space
{
    class GraphicsAPI
    {
    public:
        virtual ~GraphicsAPI() = default;

        virtual void Init() = 0;

        virtual void SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height) = 0;

        virtual void SetClearColor(const glm::vec4& color) = 0;
        virtual void Clear() = 0;

        virtual void DrawIndexed(VertexArray* vertexArray) = 0;

        static GraphicsAPI* Create();
    };
}
