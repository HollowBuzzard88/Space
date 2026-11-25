#include "SP_pch.h"
#include "Space/Scene/LogRenderer.h"

#include "Space/Core/Application.h"
#include "Space/Core/AssetManager.h"
#include "Space/Core/Input.h"

#include <glad/glad.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include <format>

namespace Space
{
    namespace
    {
        constexpr float PADDING = 8.0f;

        std::string FormatMessage(const Message& message)
        {
            return std::format("[{} - {}] {}", message.Time, message.Source == MessageSource::CLIENT ? "CLIENT" : "SERVER", message.Message);
        }

        glm::mat4 QuadTransform(const glm::vec2& center, const glm::vec2& size)
        {
            return glm::translate(glm::mat4(1.0f), glm::vec3(center, 0.0f)) *
                    glm::scale(glm::mat4(1.0f), glm::vec3(size, 1.0f));
        }
    }

    LogRenderer::LogRenderer()
        : m_VertexArray(nullptr), m_FlatColShader(nullptr), m_Renderer(nullptr)
    {
    }

    void LogRenderer::Init(std::shared_ptr<Renderer> renderer)
    {
        m_Renderer = renderer;

        m_VertexArray = VertexArray::Create();

        float vertices[4 * 3] =
        {
           -0.5f, -0.5f, 0.0f,
            0.5f, -0.5f, 0.0f,
            0.5f,  0.5f, 0.0f,
           -0.5f,  0.5f, 0.0f
        };

        std::shared_ptr<VertexBuffer> VB = VertexBuffer::Create(vertices, sizeof(vertices));
        VB->SetLayout({
            { ShaderDataType::Float3, "vPos" }
        });
        m_VertexArray->AddVertexBuffer(VB);

        uint32_t indices[6] =
        {
            0, 1, 2,
            2, 3, 0
        };
        std::shared_ptr<IndexBuffer> IB = IndexBuffer::Create(indices, sizeof(indices) / sizeof(uint32_t));
        m_VertexArray->SetIndexBuffer(IB);

        m_FlatColShader = Shader::Create(
            "FlatColor",
            AssetManager::GetAbsolutePath("Shaders/vertex.vert"),
            AssetManager::GetAbsolutePath("Shaders/fragment.frag")
        );

        if (!m_Text.Init(AssetManager::GetAbsolutePath("Fonts/consola.ttf"), 18.0f))
            m_Text.Init("C:/Windows/Fonts/consola.ttf");
    }

    void LogRenderer::DrawLog(std::vector<Message> messageHistory)
    {
        m_FlatColShader->Bind();

        glm::vec2 windowSize = glm::vec2(Application::Get().GetWindow().GetWidth(), Application::Get().GetWindow().GetHeight());

        glm::vec2 size = windowSize * 0.35f;
        glm::vec2 position = glm::vec2(windowSize.x - size.x / 2.0f, size.y / 2.0f);

        glm::vec2 innerSize = size * 0.85f;

        glm::vec2 textBoxSize = glm::vec2(size.x - (size.x * 0.7f), size.y * 0.1f);
        glm::vec2 textBoxCenter = glm::vec2(position.x, position.y - (innerSize.y / 2.0f) + PADDING + (textBoxSize.y / 2.0f));

        // -------------------- Boxes -------------------- //
        m_Renderer->Submit(m_FlatColShader, m_VertexArray, QuadTransform(position, size), MAIN_DARK_COL);
        m_Renderer->Submit(m_FlatColShader, m_VertexArray, QuadTransform(position, innerSize), MAIN_LIGHT_COL);
        m_Renderer->Submit(m_FlatColShader, m_VertexArray, QuadTransform(textBoxCenter, textBoxSize), m_Selected ? TB_SELECTED_COL : TB_MAIN_COL);

        // -------------------- Mouse: focus / unfocus the text box -------------------- //
        glm::vec2 mouse = Input::GetMousePos();

        bool isInTextBox = mouse.x > textBoxCenter.x - textBoxSize.x / 2.0f && mouse.x < textBoxCenter.x + textBoxSize.x / 2.0f &&
                            mouse.y >= textBoxCenter.y - textBoxSize.y / 2.0f && mouse.y <= textBoxCenter.y + textBoxSize.y / 2.0f;
        if (isInTextBox)
            Application::Get().GetWindow().SetWindowCursor(Cursor::Text);
        else
            Application::Get().GetWindow().SetWindowCursor(Cursor::Arrow);

        if (Input::IsMouseButtonPressed(MouseButtons::LEFT)) m_Selected = isInTextBox;


        // -------------------- Keyboard -------------------- //
        HandleTextInput();

        if (!m_Text.IsReady())
            return;

        // -------------------- Message area (between the top of the inner box and the text box) -------------------- //
        float areaLeft = position.x - innerSize.x / 2.0f + PADDING;
        float areaWidth = innerSize.x - 2.0f * PADDING;
        float areaTop = position.y + innerSize.y / 2.0f - PADDING;
        float areaBottom = textBoxCenter.y + textBoxSize.y / 2.0f + PADDING;

        DrawMessages(messageHistory, windowSize, areaLeft, areaBottom, areaWidth, areaTop);

        
        // -------------------- Input text (inside the text box) --------------------
        float textLeft = textBoxCenter.x - textBoxSize.x / 2.0f + PADDING;
        float maxTextWidth = textBoxSize.x - 2.0f * PADDING - 4.0f;
        float baseLine = textBoxCenter.y - (m_Text.GetAscent() + m_Text.GetDescent()) / 2.0f;

        if (m_Input.empty() && !m_Selected)
        {
            m_Text.Draw("Type a message or /command", glm::vec2(textLeft, baseLine), TEXT_DIM_COL, windowSize);
        }
        else
        {
            // Only show the tail of the input if it is wider than the box
            std::string visible = m_Input;
            while (!visible.empty() && m_Text.MeasureText(visible) > maxTextWidth)
                visible.erase(0, 1);
            
            float textWidth = m_Text.MeasureText(visible);

            if (m_Selected)
            {
                glm::vec2 caretSize = glm::vec2(2.0f, m_Text.GetLineHeight() * 0.8f);
                glm::vec2 caretCenter = glm::vec2(textLeft + textWidth + 1.0f + caretSize.x / 2.0f, textBoxCenter.y);

                m_Renderer->Submit(m_FlatColShader, m_VertexArray, QuadTransform(caretCenter, caretSize), TEXT_COL);
            }

            m_Text.Draw(visible, glm::vec2(textLeft, baseLine), TEXT_COL, windowSize);
        }
    }

    void LogRenderer::DrawMessages(const std::vector<Message> &messageHistory, const glm::vec2 &windowSize, float left, float bottom, float width, float top)
    {
        if (top <= bottom || width <= 0.0f)
            return;

        // Clip everything to the message area, so partially visible lines don't leak out
        glEnable(GL_SCISSOR_TEST);
        glScissor((GLint)left, (GLint)bottom, (GLint)width, (GLint)(top - bottom));

        const float lineHeight = m_Text.GetLineHeight();
        float lineBottom = bottom;      // bottom edge of the line currently being placed
        
        // Newest message at the bottom, older ones stack upwards
        for (int i = (int)messageHistory.size() - 1; i >= 0 && lineBottom < top; i--)
        {
            std::vector<std::string> lines = m_Text.WrapText(FormatMessage(messageHistory[i]), width);

            for (int j = (int)lines.size() - 1; j >= 0 && lineBottom < top; j--)
            {
                float baseLine = lineBottom - m_Text.GetDescent();
                m_Text.Draw(lines[j], glm::vec2(left, baseLine), TEXT_COL, windowSize);

                lineBottom += lineHeight;
            }
        }

        glDisable(GL_SCISSOR_TEST);
    }

    void LogRenderer::HandleTextInput()
    {
        char c;
        while ((c = Input::GetLastChar()) != 0)
        {
            if (!m_Selected) continue;

            switch (c)
            {
                case '\b':      if (!m_Input.empty()) m_Input.pop_back(); break;
                case '\r':
                case '\n':      SubmitInput(); break;
                case '\x1b':    m_Selected = false; break;
                case '\t':      break;
                default:
                    if (c >= 32 && c != 127) m_Input += c;
                    break;
            }
        }
    }

    void LogRenderer::SubmitInput()
    {
        if (m_Input.empty()) return;

        if (m_Input[0] == '/')
        {
            // Command logic
            std::istringstream stream(m_Input.substr(1));
            std::string name;
            stream >> name;

            std::vector<std::string> args;
            for (std::string arg; stream >> arg;) args.push_back(arg);

            SP_CORE_INFO("Command '{0}' with {1} args", name, args.size());
        }
        else
        {
            SP_CORE_INFO("Chat: {0}", m_Input); // TODO: push to message history
        }

        m_Input.clear();
    }
}
