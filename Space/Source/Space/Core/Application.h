#pragma once

#include "Space/Core/Core.h"

#include "Space/Core/Window.h"

#include "Space/Events/Event.h"
#include "Space/Events/ApplicationEvent.h"

#include "Space/Rendering/Renderer.h"

namespace Space
{
    class UIRenderer;

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
        bool OnWindowResize(WindowResizeEvent& e);
    private:
        std::unique_ptr<Window> m_Window;
        std::shared_ptr<Renderer> m_Renderer;
        std::shared_ptr<UIRenderer> m_UIRenderer;
        bool m_Running = true;
    private:
        static Application* s_Instance;
    };
}
