#pragma once

#include "Space/Core/Message.h"

#include "Space/Rendering/UI/UIRenderer.h"

namespace Space
{
    struct LogLayout
    {
        float PADDING = 8.0f;

        glm::vec2 windowSize;
        glm::vec2 mainSize;
        glm::vec2 mainPos;

        glm::vec2 innerSize;
        glm::vec2 innerPos;

        glm::vec2 textBoxSize;
        glm::vec2 textBoxPos;

        float areaLeft;
        float areaWidth;
        float areaTop;
        float areaBottom;
    };

    class LogRenderer
    {
    public:
        LogRenderer();

        void Init(std::shared_ptr<UIRenderer> renderer);
        void Layout(const glm::vec2& windowSize);

        void Render();

        void OnWindowResize(const glm::vec2& windowSize) { Layout(windowSize); }
    private:
        void DrawMessages(const std::deque<Message>& messageHistory, const LogLayout& layout);
    private:
        std::shared_ptr<UIRenderer> m_Renderer;

        std::unique_ptr<Panel> m_MainPanel, m_InnerPanel;
        std::unique_ptr<TextBox> m_TextBox;
        LogLayout m_Layout;
    };
}
