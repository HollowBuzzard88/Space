#include "Platform/Windows/WindowsWindow.h"

#include <windowsx.h>

#include "Space/Events/ApplicationEvent.h"
#include "Space/Events/MouseEvent.h"

#include "Space/Core/MouseCodes.h"

namespace Space
{
    std::pair<float, float> WindowsWindow::wndMousePos(0.0f, 0.0f);
    WindowsWindow::WindowData WindowsWindow::m_Data = {};
    bool WindowsWindow::wndMouseButtons[5];
    bool WindowsWindow::m_ShouldClose = false;

    Window* Window::Create(const WindowProps& props)
    {
        return new WindowsWindow(props);
    }

    WindowsWindow::WindowsWindow(const WindowProps &props)
    {
        Init(props);
    }

    WindowsWindow::~WindowsWindow()
    {
        Shutdown();
    }

    LRESULT CALLBACK WindowsWindow::WindowsEventCallback(HWND window, UINT msg, WPARAM wParam, LPARAM lParam)
    {
        switch (msg)
        {
            // Called when the window is closed
            case WM_CLOSE:
            {
                m_ShouldClose = true;

                WindowCloseEvent e;

                if (WindowsWindow::m_Data.EventCallback)
                {
                    WindowsWindow::m_Data.EventCallback(e);
                }

                return 0;
            }

            case WM_DESTROY:
            {
                return 0;
            }
            
            // Called during the act of resizing the window
            case WM_SIZING:
            {
                RECT newSize = static_cast<RECT>(lParam);
                WindowResizeEvent e(newSize.right - newSize.left, newSize.bottom - newSize.top);

                if (WindowsWindow::m_Data.EventCallback)
                {
                    WindowsWindow::m_Data.EventCallback(e);
                }

                return true;
            }

            case WM_MOUSEMOVE:
            {
                int xPos = GET_X_LPARAM(lParam);
                int yPos = GET_Y_LPARAM(lParam);

                WindowsWindow::wndMousePos = std::pair<float, float>(xPos, yPos);


                MouseMovedEvent e(xPos, yPos);

                if (WindowsWindow::m_Data.EventCallback)
                {
                    WindowsWindow::m_Data.EventCallback(e);
                }

                return 0;
            }

            // Called when the 4th and 5th mouse button is pressed or lifted respectively
            {
            case WM_XBUTTONDOWN:
            {
                if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)         // 4th mouse button
                {
                    int buttonIndex = (uint8_t)MouseButtons::BUTTON_4;

                    MouseButtonPressedEvent e(buttonIndex);
                    WindowsWindow::m_Data.EventCallback(e);

                    wndMouseButtons[buttonIndex] = true;
                }
                else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2)    // 5th mouse button
                {
                    int buttonIndex = (uint8_t)MouseButtons::BUTTON_5;

                    MouseButtonPressedEvent e(buttonIndex);
                    WindowsWindow::m_Data.EventCallback(e);

                    wndMouseButtons[buttonIndex] = true;
                }
                else
                    //TODO: Throw Error
                

                return true;
            }
            case WM_XBUTTONUP:
            {
                if (GET_XBUTTON_WPARAM(wParam) == XBUTTON1)         // 4th mouse button
                {
                    int buttonIndex = (uint8_t)MouseButtons::BUTTON_4;

                    MouseButtonPressedEvent e(buttonIndex);
                    WindowsWindow::m_Data.EventCallback(e);

                    wndMouseButtons[buttonIndex] = false;
                }
                else if (GET_XBUTTON_WPARAM(wParam) == XBUTTON2)    // 5th mouse button
                {
                    int buttonIndex = (uint8_t)MouseButtons::BUTTON_5;

                    MouseButtonPressedEvent e(buttonIndex);
                    WindowsWindow::m_Data.EventCallback(e);

                    wndMouseButtons[buttonIndex] = false;
                }
                else
                    //TODO: Throw Error
                

                return true;
            }
            }

            // Called when the left mouse button is pressed or lifted respectively
            {
            case WM_LBUTTONDOWN:
            {
                int buttonIndex = (uint8_t)MouseButtons::LEFT;

                MouseButtonPressedEvent e(buttonIndex);
                WindowsWindow::m_Data.EventCallback(e);

                wndMouseButtons[buttonIndex] = true;
                return 0;
            }
            case WM_LBUTTONUP:
            {
                int buttonIndex = (uint8_t)MouseButtons::LEFT;

                MouseButtonPressedEvent e(buttonIndex);
                WindowsWindow::m_Data.EventCallback(e);

                wndMouseButtons[buttonIndex] = false;
                return 0;
            }
            }

            // Called when the right mouse button is pressed or lifted respectively
            {
            case WM_RBUTTONDOWN:
            {
                int buttonIndex = (uint8_t)MouseButtons::RIGHT;

                MouseButtonPressedEvent e(buttonIndex);
                WindowsWindow::m_Data.EventCallback(e);

                wndMouseButtons[buttonIndex] = true;
                return 0;
            }
            case WM_RBUTTONUP:
            {
                int buttonIndex = (uint8_t)MouseButtons::RIGHT;

                MouseButtonPressedEvent e(buttonIndex);
                WindowsWindow::m_Data.EventCallback(e);

                wndMouseButtons[buttonIndex] = false;
                return 0;
            }
            }

            default:
            {
                // Invoke the default event handler for everything else
                return DefWindowProcA(window, msg, wParam, lParam);
            }
        }
    }

    void WindowsWindow::Init(const WindowProps& props)
    {
        m_Data.Title = props.Title;
        m_Data.Width = props.Width;
        m_Data.Height = props.Height;

        HINSTANCE instance = GetModuleHandleA(0);

        WNDCLASSA wc = {};
        wc.hInstance = instance;
        
        // Visuals
        wc.hIcon = LoadIcon(instance, IDI_APPLICATION); // Default app icon
        wc.hCursor = LoadCursor(NULL, IDC_ARROW);       // Default arrow Cursor

        wc.lpszClassName = m_Data.Title.c_str();         // NOT the window Title but a unique identifier
        wc.lpfnWndProc = WindowsEventCallback;          // Event callback

        if (!RegisterClassA(&wc))
        {
            //TODO: Add Error Logging
            return;
        }

        int dwStyle = WS_OVERLAPPEDWINDOW;

        m_Window = CreateWindowExA(
            0, m_Data.Title.c_str(),        //References the wc.lpszClassName
            m_Data.Title.c_str(),           // The actual window Title
            dwStyle,
            100, 100,                       // X, Y position
            m_Data.Width, m_Data.Height,    // Window size
            NULL, NULL,
            instance,
            NULL
        );

        if (m_Window == NULL)
        {
            //TODO: Add Error Logging
            return;
        }

        ShowWindow(m_Window, SW_SHOW);
    }

    //TODO: Maybe use some of the GLFW procedure to destroying a window:
    /*
   *void _glfwDestroyWindowWin32(_GLFWwindow* window)
   *{
   *    if (window->monitor)
   *        releaseMonitor(window);

   *    if (window->context.destroy)
   *        window->context.destroy(window);

   *    if (_glfw.win32.disabledCursorWindow == window)
   *        _glfw.win32.disabledCursorWindow = NULL;

   *    if (window->win32.handle)
   *    {
   *        RemovePropW(window->win32.handle, L"GLFW");
   *        DestroyWindow(window->win32.handle);
   *        window->win32.handle = NULL;
   *    }

   *    if (window->win32.bigIcon)
   *        DestroyIcon(window->win32.bigIcon);

   *    if (window->win32.smallIcon)
   *        DestroyIcon(window->win32.smallIcon);
   *}
    */

    void WindowsWindow::Shutdown()
    {
        DestroyWindow(m_Window);
    }

    void WindowsWindow::OnUpdate()
    {
        MSG msg;

        while (PeekMessageA(&msg, m_Window, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessageA(&msg);
        }
    }

    void WindowsWindow::SetVSync(bool enabled)
    {

    }

    bool WindowsWindow::IsVSync() const
    {
        return false;
    }
}
