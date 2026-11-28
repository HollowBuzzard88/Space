#pragma once

#ifdef SP_PLATFORM_WINDOWS
    #define WIN32_LEAN_AND_MEAN
    #define NOMINMAX

    #include <windows.h>
#endif

#include "Space/Core/Window.h"

#include "Space/Core/KeyCodes.h"
#include "Space/Core/MouseCodes.h"

#include <queue>

namespace Space
{
    class WindowsWindow : public Window
    {
        friend class WindowsInput;
    public:
        WindowsWindow(const WindowProps& props);
        virtual ~WindowsWindow();

        void OnUpdate() override;

        virtual void SetWindowCursor(Cursor cursor) override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }

        // ==================Window attributes==================
        inline void SetEventCallback(const EventCallbackFn& callback) override { m_Data.EventCallback = callback; }
        void SetVSync(bool enabled) override;
        bool IsVSync() const override;

        inline virtual void* GetNativeWindow() const { return m_Window; }
    protected:
        static glm::vec2 wndMousePos;

        static bool keyboard[(size_t)Key::COUNT];
        static bool keyboardLast[(size_t)Key::COUNT];

        static std::queue<char> keyboardCharQueue;

        static bool mouse[(size_t)MouseButtons::COUNT];
        static bool mouseLast[(size_t)MouseButtons::COUNT];
    private:
        virtual void Init(const WindowProps& props);
        virtual void Shutdown();

        static LRESULT CALLBACK WindowsEventCallback(HWND window, UINT msg, WPARAM wParam, LPARAM lParam);

        static WindowsWindow* Get() { return s_Instance; }
    private:
        HWND m_Window;

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        static WindowsWindow* s_Instance;

        static HDC m_DeviceContext;
        static WindowData m_Data;
        static Cursor m_CurrentCursor;
        static bool m_ShouldClose;
        static bool m_GLInitialized;
    };
}
