#pragma once

#include "Space/Rendering/Shader.h"
#include "Space/Rendering/VertexArray.h"
#include "Space/Rendering/RenderCommand.h"
#include "Space/Rendering/GraphicsAPI.h"

namespace Space
{
    class Renderer
    {
    public:
        Renderer() = default;

        void Init();
        void Shutdown();

        void OnWindowResize(uint32_t width, uint32_t height);

        void BeginScene(const glm::mat4& viewProjectionMatrix);
        
        void Submit(const std::shared_ptr<Shader>& shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform, const glm::vec4& color);

        void EndScene();
    private:
        void SortQueue();
        void Flush();
    private:
        GraphicsAPI* m_GraphicsAPI = nullptr;
        std::vector<RenderCommand> m_DrawQueue;
        glm::mat4 m_ViewProjectionMatrix;

        bool begunScene = false;
    };
}
