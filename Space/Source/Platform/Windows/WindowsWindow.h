#pragma once

#include "Space/Core/Core.h"
#include "Space/Core/Window.h"

namespace Space
{
    class WindowsWindow : public Window
    {
        friend class WindowsInput;
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }

        inline bool ShouldClose() const override { return m_ShouldClose; }
        
        // ==================Window attributes==================
        inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        inline virtual void* GetNativeWindow() const { return m_Window; }
    protected:
        static std::pair<float, float> wndMousePos;
        static bool wndMouseButtons[5];
    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();

        static LRESULT CALLBACK WindowsEventCallback(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);
    private:
        HWND m_Window;

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        static WindowData m_Data;
    
        static bool m_ShouldClose;
    };
}
