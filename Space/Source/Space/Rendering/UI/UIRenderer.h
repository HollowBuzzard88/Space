#pragma once

#include "Space/Rendering/Renderer.h"
#include "Space/Rendering/TextRenderer.h"

#include "Space/UI/Widgets.h"

namespace Space
{
    class UIRenderer
    {
    public:
        UIRenderer() = default;

        void Init(std::shared_ptr<Renderer> renderer);
        void Shutdown();

        void BeginFrame(const glm::mat4& viewProjectionMatrix);

        void Submit(Widget* widget);
        void Submit(const std::vector<Widget*> widgets);

        void DrawQuad(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
        void SubmitLate(std::function<void()> fn);

        void EndFrame();

        std::shared_ptr<TextRenderer> GetTextRenderer() const { return m_TextRenderer; }

        static std::shared_ptr<UIRenderer> Create() { s_Instance = std::make_unique<UIRenderer>(); return s_Instance; }
    private:
        void SortQueue();
        void Flush();

        static void DrawIndexed(VertexArray* vertexArray);
    private:
        static std::shared_ptr<UIRenderer> s_Instance;

        std::shared_ptr<Renderer> m_Renderer = nullptr;
        std::shared_ptr<TextRenderer> m_TextRenderer = nullptr;

        std::vector<RenderCommand> m_DrawQueue;

        std::vector<Widget*> m_Widgets;
        
        glm::mat4 m_ViewProjectionMatrix;
        bool begunScene = false;
        
        std::shared_ptr<VertexArray> m_QuadVA;
        std::shared_ptr<Shader>      m_QuadShader;
        std::vector<std::function<void()>> m_LateCallbacks;
    };
}
