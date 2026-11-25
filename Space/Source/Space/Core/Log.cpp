#include "SP_pch.h"
#include "Space/Core/Log.h"

#include "Space/Scene/LogRenderer.h"
#include "Space/Rendering/Renderer.h"

namespace Space
{
    std::vector<Message> Log::m_MessageHistory;
    std::shared_ptr<LogRenderer> Log::m_LogRenderer = nullptr;
    std::shared_ptr<Renderer> Log::m_Renderer = nullptr;

    void Log::Init(std::shared_ptr<Renderer> renderer)
    {
        m_MessageHistory = std::vector<Message>();

        m_Renderer = renderer;
        m_MessageHistory.reserve(MAX_MESSAGES);
        m_LogRenderer = std::make_shared<LogRenderer>();
        m_LogRenderer->Init(renderer);
    }

    void Log::RenderLogger()
    {
        m_LogRenderer->DrawLog(m_MessageHistory);
    }
}
