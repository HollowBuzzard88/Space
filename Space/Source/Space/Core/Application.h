#pragma once

#include "Space/Core/Core.h"

#include "Space/Core/Window.h"

#include "Space/Events/Event.h"
#include "Space/Events/ApplicationEvent.h"

namespace Space
{
    class Application
    {
    public:
        Application();
        ~Application();

        void Run();

        void OnEvent(Event &e);

        inline static Application& Get() { return *s_Instance; }

        inline Window& GetWindow() { return *m_Window; }
    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    private:
        static Application* s_Instance;
    };
}
