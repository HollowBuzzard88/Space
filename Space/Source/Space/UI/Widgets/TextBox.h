#pragma once
#include "Space/UI/Widgets.h"

#include "Space/Rendering/Rect.h"

#include "Space/Core/Input.h"

#include "Space/Commands/CommandHandler.h"

namespace Space
{
    constexpr float PADDING = 8.0f;

    class TextBox : public Widget
    {
    public:
        TextBox(const glm::vec2& position, const glm::vec2& size, const Color& mainColor, const Color& selectedColor)
            : Widget(position, size, mainColor), m_Rect(Rect(position, size)), m_MainCol(mainColor), m_SelectedCol(selectedColor)
        {
            m_VA = VertexArray::Create();
            float vertices[4 * 2] =
            {
               -0.5f, -0.5f,
                0.5f, -0.5f,
                0.5f,  0.5f,
               -0.5f,  0.5f
            };
            uint32_t indices[6] = { 0, 1, 2, 2, 3, 0 };

            std::shared_ptr<VertexBuffer> VB = VertexBuffer::Create(vertices, sizeof(vertices));
            VB->SetLayout({
                { ShaderDataType::Float2, "inPos" }
            });
            m_VA->AddVertexBuffer(VB);

            std::shared_ptr<IndexBuffer> IB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
            m_VA->SetIndexBuffer(IB);

            m_Shader = Shader::Create(AssetManager::GetAbsolutePath("Shaders/UIFlatColor.glsl"));
        }

        void SubmitInput()
        {
            if (m_Text.empty()) return;

            // Command logic
            if (m_Text[0] == '/')
            {
                // Create a string with everything behind the first character ('/')
                std::string command = m_Text.substr(1);
                CommandHandler::Execute(command);
            }
            else
            {
                CL_INFO("Chat: {0}", m_Text);
            }
            m_Text.clear();
        }
        
        void HandleTextInput()
        {
            char c;
            while ((c = Input::GetLastChar()) != 0)
            {
                if (!m_Selected) continue;

                switch (c)
                {
                    case '\b':      if (!m_Text.empty()) m_Text.pop_back(); break;
                    case '\r':
                    case '\n':      SubmitInput(); break;
                    case '\x1b':    m_Selected = false; break;
                    case '\t':      break;
                    default:
                        if (c >= 32 && c != 127) m_Text += c;
                        break;
                }
            }
        }

        virtual void EarlyUpdate(std::shared_ptr<UIRenderer> UIrenderer, std::shared_ptr<Renderer> renderer) override;
        virtual void LateUpdate(std::shared_ptr<UIRenderer> UIrenderer, std::shared_ptr<Renderer> renderer) override;
    private:
        Color m_MainCol, m_SelectedCol;

        bool m_Hovered = false, m_Selected = false;
        Rect m_Rect;

        std::string m_Text = "";
    };
}
