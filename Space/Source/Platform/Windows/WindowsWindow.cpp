#include "SP_pch.h"
#include "Platform/Windows/WindowsWindow.h"

#include <windowsx.h>
#include "KHR/wglext.h"

#include "Space/Events/ApplicationEvent.h"
#include "Space/Events/MouseEvent.h"

#include "Space/Core/MouseCodes.h"
#include "Space/Core/Application.h"

#include "Space/Rendering/Renderer.h"

namespace Space
{
    HDC WindowsWindow::m_DeviceContext;
    std::pair<float, float> WindowsWindow::wndMousePos(0.0f, 0.0f);
    WindowsWindow::WindowData WindowsWindow::m_Data = {};
    bool WindowsWindow::wndMouseButtons[5];
    bool WindowsWindow::m_ShouldClose = false;
    bool WindowsWindow::m_GLInitialized = false;

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

    void* loadGLFunction(const char* fnName)
    {
        PROC proc = wglGetProcAddress(fnName);
        if(!proc)
        {
            static HMODULE openglDLL = LoadLibraryA("opengl32.dll");
            proc = GetProcAddress(openglDLL, fnName);
            if(!proc)
            {
                //TODO: Add Error Logging
                return nullptr;
            }
        }

        return (void*)proc;
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
            
            // Called when resizing the window
            case WM_SIZE:
            {
                if (wParam == SIZE_MAXHIDE || wParam == SIZE_MINIMIZED)
                    return 0;

                int width = LOWORD(lParam);
                int height = HIWORD(lParam);

                if (width == 0 || height == 0)
                    return 0;

                // Update the viewport OpenGL uses
                if (m_GLInitialized)
                {
                    glViewport(0, 0, width, height);
                }
                
                WindowResizeEvent e(width, height);
                if (WindowsWindow::m_Data.EventCallback)
                {
                    WindowsWindow::m_Data.EventCallback(e);
                }

                return 0;
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

        PFNWGLCHOOSEPIXELFORMATARBPROC wglChoosePixelFormatARB = nullptr;
        PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = nullptr;

        // Initialize OpenGL using a 'fake' window
        {
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

            HDC fakeDC = GetDC(m_Window);

            if (!fakeDC)
            {
                //TODO: Add Error Logging
                return;
            }

            PIXELFORMATDESCRIPTOR pfd = {0};
            pfd.nSize = sizeof(pfd);
            pfd.nVersion = 1;
            pfd.dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER;
            pfd.cColorBits = 32;
            pfd.cAlphaBits = 8;
            pfd.cDepthBits = 24;

            int pixelFormat = ChoosePixelFormat(fakeDC, &pfd);
            if (!pixelFormat)
            {
                //TODO: Add Error Logging
                return;
            }

            if (!SetPixelFormat(fakeDC, pixelFormat, &pfd))
            {
                //TODO: Add Error Logging
                return;
            }

            // Create a Handle to a 'fake' OpenGL Rendering Context
            HGLRC fakeRC = wglCreateContext(fakeDC);
            if (!fakeRC)
            {
                //TODO: Add Error Logging
                return;
            }

            if (!wglMakeCurrent(fakeDC, fakeRC))
            {
                //TODO: Add Error Logging
                return;
            }

            // Load necessary OpenGL Functions
            wglChoosePixelFormatARB =
                (PFNWGLCHOOSEPIXELFORMATARBPROC)loadGLFunction("wglChoosePixelFormatARB");
            wglCreateContextAttribsARB =
                (PFNWGLCREATECONTEXTATTRIBSARBPROC)loadGLFunction("wglCreateContextAttribsARB");

            // Clean up unnecessary functions
            wglMakeCurrent(fakeDC, 0);
            wglDeleteContext(fakeRC);
            ReleaseDC(m_Window, fakeDC);

            DestroyWindow(m_Window);
        }

        // Create the actual OpenGL initialization
        {
            RECT borderRect = {};
            AdjustWindowRectEx(&borderRect, dwStyle, 0, 0);

            m_Data.Width += borderRect.right - borderRect.left;
            m_Data.Height += borderRect.bottom - borderRect.top;

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

            m_DeviceContext = GetDC(m_Window);

            if (!m_DeviceContext)
            {
                //TODO: Add Error Logging
                return;
            }

            const int pixelAttribs[] =
            {
                WGL_DRAW_TO_WINDOW_ARB, GL_TRUE,
                WGL_SUPPORT_OPENGL_ARB, GL_TRUE,
                WGL_DOUBLE_BUFFER_ARB,  GL_TRUE,
                WGL_SWAP_METHOD_ARB,    WGL_SWAP_COPY_ARB,
                WGL_PIXEL_TYPE_ARB,     WGL_TYPE_RGBA_ARB,
                WGL_ACCELERATION_ARB,   WGL_FULL_ACCELERATION_ARB,
                WGL_COLOR_BITS_ARB,     32,
                WGL_ALPHA_BITS_ARB,     8,
                WGL_DEPTH_BITS_ARB,     24,
                0   // Terminare with 0, otherwise OpenGL will throw an error
            };

            UINT numPixelFormats;
            int pixelFormat = 0;
            if (!wglChoosePixelFormatARB(m_DeviceContext, pixelAttribs, 0/*Float list*/, 1/*Max Formats*/, &pixelFormat, &numPixelFormats ))
            {
                //TODO: Add Error Logging
                return;
            }

            PIXELFORMATDESCRIPTOR pfd = {0};
            DescribePixelFormat(m_DeviceContext, pixelFormat, sizeof(PIXELFORMATDESCRIPTOR), &pfd);

            if (!SetPixelFormat(m_DeviceContext, pixelFormat, &pfd))
            {
                //TODO: Add Error Logging
                return;
            }

            const int contextAttribs[] =
            {
                WGL_CONTEXT_MAJOR_VERSION_ARB, 4,
                WGL_CONTEXT_MINOR_VERSION_ARB, 6,
                WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
                WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_DEBUG_BIT_ARB,
                0 // Terminate the array
            };

            HGLRC rc = wglCreateContextAttribsARB(m_DeviceContext, 0, contextAttribs);
            if (!rc)
            {
                //TODO: Add Error Logging
                return;
            }

            if (!wglMakeCurrent(m_DeviceContext, rc))
            {
                //TODO: Add Error Logging
                return;
            }
        }

        ShowWindow(m_Window, SW_SHOW);

        // Load all OpenGL functions
        gladLoadGL();
        m_GLInitialized = true;
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

        SwapBuffers(m_DeviceContext);
    }

    void WindowsWindow::SetVSync(bool enabled)
    {

    }

    bool WindowsWindow::IsVSync() const
    {
        return false;
    }
}
