#include "SP_pch.h"
#include "Space/Core/Application.h"

// Include Entrypoint so that C++ recognizes it
#include "Entrypoint.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Space
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(SP_BIND_EVENT_FN(Application::OnEvent));

        m_Renderer = std::make_unique<Renderer>();
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event &e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(SP_BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<WindowResizeEvent>(SP_BIND_EVENT_FN(Application::OnWindowResize));

        //TODO: Figure out how to handle events. Either layers like in TheCherno's version or different
    }

    void Application::Run()
    {
        m_Renderer->Init();

        while (m_Running)
        {
            glm::mat4 projection = glm::ortho(0.0f, (float)m_Window->GetWidth(), 0.0f, (float)m_Window->GetHeight(), -1.0f, 1.0f);
            glm::mat4 view = glm::mat4(1.0f);
            m_Renderer->BeginScene(projection * view);

            

            m_Renderer->EndScene();
            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnWindowResize(WindowResizeEvent &e)
    {
        m_Renderer->OnWindowResize(e.GetWidth(), e.GetHeight());
        return true;
    }
}
