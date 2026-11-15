
#include "Space/Core/Application.h"
#include <glad/glad.h>

// Include Entrypoint so that C++ recognizes it
#include "Entrypoint.h"

namespace Space
{
    Application* Application::s_Instance = nullptr;

    Application::Application()
    {
        s_Instance = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallback(SP_BIND_EVENT_FN(Application::OnEvent));
    }

    Application::~Application()
    {
    }

    void Application::OnEvent(Event &e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(SP_BIND_EVENT_FN(Application::OnWindowClose));

        //TODO: Figure out how to handle events. Either layers like in TheCherno's version or different
    }

    void Application::Run()
    {
        while (m_Running)
        {
            glClearColor(0.87f, 0.56f, 0.67f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            //TODO: Get an update loop going

            m_Window->OnUpdate();
        }
    }

    bool Application::OnWindowClose(WindowCloseEvent& e)
    {
        m_Running = false;
        return true;
    }
}
