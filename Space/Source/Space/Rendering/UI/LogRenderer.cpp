#include "SP_pch.h"
#include "Space/Rendering/UI/LogRenderer.h"

#include "Space/Core/Application.h"
#include "Space/Core/Log.h"

#include <glad/glad.h>

#include "Space/UI/Widgets.h"

namespace Space
{
    std::string FormatMessage(const Message& message)
    {
        return std::format("[{} - {}] {}", message.Time, message.Source == MessageSource::CLIENT ? "CLIENT" : "SERVER", message.Message);
    }

    LogRenderer::LogRenderer()
        : m_Renderer(nullptr)
    {
    }
    
    void LogRenderer::Layout(const glm::vec2& windowSize)
    {
        LogLayout& l = m_Layout;

        l.windowSize = windowSize;

        l.mainSize = windowSize * 0.35f;
        l.mainPos = glm::vec2(windowSize.x - l.mainSize.x, 0.0f);

        l.innerSize = l.mainSize * 0.85f;
        l.innerPos = l.mainPos + (l.mainSize - l.innerSize) * 0.5f;

        l.textBoxSize = glm::vec2(l.innerSize.x - 2.0f * PADDING, l.mainSize.y * 0.1f);
        l.textBoxPos = l.innerPos + glm::vec2(PADDING);

        l.areaLeft = l.innerPos.x + PADDING;
        l.areaWidth = l.innerSize.x - PADDING;
        l.areaBottom = l.textBoxPos.y + l.textBoxSize.y + PADDING;
        l.areaTop = l.textBoxPos.y + l.innerSize.y - PADDING;

        m_MainPanel->SetPosition(l.mainPos); m_MainPanel->SetSize(l.mainSize);
        m_InnerPanel->SetPosition(l.innerPos); m_InnerPanel->SetSize(l.innerSize);
        m_TextBox->SetPosition(l.textBoxPos); m_TextBox->SetSize(l.textBoxSize);
    }

    void LogRenderer::Init(std::shared_ptr<UIRenderer> renderer)
    {
        m_Renderer = renderer;
        m_MainPanel = std::make_unique<Panel>(glm::vec2(0), glm::vec2(1), Color::UI_DARK_MAIN);
        m_InnerPanel = std::make_unique<Panel>(glm::vec2(0), glm::vec2(1), Color::UI_DARK_SECONDARY);
        m_TextBox = std::make_unique<TextBox>(glm::vec2(0), glm::vec2(1), Color::UI_DARK_TERTIARY, Color::UI_DARK_QUATERNARY);

        auto& w = Application::Get().GetWindow();

        Layout(glm::vec2(w.GetWidth(), w.GetHeight()));
    }

    void LogRenderer::Render()
    {
        Widget* widgets[] = { m_MainPanel.get(), m_InnerPanel.get(), m_TextBox.get() };

        m_Renderer->Submit(std::vector<Widget*>(std::begin(widgets), std::end(widgets)));
        m_Renderer->SubmitLate([this] { DrawMessages(Log::GetMessageHistory(), m_Layout); });
    }

    void LogRenderer::DrawMessages(const std::deque<Message>& messageHistory, const LogLayout& layout)
    {
        if (layout.areaTop <= layout.areaBottom || layout.areaWidth <= 0.0f)
            return;

        // Clip everything to the message area, so partially visible lines don't leak out
        glEnable(GL_SCISSOR_TEST);
        glScissor((GLint)layout.areaLeft, (GLint)layout.areaBottom, (GLint)layout.areaWidth, (GLint)(layout.areaTop - layout.areaBottom));

        const float lineHeight = m_Renderer->GetTextRenderer()->GetLineHeight();
        float lineBottom = layout.areaBottom;      // bottom edge of the line currently being placed

        // Newest message at the bottom, older ones stack upwards
        for (int i = messageHistory.size() - 1; i >= 0 && lineBottom < layout.areaTop; i--)
        {
            std::vector<std::string> lines = m_Renderer->GetTextRenderer()->WrapText(FormatMessage(messageHistory[i]), layout.areaWidth);

            for (int j = (int)lines.size() - 1; j >= 0 && lineBottom < layout.areaTop; j--)
            {
                Color textColor = Color::UI_DARK_TEXT;
                switch (messageHistory[i].Type)
                {
                    case MessageType::INFO:  { textColor = Color::UI_DARK_TEXT_INFO; break; }
                    case MessageType::WARN:  { textColor = Color::UI_DARK_TEXT_WARN; break; }
                    case MessageType::ERROR: { textColor = Color::UI_DARK_TEXT_ERROR; break; }
                }

                float baseLine = lineBottom - m_Renderer->GetTextRenderer()->GetDescent();
                m_Renderer->GetTextRenderer()->Draw(lines[j], glm::vec2(layout.areaLeft, baseLine), textColor, layout.windowSize);

                lineBottom += lineHeight;
            }
        }

        glDisable(GL_SCISSOR_TEST);
    }
}
