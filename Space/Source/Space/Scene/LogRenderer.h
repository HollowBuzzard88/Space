#pragma once

#include "Space/Core/Message.h"

#include "Space/Rendering/Renderer.h"
#include "Space/Rendering/VertexArray.h"
#include "Space/Rendering/Shader.h"
#include "Space/Rendering/TextRenderer.h"

#define MAIN_DARK_COL glm::vec4(0.17f, 0.18f, 0.18f, 1.0f)
#define MAIN_LIGHT_COL glm::vec4(0.21f, 0.22f, 0.22f, 1.0f)

#define TB_MAIN_COL glm::vec4(0.384f, 0.392f, 0.42f, 1.0f)
#define TB_SELECTED_COL glm::vec4(0.196f, 0.373f, 0.659f, 1.0f)

#define TEXT_COL glm::vec4(0.92f, 0.92f, 0.92f, 1.0f)
#define TEXT_DIM_COL glm::vec4(0.75f, 0.76f, 0.80f, 1.0f)

namespace Space
{
    class LogRenderer
    {
    public:
        LogRenderer();

        void Init(std::shared_ptr<Renderer> renderer);

        void DrawLog(std::vector<Message> messageHistory);
    private:
        void HandleTextInput();
        void SubmitInput();

        void DrawMessages(const std::vector<Message>& messageHistory, const glm::vec2& windowSize,
                          float left, float bottom, float width, float top);
    private:
        std::shared_ptr<VertexArray> m_VertexArray;
        std::shared_ptr<Shader> m_FlatColShader;
        
        std::shared_ptr<Renderer> m_Renderer;

        TextRenderer m_Text;

        bool m_Selected = false;

        std::string m_Input = "";
    };
}
