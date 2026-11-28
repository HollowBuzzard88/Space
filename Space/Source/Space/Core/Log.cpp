#include "SP_pch.h"
#include "Space/Core/Log.h"

#include "Space/Rendering/UI/LogRenderer.h"
#include "Space/Rendering/UI/UIRenderer.h"

namespace Space
{
    std::deque<Message> Log::m_MessageHistory;
    std::shared_ptr<LogRenderer> Log::m_LogRenderer = nullptr;

    void Log::Init(std::shared_ptr<UIRenderer> UIrenderer)
    {
        m_MessageHistory = std::deque<Message>();

        m_LogRenderer = std::make_shared<LogRenderer>();
        m_LogRenderer->Init(UIrenderer);
    }

    void Log::RenderLogger()
    {
        m_LogRenderer->Render();
    }

    void Log::OnWindowResize(const glm::vec2 &windowSize)
    {
        m_LogRenderer->OnWindowResize(windowSize);
    }
}
