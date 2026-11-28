#include "SP_pch.h"
#include "Space/UI/Widgets/TextBox.h"

#include "Space/Core/Application.h"

#include "Space/Rendering/Renderer.h"
#include "Space/Rendering/UI/UIRenderer.h"

namespace Space
{
    void TextBox::EarlyUpdate(std::shared_ptr<UIRenderer> UIrenderer, std::shared_ptr<Renderer> renderer)
    {
        float minWidth = UIrenderer->GetTextRenderer()->MeasureText("Type a message or /command");
        if (m_Size.x < minWidth) m_Size.x = minWidth + PADDING * 2.0f;

        m_Transform = glm::translate(glm::mat4(1.0f), glm::vec3(m_Position + m_Size / 2.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(m_Size, 1.0f));
        m_Rect = Rect(m_Position, m_Size);

        m_Hovered = m_Rect.IsInside(Input::GetMousePos());
        Application::Get().GetWindow().SetWindowCursor(m_Hovered ? Cursor::Text : Cursor::Arrow);

        m_Color = m_Hovered ? m_SelectedCol : m_MainCol;
        
        if (Input::IsMouseButtonPressed(MouseButtons::LEFT)) m_Selected = m_Hovered;

        HandleTextInput();
    }

    void TextBox::LateUpdate(std::shared_ptr<UIRenderer> UIrenderer, std::shared_ptr<Renderer> renderer)
    {
        auto text = UIrenderer->GetTextRenderer();
        auto& win = Application::Get().GetWindow();
        glm::vec2 windowSize = glm::vec2((float)win.GetWidth(), (float)win.GetHeight());

        float caretWidth = 2.0f;
        float textLeft = m_Position.x + PADDING;         // Add padding for the caret
        float maxTextWidth = m_Size.x - 2.0f * PADDING - caretWidth * 2.0f;
        float baseLine = m_Position.y + m_Size.y * 0.5f - (text->GetAscent() + text->GetDescent()) * 0.5f;
        
        if (m_Text.empty() && !m_Selected)
        {
            UIrenderer->GetTextRenderer()->
                Draw("Type a message or /command", glm::vec2(textLeft, baseLine), Color::UI_DARK_TEXT_DIM, windowSize);
            return;
        }

        std::string visible = m_Text;
        while (!visible.empty() && UIrenderer->GetTextRenderer()->MeasureText(visible) > maxTextWidth)
            visible.erase(0, 1);

        UIrenderer->GetTextRenderer()->Draw(visible, glm::vec2(textLeft, baseLine), Color::UI_DARK_TEXT, windowSize);

        if (m_Selected)
        {
            float textWidth = UIrenderer->GetTextRenderer()->MeasureText(visible);
            glm::vec2 caretSize = glm::vec2(caretWidth, UIrenderer->GetTextRenderer()->GetLineHeight() * 0.8f);
            glm::vec2 caretPos = glm::vec2(textLeft + textWidth + 1.0f, m_Position.y);

            UIrenderer->DrawQuad(caretPos, caretSize, Color::UI_DARK_TEXT);
        }
    }
}
